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

TEST(optional_test, bad_optional_access) {
  static_assert(std::is_base_of_v<std::exception, ctl::bad_optional_access>);

  try {
    throw ctl::bad_optional_access();
  } catch (const std::exception& e) {
    ASSERT_EQ(e.what(), "ctl :: bad optional access");
  }

  {
    // Quirk of copy constructor
    struct child : ctl::bad_optional_access {
      ~child() noexcept override = default;
      const char* what() const noexcept override { return "this was derived"; }
    };
    ctl::bad_optional_access parent = child{};
    ASSERT_EQ(parent.what(), "ctl :: bad optional access");
    parent = child{};
    ASSERT_EQ(parent.what(), "ctl :: bad optional access");
  }
}

} // namespace