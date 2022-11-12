//===- bit_test.cpp - Tests for the bit utilities ---------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tests for 'ctl/object/bit.hpp'.
///
//===----------------------------------------------------------------------===//

#include "ctl/object/bit.hpp"

#include <gtest/gtest.h>

namespace {

constexpr std::size_t ptr_size = sizeof(void*) * CHAR_BIT;

TEST(bit_test, bit_sizeof_macro_with_types) {
  static_assert(CTL_BIT_SIZEOF(uint8_t) == 8);
  static_assert(CTL_BIT_SIZEOF(uint16_t) == 16);
  static_assert(CTL_BIT_SIZEOF(uint32_t) == 32);
  static_assert(CTL_BIT_SIZEOF(uint64_t) == 64);

  static_assert(CTL_BIT_SIZEOF(uint64_t[32]) == 64 * 32);
  static_assert(CTL_BIT_SIZEOF(uint64_t[32][16]) == 64 * 16 * 32);

  static_assert(CTL_BIT_SIZEOF(double) == 64);
  static_assert(CTL_BIT_SIZEOF(const double) == 64);
  static_assert(CTL_BIT_SIZEOF(volatile double) == 64);
  static_assert(CTL_BIT_SIZEOF(const volatile double) == 64);

  static_assert(CTL_BIT_SIZEOF(void*) == ptr_size);
  static_assert(CTL_BIT_SIZEOF(double*) == ptr_size);
  static_assert(CTL_BIT_SIZEOF(int (*)()) == ptr_size);

  static_assert(CTL_BIT_SIZEOF(float*) == ptr_size);
  static_assert(CTL_BIT_SIZEOF(float**) == ptr_size);
  static_assert(CTL_BIT_SIZEOF(const float* const) == ptr_size);

  static_assert(CTL_BIT_SIZEOF(int8_t&) == 8);
  static_assert(CTL_BIT_SIZEOF(int8_t&&) == 8);
  static_assert(CTL_BIT_SIZEOF(float&) == 32);
  static_assert(CTL_BIT_SIZEOF(float&&) == 32);

  struct tester {};
  static_assert(CTL_BIT_SIZEOF(tester) == 8);
  static_assert(CTL_BIT_SIZEOF(tester&) == 8);
  static_assert(CTL_BIT_SIZEOF(tester&&) == 8);
  static_assert(CTL_BIT_SIZEOF(tester*) == ptr_size);
  static_assert(CTL_BIT_SIZEOF(tester**) == ptr_size);
}

TEST(bit_test, bit_sizeof_macro_with_objects) {
  {
    uint8_t  u8{};
    uint16_t u16{};
    uint32_t u32{};
    uint64_t u64{};
    static_assert(CTL_BIT_SIZEOF(u8) == 8);
    static_assert(CTL_BIT_SIZEOF(u16) == 16);
    static_assert(CTL_BIT_SIZEOF(u32) == 32);
    static_assert(CTL_BIT_SIZEOF(u64) == 64);
  }

  {
    uint64_t u64_32[32];
    uint64_t u64_16_32[32][16];
    static_assert(CTL_BIT_SIZEOF(u64_32) == 64 * 32);
    static_assert(CTL_BIT_SIZEOF(*u64_32) == 64);
    static_assert(CTL_BIT_SIZEOF(u64_32[0]) == 64);
    static_assert(CTL_BIT_SIZEOF(u64_16_32) == 64 * 16 * 32);
    static_assert(CTL_BIT_SIZEOF(*u64_16_32) == 64 * 16);
    static_assert(CTL_BIT_SIZEOF(**u64_16_32) == 64);
    static_assert(CTL_BIT_SIZEOF(u64_16_32[0]) == 64 * 16);
    static_assert(CTL_BIT_SIZEOF(u64_16_32[0][0]) == 64);
  }

  {
    double                d{};
    const double          cd{};
    volatile double       vd{};
    const volatile double cvd{};
    static_assert(CTL_BIT_SIZEOF(d) == 64);
    static_assert(CTL_BIT_SIZEOF(cd) == 64);
    static_assert(CTL_BIT_SIZEOF(vd) == 64);
    static_assert(CTL_BIT_SIZEOF(cvd) == 64);
  }

  {
    void*   vp{};
    double* dp{};
    int (*fp)(){};
    static_assert(CTL_BIT_SIZEOF(vp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(dp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(fp) == ptr_size);
  }

  {
    float*             fp{};
    float**            fpp{};
    const float* const cfpc{};
    static_assert(CTL_BIT_SIZEOF(fp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(*fp) == 32);
    static_assert(CTL_BIT_SIZEOF(fpp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(*fpp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(**fpp) == 32);
    static_assert(CTL_BIT_SIZEOF(cfpc) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(*cfpc) == 32);
  }

  {
    int8_t   i{};
    float    f{};
    int8_t&  ir  = i;
    int8_t&& irr = {};
    float&   fr  = f;
    float&&  frr = {};
    static_assert(CTL_BIT_SIZEOF(ir) == 8);
    static_assert(CTL_BIT_SIZEOF(irr) == 8);
    static_assert(CTL_BIT_SIZEOF(fr) == 32);
    static_assert(CTL_BIT_SIZEOF(frr) == 32);
  }

  {
    struct tester {};
    tester   t{};
    tester&  tr  = t;
    tester&& trr = {};
    tester*  tp{};
    tester** tpp{};
    static_assert(CTL_BIT_SIZEOF(t) == 8);
    static_assert(CTL_BIT_SIZEOF(tr) == 8);
    static_assert(CTL_BIT_SIZEOF(trr) == 8);
    static_assert(CTL_BIT_SIZEOF(tp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(*tp) == 8);
    static_assert(CTL_BIT_SIZEOF(tpp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(*tpp) == ptr_size);
    static_assert(CTL_BIT_SIZEOF(**tpp) == 8);
  }
}

} // namespace
