//===- ctl/meta/type_traits.hpp - Metaprogamming type traits ----*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains useful metaprogramming type traits beyond the
/// regular standard library.
///
//===----------------------------------------------------------------------===//
#ifndef CTL_META_TYPE_TRAITS_HPP
#define CTL_META_TYPE_TRAITS_HPP

#include <type_traits>

namespace ctl {

//===----------------------------------------------------------------------===//
// Combinations for \c std::enable_if and type_trait predicates.
//===----------------------------------------------------------------------===//

/// \brief Combination of \c std::enable_if and \c std::is_same.
///
/// Example usage:
/// \code
/// template<typename T, ctl::enable_same_t<T, int, bool> = true>
/// int only_int_add(T l, T r) { return l + r; }
/// \endcode
///
/// \tparam T One of two types to compare if same
/// \tparam U Second of two types to compare if same
/// \tparam R Type returned if the compared types were the same
template<typename T, typename U, typename R = void>
struct enable_same : std::enable_if<std::is_same_v<T, U>, R> {};

/// \brief Alias template for \c enable_same.
template<typename T, typename U, typename R = void>
using enable_same_t = typename enable_same<T, U, R>::type;

/// \brief Negative of \c enable_same.
template<typename T, typename U, typename R = void>
struct enable_not_same : std::enable_if<!std::is_same_v<T, U>, R> {};

/// \brief Alias template for \c enable_not_same.
template<typename T, typename U, typename R = void>
using enable_not_same_t = typename enable_not_same<T, U, R>::type;

/// \brief Meta function that passes the decayed types of the input arguments to
/// \c enable_same. This would be used in situations where a template argument
/// could be passed by value or a reference type.
///
/// Example usage:
/// \code
/// template<typename T, ctl::enable_same_decay_t<T, int, bool> = true>
/// int only_int_add(T l, T r) { return l + r; }
/// \endcode
template<typename T, typename U, typename R = void>
struct enable_same_decay : enable_same<std::decay_t<T>, std::decay_t<U>, R> {};

/// \brief Alias template for \c enable_same_decay.
template<typename T, typename U, typename R = void>
using enable_same_decay_t = typename enable_same_decay<T, U, R>::type;

} // namespace ctl

#endif // CTL_META_TYPE_TRAITS_HPP