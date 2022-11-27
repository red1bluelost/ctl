//===- ctl/meta/null_traits.hpp - Traits relating to null -------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Meta programming utilities for null: either null pointers, empty objects, or
/// any other null representation.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_META_NULL_TRAITS_HPP
#define CTL_META_NULL_TRAITS_HPP

#include "ctl/config.h"
#include "ctl/meta/type_traits.hpp"

CTL_BEGIN_NAMESPACE

/// \brief Traits struct for extending a "nullable" type, allowing a uniform
/// interface for shared behavior.
///
/// User defined types can specialize this template as a customization point.
///
/// In order to be a valid \c null_traits specialization. The implementation
/// must provide the following:
///   1. An alias \c nullable_type that should match \c T with const and
///      volatile removed.
///   2. An alias \c element_type which should match the value type of the
///      pointer or optional like \c T.
///   3. A template alias \c rebind which changes the element type of the
///      nullable type.
///   4. A \c null static function which returns the "null" representation of
///      the type being specialized. The return type should be the type being
///      specialized.
///
/// \tparam T Type that should be a nullable type
/// \tparam Enable Optional type field that allows enabling on certain cases
/// such as type hierarchies where types are enabled if they are derived from a
/// base
template<typename T, typename Enable = void>
struct null_traits {};

/// \brief Removes \c const qualifier so users don't need to provide
/// specialization.
template<typename T>
struct null_traits<const T> : null_traits<T> {};
/// \brief Removes \c volatile qualifier so users don't need to provide
/// specialization.
template<typename T>
struct null_traits<volatile T> : null_traits<T> {};
/// \brief Removes \c const \c volatile qualifier so users don't need to provide
/// specialization.
template<typename T>
struct null_traits<const volatile T> : null_traits<T> {};

/// \breif Specialization of \c null_traits for any pointer type.
///
/// \tparam T Type being pointed to
template<typename T>
struct null_traits<T*> {
  using nullable_type = T*;
  using element_type  = T;

  template<typename U>
  using rebind = U*;

  static constexpr T* null() { return nullptr; }
};

/// \breif Specialization of \c null_traits for any type that has either \c
/// value_type or \c element_type as an alias and is default constructible.
///
/// - \c rebind defers to \c meta::rebind_adt
/// - \c null uses the default constructor
///
/// This catches many types that may not be intended such as \c std::vector. It
/// does serve its purpose in catching \c std::optional, \c std::unique_ptr, and
/// \c std::shared_ptr. It can catch user-defined types if they either of type
/// aliases.
///
/// \tparam T Nullable type with proper alias
template<std::default_initializable T>
requires requires { typename T::value_type; } ||
         requires { typename T::element_type; }
struct null_traits<T> {
 private:
  // Helpers for extracting the correct element_type
  template<typename U>
  struct get_value_type {};
  template<typename U>
  requires requires { typename U::value_type; }
  struct get_value_type<U> : std::type_identity<typename U::value_type> {};
  template<typename U>
  requires requires { typename U::element_type; }
  struct get_value_type<U> : std::type_identity<typename U::element_type> {};

 public:
  using nullable_type = T;
  using element_type  = typename get_value_type<T>::type;

  template<typename U>
  using rebind = ctl::meta::rebind_adt_t<T, U>;

  static constexpr nullable_type null() { return T{}; }
};

/// TODO: add unique_ptr, shared_ptr

CTL_END_NAMESPACE

#endif // CTL_META_NULL_TRAITS_HPP
