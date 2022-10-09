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

#include "ctl/config.h"

#include <exception>
#include <optional>

CTL_BEGIN_NAMESPACE

/// \brief Literal type which can be used for constructing empty optionals.
struct nullopt_t {
  /// \brief Arbitrary enum used for constructing \c nullopt_t.
  enum class construction { tag };
  /// \brief Constructor to disable the default constructor.
  explicit constexpr nullopt_t(construction) {}
};

/// \brief Object that can be used when constructing empty optionals.
inline constexpr nullopt_t nullopt{nullopt_t::construction::tag};

/// \brief Exception for any misuse of the optional class
class bad_optional_access : public std::exception {
 public:
  explicit bad_optional_access() noexcept  = default;
  ~bad_optional_access() noexcept override = default;

  const char* what() const noexcept override {
    return "ctl :: bad optional access";
  }
};

CTL_END_NAMESPACE

#endif // CTL_ADT_OPTIONAL_HPP
