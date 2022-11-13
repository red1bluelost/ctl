//===- ctl/core/types.hpp - Basic types aliases used in CTL -----*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Provides core type aliases used in the CTL. These keep each type to a single
/// identifier which can make macro uses easier. The aliases also serve to
/// encourage the use of fixed size basic types rather than platform specific.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_CORE_TYPES_HPP
#define CTL_CORE_TYPES_HPP

#include "ctl/config.h"

#include <cinttypes>
#include <cstddef>

CTL_BEGIN_NAMESPACE

inline namespace core {

/// \brief Aliases for platform specific size types. These are commonly used
/// when indexing containers and checking sizes.
using usize = std::size_t;
using isize = std::ptrdiff_t;

/// \brief Aliases for fixed size unsigned integral types.
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
// TODO: add optional support for u128

/// \brief Aliases for fixed size signed integral types.
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
// TODO: add optional support for i128

/// \brief Aliases for fixed size floating point types.
using f32  = float;
using f64  = double;
using f128 = long double;
// TODO: disable f128 if long double is not supported

} // namespace core

CTL_END_NAMESPACE

#endif // CTL_CORE_TYPES_HPP
