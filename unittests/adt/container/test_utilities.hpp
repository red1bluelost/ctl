//===- test_utilities.hpp - Utilities for container tests -------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Utilities and test fixtures for the containers and views.
///
//===----------------------------------------------------------------------===//

#ifndef UNITTESTS_ADT_CONTAINER_TEST_UTILITIES_HPP
#define UNITTESTS_ADT_CONTAINER_TEST_UTILITIES_HPP

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-matchers.h>
#include <gtest/gtest.h>

struct copy_default_type {
  copy_default_type()                                    = default;
  copy_default_type(const copy_default_type&)            = default;
  copy_default_type& operator=(const copy_default_type&) = default;
};

template<template<typename> class View>
class push_back_view_tester {
 public:
  constexpr push_back_view_tester(const char* file_) noexcept : file(file_) {}

  template<template<typename> class Container>
  void run(int line) {
    using ::testing::ElementsAre, ::testing::IsEmpty, ::testing::SizeIs,
        ::testing::Pointee;

    testing::ScopedTrace _tr(file, line, "running tests for a push_back view");
    {
      Container<int> v;
      {
        View<int> pv{v};
        (void)pv;
      }
      ASSERT_THAT(v, IsEmpty());
      ASSERT_THAT(v, ElementsAre());
    }
    {
      Container<int> v;
      {
        View<int> pv{v};
        pv.push_back(0);
        const int i = 1;
        pv.push_back(i);
        pv.push_back(2);
      }
      ASSERT_THAT(v, ElementsAre(0, 1, 2));
    }
    {
      Container<std::string> v;
      {
        View<std::string> pv{v};
        const std::string s = "hello";
        pv.push_back(s);
        pv.push_back("this is a test");
      }
      ASSERT_THAT(v, ElementsAre("hello", "this is a test"));
    }
    {
      Container<std::unique_ptr<char>> v;
      {
        View<std::unique_ptr<char>> pv{v};
        pv.push_back(std::make_unique<char>('a'));
        pv.push_back(std::make_unique<char>('b'));
      }
      ASSERT_THAT(v, ElementsAre(Pointee('a'), Pointee('b')));
    }
    {
      Container<copy_default_type> v;
      {
        View<copy_default_type> pv{v};
        copy_default_type       c{};
        pv.push_back(c);
        pv.push_back(copy_default_type{});
      }
      ASSERT_THAT(v, SizeIs(2));
    }
    {
      using type = ctl::with_copy_move<true, false, true, false>;
      Container<type> v;
      {
        View<type> pv{v};
        type       t{};
        pv.push_back(t);
      }
      ASSERT_THAT(v, SizeIs(1));
    }
  }

 private:
  const char* file;
};

template<typename T, std::size_t num_pointers>
consteval void assert_num_ptrs() {
  static_assert(sizeof(T) == sizeof(void*) * num_pointers);
}

#endif // UNITTESTS_ADT_CONTAINER_TEST_UTILITIES_HPP
