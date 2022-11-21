//===- ctl/concept/adt.hpp - Concepts for abstract data types ---*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Concepts relating to abstract data types like containers, wrappers, CRTP,
/// and mixins.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_CONCEPT_ADT_HPP
#define CTL_CONCEPT_ADT_HPP

#include "ctl/config.h"

CTL_BEGIN_NAMESPACE

template<typename T>
concept aliasing_value_type = requires { typename T::value_type; };

CTL_END_NAMESPACE

#endif // CTL_CONCEPT_ADT_HPP
