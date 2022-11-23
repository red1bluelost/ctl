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
/// This file only adds specifics for built-in pointers and members of \c std.
/// Any third-party types being supported are implemented in \c
/// null_traits_extended.hpp to avoid pulling in extra headers.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_META_NULL_TRAITS_HPP
#define CTL_META_NULL_TRAITS_HPP

#include "ctl/config.h"

#include <optional>

CTL_BEGIN_NAMESPACE

/// \brief Traits struct for extending a "nullable" type, allowing a uniform
/// interface for shared behavior.
///
/// User defined types can specialize this template as a customization point.
///
/// In order to be a valid \c null_traits specialization. The implementation
/// must provide the following:
///   1. A \c null static function which returns the "null" representation of
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
  static constexpr T* null() { return nullptr; }
};

/// \breif Specialization of \c null_traits for any \c std::optional
/// instantiation.
///
/// \tparam T The \c value_type of the \c std::optional
template<typename T>
struct null_traits<std::optional<T>> {
  static constexpr std::optional<T> null() { return std::nullopt; }
};

/// TODO: add unique_ptr, shared_ptr
/// TODO: add element_type, nullable_type

CTL_END_NAMESPACE

#endif // CTL_META_NULL_TRAITS_HPP
