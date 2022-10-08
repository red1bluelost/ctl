//===- type_traits_test.hpp - Tests for type traits -------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/meta/type_traits.hpp'.
///
//===----------------------------------------------------------------------===//
#include "ctl/meta/type_traits.hpp"

#include <gtest/gtest.h>

namespace {
template<typename T> concept has_type = requires { typename T::type; };

TEST(type_traits_test, enable_same) {
  static_assert(has_type<ctl::enable_same<int, int>>);
  static_assert(has_type<ctl::enable_same<bool, bool, long long>>);
  static_assert(!has_type<ctl::enable_same<int, float>>);
  static_assert(!has_type<ctl::enable_same<unsigned, bool, int>>);
  ASSERT_EQ((ctl::enable_same_t<double, double, long>{2}), long{2});
  static_assert(std::is_same_v<ctl::enable_same_t<double, double>, void>);
}

TEST(type_traits_test, enable_not_same) {
  static_assert(!has_type<ctl::enable_not_same<int, int>>);
  static_assert(!has_type<ctl::enable_not_same<bool, bool, long long>>);
  static_assert(has_type<ctl::enable_not_same<int, float>>);
  static_assert(has_type<ctl::enable_not_same<unsigned, bool, int>>);
  ASSERT_EQ((ctl::enable_not_same_t<double, float, long>{2}), long{2});
  static_assert(std::is_same_v<ctl::enable_not_same_t<double, char>, void>);
}

TEST(type_traits_test, enable_same_decay) {
  static_assert(has_type<ctl::enable_same_decay<int, const int&>>);
  static_assert(has_type<ctl::enable_same_decay<bool&&, const bool, long>>);
  static_assert(!has_type<ctl::enable_same_decay<const int, float&>>);
  static_assert(!has_type<ctl::enable_same_decay<unsigned, const bool, int>>);
  ASSERT_EQ((ctl::enable_same_decay_t<int&&, const int, long>{2}), long{2});
  static_assert(std::is_same_v<ctl::enable_same_decay_t<int&&, int&&>, void>);
}

} // namespace