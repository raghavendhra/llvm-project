//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>

// Test default construction:
//
// constexpr mapping() noexcept;
//
//
// Preconditions: layout_right::mapping<extents_type>().required_span_size() is representable as a value of type index_type ([basic.fundamental]).
//
// Effects: Direct-non-list-initializes extents_ with extents_type(), and for all d in the range [0, rank_),
//          direct-non-list-initializes strides_[d] with layout_right::mapping<extents_type>().stride(d).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <mdspan>

#include "test_macros.h"

template <class E>
constexpr void test_construction() {
  using M = std::layout_stride::mapping<E>;
  ASSERT_NOEXCEPT(M{});
  M m;
  E e;

  // check correct extents are returned
  ASSERT_NOEXCEPT(m.extents());
  assert(m.extents() == e);

  // check required_span_size()
  typename E::index_type expected_size = 1;
  for (typename E::rank_type r = 0; r < E::rank(); r++)
    expected_size *= e.extent(r);
  assert(m.required_span_size() == expected_size);

  // check strides: node stride function is constrained on rank>0, e.extent(r) is not
  auto strides = m.strides();
  ASSERT_NOEXCEPT(m.strides());
  if constexpr (E::rank() > 0) {
    std::layout_right::mapping<E> m_right;
    for (typename E::rank_type r = 0; r < E::rank(); r++) {
      assert(m.stride(r) == m_right.stride(r));
      assert(strides[r] == m.stride(r));
    }
  }
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_construction<std::extents<int>>();
  test_construction<std::extents<unsigned, D>>();
  test_construction<std::extents<unsigned, 7>>();
  test_construction<std::extents<unsigned, 0>>();
  test_construction<std::extents<unsigned, 7, 8>>();
  test_construction<std::extents<int64_t, D, 8, D, D>>();
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());
  return 0;
}
