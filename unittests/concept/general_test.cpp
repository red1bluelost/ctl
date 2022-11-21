//===- general_test.cpp - Tests for general concepts ------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/concept/general.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/concept/general.hpp"

#include <gtest/gtest.h>

namespace {

struct wrong_type {
  static constexpr bool type = 0;
};
struct wrong_fn_type {
  static void type(){};
};
TEST(concept_adt_test, aliasing_type) {
  static_assert(!ctl::aliasing_type<void>);
  static_assert(!ctl::aliasing_type<bool>);
  static_assert(!ctl::aliasing_type<int>);
  static_assert(!ctl::aliasing_type<double>);

  struct no_type {};
  struct yes_type {
    using type = int;
  };
  static_assert(!ctl::aliasing_type<no_type>);
  static_assert(ctl::aliasing_type<yes_type>);
  static_assert(!ctl::aliasing_type<wrong_type>);
  static_assert(!ctl::aliasing_type<wrong_fn_type>);

  static_assert(!ctl::aliasing_type<std::enable_if<false>>);
  static_assert(ctl::aliasing_type<std::enable_if<true>>);
  static_assert(ctl::aliasing_type<std::add_const<int>>);
  static_assert(ctl::aliasing_type<std::add_cv<int>>);
  static_assert(ctl::aliasing_type<std::remove_all_extents<int>>);
}

} // namespace