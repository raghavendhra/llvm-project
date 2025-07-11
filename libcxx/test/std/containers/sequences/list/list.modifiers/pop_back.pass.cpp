//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// void pop_back(); // constexpr since C++26

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

TEST_CONSTEXPR_CXX26 bool test() {
  {
    int a[] = {1, 2, 3};
    std::list<int> c(a, a + 3);
    c.pop_back();
    assert(c == std::list<int>(a, a + 2));
    c.pop_back();
    assert(c == std::list<int>(a, a + 1));
    c.pop_back();
    assert(c.empty());
  }
#if TEST_STD_VER >= 11
  {
    int a[] = {1, 2, 3};
    std::list<int, min_allocator<int>> c(a, a + 3);
    c.pop_back();
    assert((c == std::list<int, min_allocator<int>>(a, a + 2)));
    c.pop_back();
    assert((c == std::list<int, min_allocator<int>>(a, a + 1)));
    c.pop_back();
    assert(c.empty());
  }
#endif

  return true;
}

int main(int, char**) {
  assert(test());
#if TEST_STD_VER >= 26
  static_assert(test());
#endif

  return 0;
}
