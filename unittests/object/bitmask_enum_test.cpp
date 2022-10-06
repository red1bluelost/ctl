//===- bitmask_enum_test.hpp - Tests for bitmask enum define ----*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/object/bitmask_enum.def'.
///
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

#include <type_traits>

namespace {

enum class test_flag {
  none   = 0,
  first  = (1 << 0),
  second = (1 << 1),
  third  = (1 << 2),
  fourth = (1 << 3),
  fifth  = (1 << 4),
  sixth  = (1 << 5),
  all    = (1 << 6) - 1,
};

using tf_type = std::underlying_type_t<test_flag>;
constexpr auto to_underlying(test_flag tf) -> tf_type {
  return static_cast<tf_type>(tf);
}
constexpr auto to_tf(tf_type tf) -> test_flag {
  return static_cast<test_flag>(tf);
}

TEST(bitmask_enum_test, enum_sanity_check) {
  static_assert(to_underlying(test_flag::none) == 0b000000);
  static_assert(to_underlying(test_flag::first) == 0b000001);
  static_assert(to_underlying(test_flag::second) == 0b000010);
  static_assert(to_underlying(test_flag::third) == 0b000100);
  static_assert(to_underlying(test_flag::fourth) == 0b001000);
  static_assert(to_underlying(test_flag::fifth) == 0b010000);
  static_assert(to_underlying(test_flag::sixth) == 0b100000);
  static_assert(to_underlying(test_flag::all) == 0b111111);

  ASSERT_EQ(to_underlying(test_flag::none), 0b000000);
  ASSERT_EQ(to_underlying(test_flag::first), 0b000001);
  ASSERT_EQ(to_underlying(test_flag::second), 0b000010);
  ASSERT_EQ(to_underlying(test_flag::third), 0b000100);
  ASSERT_EQ(to_underlying(test_flag::fourth), 0b001000);
  ASSERT_EQ(to_underlying(test_flag::fifth), 0b010000);
  ASSERT_EQ(to_underlying(test_flag::sixth), 0b100000);
  ASSERT_EQ(to_underlying(test_flag::all), 0b111111);

  static_assert(test_flag::none == to_tf(0b000000));
  static_assert(test_flag::first == to_tf(0b000001));
  static_assert(test_flag::second == to_tf(0b000010));
  static_assert(test_flag::third == to_tf(0b000100));
  static_assert(test_flag::fourth == to_tf(0b001000));
  static_assert(test_flag::fifth == to_tf(0b010000));
  static_assert(test_flag::sixth == to_tf(0b100000));
  static_assert(test_flag::all == to_tf(0b111111));

  ASSERT_EQ(test_flag::none, to_tf(0b000000));
  ASSERT_EQ(test_flag::first, to_tf(0b000001));
  ASSERT_EQ(test_flag::second, to_tf(0b000010));
  ASSERT_EQ(test_flag::third, to_tf(0b000100));
  ASSERT_EQ(test_flag::fourth, to_tf(0b001000));
  ASSERT_EQ(test_flag::fifth, to_tf(0b010000));
  ASSERT_EQ(test_flag::sixth, to_tf(0b100000));
  ASSERT_EQ(test_flag::all, to_tf(0b111111));
}

#define CTL_BITMASK_ENUM test_flag
#include "ctl/object/bitmask_enum.def"

TEST(bitmask_enum_test, operator_and) {
  static_assert((to_tf(0b010010) & to_tf(0b000111)) == test_flag::second);
  ASSERT_EQ(to_tf(0b010010) & to_tf(0b000111), test_flag::second);

  static_assert((test_flag::all & test_flag::none) == test_flag::none);
  ASSERT_EQ(test_flag::all & test_flag::none, test_flag::none);

  static_assert((test_flag::all & test_flag::fourth) == test_flag::fourth);
  ASSERT_EQ(test_flag::all & test_flag::fourth, test_flag::fourth);

  static_assert((to_tf(0b010010) & test_flag::third) == test_flag::none);
  ASSERT_EQ(to_tf(0b010010) & test_flag::third, test_flag::none);

  test_flag        l   = test_flag::first;
  test_flag        r   = to_tf(0b000111);
  const test_flag& cr  = r;
  const test_flag  ans = test_flag::first;
  ASSERT_EQ(l & cr, ans);
  ASSERT_EQ(cr & l, ans);
}

TEST(bitmask_enum_test, operator_or) {
  static_assert((to_tf(0b010010) | to_tf(0b000111)) == to_tf(0b010111));
  ASSERT_EQ(to_tf(0b010010) | to_tf(0b000111), to_tf(0b010111));

  static_assert((test_flag::all | test_flag::none) == test_flag::all);
  ASSERT_EQ(test_flag::all | test_flag::none, test_flag::all);

  static_assert((test_flag::sixth | test_flag::fourth) == to_tf(0b101000));
  ASSERT_EQ(test_flag::sixth | test_flag::fourth, to_tf(0b101000));

  static_assert((to_tf(0b010010) | test_flag::third) == to_tf(0b010110));
  ASSERT_EQ(to_tf(0b010010) | test_flag::third, to_tf(0b010110));

  test_flag        l   = to_tf(0b000111);
  test_flag        r   = test_flag::fifth;
  const test_flag& cr  = r;
  const test_flag  ans = to_tf(0b010111);
  ASSERT_EQ(l | cr, ans);
  ASSERT_EQ(cr | l, ans);
}

TEST(bitmask_enum_test, operator_xor) {
  static_assert((to_tf(0b010010) ^ to_tf(0b000111)) == to_tf(0b010101));
  ASSERT_EQ(to_tf(0b010010) ^ to_tf(0b000111), to_tf(0b010101));

  static_assert((test_flag::all ^ test_flag::first) == to_tf(0b111110));
  ASSERT_EQ(test_flag::all ^ test_flag::first, to_tf(0b111110));

  static_assert((test_flag::all ^ test_flag::all) == test_flag::none); // NOLINT
  ASSERT_EQ(test_flag::all ^ test_flag::all, test_flag::none);

  test_flag        l   = to_tf(0b000111);
  test_flag        r   = test_flag::third;
  const test_flag& cr  = r;
  const test_flag  ans = to_tf(0b000011);
  ASSERT_EQ(l ^ cr, ans);
  ASSERT_EQ(cr ^ l, ans);
}

TEST(bitmask_enum_test, operator_assign_and) {
  {
    test_flag tf = to_tf(0b010010);
    tf &= to_tf(0b000111);
    ASSERT_EQ(tf, test_flag::second);
  }
  {
    test_flag tf = test_flag::all;
    test_flag a  = test_flag::none;
    tf &= a;
    ASSERT_EQ(tf, test_flag::none);
  }
  {
    test_flag tf = test_flag::all;
    tf &= test_flag::fourth;
    ASSERT_EQ(tf, test_flag::fourth);
  }
}

TEST(bitmask_enum_test, operator_assign_or) {
  {
    test_flag tf = to_tf(0b010010);
    tf |= to_tf(0b000111);
    ASSERT_EQ(tf, to_tf(0b010111));
  }
  {
    test_flag tf = test_flag::all;
    tf |= test_flag::none;
    ASSERT_EQ(tf, test_flag::all);
  }
  {
    test_flag tf = test_flag::sixth;
    test_flag a  = test_flag::fourth;
    tf |= a;
    ASSERT_EQ(tf, to_tf(0b101000));
  }
}

TEST(bitmask_enum_test, operator_assign_xor) {
  {
    test_flag tf = to_tf(0b010010);
    tf ^= to_tf(0b000111);
    ASSERT_EQ(tf, to_tf(0b010101));
  }
  {
    test_flag tf = test_flag::all;
    tf ^= test_flag::first;
    ASSERT_EQ(tf, to_tf(0b111110));
  }
  {
    test_flag tf = test_flag::all;
    test_flag a  = test_flag::all;
    tf ^= a;
    ASSERT_EQ(tf, test_flag::none);
  }
}

TEST(bitmask_enum_test, operator_bitwise_not) {
  static_assert(~test_flag::none == to_tf(-1));
  ASSERT_EQ(~test_flag::none, to_tf(-1));

  static_assert((to_tf(0b011011) & ~test_flag::sixth) != test_flag::none);
  ASSERT_NE(to_tf(0b011011) & ~test_flag::sixth, test_flag::none);
}

} // namespace