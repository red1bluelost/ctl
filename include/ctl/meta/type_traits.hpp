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
// Meta functions for any types.
//===----------------------------------------------------------------------===//

/// \brief Compares sizes of types if left is less than right.
///
/// Convenient for when a meta struct is needed to avoid using \c
/// std::bool_constant directly.
///
/// Example usage:
/// \code
/// template<typename L, typename R>
/// requires std::conjunction<
///     ctl::is_sizeof_lt<L, R>,
///     std::negation<std::is_same<L, R>>
/// >
/// void multiply_convert(L l, R r);
/// \endcode
template<typename LHS, typename RHS>
struct is_sizeof_lt : std::bool_constant<(sizeof(LHS) < sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_lt.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_lt_v = is_sizeof_lt<LHS, RHS>::value;

/// \brief Compares sizes of types if left is less than or equal to right.
///
/// Same reasons for usage as \c is_sizeof_lt.
template<typename LHS, typename RHS>
struct is_sizeof_le : std::bool_constant<(sizeof(LHS) <= sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_le.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_le_v = is_sizeof_le<LHS, RHS>::value;

/// \brief Compares sizes of types if left is greater than right.
///
/// Same reasons for usage as \c is_sizeof_lt.
template<typename LHS, typename RHS>
struct is_sizeof_gt : std::bool_constant<(sizeof(LHS) > sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_gt.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_gt_v = is_sizeof_gt<LHS, RHS>::value;

/// \brief Compares sizes of types if left is greater than or equal to right.
///
/// Same reasons for usage as \c is_sizeof_lt.
template<typename LHS, typename RHS>
struct is_sizeof_ge : std::bool_constant<(sizeof(LHS) >= sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_ge.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_ge_v = is_sizeof_ge<LHS, RHS>::value;

/// \brief Compares sizes of types if left is equal to right.
///
/// Same reasons for usage as \c is_sizeof_lt.
template<typename LHS, typename RHS>
struct is_sizeof_eq : std::bool_constant<(sizeof(LHS) == sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_eq.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_eq_v = is_sizeof_eq<LHS, RHS>::value;

/// \brief Compares sizes of types if left is not equal to right.
///
/// Same reasons for usage as \c is_sizeof_lt.
template<typename LHS, typename RHS>
struct is_sizeof_ne : std::bool_constant<(sizeof(LHS) != sizeof(RHS))> {};

/// \brief Alias template for \c is_sizeof_ne.
template<typename LHS, typename RHS>
inline constexpr bool is_sizeof_ne_v = is_sizeof_ne<LHS, RHS>::value;

//===----------------------------------------------------------------------===//
// Meta functions for numerics.
//===----------------------------------------------------------------------===//

/// \brief True iff all types are arithmetic and they have the same signedness.
/// Both types must be unsigned or both must be signed.
///
/// If any type is not arithmetic then this is false.
///
/// \tparam F First type to compare signedness
/// \tparam T Optional other types to compare signedness
template<typename F, typename... T>
struct is_signedness_same
    : std::conjunction<
          std::is_arithmetic<F>,
          std::is_arithmetic<T>...,
          std::bool_constant<std::is_signed_v<F> == std::is_signed_v<T>>...> {};

/// \brief Alias template for \c is_signedness_same.
template<typename F, typename... T>
inline constexpr bool is_signedness_same_v = is_signedness_same<F, T...>::value;

/// \brief True iff all types are the same arithmetic type class. When true, the
/// types are either all floating point or all integral types.
///
/// If any type is not arithmetic then this is false. If no types are provided
/// then this is true.
///
/// \tparam T Types to compare if all are same arithmetic type
template<typename... T>
struct is_arithmetic_same
    : std::disjunction<
          std::conjunction<std::is_integral<T>...>,
          std::conjunction<std::is_floating_point<T>...>> {};

/// \brief Alias template for \c is_arithmetic_same.
template<typename... T>
inline constexpr bool is_arithmetic_same_v = is_arithmetic_same<T...>::value;

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