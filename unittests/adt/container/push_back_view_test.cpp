//===- push_back_view_test.cpp - Tests for push_back_view -------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/adt/container/push_back_view.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/adt/container/push_back_view.hpp"

#include "ctl/meta/special_members.hpp"
#include "test_utilities.hpp"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <deque>
#include <list>

namespace {

using ::testing::ElementsAre;

//===----------------------------------------------------------------------===//
// Test to show an intended use case for push_back_view.
//===----------------------------------------------------------------------===//

TEST(push_back_view_test, intented_usage_example) {
  struct tester {
    static void find_values(
        const std::vector<int>&             some_data_to_look_through,
        ctl::container::push_back_view<int> output
    ) {
      for (int i : some_data_to_look_through)
        if (i % 2 == 0) output.push_back(i);
    }
  };

  {
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> output{};

    tester::find_values(input, output);

    ASSERT_THAT(output, ElementsAre(2, 4, 6, 8));
  }
  {
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> output{};

    tester::find_values(input, ctl::out_var(output));

    ASSERT_THAT(output, ElementsAre(2, 4, 6, 8));
  }
}

//===----------------------------------------------------------------------===//
// Tests relating to the specific details of push_back_view.
//===----------------------------------------------------------------------===//

TEST(push_back_view_test, view_size) {
  static_assert(
      sizeof(ctl::container::push_back_view<int>) == sizeof(void*) * 3
  );
  static_assert(
      sizeof(ctl::container::push_back_view<std::string>) == sizeof(void*) * 3
  );
  static_assert(
      sizeof(ctl::container::push_back_view<std::unique_ptr<int>>) ==
      sizeof(void*) * 2
  );

  // Both copy and move operations
  static_assert(
      sizeof(ctl::container::push_back_view<
             ctl::with_copy_move<true, true, true, true>>) == sizeof(void*) * 3
  );
  // Only move operations
  static_assert(
      sizeof(ctl::container::push_back_view<
             ctl::with_copy_move<false, true, false, true>>) ==
      sizeof(void*) * 2
  );
  // Only copy operations
  static_assert(
      sizeof(ctl::container::push_back_view<
             ctl::with_copy_move<true, false, true, false>>) ==
      sizeof(void*) * 2
  );
  // Neither copy nor move operations
  static_assert(
      sizeof(ctl::container::push_back_view<
             ctl::with_copy_move<false, false, false, false>>) ==
      sizeof(void*) * 1
  );

  // Only copy defaulted operations
  static_assert(
      sizeof(ctl::container::push_back_view<copy_default_type>) ==
      sizeof(void*) * 3
  );
}

TEST(push_back_view_test, view_tester) {
  push_back_view_tester<ctl::container::push_back_view> tester(__FILE__);
  tester.run<std::vector>(__LINE__);
  tester.run<std::deque>(__LINE__);
  tester.run<std::list>(__LINE__);
  tester.run<std::list>(__LINE__);
}

} // namespace