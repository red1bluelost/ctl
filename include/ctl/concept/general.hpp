//===- ctl/concept/general.hpp - Concepts for general use -------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Concepts with general use cases.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_CONCEPT_GENERAL_HPP
#define CTL_CONCEPT_GENERAL_HPP

#include "ctl/config.h"

CTL_BEGIN_NAMESPACE

/// \brief Checks for existence of type alias named \c type.
///
/// Useful to convert a SFINAE-based meta function to a boolean value.
///
/// Example usage:
/// \code
/// bool would_work = ctl::aliasing_type<std::enable_it<...>>;
/// \endcode
///
/// \tparam T Type to check for the alias
template<typename T> concept aliasing_type = requires { typename T::type; };

CTL_END_NAMESPACE

#endif // CTL_CONCEPT_GENERAL_HPP
