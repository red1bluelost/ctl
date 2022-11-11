//===- ctl/object/bit.hpp - Tools relating to bits. -------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Utilities based on bits and the underlying representations of objects.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_OBJECT_BIT_HPP
#define CTL_OBJECT_BIT_HPP

#include "ctl/config.h"

#include <climits>

CTL_BEGIN_NAMESPACE

#define CTL_BIT_SIZEOF(...) (sizeof(__VA_ARGS__) * CHAR_BIT)

CTL_END_NAMESPACE

#endif // CTL_OBJECT_BIT_HPP
