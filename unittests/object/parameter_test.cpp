//===- parameter_test.cpp - Tests for parameter utilities -------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/object/parameter.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/object/parameter.hpp"

#include <gtest/gtest.h>

namespace {

consteval void test_out_var_consteval(int& i) { i = 3; }
consteval int  testing_out_var_consteval() {
  int i = 0;
  test_out_var_consteval(i);
  return i;
}
constexpr void   test_out_var_constexpr(double& i) { i = 42.1; }
consteval double testing_out_var_constexpr() {
  double d = 0;
  test_out_var_constexpr(d);
  return d;
}
TEST(parameter_test, out_var_constexpr) {
  static constexpr int i = 3;
  static_assert(*ctl::out_var(i).variable == i);

  static constexpr ctl::out_var oi = i;
  static_assert(*oi.variable == i);
  static constexpr const int& ir = oi;
  static_assert(ir == i);
  static constexpr const int* ip = oi;
  static_assert(ip == &i);
  static_assert(*ip == i);

  static_assert(testing_out_var_consteval() == 3);
  static_assert(testing_out_var_constexpr() == 42.1);
}

class out_var_test : public ::testing::Test {
 protected:
  template<typename Tester>
  void _run(Tester, const char* file, int line) {
    testing::ScopedTrace t(file, line, "running tests for ctl::out_var");
    {
      std::string s;
      ASSERT_EQ(s, "");
      Tester::test_str(ctl::out_var(s));
      ASSERT_EQ(s, "the result");
    }
    {
      int i = 0;
      ASSERT_EQ(i, 0);
      Tester::test_int(ctl::out_var(i));
      ASSERT_EQ(i, 42);
    }
    {
      std::vector<double> v;
      ASSERT_EQ(v.size(), 0);
      Tester::test_vector(ctl::out_var(v));
      ASSERT_EQ(v.size(), 1);
      ASSERT_EQ(v[0], 3.4);
    }
    {
      int* ip = nullptr;
      ASSERT_EQ(ip, nullptr);
      Tester::test_int_ptr(ctl::out_var(ip));
      ASSERT_NE(ip, nullptr);
      ASSERT_EQ(*ip, 34);
    }
  }
};

#define run(...) _run(__VA_ARGS__, __FILE__, __LINE__)

TEST_F(out_var_test, function_out_by_ref) {
  struct ref_tester {
    static void test_str(std::string& s) { s = "the result"; }
    static void test_int(int& i) { i = 42; }
    static void test_vector(std::vector<double>& v) { v.push_back(3.4); }
    static void test_int_ptr(int*& ip) {
      static int i = 34;
      ip           = &i;
    }
  };
  run(ref_tester{});
}

TEST_F(out_var_test, function_out_by_ptr) {
  struct ptr_tester {
    static void test_str(std::string* s) { *s = "the result"; }
    static void test_int(int* i) { *i = 42; }
    static void test_vector(std::vector<double>* v) { v->push_back(3.4); }
    static void test_int_ptr(int** ip) {
      static int i = 34;
      *ip          = &i;
    }
  };
  run(ptr_tester{});
}

template<typename T>
struct view_value {
  view_value(ctl::out_var<T> ov) : value(ov) {}
  T& value;
};
TEST_F(out_var_test, function_out_by_view) {
  struct view_tester {
    static void test_str(view_value<std::string> s) { s.value = "the result"; }
    static void test_int(view_value<int> i) { i.value = 42; }
    static void test_vector(view_value<std::vector<double>> v) {
      v.value.push_back(3.4);
    }
    static void test_int_ptr(view_value<int*> ip) {
      static int i = 34;
      ip.value     = &i;
    }
  };
  run(view_tester{});
}

} // namespace