//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TUPLE_MAKE_TUPLE_TYPES_H
#define _LIBCPP___TUPLE_MAKE_TUPLE_TYPES_H

#include <__config>
#include <__cstddef/size_t.h>
#include <__fwd/array.h>
#include <__fwd/tuple.h>
#include <__tuple/tuple_element.h>
#include <__tuple/tuple_size.h>
#include <__tuple/tuple_types.h>
#include <__type_traits/copy_cvref.h>
#include <__type_traits/remove_cvref.h>
#include <__type_traits/remove_reference.h>
#include <__utility/integer_sequence.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

#ifndef _LIBCPP_CXX03_LANG

_LIBCPP_BEGIN_NAMESPACE_STD

// __make_tuple_types<_Tuple<_Types...>, _Ep, _Sp>::type is a
// __tuple_types<_Types...> using only those _Types in the range [_Sp, _Ep).
// _Sp defaults to 0 and _Ep defaults to tuple_size<_Tuple>.  If _Tuple is a
// lvalue_reference type, then __tuple_types<_Types&...> is the result.

template <class _TupleTypes, class _TupleIndices>
struct __make_tuple_types_flat;

template <template <class...> class _Tuple, class... _Types, size_t... _Idx>
struct __make_tuple_types_flat<_Tuple<_Types...>, __index_sequence<_Idx...>> {
  // Specialization for pair, tuple, and __tuple_types
  template <class _Tp>
  using __apply_quals _LIBCPP_NODEBUG = __tuple_types<__copy_cvref_t<_Tp, __type_pack_element<_Idx, _Types...>>...>;
};

template <class _Vt, size_t _Np, size_t... _Idx>
struct __make_tuple_types_flat<array<_Vt, _Np>, __index_sequence<_Idx...>> {
  template <size_t>
  using __value_type _LIBCPP_NODEBUG = _Vt;
  template <class _Tp>
  using __apply_quals _LIBCPP_NODEBUG = __tuple_types<__copy_cvref_t<_Tp, __value_type<_Idx>>...>;
};

template <class _Tp>
struct __make_tuple_types {
  using _RawTp _LIBCPP_NODEBUG = __remove_cvref_t<_Tp>;
  using _Maker _LIBCPP_NODEBUG =
      __make_tuple_types_flat<_RawTp, __make_index_sequence<tuple_size<__libcpp_remove_reference_t<_Tp>>::value>>;
  using type _LIBCPP_NODEBUG = typename _Maker::template __apply_quals<_Tp>;
};

template <class... _Types>
struct __make_tuple_types<tuple<_Types...>> {
  using type _LIBCPP_NODEBUG = __tuple_types<_Types...>;
};

template <class... _Types>
struct __make_tuple_types<__tuple_types<_Types...>> {
  using type _LIBCPP_NODEBUG = __tuple_types<_Types...>;
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_CXX03_LANG

#endif // _LIBCPP___TUPLE_MAKE_TUPLE_TYPES_H
