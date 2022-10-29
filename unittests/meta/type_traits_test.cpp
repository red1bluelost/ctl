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
//===----------------------------------------------------------------------===//
// Utilities for these tests.
//===----------------------------------------------------------------------===//

template<typename T> concept has_type = requires { typename T::type; };

//===----------------------------------------------------------------------===//
// Tests for meta functions on any type.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, is_sizeof_lt) {
  static_assert(ctl::is_sizeof_lt<int32_t, int64_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_lt<int32_t, int64_t>::value));
  static_assert(ctl::is_sizeof_lt_v<char, std::string>);
  ASSERT_TRUE((ctl::is_sizeof_lt_v<char, std::string>));

  static_assert(!ctl::is_sizeof_lt<int16_t, int16_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_lt<int16_t, int16_t>::value));
  static_assert(!ctl::is_sizeof_lt_v<double, short>);
  ASSERT_TRUE((!ctl::is_sizeof_lt_v<double, short>));
}

TEST(type_traits_test, is_sizeof_le) {
  static_assert(ctl::is_sizeof_le<int32_t, int32_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_le<int32_t, int32_t>::value));
  static_assert(ctl::is_sizeof_le_v<char, std::string>);
  ASSERT_TRUE((ctl::is_sizeof_le_v<char, std::string>));

  static_assert(!ctl::is_sizeof_le<int64_t, int16_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_le<int64_t, int16_t>::value));
  static_assert(!ctl::is_sizeof_le_v<double, short>);
  ASSERT_TRUE((!ctl::is_sizeof_le_v<double, short>));
}

TEST(type_traits_test, is_sizeof_gt) {
  static_assert(ctl::is_sizeof_gt<int64_t, int32_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_gt<int64_t, int32_t>::value));
  static_assert(ctl::is_sizeof_gt_v<std::string, char>);
  ASSERT_TRUE((ctl::is_sizeof_gt_v<std::string, char>));

  static_assert(!ctl::is_sizeof_gt<int16_t, int16_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_gt<int16_t, int16_t>::value));
  static_assert(!ctl::is_sizeof_gt_v<short, double>);
  ASSERT_TRUE((!ctl::is_sizeof_gt_v<short, double>));
}

TEST(type_traits_test, is_sizeof_ge) {
  static_assert(ctl::is_sizeof_ge<int32_t, int32_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_ge<int32_t, int32_t>::value));
  static_assert(ctl::is_sizeof_ge_v<std::string, char>);
  ASSERT_TRUE((ctl::is_sizeof_ge_v<std::string, char>));

  static_assert(!ctl::is_sizeof_ge<int16_t, int64_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_ge<int16_t, int64_t>::value));
  static_assert(!ctl::is_sizeof_ge_v<short, double>);
  ASSERT_TRUE((!ctl::is_sizeof_ge_v<short, double>));
}

TEST(type_traits_test, is_sizeof_eq) {
  static_assert(ctl::is_sizeof_eq<int16_t, int16_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_eq<int16_t, int16_t>::value));
  static_assert(ctl::is_sizeof_eq_v<int32_t, float>);
  ASSERT_TRUE((ctl::is_sizeof_eq_v<int32_t, float>));

  static_assert(!ctl::is_sizeof_eq<int64_t, int32_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_eq<int64_t, int32_t>::value));
  static_assert(!ctl::is_sizeof_eq_v<short, double>);
  ASSERT_TRUE((!ctl::is_sizeof_eq_v<short, double>));
}

TEST(type_traits_test, is_sizeof_ne) {
  static_assert(ctl::is_sizeof_ne<int16_t, int64_t>::value);
  ASSERT_TRUE((ctl::is_sizeof_ne<int16_t, int64_t>::value));
  static_assert(ctl::is_sizeof_ne_v<std::string, char>);
  ASSERT_TRUE((ctl::is_sizeof_ne_v<std::string, char>));

  static_assert(!ctl::is_sizeof_ne<int32_t, int32_t>::value);
  ASSERT_TRUE((!ctl::is_sizeof_ne<int32_t, int32_t>::value));
  static_assert(!ctl::is_sizeof_ne_v<int64_t, double>);
  ASSERT_TRUE((!ctl::is_sizeof_ne_v<int64_t, double>));
}

//===----------------------------------------------------------------------===//
// Tests for numerics based meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, signedness_same) {
  static_assert(ctl::is_signedness_same<int, long, long long>::value);
  ASSERT_TRUE((ctl::is_signedness_same<int, long, long long>::value));
  static_assert(ctl::is_signedness_same<float, std::ptrdiff_t>::value);
  ASSERT_TRUE((ctl::is_signedness_same<float, std::ptrdiff_t>::value));
  static_assert(ctl::is_signedness_same_v<unsigned, unsigned char>);
  ASSERT_TRUE((ctl::is_signedness_same_v<unsigned, unsigned char>));
  static_assert(ctl::is_signedness_same_v<long long, long long>);
  ASSERT_TRUE((ctl::is_signedness_same_v<long long, long long>));

  static_assert(!ctl::is_signedness_same<int, unsigned>::value);
  ASSERT_FALSE((ctl::is_signedness_same<int, unsigned>::value));
  static_assert(!ctl::is_signedness_same<char, unsigned>::value);
  ASSERT_FALSE((ctl::is_signedness_same<char, unsigned>::value));
  static_assert(!ctl::is_signedness_same_v<double, std::size_t, int>);
  ASSERT_FALSE((ctl::is_signedness_same_v<double, std::size_t, int>));
  static_assert(!ctl::is_signedness_same_v<unsigned short, short>);
  ASSERT_FALSE((ctl::is_signedness_same_v<unsigned short, short>));

  struct Tester {};
  static_assert(!ctl::is_signedness_same<int, void>::value);
  ASSERT_FALSE((ctl::is_signedness_same<int, void>::value));
  static_assert(!ctl::is_signedness_same<Tester, unsigned>::value);
  ASSERT_FALSE((ctl::is_signedness_same<Tester, unsigned>::value));
  static_assert(!ctl::is_signedness_same_v<double, int[3]>);
  ASSERT_FALSE((ctl::is_signedness_same_v<double, int[3]>));
  static_assert(!ctl::is_signedness_same_v<float*, char, volatile void>);
  ASSERT_FALSE((ctl::is_signedness_same_v<float*, char, volatile void>));
  static_assert(!ctl::is_signedness_same_v<long&, const char**>);
  ASSERT_FALSE((ctl::is_signedness_same_v<long&, const char**>));
}

//===----------------------------------------------------------------------===//
// Tests for combinations with \c std::enable_if meta functions.
//===----------------------------------------------------------------------===//

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