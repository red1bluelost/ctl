//===- adt_test.cpp - Tests for ADT concepts --------------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/concept/adt.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/concept/adt.hpp"

#include <gtest/gtest.h>

namespace {

struct wrong_value_type {
  static constexpr bool value_type = 0;
};
TEST(concept_adt_test, aliasing_value_type) {
  struct wrong_enum_value_type {
    enum { value_type = 0 };
  };
  struct wrong_fn_value_type {
    static void value_type(){};
  };

  static_assert(!ctl::aliasing_value_type<void>);
  static_assert(!ctl::aliasing_value_type<bool>);
  static_assert(!ctl::aliasing_value_type<int>);
  static_assert(!ctl::aliasing_value_type<double>);

  struct no_value_type {};
  struct yes_value_type {
    using value_type = int;
  };
  static_assert(!ctl::aliasing_value_type<no_value_type>);
  static_assert(ctl::aliasing_value_type<yes_value_type>);
  static_assert(!ctl::aliasing_value_type<wrong_value_type>);
  static_assert(!ctl::aliasing_value_type<wrong_enum_value_type>);
  static_assert(!ctl::aliasing_value_type<wrong_fn_value_type>);

  static_assert(ctl::aliasing_value_type<std::vector<double>>);
  static_assert(ctl::aliasing_value_type<std::optional<std::string>>);
  static_assert(ctl::aliasing_value_type<std::string>);
  static_assert(!ctl::aliasing_value_type<std::ios_base>);
  static_assert(!ctl::aliasing_value_type<std::remove_all_extents<int>>);
}

} // namespace