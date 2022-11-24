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

#include "ctl/meta/type_traits.hpp"

#include <gtest/gtest.h>

#include <concepts>
#include <type_traits>

namespace {

template<typename Nullable, typename TypeForRebind>
struct rebind_tester : ctl::meta::rebind_adt<Nullable, TypeForRebind> {};
template<typename T, typename TypeForRebind>
struct rebind_tester<T*, TypeForRebind> : std::type_identity<TypeForRebind*> {};

template<typename T>
consteval void assert_proper_nullable_type() {
  using tcv = std::remove_cv_t<T>;

  using nt = ctl::null_traits<T>;
  static_assert(std::same_as<typename nt::nullable_type, tcv>);

  // We rely on all the supported types having a * operator that exposes
  // value_type.
  using value_type = std::remove_reference_t<decltype(*std::declval<tcv>())>;
  static_assert(std::same_as<typename nt::element_type, value_type>);

  struct tester_type {};
  static_assert(std::same_as<
                typename nt::template rebind<tester_type>,
                typename rebind_tester<tcv, tester_type>::type>);

  using null_ret = decltype(nt::null());
  static_assert(std::same_as<null_ret, tcv>);
  static_assert(!std::is_const_v<null_ret>);
  static_assert(!std::is_volatile_v<null_ret>);
}

TEST(null_traits_test, nullable_cv_qualification) {
  assert_proper_nullable_type<int* const>();
  assert_proper_nullable_type<const std::optional<double>>();
  assert_proper_nullable_type<const std::unique_ptr<long>>();
  assert_proper_nullable_type<const std::shared_ptr<char>>();

  assert_proper_nullable_type<int* volatile>();
  assert_proper_nullable_type<volatile std::optional<double>>();
  assert_proper_nullable_type<volatile std::unique_ptr<long>>();
  assert_proper_nullable_type<volatile std::shared_ptr<char>>();

  assert_proper_nullable_type<int* const volatile>();
  assert_proper_nullable_type<const volatile std::optional<double>>();
  assert_proper_nullable_type<const volatile std::unique_ptr<long>>();
  assert_proper_nullable_type<const volatile std::shared_ptr<char>>();
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

template<typename T>
struct no_deleter {
  constexpr void operator()(T*) noexcept {}
};
template<typename T>
struct no_deleter_rebind {
  constexpr void operator()(T*) noexcept {}

  template<typename U>
  using rebind = no_deleter_rebind<U>;
};
TEST(null_traits_test, nullable_unique_ptr) {
  assert_proper_nullable_type<std::unique_ptr<const char>>();
  assert_proper_nullable_type<std::unique_ptr<long>>();
  assert_proper_nullable_type<std::unique_ptr<std::string>>();

  assert_proper_nullable_type<std::unique_ptr<int, no_deleter<int>>>();
  static_assert(std::same_as<
                ctl::null_traits<std::unique_ptr<int, no_deleter<int>>>::rebind<
                    double>,
                std::unique_ptr<double, no_deleter<double>>>);
  assert_proper_nullable_type<std::unique_ptr<int, no_deleter_rebind<int>>>();
  static_assert(std::same_as<
                ctl::null_traits<std::unique_ptr<int, no_deleter_rebind<int>>>::
                    rebind<double>,
                std::unique_ptr<double, no_deleter_rebind<double>>>);

  // comparison is not yet constexpr

  ASSERT_EQ(ctl::null_traits<std::unique_ptr<unsigned>>::null(), nullptr);
  ASSERT_EQ(ctl::null_traits<std::unique_ptr<double>>::null(), nullptr);
  ASSERT_EQ(
      ctl::null_traits<std::unique_ptr<std::string_view>>::null(), nullptr
  );
}

TEST(null_traits_test, nullable_shared_ptr) {
  assert_proper_nullable_type<std::shared_ptr<const char>>();
  assert_proper_nullable_type<std::shared_ptr<long>>();
  assert_proper_nullable_type<std::shared_ptr<std::string>>();

  // comparison is not yet constexpr

  ASSERT_EQ(ctl::null_traits<std::shared_ptr<unsigned>>::null(), nullptr);
  ASSERT_EQ(ctl::null_traits<std::shared_ptr<double>>::null(), nullptr);
  ASSERT_EQ(
      ctl::null_traits<std::shared_ptr<std::string_view>>::null(), nullptr
  );
}

} // namespace