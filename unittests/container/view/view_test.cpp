//===- view_test.cpp - Tests for combining view -----------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// TODO: description
///
//===----------------------------------------------------------------------===//

#include "ctl/container/view/view.hpp"

#include "ctl/meta/special_members.hpp"
#include "test_utilities.hpp"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <deque>
#include <list>

namespace {

//===----------------------------------------------------------------------===//
// Tests relating to the specific details of composition views.
//===----------------------------------------------------------------------===//

template<typename T>
using push_back_only_composition =
    typename ctl::container::view<T>::template of<ctl::cvt::push_back>;
TEST(composition_view_test, push_back_only_size) {
  assert_num_ptrs<push_back_only_composition<int>, 3>();
  assert_num_ptrs<push_back_only_composition<std::string>, 3>();
  assert_num_ptrs<push_back_only_composition<std::unique_ptr<int>>, 2>();

  // Both copy and move operations
  assert_num_ptrs<
      push_back_only_composition<ctl::with_copy_move<true, true, true, true>>,
      3>();
  // Only move operations
  assert_num_ptrs<
      push_back_only_composition<ctl::with_copy_move<false, true, false, true>>,
      2>();
  // Only copy operations
  assert_num_ptrs<
      push_back_only_composition<ctl::with_copy_move<true, false, true, false>>,
      2>();
  // Neither copy nor move operations
  assert_num_ptrs<
      push_back_only_composition<
          ctl::with_copy_move<false, false, false, false>>,
      1>();

  // Only copy defaulted operations
  assert_num_ptrs<push_back_only_composition<copy_default_type>, 3>();
}
TEST(composition_view_test, push_back_only_view) {
  push_back_view_tester<push_back_only_composition> tester;
  tester.run<std::vector>();
  tester.run<std::deque>();
  tester.run<std::list>();
  tester.run<std::list>();
}

} // namespace