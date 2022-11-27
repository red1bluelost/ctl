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

/// \brief Alias for platform specific unsigned size types. These are commonly
/// used when indexing containers and checking sizes.
using usize = std::size_t;
/// \brief Alias for platform specific signed size types. These are commonly
/// used when indexing containers and subtracting pointers.
using isize = std::ptrdiff_t;

/// \brief Aliases for fixed size unsigned 8 bit integral.
using u8  = std::uint8_t;
/// \brief Aliases for fixed size unsigned 16 bit integral.
using u16 = std::uint16_t;
/// \brief Aliases for fixed size unsigned 32 bit integral.
using u32 = std::uint32_t;
/// \brief Aliases for fixed size unsigned 64 bit integral.
using u64 = std::uint64_t;
// TODO: add optional support for u128

/// \brief Aliases for fixed size signed 8 bit twos-complement integral.
using i8  = std::int8_t;
/// \brief Aliases for fixed size signed 16 bit twos-complement integral.
using i16 = std::int16_t;
/// \brief Aliases for fixed size signed 32 bit twos-complement integral.
using i32 = std::int32_t;
/// \brief Aliases for fixed size signed 64 bit twos-complement integral.
using i64 = std::int64_t;
// TODO: add optional support for i128

/// \brief Aliases for fixed size 32-bit floating point type.
using f32  = float;
/// \brief Aliases for fixed size 64-bit floating point type.
using f64  = double;
/// \brief Aliases for fixed size 128-bit floating point type.
using f128 = long double;
// TODO: disable f128 if long double is not supported

} // namespace core

CTL_END_NAMESPACE

#endif // CTL_CORE_TYPES_HPP
