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

/// \brief Asserts at both compile time and runtime. Useful for seeing the tests
/// when running GoogleTest but also prevents compilation is incorrect.
///
/// \param _expr_ The expression to assert on. Should be convertable to bool.
#define CAR_ASSERT(_expr_)                                                     \
 do {                                                                          \
  static_assert(_expr_);                                                       \
  ASSERT_TRUE(_expr_);                                                         \
 } while (false)

//===----------------------------------------------------------------------===//
// Tests for meta functions on any type.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, is_sizeof_lt) {
  CAR_ASSERT((ctl::is_sizeof_lt<int32_t, int64_t>::value));
  CAR_ASSERT((ctl::is_sizeof_lt_v<char, std::string>));

  CAR_ASSERT((!ctl::is_sizeof_lt<int16_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_lt_v<double, short>));
}

TEST(type_traits_test, is_sizeof_le) {
  CAR_ASSERT((ctl::is_sizeof_le<int32_t, int32_t>::value));
  CAR_ASSERT((ctl::is_sizeof_le_v<char, std::string>));

  CAR_ASSERT((!ctl::is_sizeof_le<int64_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_le_v<double, short>));
}

TEST(type_traits_test, is_sizeof_gt) {
  CAR_ASSERT((ctl::is_sizeof_gt<int64_t, int32_t>::value));
  CAR_ASSERT((ctl::is_sizeof_gt_v<std::string, char>));

  CAR_ASSERT((!ctl::is_sizeof_gt<int16_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_gt_v<short, double>));
}

TEST(type_traits_test, is_sizeof_ge) {
  CAR_ASSERT((ctl::is_sizeof_ge<int32_t, int32_t>::value));
  CAR_ASSERT((ctl::is_sizeof_ge_v<std::string, char>));

  CAR_ASSERT((!ctl::is_sizeof_ge<int16_t, int64_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_ge_v<short, double>));
}

TEST(type_traits_test, is_sizeof_eq) {
  CAR_ASSERT((ctl::is_sizeof_eq<int16_t, int16_t>::value));
  CAR_ASSERT((ctl::is_sizeof_eq_v<int32_t, float>));

  CAR_ASSERT((!ctl::is_sizeof_eq<int64_t, int32_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_eq_v<short, double>));
}

TEST(type_traits_test, is_sizeof_ne) {
  CAR_ASSERT((ctl::is_sizeof_ne<int16_t, int64_t>::value));
  CAR_ASSERT((ctl::is_sizeof_ne_v<std::string, char>));

  CAR_ASSERT((!ctl::is_sizeof_ne<int32_t, int32_t>::value));
  CAR_ASSERT((!ctl::is_sizeof_ne_v<int64_t, double>));
}

//===----------------------------------------------------------------------===//
// Tests for numerics based meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, is_signedness_same) {
  CAR_ASSERT((ctl::is_signedness_same<int, long, long long>::value));
  CAR_ASSERT((ctl::is_signedness_same<float, std::ptrdiff_t>::value));
  CAR_ASSERT((ctl::is_signedness_same<unsigned>::value));
  CAR_ASSERT((ctl::is_signedness_same_v<unsigned, unsigned char>));
  CAR_ASSERT((ctl::is_signedness_same_v<long long, long long>));
  CAR_ASSERT((ctl::is_signedness_same_v<long long int>));

  CAR_ASSERT((!ctl::is_signedness_same<int, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same<char, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same_v<double, std::size_t, int>));
  CAR_ASSERT((!ctl::is_signedness_same_v<unsigned short, short>));

  struct Tester {};
  CAR_ASSERT((!ctl::is_signedness_same<int, void>::value));
  CAR_ASSERT((!ctl::is_signedness_same<Tester, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same_v<double, int[3]>));
  CAR_ASSERT((!ctl::is_signedness_same_v<float*, char, volatile void>));
  CAR_ASSERT((!ctl::is_signedness_same_v<long&, const char**>));
  CAR_ASSERT((!ctl::is_signedness_same_v<Tester>));
}

//===----------------------------------------------------------------------===//
// Tests for combinations with \c std::enable_if meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, enable_same) {
  CAR_ASSERT((has_type<ctl::enable_same<int, int>>));
  CAR_ASSERT((has_type<ctl::enable_same<bool, bool, long long>>));
  CAR_ASSERT((!has_type<ctl::enable_same<int, float>>));
  CAR_ASSERT((!has_type<ctl::enable_same<unsigned, bool, int>>));

  CAR_ASSERT((std::is_same_v<ctl::enable_same_t<double, double>, void>));
  ASSERT_EQ((ctl::enable_same_t<double, double, long>{2}), long{2});
}

TEST(type_traits_test, enable_not_same) {
  CAR_ASSERT((!has_type<ctl::enable_not_same<int, int>>));
  CAR_ASSERT((!has_type<ctl::enable_not_same<bool, bool, long long>>));
  CAR_ASSERT((has_type<ctl::enable_not_same<int, float>>));
  CAR_ASSERT((has_type<ctl::enable_not_same<unsigned, bool, int>>));

  CAR_ASSERT((std::is_same_v<ctl::enable_not_same_t<double, char>, void>));
  ASSERT_EQ((ctl::enable_not_same_t<double, float, long>{2}), long{2});
}

TEST(type_traits_test, enable_same_decay) {
  CAR_ASSERT((has_type<ctl::enable_same_decay<int, const int&>>));
  CAR_ASSERT((has_type<ctl::enable_same_decay<bool&&, const bool, long>>));
  CAR_ASSERT((!has_type<ctl::enable_same_decay<const int, float&>>));
  CAR_ASSERT((!has_type<ctl::enable_same_decay<unsigned, const bool, int>>));

  CAR_ASSERT((std::is_same_v<ctl::enable_same_decay_t<int&&, int&&>, void>));
  ASSERT_EQ((ctl::enable_same_decay_t<int&&, const int, long>{2}), long{2});
}

} // namespace