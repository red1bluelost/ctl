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

#include "ctl_test_util/scoped_trace.hpp"

#include <gtest/gtest.h>

namespace {

struct Base {
  int i = 0;
};
struct DerivedPublic : public Base {};

consteval void test_out_var_consteval_ref(int& i) { i = 3; }
consteval int  testing_out_var_consteval_ref() {
  int i = 0;
  test_out_var_consteval_ref(ctl::out_var(i));
  return i;
}
consteval void test_out_var_consteval_ptr(int* i) { *i = 3; }
consteval int  testing_out_var_consteval_ptr() {
  int i = 0;
  test_out_var_consteval_ptr(ctl::out_var(i));
  return i;
}
constexpr void   test_out_var_constexpr_ref(double& d) { d = 42.1; }
consteval double testing_out_var_constexpr_ref() {
  double d = 0;
  test_out_var_constexpr_ref(ctl::out_var(d));
  return d;
}
constexpr void   test_out_var_constexpr_ptr(double* d) { *d = 42.1; }
consteval double testing_out_var_constexpr_ptr() {
  double d = 0;
  test_out_var_constexpr_ptr(ctl::out_var(d));
  return d;
}
consteval void test_out_var_consteval_base_class_ref(Base& b) { b.i = 21; }
consteval int  testing_out_var_consteval_base_class_ref() {
  DerivedPublic d{};
  test_out_var_consteval_base_class_ref(ctl::out_var(d));
  return d.i;
}
consteval void test_out_var_consteval_base_class_ptr(Base* b) { b->i = 21; }
consteval int  testing_out_var_consteval_base_class_ptr() {
  DerivedPublic d{};
  test_out_var_consteval_base_class_ptr(ctl::out_var(d));
  return d.i;
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

  static_assert(testing_out_var_consteval_ref() == 3);
  static_assert(testing_out_var_constexpr_ref() == 42.1);
  static_assert(testing_out_var_consteval_base_class_ref() == 21);

  static_assert(testing_out_var_consteval_ptr() == 3);
  static_assert(testing_out_var_constexpr_ptr() == 42.1);
  static_assert(testing_out_var_consteval_base_class_ptr() == 21);
}

class out_var_test : public ::testing::Test {
 protected:
  template<typename Tester>
  void run(ctl_tu::src_loc call_loc = ctl_tu::src_loc::current()) {
    auto run_scope_trace =
        ctl_tu::make_scoped_trace(call_loc, "running tests for ctl::out_var");
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
    {
      DerivedPublic d{};
      ASSERT_EQ(d.i, 0);
      Tester::test_base_class(ctl::out_var(d));
      ASSERT_EQ(d.i, 21);
    }
  }

 private:
  testing::ScopedTrace tester_scope_trace = ctl_tu::make_scoped_trace(
      ctl_tu::src_loc::current(), "fixture for testing out_var"
  );
};

TEST_F(out_var_test, function_out_by_ref) {
  struct ref_tester {
    static void test_str(std::string& s) { s = "the result"; }
    static void test_int(int& i) { i = 42; }
    static void test_vector(std::vector<double>& v) { v.push_back(3.4); }
    static void test_int_ptr(int*& ip) {
      static int i = 34;
      ip           = &i;
    }
    static void test_base_class(Base& b) { b.i = 21; }
  };
  run<ref_tester>();
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
    static void test_base_class(Base* b) { b->i = 21; }
  };
  run<ptr_tester>();
}

template<typename T>
struct view_value_ref {
  template<std::common_reference_with<T> CommonRefType>
  view_value_ref(ctl::out_var<CommonRefType> ov) : value(ov) {}
  T& value;
};
TEST_F(out_var_test, function_out_by_view_ref) {
  struct view_tester {
    static void test_str(view_value_ref<std::string> s) {
      s.value = "the result";
    }
    static void test_int(view_value_ref<int> i) { i.value = 42; }
    static void test_vector(view_value_ref<std::vector<double>> v) {
      v.value.push_back(3.4);
    }
    static void test_int_ptr(view_value_ref<int*> ip) {
      static int i = 34;
      ip.value     = &i;
    }
    static void test_base_class(view_value_ref<Base> b) { b.value.i = 21; }
  };
  run<view_tester>();
}

template<typename T>
struct view_value_ptr {
  template<std::common_reference_with<T> CommonRefType>
  view_value_ptr(ctl::out_var<CommonRefType> ov) : value(ov) {}
  T* value;
};
TEST_F(out_var_test, function_out_by_view_ptr) {
  struct view_tester {
    static void test_str(view_value_ptr<std::string> s) {
      *s.value = "the result";
    }
    static void test_int(view_value_ptr<int> i) { *i.value = 42; }
    static void test_vector(view_value_ptr<std::vector<double>> v) {
      v.value->push_back(3.4);
    }
    static void test_int_ptr(view_value_ptr<int*> ip) {
      static int i = 34;
      *ip.value    = &i;
    }
    static void test_base_class(view_value_ptr<Base> b) { b.value->i = 21; }
  };
  run<view_tester>();
}

} // namespace