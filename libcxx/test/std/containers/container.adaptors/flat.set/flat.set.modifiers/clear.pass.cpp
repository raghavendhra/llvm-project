//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// class flat_set

// void clear() noexcept;

#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>
#include <vector>

#include "MinSequenceContainer.h"
#include "../helpers.h"
#include "test_macros.h"
#include "min_allocator.h"

// test noexcept

template <class T>
concept NoExceptClear = requires(T t) {
  { t.clear() } noexcept;
};

static_assert(NoExceptClear<std::flat_set<int>>);
#ifndef TEST_HAS_NO_EXCEPTIONS
static_assert(NoExceptClear<std::flat_set<int, std::less<int>, ThrowOnMoveContainer<int>>>);
#endif

template <class KeyContainer>
constexpr void test_one() {
  using Key = typename KeyContainer::value_type;
  using M   = std::flat_set<Key, std::less<Key>, KeyContainer>;
  {
    M m = {1, 2, 3, 4, 5};
    assert(m.size() == 5);
    ASSERT_NOEXCEPT(m.clear());
    ASSERT_SAME_TYPE(decltype(m.clear()), void);
    m.clear();
    assert(m.size() == 0);
  }
  {
    // was empty
    M m;
    assert(m.size() == 0);
    m.clear();
    assert(m.size() == 0);
  }
}

constexpr bool test() {
  test_one<std::vector<int>>();
#ifndef __cpp_lib_constexpr_deque
  if (!TEST_IS_CONSTANT_EVALUATED)
#endif
    test_one<std::deque<int>>();
  test_one<MinSequenceContainer<int>>();
  test_one<std::vector<int, min_allocator<int>>>();
  test_one<std::vector<int, min_allocator<int>>>();

  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER >= 26
  static_assert(test());
#endif

  return 0;
}
