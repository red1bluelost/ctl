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

#include "ctl/adt/container/view.hpp"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-matchers.h>
#include <gtest/gtest.h>

#include <deque>

namespace {

using ::testing::ElementsAre, ::testing::IsEmpty, ::testing::Pointee;

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

// TODO: improve testing approach probably by using a fixture

TEST(push_back_view_test, vector) {
  {
    std::vector<int> v;
    {
      ctl::container::push_back_view<int> pv{v};
      static_assert(sizeof(pv) == sizeof(void*) * 3);
      (void)pv;
    }
    ASSERT_THAT(v, IsEmpty());
    ASSERT_THAT(v, ElementsAre());
  }

  {
    std::vector<int> v;
    {
      ctl::container::view<int>::of<ctl::cvt::push_back> pv{v};
      static_assert(sizeof(pv) == sizeof(void*) * 3);
      pv.push_back(0);
      const int i = 1;
      pv.push_back(i);
      pv.push_back(2);
    }
    ASSERT_THAT(v, ElementsAre(0, 1, 2));
  }
  {
    std::vector<std::string> v;
    {
      ctl::container::push_back_view<std::string> pv{v};
      static_assert(sizeof(pv) == sizeof(void*) * 3);
      const std::string s = "hello";
      pv.push_back(s);
      pv.push_back("this is a test");
    }
    ASSERT_THAT(v, ElementsAre("hello", "this is a test"));
  }
  {
    std::vector<std::unique_ptr<char>> v;
    {
      ctl::container::push_back_view<std::unique_ptr<char>> pv{v};
      static_assert(sizeof(pv) == sizeof(void*) * 2);
      pv.push_back(std::make_unique<char>('a'));
      pv.push_back(std::make_unique<char>('b'));
    }
    ASSERT_THAT(v, ElementsAre(Pointee('a'), Pointee('b')));
  }
}

TEST(push_back_view_test, deque) {
  {
    std::deque<int> v;
    {
      ctl::container::push_back_view<int> pv{v};
      (void)pv;
    }
    ASSERT_THAT(v, IsEmpty());
    ASSERT_THAT(v, ElementsAre());
  }

  {
    std::deque<int> v;
    {
      ctl::container::push_back_view<int> pv{v};
      pv.push_back(0);
      const int i = 1;
      pv.push_back(i);
      pv.push_back(2);
    }
    ASSERT_THAT(v, ElementsAre(0, 1, 2));
  }
  {
    std::deque<std::string> v;
    {
      ctl::container::push_back_view<std::string> pv{v};
      const std::string s = "hello";
      pv.push_back(s);
      pv.push_back("this is a test");
    }
    ASSERT_THAT(v, ElementsAre("hello", "this is a test"));
  }
  {
    std::deque<std::unique_ptr<char>> v;
    {
      ctl::container::push_back_view<std::unique_ptr<char>> pv{v};
      static_assert(sizeof(pv) == sizeof(void*) * 2);
      pv.push_back(std::make_unique<char>('a'));
      pv.push_back(std::make_unique<char>('b'));
    }
    ASSERT_THAT(v, ElementsAre(Pointee('a'), Pointee('b')));
  }
}

} // namespace