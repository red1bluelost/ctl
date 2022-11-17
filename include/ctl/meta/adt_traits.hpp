//===- ctl/meta/adt_traits.hpp - ADT related type traits --------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Useful type traits relating to abstract data types like containers and other
/// wrappers.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_META_ADT_TRAITS_HPP
#define CTL_META_ADT_TRAITS_HPP

#include "ctl/config.h"
#include "ctl/meta/type_traits.hpp"

CTL_BEGIN_NAMESPACE

/// \brief Checks for the existence of a type alias named \c value_type.
///
/// Useful for checking the conformance of abstract data types.
///
/// Example usage:
/// \code
/// bool provides_alias = ctl::has_value_type_v<some_custom_container>;
/// \endcode
CTL_GENERATE_TYPE_ALIAS_CHECK(value_type);

CTL_END_NAMESPACE

#endif // CTL_META_ADT_TRAITS_HPP
