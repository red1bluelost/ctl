#include "ctl/meta/type_traits.hpp"

#include <gtest/gtest.h>

namespace {
template<typename T> concept has_type = requires { typename T::type; };

TEST(type_traits_test, enable_same) {
  static_assert(has_type<ctl::enable_same<int, int>>);
  static_assert(has_type<ctl::enable_same<bool, bool, long long>>);
  static_assert(!has_type<ctl::enable_same<int, float>>);
  static_assert(!has_type<ctl::enable_same<unsigned, bool, int>>);
  ASSERT_EQ((ctl::enable_same_t<double, double, long>{2}), long{2});
  static_assert(std::is_same_v<ctl::enable_same_t<double, double>, void>);
}
} // namespace