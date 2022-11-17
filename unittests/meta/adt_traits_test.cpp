//===- type_traits_test.hpp - Tests for ADT type traits ---------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/meta/adt_traits.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/meta/adt_traits.hpp"

#include <gtest/gtest.h>

namespace {

//===----------------------------------------------------------------------===//
// Tests for meta functions on abstract data types.
//===----------------------------------------------------------------------===//

struct wrong_value_type {
  static constexpr bool value_type = 0;
};
struct wrong_fn_value_type {
  static void value_type(){};
};
TEST(type_traits_test, has_value_type) {
  static_assert(!ctl::has_value_type<void>::value);
  static_assert(!ctl::has_value_type<bool>::value);
  static_assert(!ctl::has_value_type<int>::value);
  static_assert(!ctl::has_value_type<double>::value);

  struct no_value_type {};
  struct yes_value_type {
    using value_type = int;
  };
  static_assert(!ctl::has_value_type_v<no_value_type>);
  static_assert(ctl::has_value_type_v<yes_value_type>);
  static_assert(!ctl::has_value_type_v<wrong_value_type>);
  static_assert(!ctl::has_value_type_v<wrong_fn_value_type>);

  static_assert(ctl::has_value_type<std::vector<double>>::value);
  static_assert(ctl::has_value_type<std::optional<std::string>>::value);
  static_assert(ctl::has_value_type_v<std::string>);
  static_assert(!ctl::has_value_type_v<std::ios_base>);
  static_assert(!ctl::has_value_type_v<std::remove_all_extents<int>>);
}

} // namespace