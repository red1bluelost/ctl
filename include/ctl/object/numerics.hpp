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

// TODO: change error handling to be more general
/// This is merely a temporary stop gap until ctl except and assert policy is
/// implemented.
#define TEMP_ASSERT(_expr_, _msg_)                                             \
  do {                                                                         \
    if (!(_expr_)) throw ::std::range_error((_msg_));                          \
  } while (false)

// TODO: enable turning off of static_assert's

/// \brief Internal helper for \c lossless_cast implements the conversion and
/// selects only the necessary checks at compile time.
template<typename To, typename From>
class llc_impl {
  /// \brief All conversion cases covered by \c lossless_cast.
  enum class llc_case {
    never_loss,
    float_to_any,
    signed_int_to_float,
    unsigned_int_to_float,
    signed_to_unsigned_int,
    unsigned_to_signed_int,
    same_sign_int_to_int,
  };

  /// \brief Determines the conversion case for \c From value to \c To.
  /// \return The enum case for this conversion scenario
  static constexpr llc_case case_for_this() noexcept {
    if (::ctl::is_lossless_convertible_v<From, To>) return llc_case::never_loss;
    if (std::floating_point<From>) return llc_case::float_to_any;
    if (std::signed_integral<From> && std::floating_point<To>)
      return llc_case::signed_int_to_float;
    if (std::unsigned_integral<From> && std::floating_point<To>)
      return llc_case::unsigned_int_to_float;
    if (std::signed_integral<From> && std::unsigned_integral<To>)
      return llc_case::signed_to_unsigned_int;
    if (std::unsigned_integral<From> && std::signed_integral<To>)
      return llc_case::unsigned_to_signed_int;
    return llc_case::same_sign_int_to_int;
  }

 public:
  /// \brief Entry point that dispatches to helpers based on signedness and
  /// float or integral type of input and output.
  static constexpr To apply(From f) {
    constexpr llc_case lc = case_for_this();

    // Impossible to change underlying value so no checks performed
    if constexpr (lc == llc_case::never_loss) return static_cast<To>(f);

    // Handles case of float to integral or float to float. This checks the
    // value by comparing the before and after. Treats NaN to NaN as valid
    // TODO: maybe don't allow NaN?
    if constexpr (lc == llc_case::float_to_any) {
      const To t = static_cast<To>(f);
      // f == f to ignore NaN values
      TEMP_ASSERT(
          (f == static_cast<From>(t) || !(f == f)),
          "float to float conversion lost precision"
      );
      return t;
    }

    // Handles case of signed integral to float. Checks that the value falls
    // within the range of the float's digits
    if constexpr (lc == llc_case::signed_int_to_float) {
      TEMP_ASSERT(
          (f >= -(From{1} << From{std::numeric_limits<To>::digits}) &&
           f <= (From{1} << From{std::numeric_limits<To>::digits})),
          "integral input is out of range of float output"
      );
      return static_cast<To>(f);
    }

    // Handles case of unsigned integral to float. Checks that the value falls
    // within the range of the float's digits
    if constexpr (lc == llc_case::unsigned_int_to_float) {
      TEMP_ASSERT(
          (f <= (From{1} << From{std::numeric_limits<To>::digits})),
          "integral input is out of range of float output"
      );
      return static_cast<To>(f);
    }

    // Handles case of signed to unsigned integral. Checks that the value is
    // within the output range
    if constexpr (lc == llc_case::signed_to_unsigned_int) {
      TEMP_ASSERT(
          (f >= static_cast<From>(std::numeric_limits<To>::min()) &&
           (::ctl::is_sizeof_le_v<From, To> ||
            f <= static_cast<From>(std::numeric_limits<To>::max()))),
          "signed integral out of range of unsigned integral"
      );
      return static_cast<To>(f);
    }

    // Handles case of unsigned to signed integral. Checks that the value is
    // within the output range
    if constexpr (lc == llc_case::unsigned_to_signed_int) {
      TEMP_ASSERT(
          (f <= static_cast<From>(std::numeric_limits<To>::max())),
          "unsigned input outside of range of output range"
      );
      return static_cast<To>(f);
    }

    // Handles case of integral conversion with same sign. Checks that the input
    // value is in the range of the output value
    if constexpr (lc == llc_case::same_sign_int_to_int) {
      TEMP_ASSERT(
          (f >= From{std::numeric_limits<To>::min()} &&
           f <= From{std::numeric_limits<To>::max()}),
          "integral input is out of range of integral output type"
      );
      return static_cast<To>(f);
    }

    // unreachable
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
template<typename To, typename From>
requires std::is_arithmetic_v<To> && std::is_arithmetic_v<From> &&
         std::same_as<std::decay_t<To>, To>
[[nodiscard]] constexpr To lossless_cast(From f) {
  return detail_llc::llc_impl<std::decay_t<To>, std::decay_t<From>>::apply(f);
}

CTL_END_NAMESPACE

#endif // CTL_OBJECT_NUMERICS_HPP
