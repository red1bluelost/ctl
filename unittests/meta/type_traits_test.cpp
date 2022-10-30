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
  CAR_ASSERT((ctl::is_signedness_same_v<bool, unsigned>));

  CAR_ASSERT((!ctl::is_signedness_same<int, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same<signed char, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same_v<double, std::size_t, int>));
  CAR_ASSERT((!ctl::is_signedness_same_v<unsigned short, short>));
  CAR_ASSERT((!ctl::is_signedness_same_v<bool, int>));

  struct tester {};
  CAR_ASSERT((!ctl::is_signedness_same<int, void>::value));
  CAR_ASSERT((!ctl::is_signedness_same<tester, unsigned>::value));
  CAR_ASSERT((!ctl::is_signedness_same_v<double, int[3]>));
  CAR_ASSERT((!ctl::is_signedness_same_v<float*, signed char, volatile void>));
  CAR_ASSERT((!ctl::is_signedness_same_v<long&, const char**>));
  CAR_ASSERT((!ctl::is_signedness_same_v<tester>));
}

TEST(type_traits_test, is_arithmetic_same) {
  CAR_ASSERT((ctl::is_arithmetic_same<>::value));
  CAR_ASSERT((ctl::is_arithmetic_same_v<>));
  CAR_ASSERT((ctl::is_arithmetic_same<int>::value));
  CAR_ASSERT((ctl::is_arithmetic_same_v<long>));
  CAR_ASSERT((ctl::is_arithmetic_same<long double>::value));
  CAR_ASSERT((ctl::is_arithmetic_same_v<float>));
  CAR_ASSERT((!ctl::is_arithmetic_same_v<std::string>));

  CAR_ASSERT((ctl::is_arithmetic_same<
              unsigned char,
              short,
              unsigned int,
              char,
              unsigned long long,
              long>::value));
  CAR_ASSERT((ctl::is_arithmetic_same_v<
              bool,
              char,
              short,
              int,
              long,
              long long,
              wchar_t>));
  CAR_ASSERT((ctl::is_arithmetic_same_v<
              unsigned char,
              unsigned short,
              unsigned int,
              unsigned long,
              unsigned long long,
              char8_t,
              char16_t,
              char32_t>));
  CAR_ASSERT((ctl::is_arithmetic_same<float, double, long double>::value));

  CAR_ASSERT((!ctl::is_arithmetic_same<float, int>::value));
  CAR_ASSERT((!ctl::is_arithmetic_same_v<double, unsigned long>));
  CAR_ASSERT((!ctl::is_arithmetic_same_v<double, long, int, float>));

  CAR_ASSERT((!ctl::is_arithmetic_same_v<std::string, std::vector<int>>));
  CAR_ASSERT((!ctl::is_arithmetic_same_v<std::string_view, int>));
  CAR_ASSERT((!ctl::is_arithmetic_same_v<bool, std::array<double, 3>, float>));
}

TEST(type_traits_is_lossless_convertible_test, floating_point) {
  CAR_ASSERT((ctl::is_lossless_convertible<long double, long double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<double, long double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<float, long double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<double, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<float, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<float, float>));

  CAR_ASSERT((!ctl::is_lossless_convertible<long double, double>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<long double, float>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<double, float>));

  CAR_ASSERT((ctl::is_lossless_convertible<const double, long double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<float, const long double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<volatile double, double>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<float, volatile double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<const float, volatile float>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<const volatile float, float>));
}

TEST(type_traits_is_lossless_convertible_test, signed_integral) {
  CAR_ASSERT((ctl::is_lossless_convertible<int64_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<int32_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<int16_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<int8_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<int32_t, int32_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, int16_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, int16_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, int8_t>));

  CAR_ASSERT((!ctl::is_lossless_convertible<int64_t, int32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int64_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int32_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int64_t, int8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int32_t, int8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int16_t, int8_t>));

  CAR_ASSERT((ctl::is_lossless_convertible<const int16_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<int8_t, const int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<volatile int32_t, int32_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, volatile int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<const volatile int8_t, int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<const int16_t, volatile int16_t>));
}

TEST(type_traits_is_lossless_convertible_test, unsigned_integral) {
  CAR_ASSERT((ctl::is_lossless_convertible<uint64_t, uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint32_t, uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint16_t, uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint8_t, uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint32_t, uint32_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, uint32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, uint32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, uint16_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, uint16_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, uint8_t>));

  CAR_ASSERT((!ctl::is_lossless_convertible<uint64_t, uint32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint64_t, uint16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint32_t, uint16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint64_t, uint8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint32_t, uint8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint16_t, uint8_t>));

  CAR_ASSERT((ctl::is_lossless_convertible<const uint16_t, uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint8_t, const uint64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<volatile uint32_t, uint32_t>::value)
  );
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, volatile uint32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<const volatile uint8_t, uint32_t>)
  );
  CAR_ASSERT((ctl::is_lossless_convertible_v<const uint16_t, volatile uint16_t>)
  );
}

TEST(type_traits_is_lossless_convertible_test, signedness_mixed) {
  CAR_ASSERT((ctl::is_lossless_convertible<uint32_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint16_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible<uint8_t, int64_t>::value));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, int32_t>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, int16_t>));

  CAR_ASSERT((!ctl::is_lossless_convertible<uint64_t, int64_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint64_t, int32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint32_t, int32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint64_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<uint32_t, int16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint16_t, int16_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint64_t, int8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint32_t, int8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint16_t, int8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint8_t, int8_t>));

  CAR_ASSERT((!ctl::is_lossless_convertible<int64_t, uint64_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int32_t, uint64_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int16_t, uint64_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int8_t, uint64_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int64_t, uint32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int32_t, uint32_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int16_t, uint32_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int8_t, uint32_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible<int64_t, uint16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<int32_t, uint16_t>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int16_t, uint16_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int8_t, uint16_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int64_t, uint8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int32_t, uint8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int16_t, uint8_t>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int8_t, uint8_t>));
}

TEST(type_traits_is_lossless_convertible_test, arithmetic_mixed) {
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint64_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int64_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint32_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int32_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, long double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, long double>));

  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint64_t, double>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int64_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint32_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int32_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, double>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, double>));

  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint64_t, float>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int64_t, float>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<uint32_t, float>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int32_t, float>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint16_t, float>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int16_t, float>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<uint8_t, float>));
  CAR_ASSERT((ctl::is_lossless_convertible_v<int8_t, float>));

  CAR_ASSERT((!ctl::is_lossless_convertible<float, int>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<float, long long>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<double, unsigned>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible<long double, char>::value));
}

TEST(type_traits_is_lossless_convertible_test, non_arithmetic) {
  struct s_int {
    int i;
    s_int(int i) : i(i) {}
  };
  CAR_ASSERT((std::is_convertible_v<int, s_int>));

  CAR_ASSERT((!ctl::is_lossless_convertible<int, s_int>::value));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<s_int, int>));

  CAR_ASSERT((!ctl::is_lossless_convertible_v<float, std::string>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<int, std::string>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<char, std::string>));

  CAR_ASSERT((!ctl::is_lossless_convertible_v<std::array<long, 1>, long>));
  CAR_ASSERT((!ctl::is_lossless_convertible_v<long, std::array<long, 1>>));
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