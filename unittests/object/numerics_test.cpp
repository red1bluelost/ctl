//===- numerics_test.cpp - Tests for numerics utilities ---------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/object/numerics.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/object/numerics.hpp"

#include <gtest/gtest.h>

namespace {

TEST(numerics_test, lossless_cast_no_loss) {
  {
    int32_t i32_ = std::numeric_limits<int32_t>::min();
    int64_t i64_ = ctl::lossless_cast<int64_t>(i32_);
    ASSERT_EQ(i64_, std::numeric_limits<int32_t>::min());
  }
  {
    uint32_t u32_ = std::numeric_limits<uint32_t>::max();
    int64_t  i64_ = ctl::lossless_cast<int64_t>(u32_);
    ASSERT_EQ(i64_, std::numeric_limits<uint32_t>::max());
  }
}

} // namespace