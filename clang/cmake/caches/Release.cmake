# Plain options configure the first build.
# BOOTSTRAP_* options configure the second build.
# BOOTSTRAP_BOOTSTRAP_* options configure the third build.
# PGO Builds have 3 stages (stage1, stage2-instrumented, stage2)
# non-PGO Builds have 2 stages (stage1, stage2)


function (set_final_stage_var name value type)
  if (LLVM_RELEASE_ENABLE_PGO)
    set(BOOTSTRAP_BOOTSTRAP_${name} ${value} CACHE ${type} "")
  else()
    set(BOOTSTRAP_${name} ${value} CACHE ${type} "")
  endif()
endfunction()

function (set_instrument_and_final_stage_var name value type)
  # This sets the varaible for the final stage in non-PGO builds and in
  # the stage2-instrumented stage for PGO builds.
  set(BOOTSTRAP_${name} ${value} CACHE ${type} "")
  if (LLVM_RELEASE_ENABLE_PGO)
    # Set the variable in the final stage for PGO builds.
    set(BOOTSTRAP_BOOTSTRAP_${name} ${value} CACHE ${type} "")
  endif()
endfunction()

# General Options:
# If you want to override any of the LLVM_RELEASE_* variables you can set them
# on the command line via -D, but you need to do this before you pass this
# cache file to CMake via -C. e.g.
#
# cmake -D LLVM_RELEASE_ENABLE_PGO=ON -C Release.cmake

set (DEFAULT_PROJECTS "clang;lld;lldb;clang-tools-extra;polly;mlir;flang")
# bolt only supports ELF, so only enable it for Linux.
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  list(APPEND DEFAULT_PROJECTS "bolt")
endif()

set (DEFAULT_RUNTIMES "compiler-rt;libcxx")
if (NOT WIN32)
  list(APPEND DEFAULT_RUNTIMES "libcxxabi" "libunwind")
endif()
set(LLVM_RELEASE_ENABLE_LTO THIN CACHE STRING "")
set(LLVM_RELEASE_ENABLE_PGO ON CACHE BOOL "")
set(LLVM_RELEASE_ENABLE_RUNTIMES ${DEFAULT_RUNTIMES} CACHE STRING "")
set(LLVM_RELEASE_ENABLE_PROJECTS ${DEFAULT_PROJECTS} CACHE STRING "")
# Note we don't need to add install here, since it is one of the pre-defined
# steps.
set(LLVM_RELEASE_FINAL_STAGE_TARGETS "clang;package;check-all;check-llvm;check-clang" CACHE STRING "")
set(CMAKE_BUILD_TYPE RELEASE CACHE STRING "")

# Stage 1 Options
set(LLVM_TARGETS_TO_BUILD Native CACHE STRING "")
set(CLANG_ENABLE_BOOTSTRAP ON CACHE BOOL "")

set(STAGE1_PROJECTS "clang")

# Build all runtimes so we can statically link them into the stage2 compiler.
set(STAGE1_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind")

if (LLVM_RELEASE_ENABLE_PGO)
  list(APPEND STAGE1_PROJECTS "lld")
  set(tmp_targets
    generate-profdata
    stage2-package
    stage2-clang
    stage2
    stage2-install
    stage2-check-all
    stage2-check-llvm
    stage2-check-clang)

  foreach(X IN LISTS LLVM_RELEASE_FINAL_STAGE_TARGETS)
    list(APPEND tmp_targets "stage2-${X}")
  endforeach()
  list(REMOVE_DUPLICATES tmp_targets)

  set(CLANG_BOOTSTRAP_TARGETS "${tmp_targets}" CACHE STRING "")

  # Configuration for stage2-instrumented
  set(BOOTSTRAP_CLANG_ENABLE_BOOTSTRAP ON CACHE STRING "")
  # This enables the build targets for the final stage which is called stage2.
  set(BOOTSTRAP_CLANG_BOOTSTRAP_TARGETS ${LLVM_RELEASE_FINAL_STAGE_TARGETS} CACHE STRING "")
  set(BOOTSTRAP_LLVM_BUILD_INSTRUMENTED IR CACHE STRING "")
  set(BOOTSTRAP_LLVM_ENABLE_RUNTIMES "compiler-rt" CACHE STRING "")
  set(BOOTSTRAP_LLVM_ENABLE_PROJECTS "clang;lld" CACHE STRING "")

else()
  if (LLVM_RELEASE_ENABLE_LTO)
    list(APPEND STAGE1_PROJECTS "lld")
  endif()
  # Any targets added here will be given the target name stage2-${target}, so
  # if you want to run them you can just use:
  # ninja -C $BUILDDIR stage2-${target}
  set(CLANG_BOOTSTRAP_TARGETS ${LLVM_RELEASE_FINAL_STAGE_TARGETS} CACHE STRING "")
endif()

if (LLVM_RELEASE_ENABLE_LTO)
  # Enable LTO for the runtimes.  We need to configure stage1 clang to default
  # to using lld as the linker because the stage1 toolchain will be used to
  # build and link the runtimes.
  # FIXME: We can't use LLVM_ENABLE_LTO=Thin here, because it causes the CMake
  # step for the libcxx build to fail.  CMAKE_INTERPROCEDURAL_OPTIMIZATION does
  # enable ThinLTO, though.
  set(RUNTIMES_CMAKE_ARGS "-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON -DLLVM_ENABLE_LLD=ON -DLLVM_ENABLE_FATLTO=ON" CACHE STRING "")
endif()

# Stage 1 Common Config
set(LLVM_ENABLE_RUNTIMES ${STAGE1_RUNTIMES} CACHE STRING "")
set(LLVM_ENABLE_PROJECTS ${STAGE1_PROJECTS} CACHE STRING "")
set(LIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY ON CACHE STRING "")

# stage2-instrumented and Final Stage Config:
# Options that need to be set in both the instrumented stage (if we are doing
# a pgo build) and the final stage.
set_instrument_and_final_stage_var(CMAKE_POSITION_INDEPENDENT_CODE "ON" STRING)
set_instrument_and_final_stage_var(LLVM_ENABLE_LTO "${LLVM_RELEASE_ENABLE_LTO}" STRING)
if (LLVM_RELEASE_ENABLE_LTO)
  set_instrument_and_final_stage_var(LLVM_ENABLE_LLD "ON" BOOL)
endif()
set_instrument_and_final_stage_var(LLVM_ENABLE_LIBCXX "ON" BOOL)
set_instrument_and_final_stage_var(LLVM_STATIC_LINK_CXX_STDLIB "ON" BOOL)
set(RELEASE_LINKER_FLAGS "-rtlib=compiler-rt --unwindlib=libunwind")
if(NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
  set(RELEASE_LINKER_FLAGS "${RELEASE_LINKER_FLAGS} -static-libgcc")
endif()

# Set flags for bolt
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  set(RELEASE_LINKER_FLAGS "${RELEASE_LINKER_FLAGS} -Wl,--emit-relocs,-znow")
endif()

set_instrument_and_final_stage_var(CMAKE_EXE_LINKER_FLAGS ${RELEASE_LINKER_FLAGS} STRING)
set_instrument_and_final_stage_var(CMAKE_SHARED_LINKER_FLAGS ${RELEASE_LINKER_FLAGS} STRING)
set_instrument_and_final_stage_var(CMAKE_MODULE_LINKER_FLAGS ${RELEASE_LINKER_FLAGS} STRING)

# Final Stage Config (stage2)
set_final_stage_var(LLVM_ENABLE_RUNTIMES "${LLVM_RELEASE_ENABLE_RUNTIMES}" STRING)
set_final_stage_var(LLVM_ENABLE_PROJECTS "${LLVM_RELEASE_ENABLE_PROJECTS}" STRING)
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  set_final_stage_var(CLANG_BOLT "INSTRUMENT" STRING)
endif()
set_final_stage_var(CPACK_GENERATOR "TXZ" STRING)
set_final_stage_var(CPACK_ARCHIVE_THREADS "0" STRING)

set_final_stage_var(LLVM_USE_STATIC_ZSTD "ON" BOOL)
if (LLVM_RELEASE_ENABLE_LTO)
  set_final_stage_var(LLVM_ENABLE_FATLTO "ON" BOOL)
  set_final_stage_var(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_CURRENT_LIST_DIR}/release_cpack_pre_build_strip_lto.cmake" STRING)
endif()
