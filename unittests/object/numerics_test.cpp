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

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <random>
#include <ranges>

namespace {

//===----------------------------------------------------------------------===//
// Utilities for these tests.
//===----------------------------------------------------------------------===//

/// \brief Global constant for the number of times to repeat random based tests.
static constexpr int test_repeats = 100;

//===----------------------------------------------------------------------===//
// Utilities for conversion tests.
//===----------------------------------------------------------------------===//

/// \brief Checks \c ctl::lossless_cast at compile time.
#define STATIC_CHECK_CONV(_type1_, _type2_, _expr_)                            \
  do {                                                                         \
    constexpr _type1_ input  = static_cast<_type1_>(_expr_);                   \
    constexpr _type2_ output = ctl::lossless_cast<_type2_>(input);             \
    static_assert(output == input);                                            \
  } while (false)

/// \brief Checks \c ctl::lossless_cast at runtime time.
#define CHECK_CONV(_type1_, _type2_, _expr_)                                   \
  do {                                                                         \
    _type1_ input  = static_cast<_type1_>(_expr_);                             \
    _type2_ output = ctl::lossless_cast<_type2_>(input);                       \
    ASSERT_EQ(output, input);                                                  \
  } while (false)

/// \brief Checks \c ctl::lossless_cast at compile and runtime time.
#define SAR_CHECK_CONV(_type1_, _type2_, _expr_)                               \
  do {                                                                         \
    STATIC_CHECK_CONV(_type1_, _type2_, _expr_);                               \
    CHECK_CONV(_type1_, _type2_, _expr_);                                      \
  } while (false)

/// \brief Checks \c ctl::lossless_cast at runtime time where input is NaN.
#define CHECK_CONV_NAN(_type1_, _type2_, _expr_)                               \
  do {                                                                         \
    _type1_ input  = static_cast<_type1_>(_expr_);                             \
    _type2_ output = ctl::lossless_cast<_type2_>(input);                       \
    ASSERT_THAT(output, ::testing::IsNan());                                   \
  } while (false)

/// \brief Checks \c ctl::lossless_cast at compile time to ensure a throw.
template<typename T1, typename T2>
void check_failure(auto val) {
  T1 input = static_cast<T1>(val);
  ASSERT_THROW((void)ctl::lossless_cast<T2>(input), std::range_error);
}

TEST(numerics_lossless_cast_test, never_loss_integral) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(ctl::is_lossless_convertible_v<_type1_, _type2_>);           \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::min());     \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::max());     \
                                                                               \
    std::uniform_int_distribution<_type1_> dist;                               \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(uint64_t, uint64_t);
  CHECK_NOLOSS(uint32_t, uint64_t);
  CHECK_NOLOSS(uint16_t, uint64_t);
  CHECK_NOLOSS(uint8_t, uint64_t);

  CHECK_NOLOSS(uint32_t, uint32_t);
  CHECK_NOLOSS(uint16_t, uint32_t);
  CHECK_NOLOSS(uint8_t, uint32_t);

  CHECK_NOLOSS(uint16_t, uint16_t);
  CHECK_NOLOSS(uint8_t, uint16_t);

  CHECK_NOLOSS(uint8_t, uint8_t);

  CHECK_NOLOSS(int64_t, int64_t);
  CHECK_NOLOSS(uint32_t, int64_t);
  CHECK_NOLOSS(int32_t, int64_t);
  CHECK_NOLOSS(uint16_t, int64_t);
  CHECK_NOLOSS(int16_t, int64_t);
  CHECK_NOLOSS(uint8_t, int64_t);
  CHECK_NOLOSS(int8_t, int64_t);

  CHECK_NOLOSS(int32_t, int32_t);
  CHECK_NOLOSS(uint16_t, int32_t);
  CHECK_NOLOSS(int16_t, int32_t);
  CHECK_NOLOSS(uint8_t, int32_t);
  CHECK_NOLOSS(int8_t, int32_t);

  CHECK_NOLOSS(int16_t, int16_t);
  CHECK_NOLOSS(uint8_t, int16_t);
  CHECK_NOLOSS(int8_t, int16_t);

  CHECK_NOLOSS(int8_t, int8_t);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, never_loss_floating_point) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(ctl::is_lossless_convertible_v<_type1_, _type2_>);           \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::epsilon()); \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::min());     \
    SAR_CHECK_CONV(                                                            \
        _type1_, _type2_, std::numeric_limits<_type1_>::denorm_min()           \
    );                                                                         \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::lowest());  \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::max());     \
    SAR_CHECK_CONV(                                                            \
        _type1_, _type2_, std::numeric_limits<_type1_>::infinity()             \
    );                                                                         \
                                                                               \
    CHECK_CONV_NAN(                                                            \
        _type1_, _type2_, std::numeric_limits<_type1_>::quiet_NaN()            \
    );                                                                         \
    CHECK_CONV_NAN(                                                            \
        _type1_, _type2_, std::numeric_limits<_type1_>::signaling_NaN()        \
    );                                                                         \
                                                                               \
    std::normal_distribution<_type1_> dist(                                    \
        0.0, std::numeric_limits<_type1_>::max() / 4                           \
    );                                                                         \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(long double, long double);
  CHECK_NOLOSS(double, long double);
  CHECK_NOLOSS(float, long double);

  CHECK_NOLOSS(double, double);
  CHECK_NOLOSS(float, double);

  CHECK_NOLOSS(float, float);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, never_loss_integral_to_float) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(ctl::is_lossless_convertible_v<_type1_, _type2_>);           \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::min());     \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type1_>::max());     \
                                                                               \
    for (int i = 0; i < test_repeats; ++i) {                                   \
      CHECK_CONV(                                                              \
          _type1_,                                                             \
          _type2_,                                                             \
          std::numeric_limits<_type1_>::min() + static_cast<_type1_>(i)        \
      );                                                                       \
      CHECK_CONV(                                                              \
          _type1_,                                                             \
          _type2_,                                                             \
          std::numeric_limits<_type1_>::max() - static_cast<_type1_>(i)        \
      );                                                                       \
    }                                                                          \
                                                                               \
    std::uniform_int_distribution<_type1_> dist;                               \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(uint64_t, long double);
  CHECK_NOLOSS(int64_t, long double);
  CHECK_NOLOSS(uint32_t, long double);
  CHECK_NOLOSS(int32_t, long double);
  CHECK_NOLOSS(uint16_t, long double);
  CHECK_NOLOSS(int16_t, long double);
  CHECK_NOLOSS(uint8_t, long double);
  CHECK_NOLOSS(int8_t, long double);

  CHECK_NOLOSS(uint32_t, double);
  CHECK_NOLOSS(int32_t, double);
  CHECK_NOLOSS(uint16_t, double);
  CHECK_NOLOSS(int16_t, double);
  CHECK_NOLOSS(uint8_t, double);
  CHECK_NOLOSS(int8_t, double);

  CHECK_NOLOSS(uint16_t, float);
  CHECK_NOLOSS(int16_t, float);
  CHECK_NOLOSS(uint8_t, float);
  CHECK_NOLOSS(int8_t, float);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, maybe_loss_integral) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    constexpr int64_t low = std::max<int64_t>(                                 \
        static_cast<int64_t>(std::numeric_limits<_type1_>::min()),             \
        static_cast<int64_t>(std::numeric_limits<_type2_>::min())              \
    );                                                                         \
    constexpr uint64_t high = std::min<uint64_t>(                              \
        static_cast<uint64_t>(std::numeric_limits<_type1_>::max()),            \
        static_cast<uint64_t>(std::numeric_limits<_type2_>::max())             \
    );                                                                         \
    SAR_CHECK_CONV(_type1_, _type2_, low);                                     \
    SAR_CHECK_CONV(_type1_, _type2_, high);                                    \
                                                                               \
    std::uniform_int_distribution<_type1_> dist(low, high);                    \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(int64_t, uint64_t);
  CHECK_NOLOSS(int32_t, uint64_t);
  CHECK_NOLOSS(int16_t, uint64_t);
  CHECK_NOLOSS(int8_t, uint64_t);

  CHECK_NOLOSS(uint64_t, uint32_t);
  CHECK_NOLOSS(int64_t, uint32_t);
  CHECK_NOLOSS(int32_t, uint32_t);
  CHECK_NOLOSS(int16_t, uint32_t);
  CHECK_NOLOSS(int8_t, uint32_t);

  CHECK_NOLOSS(uint64_t, uint16_t);
  CHECK_NOLOSS(int64_t, uint16_t);
  CHECK_NOLOSS(int32_t, uint16_t);
  CHECK_NOLOSS(int32_t, uint16_t);
  CHECK_NOLOSS(int16_t, uint16_t);
  CHECK_NOLOSS(int8_t, uint16_t);

  CHECK_NOLOSS(uint64_t, uint8_t);
  CHECK_NOLOSS(int64_t, uint8_t);
  CHECK_NOLOSS(int32_t, uint8_t);
  CHECK_NOLOSS(int32_t, uint8_t);
  CHECK_NOLOSS(uint16_t, uint8_t);
  CHECK_NOLOSS(int16_t, uint8_t);
  CHECK_NOLOSS(int8_t, uint8_t);

  CHECK_NOLOSS(uint64_t, int64_t);

  CHECK_NOLOSS(uint64_t, int32_t);
  CHECK_NOLOSS(int64_t, int32_t);
  CHECK_NOLOSS(uint32_t, int32_t);

  CHECK_NOLOSS(uint64_t, int16_t);
  CHECK_NOLOSS(int64_t, int16_t);
  CHECK_NOLOSS(uint32_t, int16_t);
  CHECK_NOLOSS(int32_t, int16_t);
  CHECK_NOLOSS(uint16_t, int16_t);

  CHECK_NOLOSS(uint64_t, int8_t);
  CHECK_NOLOSS(int64_t, int8_t);
  CHECK_NOLOSS(uint32_t, int8_t);
  CHECK_NOLOSS(int32_t, int8_t);
  CHECK_NOLOSS(uint16_t, int8_t);
  CHECK_NOLOSS(int16_t, int8_t);
  CHECK_NOLOSS(uint8_t, int8_t);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, maybe_loss_floating_point) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type2_>::epsilon()); \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type2_>::min());     \
    SAR_CHECK_CONV(                                                            \
        _type1_, _type2_, std::numeric_limits<_type2_>::denorm_min()           \
    );                                                                         \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type2_>::lowest());  \
    SAR_CHECK_CONV(_type1_, _type2_, std::numeric_limits<_type2_>::max());     \
    SAR_CHECK_CONV(                                                            \
        _type1_, _type2_, std::numeric_limits<_type2_>::infinity()             \
    );                                                                         \
                                                                               \
    std::normal_distribution<_type2_> dist(                                    \
        0.0, std::numeric_limits<_type2_>::max() / 4                           \
    );                                                                         \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(long double, double);

  CHECK_NOLOSS(long double, float);
  CHECK_NOLOSS(double, float);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, maybe_loss_integral_to_float) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    constexpr int64_t low = std::max<int64_t>(                                 \
        static_cast<int64_t>(std::numeric_limits<_type1_>::min()),             \
        -static_cast<int64_t>(                                                 \
            (int64_t{1} << std::numeric_limits<_type2_>::digits) - 1           \
        )                                                                      \
    );                                                                         \
    constexpr uint64_t high = std::min<uint64_t>(                              \
        static_cast<uint64_t>(std::numeric_limits<_type1_>::max()),            \
        static_cast<uint64_t>(                                                 \
            (uint64_t{1} << std::numeric_limits<_type2_>::digits) - 1          \
        )                                                                      \
    );                                                                         \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, low);                                     \
    SAR_CHECK_CONV(_type1_, _type2_, high);                                    \
                                                                               \
    std::uniform_int_distribution<_type1_> dist(low, high);                    \
    for (int i = 0; i < test_repeats; ++i)                                     \
      CHECK_CONV(_type1_, _type2_, dist(engine));                              \
  } while (false)

  CHECK_NOLOSS(uint64_t, double);
  CHECK_NOLOSS(int64_t, double);

  CHECK_NOLOSS(uint64_t, float);
  CHECK_NOLOSS(int64_t, float);
  CHECK_NOLOSS(uint32_t, float);
  CHECK_NOLOSS(int32_t, float);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, maybe_loss_float_to_integral) {
#define CHECK_NOLOSS(_type1_, _type2_)                                         \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    SAR_CHECK_CONV(_type1_, _type2_, 0);                                       \
                                                                               \
    constexpr int64_t low = std::is_signed_v<_type2_> ? -test_repeats : 0;     \
    using type1_alias     = _type1_;                                           \
    for (type1_alias f = static_cast<type1_alias>(low);                        \
         f < static_cast<type1_alias>(low + test_repeats * 2);                 \
         f += type1_alias{1.0})                                                \
      CHECK_CONV(_type1_, _type2_, f);                                         \
  } while (false)

  CHECK_NOLOSS(long double, uint64_t);
  CHECK_NOLOSS(long double, int64_t);
  CHECK_NOLOSS(long double, uint32_t);
  CHECK_NOLOSS(long double, int32_t);
  CHECK_NOLOSS(long double, uint16_t);
  CHECK_NOLOSS(long double, int16_t);
  CHECK_NOLOSS(long double, uint8_t);
  CHECK_NOLOSS(long double, int8_t);

  CHECK_NOLOSS(double, uint64_t);
  CHECK_NOLOSS(double, int64_t);
  CHECK_NOLOSS(double, uint32_t);
  CHECK_NOLOSS(double, int32_t);
  CHECK_NOLOSS(double, uint16_t);
  CHECK_NOLOSS(double, int16_t);
  CHECK_NOLOSS(double, uint8_t);
  CHECK_NOLOSS(double, int8_t);

  CHECK_NOLOSS(float, uint64_t);
  CHECK_NOLOSS(float, int64_t);
  CHECK_NOLOSS(float, uint32_t);
  CHECK_NOLOSS(float, int32_t);
  CHECK_NOLOSS(float, uint16_t);
  CHECK_NOLOSS(float, int16_t);
  CHECK_NOLOSS(float, uint8_t);
  CHECK_NOLOSS(float, int8_t);

#undef CHECK_NOLOSS
}

TEST(numerics_lossless_cast_test, maybe_loss_integral_fail) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_FAILURE(_type1_, _type2_)                                        \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    constexpr int64_t t1_min =                                                 \
        static_cast<int64_t>(std::numeric_limits<_type1_>::min());             \
    constexpr int64_t t2_min =                                                 \
        static_cast<int64_t>(std::numeric_limits<_type2_>::min());             \
    constexpr uint64_t t1_max =                                                \
        static_cast<uint64_t>(std::numeric_limits<_type1_>::max());            \
    constexpr uint64_t t2_max =                                                \
        static_cast<uint64_t>(std::numeric_limits<_type2_>::max());            \
                                                                               \
    constexpr int64_t  low  = std::min<int64_t>(t1_min, t2_min);               \
    constexpr uint64_t high = std::max<uint64_t>(t1_max, t2_max);              \
                                                                               \
    if constexpr (t1_min < t2_min) {                                           \
      check_failure<_type1_, _type2_>(low);                                    \
                                                                               \
      constexpr int64_t low_inside = std::max<int64_t>(t1_min, t2_min);        \
      std::uniform_int_distribution<int64_t> dist(low, low_inside - 1);        \
      for (int i = 0; i < test_repeats; ++i)                                   \
        check_failure<_type1_, _type2_>(dist(engine));                         \
    }                                                                          \
    if constexpr (t1_max > t2_max) {                                           \
      check_failure<_type1_, _type2_>(high);                                   \
                                                                               \
      constexpr uint64_t high_inside = std::min<uint64_t>(t1_max, t2_max);     \
      std::uniform_int_distribution<uint64_t> dist(high_inside + 1, high);     \
      for (int i = 0; i < test_repeats; ++i)                                   \
        check_failure<_type1_, _type2_>(dist(engine));                         \
    }                                                                          \
  } while (false)

  CHECK_FAILURE(int64_t, uint64_t);
  CHECK_FAILURE(int32_t, uint64_t);
  CHECK_FAILURE(int16_t, uint64_t);
  CHECK_FAILURE(int8_t, uint64_t);

  CHECK_FAILURE(uint64_t, uint32_t);
  CHECK_FAILURE(int64_t, uint32_t);
  CHECK_FAILURE(int32_t, uint32_t);
  CHECK_FAILURE(int16_t, uint32_t);
  CHECK_FAILURE(int8_t, uint32_t);

  CHECK_FAILURE(uint64_t, uint16_t);
  CHECK_FAILURE(int64_t, uint16_t);
  CHECK_FAILURE(int32_t, uint16_t);
  CHECK_FAILURE(int32_t, uint16_t);
  CHECK_FAILURE(int16_t, uint16_t);
  CHECK_FAILURE(int8_t, uint16_t);

  CHECK_FAILURE(uint64_t, uint8_t);
  CHECK_FAILURE(int64_t, uint8_t);
  CHECK_FAILURE(int32_t, uint8_t);
  CHECK_FAILURE(int32_t, uint8_t);
  CHECK_FAILURE(uint16_t, uint8_t);
  CHECK_FAILURE(int16_t, uint8_t);
  CHECK_FAILURE(int8_t, uint8_t);

  CHECK_FAILURE(uint64_t, int64_t);

  CHECK_FAILURE(uint64_t, int32_t);
  CHECK_FAILURE(int64_t, int32_t);
  CHECK_FAILURE(uint32_t, int32_t);

  CHECK_FAILURE(uint64_t, int16_t);
  CHECK_FAILURE(int64_t, int16_t);
  CHECK_FAILURE(uint32_t, int16_t);
  CHECK_FAILURE(int32_t, int16_t);
  CHECK_FAILURE(uint16_t, int16_t);

  CHECK_FAILURE(uint64_t, int8_t);
  CHECK_FAILURE(int64_t, int8_t);
  CHECK_FAILURE(uint32_t, int8_t);
  CHECK_FAILURE(int32_t, int8_t);
  CHECK_FAILURE(uint16_t, int8_t);
  CHECK_FAILURE(int16_t, int8_t);
  CHECK_FAILURE(uint8_t, int8_t);

#undef CHECK_FAILURE
}

TEST(numerics_lossless_cast_test, maybe_loss_floating_point_fail) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_FAILURE(_type1_, _type2_)                                        \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    check_failure<_type1_, _type2_>(std::numeric_limits<_type1_>::min());      \
    check_failure<_type1_, _type2_>(std::numeric_limits<_type1_>::max());      \
                                                                               \
    check_failure<_type1_, _type2_>(std::numeric_limits<_type2_>::quiet_NaN()  \
    );                                                                         \
    check_failure<_type1_, _type2_>(                                           \
        std::numeric_limits<_type2_>::signaling_NaN()                          \
    );                                                                         \
                                                                               \
    for (int i = 0; i < test_repeats; ++i) {                                   \
      check_failure<_type1_, _type2_>(                                         \
          std::numeric_limits<_type1_>::max() -                                \
          (std::numeric_limits<_type1_>::epsilon() * static_cast<_type1_>(i))  \
      );                                                                       \
    }                                                                          \
  } while (false)

  CHECK_FAILURE(long double, double);

  CHECK_FAILURE(long double, float);
  CHECK_FAILURE(double, float);

#undef CHECK_FAILURE
}

TEST(numerics_lossless_cast_test, maybe_loss_integral_to_float_fail) {
  std::random_device dev;
  std::mt19937       engine(dev());

#define CHECK_FAILURE(_type1_, _type2_)                                        \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    constexpr int64_t t1_min =                                                 \
        static_cast<int64_t>(std::numeric_limits<_type1_>::min());             \
    constexpr int64_t t2_min = -static_cast<int64_t>(                          \
        (int64_t{1} << std::numeric_limits<_type2_>::digits) - 1               \
    );                                                                         \
    constexpr uint64_t t1_max =                                                \
        static_cast<uint64_t>(std::numeric_limits<_type1_>::max());            \
    constexpr uint64_t t2_max = static_cast<uint64_t>(                         \
        (uint64_t{1} << std::numeric_limits<_type2_>::digits) - 1              \
    );                                                                         \
                                                                               \
    constexpr int64_t  low  = std::min<int64_t>(t1_min, t2_min);               \
    constexpr uint64_t high = std::max<uint64_t>(t1_max, t2_max);              \
                                                                               \
    if constexpr (t1_min < t2_min) {                                           \
      check_failure<_type1_, _type2_>(low);                                    \
                                                                               \
      constexpr int64_t low_inside = std::max<int64_t>(t1_min, t2_min);        \
      std::uniform_int_distribution<int64_t> dist(low, low_inside - 1);        \
      for (int i = 0; i < test_repeats; ++i)                                   \
        check_failure<_type1_, _type2_>(dist(engine));                         \
    }                                                                          \
    if constexpr (t1_max > t2_max) {                                           \
      check_failure<_type1_, _type2_>(high);                                   \
                                                                               \
      constexpr uint64_t high_inside = std::min<uint64_t>(t1_max, t2_max);     \
      std::uniform_int_distribution<uint64_t> dist(high_inside + 1, high);     \
      for (int i = 0; i < test_repeats; ++i)                                   \
        check_failure<_type1_, _type2_>(dist(engine));                         \
    }                                                                          \
  } while (false)

  CHECK_FAILURE(uint64_t, double);
  CHECK_FAILURE(int64_t, double);

  CHECK_FAILURE(uint64_t, float);
  CHECK_FAILURE(int64_t, float);
  CHECK_FAILURE(uint32_t, float);
  CHECK_FAILURE(int32_t, float);

#undef CHECK_FAILURE
}

TEST(numerics_lossless_cast_test, maybe_loss_float_to_integral_fail) {
#define CHECK_FAILURE(_type1_, _type2_)                                        \
  do {                                                                         \
    static_assert(!ctl::is_lossless_convertible_v<_type1_, _type2_>);          \
                                                                               \
    constexpr int64_t low = std::is_signed_v<_type2_> ? -test_repeats : 0;     \
    using type1_alias     = _type1_;                                           \
    for (type1_alias f = static_cast<type1_alias>(low) + type1_alias{0.5};     \
         f <                                                                   \
         static_cast<type1_alias>(low + test_repeats * 2) + type1_alias{0.5};  \
         f += type1_alias{1.0})                                                \
      check_failure<_type1_, _type2_>(f);                                      \
  } while (false)

  CHECK_FAILURE(long double, uint64_t);
  CHECK_FAILURE(long double, int64_t);
  CHECK_FAILURE(long double, uint32_t);
  CHECK_FAILURE(long double, int32_t);
  CHECK_FAILURE(long double, uint16_t);
  CHECK_FAILURE(long double, int16_t);
  CHECK_FAILURE(long double, uint8_t);
  CHECK_FAILURE(long double, int8_t);

  CHECK_FAILURE(double, uint64_t);
  CHECK_FAILURE(double, int64_t);
  CHECK_FAILURE(double, int32_t);
  CHECK_FAILURE(double, uint32_t);
  CHECK_FAILURE(double, uint16_t);
  CHECK_FAILURE(double, int16_t);
  CHECK_FAILURE(double, uint8_t);
  CHECK_FAILURE(double, int8_t);

  CHECK_FAILURE(float, uint64_t);
  CHECK_FAILURE(float, int64_t);
  CHECK_FAILURE(float, uint32_t);
  CHECK_FAILURE(float, int32_t);
  CHECK_FAILURE(float, uint16_t);
  CHECK_FAILURE(float, int16_t);
  CHECK_FAILURE(float, uint8_t);
  CHECK_FAILURE(float, int8_t);

#undef CHECK_FAILURE
}

} // namespace