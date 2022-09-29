//===- ctl/meta/type_traits.hpp - Metaprogamming type traits ----*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// This file contains useful metaprogramming type traits beyond the regular
/// standard library.
///
//===----------------------------------------------------------------------===//
#ifndef CTL_META_TYPE_TRAITS_HPP
#define CTL_META_TYPE_TRAITS_HPP

#include <type_traits>

namespace ctl {

template<typename Unknown, typename Type>
struct enable_same : std::enable_if<std::is_same_v<Unknown, Type>, Type> {};

template<typename Unknown, typename Type>
using enable_same_t = typename enable_same<Unknown, Type>::type;

} // namespace ctl

#endif // CTL_META_TYPE_TRAITS_HPP