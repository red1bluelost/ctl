//===- optional.hpp - Extended optional object ------------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// An optional object where it has a value or nothing. Mimics \c std::optional
/// but with extensions such as monadic functions and optional reference.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_ADT_OPTIONAL_HPP
#define CTL_ADT_OPTIONAL_HPP

#include <optional>

namespace ctl {

/// \brief Literal type which can be used for constructing empty optionals.
struct nullopt_t {
  /// \brief Arbitrary enum used for constructing \c nullopt_t.
  enum class construction { tag };
  /// \brief Constructor to disable the default constructor.
  explicit constexpr nullopt_t(construction) {}
};

/// \brief Object that can be used when constructing empty optionals.
inline constexpr nullopt_t nullopt{nullopt_t::construction::tag};

} // namespace ctl

#endif // CTL_ADT_OPTIONAL_HPP
