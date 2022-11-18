//===- ctl/object/parameter.hpp - Function parameter utilities --*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Utilities for parameters to methods, functions, constructors, and other
/// callables which take arguments. Helping make intentions more explicit.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_OBJECT_PARAMETER_HPP
#define CTL_OBJECT_PARAMETER_HPP

#include "ctl/config.h"

CTL_BEGIN_NAMESPACE

/// \brief A minimal wrapper that is only used for exposition only. This is
/// meant for use only at a call site to show a reader that the variable will be
/// an out variable.
///
/// A compiler should be able to optimize away its existence. This wrapper will
/// automatically convert to a reference or pointer depending on the function
/// parameter it is passed to.
///
/// Non-owning view objects that can modify the original location should provide
/// a constructor that takes an \c out_var to properly convert. The wrapped
/// variable is left public for this purpose.
///
/// Example usage:
/// \code
/// void process_file_name(int fd, std::string &name);
///
/// std::string name;
/// process_file_name(1, ctl::out_var(name));
/// std::print("file name: {}\n", name);
/// \endcode
///
/// \tparam T Type of variable which will be addressed for the out variable
template<typename T>
struct out_var {
  /// Type aliases common to abstract data types.
  using value_type = T;
  using pointer    = value_type*;
  using reference  = value_type&;

  /// \brief Constructs \c out_var by taking the address of the variable that
  /// will be used.
  ///
  /// \param r Reference to the variable which is being provided as out var
  constexpr out_var(value_type& r) noexcept : variable(&r) {}

  /// \brief Overloads conversion for pointer based out variables.
  ///
  /// \return The address of the variable
  constexpr operator pointer() const noexcept { return variable; }

  /// \brief Overloads conversion for reference based out variables.
  ///
  /// \return Reference to the variable
  constexpr operator reference() const noexcept { return *variable; }

  /// \brief Pointer to allow copy and move assignment to automatically
  /// generate. Public so that abstract data types can provide constructors
  /// taking \c out_var.
  value_type* variable;
};

CTL_END_NAMESPACE

#endif // CTL_OBJECT_PARAMETER_HPP
