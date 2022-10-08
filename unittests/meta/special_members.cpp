//===- special_members.cpp - Tests for special members ----------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/meta/special_members.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/meta/special_members.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace {

TEST(special_members_test, with_copy_move__all_members) {
  {
    using T = ctl::with_copy_move<true, true, true, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, true, true, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__no_move_assign) {
  {
    using T = ctl::with_copy_move<true, true, true, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, true, true, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__no_copy_assign) {
  {
    using T = ctl::with_copy_move<true, true, false, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, true, false, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_con) {
  {
    using T = ctl::with_copy_move<true, true, false, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, true, false, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__no_move_con) {
  {
    using T = ctl::with_copy_move<true, false, true, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, false, true, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_copy) {
  {
    using T = ctl::with_copy_move<true, false, true, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, false, true, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__copy_con_move_assign) {
  {
    using T = ctl::with_copy_move<true, false, false, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, false, false, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_copy_con) {
  {
    using T = ctl::with_copy_move<true, false, false, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<true, false, false, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__no_copy_con) {
  {
    using T = ctl::with_copy_move<false, true, true, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, true, true, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__move_con_copy_assign) {
  {
    using T = ctl::with_copy_move<false, true, true, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, true, true, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_move) {
  {
    using T = ctl::with_copy_move<false, true, false, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, true, false, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_move_con) {
  {
    using T = ctl::with_copy_move<false, true, false, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, true, false, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_assign) {
  {
    using T = ctl::with_copy_move<false, false, true, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, false, true, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_copy_assign) {
  {
    using T = ctl::with_copy_move<false, false, true, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, false, true, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__only_move_assign) {
  {
    using T = ctl::with_copy_move<false, false, false, true>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, false, false, true, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

TEST(special_members_test, with_copy_move__none) {
  {
    using T = ctl::with_copy_move<false, false, false, false>;
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
  {
    struct T : private ctl::with_copy_move<false, false, false, false, T> {};
    static_assert(std::is_default_constructible_v<T>);
    static_assert(!std::is_copy_constructible_v<T>);
    static_assert(!std::is_move_constructible_v<T>);
    static_assert(!std::is_copy_assignable_v<T>);
    static_assert(!std::is_move_assignable_v<T>);
    static_assert(std::is_destructible_v<T>);
  }
}

} // namespace