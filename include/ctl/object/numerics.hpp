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

// TODO: change error handling to be more general
/// This is merely a temporary stop gap until ctl except and assert policy is
/// implemented.
#define TEMP_ASSERT(_expr_, _msg_)                                             \
  do {                                                                         \
    if (!(_expr_)) throw ::std::range_error((_msg_));                          \
  } while (false)

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
  // Impossible to change underlying value so no checks performed
  if constexpr (CTL::is_lossless_convertible_v<From, To>)
    return static_cast<To>(f);

  // Handles case of float to integral or float to float. This checks the
  // value by comparing the before and after. Treats NaN to NaN as invalid
  // TODO: maybe don't allow NaN?
  else if constexpr (std::floating_point<From>) {
    const To t = static_cast<To>(f);
    TEMP_ASSERT(
        f == static_cast<From>(t), "float to float conversion lost precision"
    );
    return t;
  }

  // Handles case of signed integral to float. Checks that the value falls
  // within the range of the float's digits
  else if constexpr (std::signed_integral<From> && std::floating_point<To>) {
    TEMP_ASSERT(
        (f >= -(From{1} << From{std::numeric_limits<To>::digits}) &&
         f <= (From{1} << From{std::numeric_limits<To>::digits})),
        "integral input is out of range of float output"
    );
    return static_cast<To>(f);
  }

  // Handles case of unsigned integral to float. Checks that the value falls
  // within the range of the float's digits
  else if constexpr (std::unsigned_integral<From> && std::floating_point<To>) {
    TEMP_ASSERT(
        (f <= (From{1} << From{std::numeric_limits<To>::digits})),
        "integral input is out of range of float output"
    );
    return static_cast<To>(f);
  }

  // Handles case of signed to unsigned integral. Checks that the value is
  // within the output range
  else if constexpr (std::signed_integral<From> && std::unsigned_integral<To>) {
    TEMP_ASSERT(
        (f >= static_cast<From>(std::numeric_limits<To>::min()) &&
         (CTL::is_sizeof_le_v<From, To> ||
          f <= static_cast<From>(std::numeric_limits<To>::max()))),
        "signed integral out of range of unsigned integral"
    );
    return static_cast<To>(f);
  }

  // Handles case of unsigned to signed integral. Checks that the value is
  // within the output range
  else if constexpr (std::unsigned_integral<From> && std::signed_integral<To>) {
    TEMP_ASSERT(
        (f <= static_cast<From>(std::numeric_limits<To>::max())),
        "unsigned input outside of range of output range"
    );
    return static_cast<To>(f);
  }

  // Handles case of integral conversion with same sign. Checks that the input
  // value is in the range of the output value
  else if constexpr (std::integral<From> && std::integral<To>) {
    TEMP_ASSERT(
        (f >= From{std::numeric_limits<To>::min()} &&
         f <= From{std::numeric_limits<To>::max()}),
        "integral input is out of range of integral output type"
    );
    return static_cast<To>(f);
  }

  // unreachable
}

#undef TEMP_ASSERT

CTL_END_NAMESPACE

#endif // CTL_OBJECT_NUMERICS_HPP
