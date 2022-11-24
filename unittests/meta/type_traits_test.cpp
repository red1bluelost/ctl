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

#include "ctl/concept/general.hpp"

#include <gtest/gtest.h>

namespace {
//===----------------------------------------------------------------------===//
// Tests for meta functions on meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, meta_all) {
  static_assert(ctl::meta::all<std::is_integral, int, char, long>::value);
  static_assert(ctl::meta::all<std::is_floating_point, double>::value);
  static_assert(ctl::meta::all_v<std::is_signed, int, short, long, long long>);
  static_assert(ctl::meta::all_v<std::is_abstract>);

  static_assert(
      !ctl::meta::all<std::is_array, long[3], int*, const int[]>::value
  );
  static_assert(!ctl::meta::all<std::is_class, std::string, double>::value);
  static_assert(!ctl::meta::all_v<std::is_empty, int>);
  static_assert(!ctl::meta::all_v<std::is_function, int, int (*)()>);
}

TEST(type_traits_test, meta_none) {
  static_assert(ctl::meta::none<std::is_integral, float, std::string>::value);
  static_assert(ctl::meta::none<std::is_signed, float*, double (*[3])()>::value
  );
  static_assert(ctl::meta::none_v<std::is_const>);
  static_assert(ctl::meta::
                    none_v<std::is_volatile, float*, const char*, int()>);

  static_assert(!ctl::meta::none<std::is_floating_point, float, long>::value);
  static_assert(!ctl::meta::none<std::is_signed, float*, int, char>::value);
  static_assert(!ctl::meta::none_v<std::is_const, const int>);
  static_assert(!ctl::meta::none_v<std::is_enum, std::byte, std::string>);
}

TEST(type_traits_test, meta_any) {
  static_assert(ctl::meta::any<std::is_integral, int, char, long>::value);
  static_assert(ctl::meta::any<std::is_floating_point, float, long>::value);
  static_assert(ctl::meta::any_v<std::is_const, const int>);
  static_assert(ctl::meta::any_v<std::is_signed, int, float, long, long long>);

  static_assert(!ctl::meta::any<std::is_integral, float, std::string>::value);
  static_assert(!ctl::meta::any<std::is_signed, float*, double (*[3])()>::value
  );
  static_assert(!ctl::meta::
                    any_v<std::is_volatile, float*, const char*, int()>);
  static_assert(!ctl::meta::any_v<std::is_abstract>);
}

TEST(type_traits_test, meta_same) {
  static_assert(ctl::meta::same<>::value);
  static_assert(ctl::meta::same<
                std::true_type,
                std::is_signed<int>,
                std::is_floating_point<float>>::value);
  static_assert(ctl::meta::same_v<long double>);
  static_assert(ctl::meta::same_v<
                std::false_type,
                std::is_class<int>,
                std::is_const<double>>);
  static_assert(ctl::meta::same_v<
                std::integral_constant<int, 3>,
                std::integral_constant<int, 2 + 1>,
                std::integral_constant<int, 7 - 4>,
                std::integral_constant<int, 9 / 3>>);

  static_assert(!ctl::meta::same<
                std::is_signed<int>,
                std::is_signed<unsigned>,
                std::is_signed<long>>::value);
  static_assert(!ctl::meta::same<
                std::integral_constant<char, 'a'>,
                std::integral_constant<char, 'b'>>::value);
  static_assert(!ctl::meta::same_v<
                std::is_scalar<int>,
                std::is_const<const long long>,
                std::is_volatile<long>,
                std::is_default_constructible<double>>);
  static_assert(!ctl::meta::same_v<
                std::integral_constant<int, 3>,
                std::integral_constant<int, 2>,
                std::integral_constant<int, 7>,
                std::integral_constant<int, 9 / 3>>);
}

TEST(type_traits_test, meta_apply_if) {
  static_assert(std::is_same_v<
                ctl::meta::apply_if<true, std::add_const, int>::type,
                const int>);
  static_assert(std::is_same_v<
                ctl::meta::apply_if<false, std::add_const, int>::type,
                int>);

  static_assert(std::is_same_v<
                ctl::meta::apply_if<true, std::make_signed, int>::type,
                int>);
  static_assert(std::is_same_v<
                ctl::meta::apply_if<false, std::make_signed, int>::type,
                int>);

  static_assert(std::is_same_v<
                ctl::meta::apply_if_t<true, std::make_unsigned, int>,
                unsigned int>);
  static_assert(std::is_same_v<
                ctl::meta::apply_if_t<false, std::make_unsigned, int>,
                int>);

  static_assert(std::is_same_v<
                ctl::meta::apply_if_t<true, std::decay, const float&>,
                float>);
  static_assert(std::is_same_v<
                ctl::meta::apply_if_t<false, std::decay, const float&>,
                const float&>);

  // Not intended use case
  static_assert(std::is_base_of_v<
                std::unique_ptr<int>,
                ctl::meta::apply_if<true, std::unique_ptr, int>>);
  static_assert(std::is_same_v<
                ctl::meta::apply_if_t<false, std::unique_ptr, int>,
                int>);
}

TEST(type_traits_test, meta_wrap_if) {
  static_assert(std::is_same_v<
                ctl::meta::wrap_if<true, std::unique_ptr, int>::type,
                std::unique_ptr<int>>);
  static_assert(std::is_same_v<
                ctl::meta::wrap_if<false, std::unique_ptr, int>::type,
                int>);

  static_assert(std::is_same_v<
                ctl::meta::wrap_if<true, std::vector, double>::type,
                std::vector<double>>);
  static_assert(std::is_same_v<
                ctl::meta::wrap_if<false, std::vector, double>::type,
                double>);

  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<true, std::optional, std::string_view>,
                std::optional<std::string_view>>);
  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<false, std::optional, std::string_view>,
                std::string_view>);

  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<true, std::variant, std::string>,
                std::variant<std::string>>);
  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<false, std::variant, std::string>,
                std::string>);

  // Not intended use case
  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<true, std::add_const, int>,
                std::add_const<int>>);
  static_assert(std::is_same_v<
                ctl::meta::wrap_if_t<false, std::add_const, int>,
                int>);
}

template<typename...>
struct replace_tester {};
TEST(type_traits_test, meta_rebind_first) {
  static_assert(std::is_same_v<
                ctl::meta::replace_first<std::optional<int>, double>::type,
                std::optional<double>>);
  static_assert(std::is_same_v<
                ctl::meta::replace_first<std::unique_ptr<int>, float>::type,
                std::unique_ptr<float, std::default_delete<int>>>);
  static_assert(std::is_same_v<
                ctl::meta::replace_first_t<std::vector<long>, char>,
                std::vector<char, std::allocator<long>>>);

  static_assert(std::is_same_v<
                ctl::meta::replace_first<
                    replace_tester<std::string, long, float>,
                    std::vector<int>>::type,
                replace_tester<std::vector<int>, long, float>>);
  static_assert(std::is_same_v<
                ctl::meta::
                    replace_first_t<replace_tester<unsigned>, long double>,
                replace_tester<long double>>);
  static_assert(std::is_same_v<
                ctl::meta::
                    replace_first_t<replace_tester<int, int, int, int>, int>,
                replace_tester<int, int, int, int>>);
  static_assert(std::is_same_v<
                ctl::meta::replace_first_t<replace_tester<int>, int>,
                replace_tester<int>>);

  struct not_template {};
  static_assert(!ctl::aliasing_type<
                ctl::meta::replace_first<not_template, int>>);
  static_assert(!ctl::aliasing_type<ctl::meta::replace_first<double, int>>);
  static_assert(!ctl::aliasing_type<ctl::meta::replace_first<void, int>>);
  static_assert(!ctl::aliasing_type<
                ctl::meta::replace_first<replace_tester<>, int>>);
}

//===----------------------------------------------------------------------===//
// Tests for meta functions on any type.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, is_sizeof_lt) {
  static_assert(ctl::is_sizeof_lt<int32_t, int64_t>::value);
  static_assert(ctl::is_sizeof_lt_v<char, std::string>);

  static_assert(!ctl::is_sizeof_lt<int16_t, int16_t>::value);
  static_assert(!ctl::is_sizeof_lt_v<double, short>);
}

TEST(type_traits_test, is_sizeof_le) {
  static_assert(ctl::is_sizeof_le<int32_t, int32_t>::value);
  static_assert(ctl::is_sizeof_le_v<char, std::string>);

  static_assert(!ctl::is_sizeof_le<int64_t, int16_t>::value);
  static_assert(!ctl::is_sizeof_le_v<double, short>);
}

TEST(type_traits_test, is_sizeof_gt) {
  static_assert(ctl::is_sizeof_gt<int64_t, int32_t>::value);
  static_assert(ctl::is_sizeof_gt_v<std::string, char>);

  static_assert(!ctl::is_sizeof_gt<int16_t, int16_t>::value);
  static_assert(!ctl::is_sizeof_gt_v<short, double>);
}

TEST(type_traits_test, is_sizeof_ge) {
  static_assert(ctl::is_sizeof_ge<int32_t, int32_t>::value);
  static_assert(ctl::is_sizeof_ge_v<std::string, char>);

  static_assert(!ctl::is_sizeof_ge<int16_t, int64_t>::value);
  static_assert(!ctl::is_sizeof_ge_v<short, double>);
}

TEST(type_traits_test, is_sizeof_eq) {
  static_assert(ctl::is_sizeof_eq<int16_t, int16_t>::value);
  static_assert(ctl::is_sizeof_eq_v<int32_t, float>);

  static_assert(!ctl::is_sizeof_eq<int64_t, int32_t>::value);
  static_assert(!ctl::is_sizeof_eq_v<short, double>);
}

TEST(type_traits_test, is_sizeof_ne) {
  static_assert(ctl::is_sizeof_ne<int16_t, int64_t>::value);
  static_assert(ctl::is_sizeof_ne_v<std::string, char>);

  static_assert(!ctl::is_sizeof_ne<int32_t, int32_t>::value);
  static_assert(!ctl::is_sizeof_ne_v<int64_t, double>);
}

//===----------------------------------------------------------------------===//
// Tests for numerics based meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, is_signedness_same) {
  static_assert(ctl::is_signedness_same<>::value);
  static_assert(ctl::is_signedness_same<unsigned>::value);

  static_assert(ctl::is_signedness_same<int, long, long long>::value);
  static_assert(ctl::is_signedness_same<float, std::ptrdiff_t>::value);
  static_assert(ctl::is_signedness_same<unsigned>::value);
  static_assert(ctl::is_signedness_same_v<unsigned, unsigned char>);
  static_assert(ctl::is_signedness_same_v<long long, long long>);
  static_assert(ctl::is_signedness_same_v<long long int>);
  static_assert(ctl::is_signedness_same_v<bool, unsigned>);

  static_assert(!ctl::is_signedness_same<int, unsigned>::value);
  static_assert(!ctl::is_signedness_same<signed char, unsigned>::value);
  static_assert(!ctl::is_signedness_same_v<double, std::size_t, int>);
  static_assert(!ctl::is_signedness_same_v<unsigned short, short>);
  static_assert(!ctl::is_signedness_same_v<bool, int>);

  struct tester {};
  static_assert(!ctl::is_signedness_same<tester>::value);
  static_assert(!ctl::is_signedness_same<int, void>::value);
  static_assert(!ctl::is_signedness_same<tester, unsigned>::value);
  static_assert(!ctl::is_signedness_same_v<double, int[3]>);
  static_assert(!ctl::is_signedness_same_v<float*, signed char, volatile void>);
  static_assert(!ctl::is_signedness_same_v<long&, const char**>);
  static_assert(!ctl::is_signedness_same_v<tester>);
}

TEST(type_traits_test, is_arithmetic_same) {
  static_assert(ctl::is_arithmetic_same<>::value);
  static_assert(ctl::is_arithmetic_same_v<>);
  static_assert(ctl::is_arithmetic_same<int>::value);
  static_assert(ctl::is_arithmetic_same_v<long>);
  static_assert(ctl::is_arithmetic_same<long double>::value);
  static_assert(ctl::is_arithmetic_same_v<float>);
  static_assert(!ctl::is_arithmetic_same_v<std::string>);

  static_assert(ctl::is_arithmetic_same<
                unsigned char,
                short,
                unsigned int,
                char,
                unsigned long long,
                long>::value);
  static_assert(ctl::is_arithmetic_same_v<
                bool,
                char,
                short,
                int,
                long,
                long long,
                wchar_t>);
  static_assert(ctl::is_arithmetic_same_v<
                unsigned char,
                unsigned short,
                unsigned int,
                unsigned long,
                unsigned long long,
                char8_t,
                char16_t,
                char32_t>);
  static_assert(ctl::is_arithmetic_same<float, double, long double>::value);

  static_assert(!ctl::is_arithmetic_same<float, int>::value);
  static_assert(!ctl::is_arithmetic_same_v<double, unsigned long>);
  static_assert(!ctl::is_arithmetic_same_v<double, long, int, float>);

  static_assert(!ctl::is_arithmetic_same_v<std::string, std::vector<int>>);
  static_assert(!ctl::is_arithmetic_same_v<std::string_view>);
  static_assert(!ctl::is_arithmetic_same_v<std::string_view, int>);
  static_assert(!ctl::is_arithmetic_same_v<bool, std::array<double, 3>, float>);
}

TEST(type_traits_is_lossless_convertible_test, floating_point) {
  static_assert(ctl::is_lossless_convertible<long double, long double>::value);
  static_assert(ctl::is_lossless_convertible<double, long double>::value);
  static_assert(ctl::is_lossless_convertible<float, long double>::value);
  static_assert(ctl::is_lossless_convertible_v<double, double>);
  static_assert(ctl::is_lossless_convertible_v<float, double>);
  static_assert(ctl::is_lossless_convertible_v<float, float>);

  static_assert(!ctl::is_lossless_convertible<long double, double>::value);
  static_assert(!ctl::is_lossless_convertible<long double, float>::value);
  static_assert(!ctl::is_lossless_convertible_v<double, float>);

  static_assert(ctl::is_lossless_convertible<const double, long double>::value);
  static_assert(ctl::is_lossless_convertible<float, const long double>::value);
  static_assert(ctl::is_lossless_convertible<volatile double, double>::value);
  static_assert(ctl::is_lossless_convertible_v<float, volatile double>);
  static_assert(ctl::is_lossless_convertible_v<const float, volatile float>);
  static_assert(ctl::is_lossless_convertible_v<const volatile float, float>);
}

TEST(type_traits_is_lossless_convertible_test, signed_integral) {
  static_assert(ctl::is_lossless_convertible<int64_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<int32_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<int16_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<int8_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<int32_t, int32_t>::value);
  static_assert(ctl::is_lossless_convertible_v<int16_t, int32_t>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, int32_t>);
  static_assert(ctl::is_lossless_convertible_v<int16_t, int16_t>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, int16_t>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, int8_t>);

  static_assert(!ctl::is_lossless_convertible<int64_t, int32_t>::value);
  static_assert(!ctl::is_lossless_convertible<int64_t, int16_t>::value);
  static_assert(!ctl::is_lossless_convertible<int32_t, int16_t>::value);
  static_assert(!ctl::is_lossless_convertible_v<int64_t, int8_t>);
  static_assert(!ctl::is_lossless_convertible_v<int32_t, int8_t>);
  static_assert(!ctl::is_lossless_convertible_v<int16_t, int8_t>);

  static_assert(ctl::is_lossless_convertible<const int16_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<int8_t, const int64_t>::value);
  static_assert(ctl::is_lossless_convertible<volatile int32_t, int32_t>::value);
  static_assert(ctl::is_lossless_convertible_v<int16_t, volatile int32_t>);
  static_assert(ctl::is_lossless_convertible_v<const volatile int8_t, int32_t>);
  static_assert(ctl::
                    is_lossless_convertible_v<const int16_t, volatile int16_t>);
}

TEST(type_traits_is_lossless_convertible_test, unsigned_integral) {
  static_assert(ctl::is_lossless_convertible<uint64_t, uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint32_t, uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint16_t, uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint8_t, uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint32_t, uint32_t>::value);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, uint32_t>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, uint32_t>);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, uint16_t>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, uint16_t>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, uint8_t>);

  static_assert(!ctl::is_lossless_convertible<uint64_t, uint32_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint64_t, uint16_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint32_t, uint16_t>::value);
  static_assert(!ctl::is_lossless_convertible_v<uint64_t, uint8_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint32_t, uint8_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint16_t, uint8_t>);

  static_assert(ctl::is_lossless_convertible<const uint16_t, uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint8_t, const uint64_t>::value);
  static_assert(ctl::is_lossless_convertible<volatile uint32_t, uint32_t>::value
  );
  static_assert(ctl::is_lossless_convertible_v<uint16_t, volatile uint32_t>);
  static_assert(ctl::is_lossless_convertible_v<
                const volatile uint8_t,
                uint32_t>);
  static_assert(ctl::is_lossless_convertible_v<
                const uint16_t,
                volatile uint16_t>);
}

TEST(type_traits_is_lossless_convertible_test, signedness_mixed) {
  static_assert(ctl::is_lossless_convertible<uint32_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint16_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible<uint8_t, int64_t>::value);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, int32_t>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, int32_t>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, int16_t>);

  static_assert(!ctl::is_lossless_convertible<uint64_t, int64_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint64_t, int32_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint32_t, int32_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint64_t, int16_t>::value);
  static_assert(!ctl::is_lossless_convertible<uint32_t, int16_t>::value);
  static_assert(!ctl::is_lossless_convertible_v<uint16_t, int16_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint64_t, int8_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint32_t, int8_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint16_t, int8_t>);
  static_assert(!ctl::is_lossless_convertible_v<uint8_t, int8_t>);

  static_assert(!ctl::is_lossless_convertible<int64_t, uint64_t>::value);
  static_assert(!ctl::is_lossless_convertible<int32_t, uint64_t>::value);
  static_assert(!ctl::is_lossless_convertible<int16_t, uint64_t>::value);
  static_assert(!ctl::is_lossless_convertible<int8_t, uint64_t>::value);
  static_assert(!ctl::is_lossless_convertible<int64_t, uint32_t>::value);
  static_assert(!ctl::is_lossless_convertible<int32_t, uint32_t>::value);
  static_assert(!ctl::is_lossless_convertible_v<int16_t, uint32_t>);
  static_assert(!ctl::is_lossless_convertible_v<int8_t, uint32_t>);
  static_assert(!ctl::is_lossless_convertible<int64_t, uint16_t>::value);
  static_assert(!ctl::is_lossless_convertible<int32_t, uint16_t>::value);
  static_assert(!ctl::is_lossless_convertible_v<int16_t, uint16_t>);
  static_assert(!ctl::is_lossless_convertible_v<int8_t, uint16_t>);
  static_assert(!ctl::is_lossless_convertible_v<int64_t, uint8_t>);
  static_assert(!ctl::is_lossless_convertible_v<int32_t, uint8_t>);
  static_assert(!ctl::is_lossless_convertible_v<int16_t, uint8_t>);
  static_assert(!ctl::is_lossless_convertible_v<int8_t, uint8_t>);
}

TEST(type_traits_is_lossless_convertible_test, arithmetic_mixed) {
  static_assert(ctl::is_lossless_convertible_v<uint64_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<int64_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<uint32_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<int32_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<int16_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, long double>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, long double>);

  static_assert(!ctl::is_lossless_convertible_v<uint64_t, double>);
  static_assert(!ctl::is_lossless_convertible_v<int64_t, double>);
  static_assert(ctl::is_lossless_convertible_v<uint32_t, double>);
  static_assert(ctl::is_lossless_convertible_v<int32_t, double>);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, double>);
  static_assert(ctl::is_lossless_convertible_v<int16_t, double>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, double>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, double>);

  static_assert(!ctl::is_lossless_convertible_v<uint64_t, float>);
  static_assert(!ctl::is_lossless_convertible_v<int64_t, float>);
  static_assert(!ctl::is_lossless_convertible_v<uint32_t, float>);
  static_assert(!ctl::is_lossless_convertible_v<int32_t, float>);
  static_assert(ctl::is_lossless_convertible_v<uint16_t, float>);
  static_assert(ctl::is_lossless_convertible_v<int16_t, float>);
  static_assert(ctl::is_lossless_convertible_v<uint8_t, float>);
  static_assert(ctl::is_lossless_convertible_v<int8_t, float>);

  static_assert(!ctl::is_lossless_convertible<float, int>::value);
  static_assert(!ctl::is_lossless_convertible<float, long long>::value);
  static_assert(!ctl::is_lossless_convertible<double, unsigned>::value);
  static_assert(!ctl::is_lossless_convertible<long double, char>::value);
}

TEST(type_traits_is_lossless_convertible_test, non_arithmetic) {
  struct s_int {
    int i;
    s_int(int i_) : i(i_) {}
  };
  static_assert(std::is_convertible_v<int, s_int>);

  static_assert(!ctl::is_lossless_convertible<int, s_int>::value);
  static_assert(!ctl::is_lossless_convertible_v<s_int, int>);

  static_assert(!ctl::is_lossless_convertible_v<float, std::string>);
  static_assert(!ctl::is_lossless_convertible_v<int, std::string>);
  static_assert(!ctl::is_lossless_convertible_v<char, std::string>);

  static_assert(!ctl::is_lossless_convertible_v<std::array<long, 1>, long>);
  static_assert(!ctl::is_lossless_convertible_v<long, std::array<long, 1>>);
}

//===----------------------------------------------------------------------===//
// Tests for combinations with \c std::enable_if meta functions.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, enable_same) {
  static_assert(ctl::aliasing_type<ctl::enable_same<int, int>>);
  static_assert(ctl::aliasing_type<ctl::enable_same<bool, bool, long long>>);
  static_assert(!ctl::aliasing_type<ctl::enable_same<int, float>>);
  static_assert(!ctl::aliasing_type<ctl::enable_same<unsigned, bool, int>>);

  static_assert(std::is_same_v<ctl::enable_same_t<double, double>, void>);
  ASSERT_EQ((ctl::enable_same_t<double, double, long>{2}), long{2});
}

TEST(type_traits_test, enable_not_same) {
  static_assert(!ctl::aliasing_type<ctl::enable_not_same<int, int>>);
  static_assert(!ctl::aliasing_type<
                ctl::enable_not_same<bool, bool, long long>>);
  static_assert(ctl::aliasing_type<ctl::enable_not_same<int, float>>);
  static_assert(ctl::aliasing_type<ctl::enable_not_same<unsigned, bool, int>>);

  static_assert(std::is_same_v<ctl::enable_not_same_t<double, char>, void>);
  ASSERT_EQ((ctl::enable_not_same_t<double, float, long>{2}), long{2});
}

TEST(type_traits_test, enable_same_decay) {
  static_assert(ctl::aliasing_type<ctl::enable_same_decay<int, const int&>>);
  static_assert(ctl::aliasing_type<
                ctl::enable_same_decay<bool&&, const bool, long>>);
  static_assert(!ctl::aliasing_type<ctl::enable_same_decay<const int, float&>>);
  static_assert(!ctl::aliasing_type<
                ctl::enable_same_decay<unsigned, const bool, int>>);

  static_assert(std::is_same_v<ctl::enable_same_decay_t<int&&, int&&>, void>);
  ASSERT_EQ((ctl::enable_same_decay_t<int&&, const int, long>{2}), long{2});
}

//===----------------------------------------------------------------------===//
// Tests for meta functions that modify the qualifiers of a type.
//===----------------------------------------------------------------------===//

TEST(type_traits_test, match_const) {
  static_assert(std::is_same_v<ctl::match_const<double, int>::type, int>);
  static_assert(std::is_same_v<ctl::match_const<long, const int>::type, int>);

  static_assert(std::is_same_v<ctl::match_const_t<const long, int>, const int>);
  static_assert(std::is_same_v<
                ctl::match_const_t<const char, const int>,
                const int>);

  static_assert(std::is_same_v<
                ctl::match_const_t<long, const volatile int>,
                volatile int>);
  static_assert(std::is_same_v<
                ctl::match_const_t<const char, volatile int>,
                const volatile int>);

  static_assert(std::is_same_v<
                ctl::match_const_t<float, const int*>,
                const int*>);
  static_assert(std::is_same_v<
                ctl::match_const_t<const float, const int*>,
                const int* const>);
}

TEST(type_traits_test, match_volatile) {
  static_assert(std::is_same_v<ctl::match_volatile<double, int>::type, int>);
  static_assert(std::is_same_v<
                ctl::match_volatile<long, volatile int>::type,
                int>);

  static_assert(std::is_same_v<
                ctl::match_volatile_t<volatile long, int>,
                volatile int>);
  static_assert(std::is_same_v<
                ctl::match_volatile_t<volatile char, volatile int>,
                volatile int>);

  static_assert(std::is_same_v<
                ctl::match_volatile_t<long, const volatile int>,
                const int>);
  static_assert(std::is_same_v<
                ctl::match_volatile_t<volatile char, const int>,
                const volatile int>);

  static_assert(std::is_same_v<
                ctl::match_volatile_t<float, volatile int*>,
                volatile int*>);
  static_assert(std::is_same_v<
                ctl::match_volatile_t<volatile float, volatile int*>,
                volatile int* volatile>);
}

TEST(type_traits_test, match_cv) {
  static_assert(std::is_same_v<ctl::match_cv<double, int>::type, int>);
  static_assert(std::is_same_v<ctl::match_cv<long, const int>::type, int>);
  static_assert(std::is_same_v<ctl::match_cv<double, int>::type, int>);
  static_assert(std::is_same_v<ctl::match_cv<long, volatile int>::type, int>);

  static_assert(std::is_same_v<ctl::match_cv_t<const long, int>, const int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<const char, const int>,
                const int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile long, int>,
                volatile int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile char, volatile int>,
                volatile int>);

  static_assert(std::is_same_v<ctl::match_cv_t<long, const volatile int>, int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<const char, volatile int>,
                const int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile char, const int>,
                volatile int>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile long, const volatile int>,
                volatile int>);

  static_assert(std::is_same_v<ctl::match_cv_t<float, const int*>, const int*>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<const float, const int*>,
                const int* const>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<float, volatile int*>,
                volatile int*>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile float, volatile int*>,
                volatile int* volatile>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<const float, const volatile int*>,
                const volatile int* const>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<volatile float, const volatile int*>,
                const volatile int* volatile>);
  static_assert(std::is_same_v<
                ctl::match_cv_t<const volatile float, const volatile int*>,
                const volatile int* const volatile>);
}

TEST(type_traits_test, match_sign) {
  static_assert(std::is_same_v<ctl::match_sign<double, int>::type, int>);
  static_assert(std::is_same_v<ctl::match_sign<long, unsigned int>::type, int>);

  static_assert(std::is_same_v<ctl::match_sign_t<unsigned, int>, unsigned>);
  static_assert(std::is_same_v<
                ctl::match_sign_t<unsigned char, unsigned>,
                unsigned>);

  static_assert(std::is_same_v<
                ctl::match_sign_t<long, const volatile int>,
                const volatile int>);
  static_assert(std::is_same_v<
                ctl::match_sign_t<long, const volatile unsigned int>,
                const volatile int>);
  static_assert(std::is_same_v<
                ctl::match_sign_t<unsigned long, const volatile int>,
                const volatile unsigned int>);
  static_assert(std::is_same_v<
                ctl::match_sign_t<unsigned long, const volatile unsigned int>,
                const volatile unsigned int>);
}

} // namespace