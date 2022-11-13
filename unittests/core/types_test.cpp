//===- types_test.cpp - Tests for the core CTL types ------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/core/types.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/core/types.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace {

TEST(types_test, usize) {
  static_assert(std::is_same_v<ctl::usize, ctl::core::usize>);
  static_assert(sizeof(ctl::usize) == sizeof(std::size_t));
  static_assert(std::is_integral_v<ctl::usize>);
  static_assert(std::is_unsigned_v<ctl::usize>);
}
TEST(types_test, isize) {
  static_assert(std::is_same_v<ctl::isize, ctl::core::isize>);
  static_assert(sizeof(ctl::isize) == sizeof(std::ptrdiff_t));
  static_assert(std::is_integral_v<ctl::isize>);
  static_assert(std::is_signed_v<ctl::isize>);
}

TEST(types_test, u8) {
  static_assert(std::is_same_v<ctl::u8, ctl::core::u8>);
  static_assert(sizeof(ctl::u8) == 1);
  static_assert(std::is_integral_v<ctl::u8>);
  static_assert(std::is_unsigned_v<ctl::u8>);
}
TEST(types_test, u16) {
  static_assert(std::is_same_v<ctl::u16, ctl::core::u16>);
  static_assert(sizeof(ctl::u16) == 2);
  static_assert(std::is_integral_v<ctl::u16>);
  static_assert(std::is_unsigned_v<ctl::u16>);
}
TEST(types_test, u32) {
  static_assert(std::is_same_v<ctl::u32, ctl::core::u32>);
  static_assert(sizeof(ctl::u32) == 4);
  static_assert(std::is_integral_v<ctl::u32>);
  static_assert(std::is_unsigned_v<ctl::u32>);
}
TEST(types_test, u64) {
  static_assert(std::is_same_v<ctl::u64, ctl::core::u64>);
  static_assert(sizeof(ctl::u64) == 8);
  static_assert(std::is_integral_v<ctl::u64>);
  static_assert(std::is_unsigned_v<ctl::u64>);
}

TEST(types_test, i8) {
  static_assert(std::is_same_v<ctl::i8, ctl::core::i8>);
  static_assert(sizeof(ctl::i8) == 1);
  static_assert(std::is_integral_v<ctl::i8>);
  static_assert(std::is_signed_v<ctl::i8>);
}
TEST(types_test, i16) {
  static_assert(std::is_same_v<ctl::i16, ctl::core::i16>);
  static_assert(sizeof(ctl::i16) == 2);
  static_assert(std::is_integral_v<ctl::i16>);
  static_assert(std::is_signed_v<ctl::i16>);
}
TEST(types_test, i32) {
  static_assert(std::is_same_v<ctl::i32, ctl::core::i32>);
  static_assert(sizeof(ctl::i32) == 4);
  static_assert(std::is_integral_v<ctl::i32>);
  static_assert(std::is_signed_v<ctl::i32>);
}
TEST(types_test, i64) {
  static_assert(std::is_same_v<ctl::i64, ctl::core::i64>);
  static_assert(sizeof(ctl::i64) == 8);
  static_assert(std::is_integral_v<ctl::i64>);
  static_assert(std::is_signed_v<ctl::i64>);
}

TEST(types_test, f32) {
  static_assert(std::is_same_v<ctl::f32, ctl::core::f32>);
  static_assert(sizeof(ctl::f32) == 4);
  static_assert(std::is_floating_point_v<ctl::f32>);
}
TEST(types_test, f64) {
  static_assert(std::is_same_v<ctl::f64, ctl::core::f64>);
  static_assert(sizeof(ctl::f64) == 8);
  static_assert(std::is_floating_point_v<ctl::f64>);
}
TEST(types_test, f128) {
  static_assert(std::is_same_v<ctl::f128, ctl::core::f128>);
  static_assert(sizeof(ctl::f128) == 16);
  static_assert(std::is_floating_point_v<ctl::f128>);
}

} // namespace
