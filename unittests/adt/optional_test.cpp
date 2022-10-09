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

//===----------------------------------------------------------------------===//
// Tests for nullopt and nullopt_t.
//===----------------------------------------------------------------------===//

TEST(optional_nullopt_test, nullopt_t) {
  static_assert(!std::default_initializable<ctl::nullopt_t>);

  static_assert(std::copy_constructible<ctl::nullopt_t>);
  static_assert(std::move_constructible<ctl::nullopt_t>);

  static_assert(std::constructible_from<
                ctl::nullopt_t,
                ctl::nullopt_t::construction>);

  static_assert(sizeof(ctl::nullopt_t) == 1);
}

TEST(optional_nullopt_test, nullopt) {
  ctl::nullopt_t temp  = ctl::nullopt;
  ctl::nullopt_t moved = std::move(temp);
  (void)moved;
}

//===----------------------------------------------------------------------===//
// Tests for bad_optional_access.
//===----------------------------------------------------------------------===//

TEST(optional_exception_test, bad_optional_access) {
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

//===----------------------------------------------------------------------===//
// Tests for optional.
//===----------------------------------------------------------------------===//

template<typename... T>
bool optional_is_nothrow_default_constructible_test() {
  ((void)ctl::optional<T>{}, ...);
  return (std::is_nothrow_default_constructible_v<ctl::optional<T>> && ...);
}

TEST(optional_test, default_constructor) {
  {
    ASSERT_TRUE((optional_is_nothrow_default_constructible_test<
                 int,
                 double,
                 std::string,
                 std::vector<double>,
                 int*,
                 unsigned&>()));
    static_assert(((void)ctl::optional<double>{}, true)); // check constexpr
    ctl::optional<std::valarray<int>> curly_con{};
    ctl::optional<std::vector<int>>   non_curly_con;
    ctl::optional<std::string_view>   implicit_default = {};
    (void)curly_con, (void)non_curly_con, (void)implicit_default;
  }

  // TODO: add tests using predicate methods once those are added.
}

} // namespace