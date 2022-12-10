//===- ctl/meta/type_traits.hpp - Metaprogramming type traits ---*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Useful metaprogramming type traits beyond the regular standard library.
///
//===----------------------------------------------------------------------===//
#ifndef CTL_META_TYPE_TRAITS_HPP
#define CTL_META_TYPE_TRAITS_HPP

#include "ctl/config.h"

#include <concepts>
#include <limits>
#include <type_traits>

CTL_BEGIN_NAMESPACE

//===----------------------------------------------------------------------===//
// Meta functions for meta functions.
//===----------------------------------------------------------------------===//
namespace meta {

/// \brief True iff the predicate applied to each type is true for all of the
/// instantiations. Short circuits on the first value that is false.
///
/// Example usage:
/// \code
/// template<typename... MemberTypes>
/// requires ctl::meta::all_v<std::is_integral, MemberTypes...>
/// struct IntVisitor { ... };
/// \endcode
///
/// \tparam Pred The meta function predicate to apply
/// \tparam T Every type to be tested by the predicate
template<template<typename> class Pred, typename... T>
struct all : std::conjunction<Pred<T>...> {};

/// \brief Alias template for \c meta::all.
template<template<typename> class Pred, typename... T>
inline constexpr bool all_v = all<Pred, T...>::value;

/// \brief True iff the predicate applied to each type is true for none of the
/// instantiations. Short circuits on the first value that is true.
///
/// \tparam Pred The meta function predicate to apply
/// \tparam T Every type to be tested by the predicate
template<template<typename> class Pred, typename... T>
struct none : std::conjunction<std::negation<Pred<T>>...> {};

/// \brief Alias template for \c meta::none.
template<template<typename> class Pred, typename... T>
inline constexpr bool none_v = none<Pred, T...>::value;

/// \brief True iff the predicate applied to each type is true for at least of
/// the instantiations. Short circuits on the first value that is true.
///
/// \tparam Pred The meta function predicate to apply
/// \tparam T Every type to be tested by the predicate
template<template<typename> class Pred, typename... T>
struct any : std::disjunction<Pred<T>...> {};

/// \brief Alias template for \c meta::any.
template<template<typename> class Pred, typename... T>
inline constexpr bool any_v = any<Pred, T...>::value;

/// \brief True iff each value of the meta type is the same via equality
/// comparison.
///
/// For the case of zero or one types this is true. A quirk in the
/// implementation of one type case, the type provided does not need to have a
/// value member.
///
/// Example usage:
/// \code
/// template<typename... MemberTypes>
/// requires ctl::meta::same_v<std::is_signed<MemberTypes>...>
/// struct IntegralPromoter { ... };
/// \endcode
///
/// \tparam ... All meta types to compare for sameness
template<typename...>
struct same : std::true_type {};
template<typename F, typename... T>
struct same<F, T...>
    : std::conjunction<std::bool_constant<F::value == T::value>...> {};

/// \brief Alias template for \c meta::same.
template<typename... T>
inline constexpr bool same_v = same<T...>::value;

/// \brief Applies a given meta function if the bool condition is true and
/// inherits its type alias. Otherwise, the type passed in is used as the type
/// alias.
///
/// Useful when applying qualifiers to types based on a templated condition.
///
/// Example usage:
/// \code
/// template<typename T, bool make_const>
/// struct RefPair {
///   using value_type = ctl::meta::apply_if_t<make_const, std::add_const, T>;
///   value_type& first;
///   value_type& second;
/// };
/// \endcode
///
/// \warning Behavior might be unexpected if the application of MFunctor does
/// not have a type alias named \c type. For situation where this would be the
/// case, such as \c std::unique_ptr or other abstract data types, you likely
/// want to use \c meta::wrap_if.
///
/// \tparam condition Whether or not to apply the meta functor
/// \tparam MFunctor A type modifying meta function
/// \tparam T The type to pass to the meta functor or just assign to type
template<bool condition, template<typename> class MFunctor, typename T>
struct apply_if
    : std::conditional<condition, MFunctor<T>, std::type_identity<T>>::type {};

/// \brief Alias template for \c meta::apply_if.
template<bool condition, template<typename> class MFunctor, typename T>
using apply_if_t = typename apply_if<condition, MFunctor, T>::type;

/// \brief Instantiates the templated abstract data type if the condition is
/// true and assigns to the \c type alias. Otherwise, it assigns \c T directly
/// to \c type alias.
///
/// Useful when selecting a variable type based on a templated parameter.
///
/// Example usage:
/// \code
/// template<typename T, bool make_val_unique>
/// struct MaybeUnique {
///   ctl::meta::wrap_if_t<make_val_unique, std::unique_ptr, T> value;
/// };
/// \endcode
///
/// \warning Behavior might be unexpected if Wrapper is actually a meta function
/// that is only meant to supply a type alias. In cases where you want to use
/// meta functions like \c std::add_const, you probably want to use \c
/// meta::apply_if.
///
/// \tparam condition Whether or not to apply the wrapping abstract data type
/// \tparam Wrapper A templated abstract data type that is instantiated with a
/// single type
/// \tparam T The type to either pass through or instantiate the wrapper with
template<bool condition, template<typename> class Wrapper, typename T>
struct wrap_if : std::conditional<condition, Wrapper<T>, T> {};

/// \brief Alias template for \c meta::wrap_if.
template<bool condition, template<typename> class Wrapper, typename T>
using wrap_if_t = typename wrap_if<condition, Wrapper, T>::type;

namespace detail_replace_first {

/// \brief Default implementation of \c replace_first that does not contain a \c
/// type alias.
///
/// \tparam TemplateType Non template type or zero arguments template
/// \tparam ForReplace Type which would have replaced the first
template<typename TemplateType, typename ForReplace>
struct replace_first_impl {};

/// \brief Implements \c replace_first by providing a specialization for
/// template types with one or more template arguments.
///
/// \tparam FirstTArg Type which will be replaced
/// \tparam RestTArg Remaining types to leave unchanged
/// \tparam TemplateType Templated type which will have first argument replaced
/// \tparam ForReplace Type to be used in the replacement
template<
    typename FirstTArg,
    typename... RestTArg,
    template<typename, typename...>
    class TemplateType,
    typename ForReplace>
struct replace_first_impl<TemplateType<FirstTArg, RestTArg...>, ForReplace>
    : std::type_identity<TemplateType<ForReplace, RestTArg...>> {};

} // namespace detail_replace_first

/// \brief Replaces the first type argument of a templated type.
///
/// If \c TemplateType is not a template type with one or more arguments then \c
/// replace_first will not have a \c type alias.
///
/// \note This might not have the intended effects when using with STL types
/// like \c std::vector or \c std::unique_ptr. They have default template
/// arguments depending on that first argument which won't make sense if the
/// first is replaced. For this, use \c meta::rebind.
///
/// \tparam TemplateType Template type which will have its first template
/// argument replaced
/// \tparam ForReplace Type to replace as the first argument
template<typename TemplateType, typename ForReplace>
struct replace_first
    : detail_replace_first::replace_first_impl<TemplateType, ForReplace> {};

/// \brief Alias template for \c meta::replace_first.
template<typename TemplateType, typename ForReplace>
using replace_first_t = typename replace_first<TemplateType, ForReplace>::type;

namespace detail_rebind {

/// \brief Default implementation of \c rebind that does not contain a \c type
/// alias.
///
/// \tparam TemplateType Non template type
/// \tparam NewArgs Types which would have rebound
template<typename TemplateType, typename... NewArgs>
struct rebind_impl {};

/// \brief Implements \c rebind by providing a specialization for template types
/// with one or more template arguments.
///
/// \tparam OldArgs Original type arguments in the template
/// \tparam TemplateType Template type whose arguments will be rebound
/// \tparam NewArgs New types to bind with the template
template<
    typename... OldArgs,
    template<typename...>
    class TemplateType,
    typename... NewArgs>
struct rebind_impl<TemplateType<OldArgs...>, NewArgs...>
    : std::type_identity<TemplateType<NewArgs...>> {};

} // namespace detail_rebind

/// \brief Rebinds all type arguments for the passed \c TemplateType with given
/// new arguments.
///
/// If \c TemplateType is not a template type then \c rebind will not have a \c
/// type alias.
///
/// Useful for creating traits objects that use rebind and other templated
/// types.
///
/// Example usage:
/// \code
/// template<typename T>
/// struct SomeWrapper {
///   template<typename U>
///   using rebind = ctl::meta::rebind_t<SomeWrapper<T>, U>;
/// };
/// \endcode
///
/// \note For STL types like \c std::vector or \c std::unique_ptr which have
/// defaulted type arguments depending on the provide type, this \c rebind will
/// not take those into account. For example, rebinding \c std::pmr::vector<int>
/// with \c double will become \c std::vector<double> and not \c
/// std::pmr::vector<double>.
///
/// \tparam TemplateType Template type which will have arguments rebound
/// \tparam NewArgs Types for replacements in rebound
template<typename TemplateType, typename... NewArgs>
struct rebind : detail_rebind::rebind_impl<TemplateType, NewArgs...> {};

/// \brief Alias template for \c meta::rebind.
template<typename TemplateType, typename... NewArgs>
using rebind_t = typename rebind<TemplateType, NewArgs...>::type;

namespace detail_rebind_adt {

/// \brief Handles non-ADT types so there is no \c type alias.
template<typename TemplateType, typename NewValueType>
struct rebind_adt_impl {};

/// \brief When a type argument does not match the original value type, it is
/// preserved. This is also the catch-all case.
template<typename OldValueType, typename ArgType, typename NewValueType>
struct rebind_adt_arg : std::type_identity<ArgType> {};

/// \brief When a type argument does match the original value type, the new
/// value type is used.
template<
    typename OldValueType,
    std::same_as<OldValueType> ArgType,
    typename NewValueType>
struct rebind_adt_arg<OldValueType, ArgType, NewValueType>
    : std::type_identity<NewValueType> {};

/// \brief When a type argument has a rebind alias, then use the rebind while
/// recursing down its template arguments.
template<typename OldValueType, typename ArgType, typename NewValueType>
requires requires { typename ArgType::template rebind<NewValueType>; }
struct rebind_adt_arg<OldValueType, ArgType, NewValueType>
    : std::type_identity<typename ArgType::template rebind<
          typename rebind_adt_impl<ArgType, NewValueType>::type>> {};

/// \brief When a type argument does not have a rebind but the first template
/// argument matches the original value type, rebind with new value type and
/// recurse down its template arguments.
template<
    typename OldValueType,
    std::same_as<OldValueType> FirstInner,
    typename... RestInner,
    template<typename, typename...>
    class ArgType,
    typename NewValueType>
struct rebind_adt_arg<
    OldValueType,
    ArgType<FirstInner, RestInner...>,
    NewValueType>
    : std::type_identity<ArgType<
          NewValueType,
          typename rebind_adt_arg<OldValueType, RestInner, NewValueType>::
              type...>> {};

/// \brief Dispatches rebinding for a templated type.
///
/// \tparam OldValueType Original value type which is being rebound
/// \tparam RestOld Other template parameters that will be rebound as well
/// \tparam TemplateType Type being rebound with a new value type
/// \tparam NewValueType New value type that is used for rebinding
template<
    typename OldValueType,
    typename... RestOld,
    template<typename, typename...>
    class TemplateType,
    typename NewValueType>
struct rebind_adt_impl<TemplateType<OldValueType, RestOld...>, NewValueType>
    : std::type_identity<TemplateType<
          NewValueType,
          typename rebind_adt_arg<OldValueType, RestOld, NewValueType>::
              type...>> {};

} // namespace detail_rebind_adt

/// \brief Rebinds an abstract data type by recursively swapping the main type
/// parameter with the new provided one. This attempts to recurse down dependent
/// template parameters as well. For example, this preserves the same type of
/// allocator, deleter, or comparator but using the new value type provided.
///
/// If the \c TemplateType is not a templated type then \c type alias will not
/// exist.
///
/// \warning This only works for single type abstract data types like vector,
/// optional, and more. Multi-type like map do not work properly. Also,
/// non-straight forward usages may have bugs in the implementation. For the
/// time, it serves its purpose and those edge cases can be handled when
/// encountered.
///
/// \tparam TemplateType Type which will be rebound
/// \tparam NewValueType New value type to use when rebinding
template<typename TemplateType, typename NewValueType>
struct rebind_adt
    : detail_rebind_adt::rebind_adt_impl<TemplateType, NewValueType> {};

/// \brief Alias template for \c meta::rebind_adt.
template<typename TemplateType, typename NewValueType>
using rebind_adt_t = typename rebind_adt<TemplateType, NewValueType>::type;

} // namespace meta

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
/// requires std::conjunction_v<
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
/// All types must be unsigned or both must be signed.
///
/// If any type is not arithmetic then this is false.
///
/// \tparam T All types to compare signedness
template<typename... T>
struct is_signedness_same
    : std::conjunction<
          std::is_arithmetic<T>...,
          CTL::meta::same<std::is_signed<T>...>> {};

/// \brief Alias template for \c is_signedness_same.
template<typename... T>
inline constexpr bool is_signedness_same_v = is_signedness_same<T...>::value;

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
          CTL::meta::all<std::is_integral, T...>,
          CTL::meta::all<std::is_floating_point, T...>> {};

/// \brief Alias template for \c is_arithmetic_same.
template<typename... T>
inline constexpr bool is_arithmetic_same_v = is_arithmetic_same<T...>::value;

namespace detail_ilc {

/// \brief Helper for \c is_lossless_convertible that checks when the types are
/// both integral or both floating point.
///
/// If the signedness match then this is simply a check of the size being able
/// to fit the type.
///
/// If the From is unsigned and To is signed then it checks that the size fits
/// the extra bit.
template<typename From, typename To, bool = is_arithmetic_same_v<From, To>>
struct ilc_helper
    : std::disjunction<
          std::
              conjunction<is_signedness_same<From, To>, is_sizeof_le<From, To>>,
          std::conjunction<
              std::is_unsigned<From>,
              std::is_signed<To>,
              is_sizeof_lt<From, To>>> {};

/// \brief Helper for \c is_lossless_convertible that checks when the types are
/// one floating point and one integral.
///
/// The only case possible is an integral to float. The float must have enough
/// digits to fit the integral type.
template<typename From, typename To>
struct ilc_helper<From, To, false>
    : std::conjunction<
          std::is_integral<From>,
          std::is_floating_point<To>,
          std::bool_constant<
              (std::numeric_limits<From>::digits <=
               std::numeric_limits<To>::digits)>> {};

} // namespace detail_ilc

/// \brief True iff the From type can be converted to the To type without the
/// actual number value changing.
///
/// This is done through a heuristic that the destination is either bigger to
/// fit the value or the destination is the same type.
///
/// For the case where an integral is being converted to a floating point value.
/// It checks that all the digits of the integral can fit into the digits of the
/// float.
///
/// \tparam From The input type that would be converted
/// \tparam To The output type that is converted to
template<typename From, typename To>
struct is_lossless_convertible
    : std::conjunction<
          std::is_convertible<From, To>,
          detail_ilc::ilc_helper<From, To>> {};

/// \brief Alias template for \c is_lossless_convertible.
template<typename From, typename To>
inline constexpr auto is_lossless_convertible_v =
    is_lossless_convertible<From, To>::value;

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

//===----------------------------------------------------------------------===//
// Meta functions to modify the qualifiers on a type.
//===----------------------------------------------------------------------===//

/// \brief Matches the const qualifier of the first type by modifying the second
/// type.
///
/// This could be useful inside of a deducing-this methods, templates, and other
/// situations.
///
/// \tparam Matching Type being referenced for const qualification
/// \tparam Changing Type to change const qualification
template<typename Matching, typename Changing>
struct match_const
    : std::conditional_t<
          std::is_const_v<Matching>,
          std::add_const<Changing>,
          std::remove_const<Changing>> {};

/// \brief Alias template for \c match_const.
template<typename Matching, typename Changing>
using match_const_t = typename match_const<Matching, Changing>::type;

/// \brief Matches the volatile qualifier of the first type by modifying the
/// second type.
///
/// This could be useful inside of a deducing-this methods, templates, and other
/// situations.
///
/// \tparam Matching Type being referenced for volatile qualification
/// \tparam Changing Type to change volatile qualification
template<typename Matching, typename Changing>
struct match_volatile
    : std::conditional_t<
          std::is_volatile_v<Matching>,
          std::add_volatile<Changing>,
          std::remove_volatile<Changing>> {};

/// \brief Alias template for \c match_volatile.
template<typename Matching, typename Changing>
using match_volatile_t = typename match_volatile<Matching, Changing>::type;

/// \brief Matches the const and volatile qualifier of the first type by
/// modifying the second type.
///
/// This could be useful inside of a deducing-this methods, templates, and other
/// situations.
///
/// \tparam Matching Type being referenced for const volatile qualification
/// \tparam Changing Type to change const volatile qualification
template<typename Matching, typename Changing>
struct match_cv
    : match_const<Matching, match_volatile_t<Matching, Changing>> {};

/// \brief Alias template for \c match_cv.
template<typename Matching, typename Changing>
using match_cv_t = typename match_cv<Matching, Changing>::type;

/// \brief Matches the signedness of integral types by changing the second type
/// to be the same as the first.
///
/// This could be useful inside of templates.
///
/// \warning Program behavior is undefined if either type is not an integral
/// type.
///
/// \tparam Matching Type being referenced for signedness
/// \tparam Changing Type to change integral sign
template<typename Matching, typename Changing>
struct match_sign
    : std::conditional_t<
          std::is_signed_v<Matching>,
          std::make_signed<Changing>,
          std::make_unsigned<Changing>> {};

/// \brief Alias template for \c match_sign.
template<typename Matching, typename Changing>
using match_sign_t = typename match_sign<Matching, Changing>::type;

CTL_END_NAMESPACE

#endif // CTL_META_TYPE_TRAITS_HPP