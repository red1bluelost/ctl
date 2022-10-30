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
#include <type_traits>

CTL_BEGIN_NAMESPACE

namespace detail {

template<
    typename To,
    typename From,
    bool = ::ctl::is_lossless_convertible_v<From, To>>
struct llc_impl {
  static To apply(From f) { return To{f}; }
};

template<typename To, typename From>
struct llc_impl<To, From, false> {
  static To apply(From f) {
    // TODO: implement checks
    assert(false);
    return static_cast<To>(f);
  }
};

} // namespace detail

template<
    typename To,
    typename From,
    std::enable_if_t<
        std::conjunction_v<std::is_arithmetic<To>, std::is_arithmetic<From>>,
        int> = 0>
[[nodiscard]] constexpr To lossless_cast(From f) {
  return detail::llc_impl<std::decay_t<To>, std::decay_t<From>>::apply(f);
}

CTL_END_NAMESPACE

#endif // CTL_OBJECT_NUMERICS_HPP
