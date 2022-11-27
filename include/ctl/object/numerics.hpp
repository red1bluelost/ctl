//===- ctl/object/numerics.hpp - Utilities relating to numbers --*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Tools and utilities relating to integral, floating point, and other number
/// types.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_OBJECT_NUMERICS_HPP
#define CTL_OBJECT_NUMERICS_HPP

#include "ctl/config.h"
#include "ctl/meta/type_traits.hpp"

#include <cassert>
#include <concepts>
#include <stdexcept>
#include <type_traits>

CTL_BEGIN_NAMESPACE

namespace detail_llc {

/// \brief Internal helper for \c lossless_cast which is enabled for conversions
/// that are impossible to change the underlying value. Avoids unnecessary
/// checks.
template<
    typename To,
    typename From,
    bool = ::ctl::is_lossless_convertible_v<From, To>>
struct llc_impl {
  static constexpr To apply(From f) { return static_cast<To>(f); }
};

// TODO: change error handling to be more general
/// This is merely a temporary stop gap until ctl except and assert policy is
/// implemented.
#define TEMP_ASSERT(_expr_, _msg_)                                             \
  do {                                                                         \
    if (!(_expr_)) throw ::std::range_error((_msg_));                          \
  } while (false)

// TODO: enable turning off of static_assert's

/// \brief Internal helper for \c lossless_cast which is enabled for conversions
/// that could potentially change the underlying value.
template<typename To, typename From>
struct llc_impl<To, From, false> {
  /// \brief Handles case of float to integral or float to float. This checks
  /// the value by comparing the before and after. Treats NaN to NaN as valid.
  static constexpr To internal(
      bool /*unnecessary*/,
      bool /*unnecessary*/,
      std::true_type /*float_from*/,
      bool /*float_or_integral_to*/,
      From f
  ) {
    To t = static_cast<To>(f);
    // f == f to ignore NaN values
    TEMP_ASSERT(
        (f == static_cast<From>(t) || !(f == f)),
        "float to float conversion lost precision"
    );
    return t;
  }

  /// \brief Handles case of signed integral to float. Checks that the value
  /// falls within the range of the float's digits.
  static constexpr To internal(
      std::true_type /*signed_from*/,
      bool /*unnecessary*/,
      std::false_type /*integral_from*/,
      std::true_type /*float_to*/,
      From f
  ) {
    TEMP_ASSERT(
        (f >= -(From{1} << From{std::numeric_limits<To>::digits}) &&
         f <= (From{1} << From{std::numeric_limits<To>::digits})),
        "int to float conversion lost precision"
    );
    return static_cast<To>(f);
  }

  /// \brief Handles case of unsigned integral to float. Checks that the value
  /// falls within the range of the float's digits.
  static constexpr To internal(
      std::false_type /*unsigned_from*/,
      bool /*unnecessary*/,
      std::false_type /*integral_from*/,
      std::true_type /*float_to*/,
      From f
  ) {
    TEMP_ASSERT(
        (f <= (From{1} << From{std::numeric_limits<To>::digits})),
        "integral input is out of range of float output"
    );
    return static_cast<To>(f);
  }

  /// \brief Handles case of signed to unsigned integral. Checks that the value
  /// is within the output range.
  static constexpr To internal(
      std::true_type /*signed_from*/,
      std::false_type /*unsigned_to*/,
      std::false_type /*integral_from*/,
      std::false_type /*integral_to*/,
      From f
  ) {
    TEMP_ASSERT(
        (f >= static_cast<From>(std::numeric_limits<To>::min()) &&
         (::ctl::is_sizeof_le_v<From, To> ||
          f <= static_cast<From>(std::numeric_limits<To>::max()))),
        "signed integral out of range of unsigned integral"
    );
    return static_cast<To>(f);
  }

  /// \brief Handles case of unsigned to signed integral. Checks that the value
  /// is within the output range.
  static constexpr To internal(
      std::false_type /*unsigned_from*/,
      std::true_type /*signed_to*/,
      std::false_type /*integral_from*/,
      std::false_type /*integral_to*/,
      From f
  ) {
    static_assert(
        ::ctl::is_sizeof_ge_v<From, To>,
        "sizeof(From) should be larger than sizeof(To)"
    );
    TEMP_ASSERT(
        (f <= static_cast<From>(std::numeric_limits<To>::max())),
        "unsigned input outside of range of output range"
    );
    return static_cast<To>(f);
  }

  /// \brief Handles case of signed to signed or unsigned to unsigned integral
  /// conversion. Checks that the input value is in the range of the output
  /// value.
  static constexpr To internal(
      bool /*same_sign*/,
      bool /*same_sign*/,
      std::false_type /*integral_from*/,
      std::false_type /*integral_to*/,
      From f
  ) {
    static_assert(
        ::ctl::is_sizeof_gt_v<From, To>,
        "sizeof(From) should be larger than sizeof(To)"
    );
    TEMP_ASSERT(
        (f >= From{std::numeric_limits<To>::min()} &&
         f <= From{std::numeric_limits<To>::max()}),
        "integral input is out of range of integral output type"
    );
    return static_cast<To>(f);
  }

  /// \brief Entry point that dispatches to helpers based on signedness and
  /// float or integral type of input and output.
  static constexpr To apply(From f) {
    return internal(
        ::std::is_signed<From>{},
        ::std::is_signed<To>{},
        ::std::is_floating_point<From>{},
        ::std::is_floating_point<To>{},
        f
    );
  }
};

} // namespace detail_llc

/// \brief Converts one number type to another while checking to ensure that no
/// loss happens.
///
/// No checks are performed if the From value fits entirely within
/// the range of the To value. Otherwise it performs necessary checks.
///
/// Example usage:
/// \code
/// int character = get_input();
/// char c = ctl::lossless_cast<char>(character);
/// \endcode
///
/// \tparam To The output type which is being converted to
/// \tparam From The input type which is being converted from (easily deduced)
/// \param f The input value to convert from
/// \return The output value converted to which can be converted back to \c f
template<
    typename To,
    typename From,
    std::enable_if_t<
        std::conjunction_v<std::is_arithmetic<To>, std::is_arithmetic<From>>,
        int> = 0>
[[nodiscard]] constexpr To lossless_cast(From f) {
  return detail_llc::llc_impl<std::decay_t<To>, std::decay_t<From>>::apply(f);
}

CTL_END_NAMESPACE

#endif // CTL_OBJECT_NUMERICS_HPP
