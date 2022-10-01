//===- optional_test.cpp - Tests for optional -------------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/adt/optional.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/adt/optional.hpp"

#include <gtest/gtest.h>

#include <concepts>

namespace {

TEST(optional_test, nullopt_t) {
  static_assert(!std::default_initializable<ctl::nullopt_t>);

  static_assert(std::copy_constructible<ctl::nullopt_t>);
  static_assert(std::move_constructible<ctl::nullopt_t>);

  static_assert(std::constructible_from<
                ctl::nullopt_t,
                ctl::nullopt_t::construction>);

  static_assert(sizeof(ctl::nullopt_t) == 1);
}

TEST(optional_test, nullopt) {
  ctl::nullopt_t temp  = ctl::nullopt;
  ctl::nullopt_t moved = std::move(temp);
  (void)moved;
}

} // namespace