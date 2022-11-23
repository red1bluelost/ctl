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

template<typename Nullable, typename TypeForRebind>
struct rebind_tester;
template<typename T, typename TypeForRebind>
struct rebind_tester<T*, TypeForRebind> : std::type_identity<TypeForRebind*> {};
template<
    typename ValueType,
    template<typename, typename...>
    class SmartPtrLike,
    typename TypeForRebind>
struct rebind_tester<SmartPtrLike<ValueType>, TypeForRebind>
    : std::type_identity<SmartPtrLike<TypeForRebind>> {};

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