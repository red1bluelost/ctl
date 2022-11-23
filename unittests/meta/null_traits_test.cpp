//===- null_traits_test.cpp - Tests for null meta utilities -----*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/meta/null_traits.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/meta/null_traits.hpp"

#include <gtest/gtest.h>

#include <concepts>
#include <type_traits>

namespace {

template<typename T>
consteval void assert_proper_nullable_type() {
  using null_ret = decltype(ctl::null_traits<T>::null());
  static_assert(std::same_as<null_ret, std::remove_cv_t<T>>);
  static_assert(!std::is_const_v<null_ret>);
  static_assert(!std::is_volatile_v<null_ret>);
}

TEST(null_traits_test, nullable_cv_qualification) {
  assert_proper_nullable_type<int* const>();
  assert_proper_nullable_type<const std::optional<double>>();

  assert_proper_nullable_type<int* volatile>();
  assert_proper_nullable_type<volatile std::optional<double>>();

  assert_proper_nullable_type<int* const volatile>();
  assert_proper_nullable_type<const volatile std::optional<double>>();
}

TEST(null_traits_test, nullable_pointers) {
  assert_proper_nullable_type<const char*>();
  assert_proper_nullable_type<long*>();
  assert_proper_nullable_type<std::string*>();

  static_assert(ctl::null_traits<int*>::null() == nullptr);
  static_assert(ctl::null_traits<float*>::null() == nullptr);
  static_assert(ctl::null_traits<std::string*>::null() == nullptr);

  ASSERT_EQ(ctl::null_traits<unsigned*>::null(), nullptr);
  ASSERT_EQ(ctl::null_traits<double*>::null(), nullptr);
  ASSERT_EQ(ctl::null_traits<std::string_view*>::null(), nullptr);
}

TEST(null_traits_test, nullable_optional) {
  assert_proper_nullable_type<std::optional<const char>>();
  assert_proper_nullable_type<std::optional<long>>();
  assert_proper_nullable_type<std::optional<std::string>>();

  static_assert(ctl::null_traits<std::optional<int>>::null() == std::nullopt);
  static_assert(ctl::null_traits<std::optional<float>>::null() == std::nullopt);
  static_assert(
      ctl::null_traits<std::optional<std::string>>::null() == std::nullopt
  );

  ASSERT_EQ(ctl::null_traits<std::optional<unsigned>>::null(), std::nullopt);
  ASSERT_EQ(ctl::null_traits<std::optional<double>>::null(), std::nullopt);
  ASSERT_EQ(
      ctl::null_traits<std::optional<std::string_view>>::null(), std::nullopt
  );
}

} // namespace