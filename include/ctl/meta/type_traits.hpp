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

#include "ctl/config.h"

#include <type_traits>

CTL_BEGIN_NAMESPACE

//===----------------------------------------------------------------------===//
// Meta functions for numerics.
//===----------------------------------------------------------------------===//

/// \brief True iff both types are arithmetic and they have the same signedness.
/// Both types must be unsigned or both must be signed.
///
/// \tparam T One of two types to compare signedness
/// \tparam U Second of two types to compare signedness
template<typename L, typename R>
struct is_signedness_same
    : std::conjunction<
          std::is_arithmetic<L>,
          std::is_arithmetic<R>,
          std::bool_constant<std::is_signed_v<L> == std::is_signed_v<R>>> {};

/// \brief Alias template for \c is_signedness_same.
template<typename L, typename R>
inline constexpr bool is_signedness_same_v = is_signedness_same<L, R>::value;

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

CTL_END_NAMESPACE

#endif // CTL_META_TYPE_TRAITS_HPP