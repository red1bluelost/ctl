//===- ctl/adt/container/view_base.hpp - Common base for views --*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Common code and utilities for all of the container views.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_ADT_CONTAINER_VIEW_BASE_HPP
#define CTL_ADT_CONTAINER_VIEW_BASE_HPP

#include "ctl/config.h"

CTL_BEGIN_NAMESPACE

namespace container::detail {

/// \brief A tag type used for internal construction of view CRTP classes. This
/// allows the container type which is being viewed to be easily passed to and
/// deduced by other constructors.
///
/// \tparam Container The container type that the view is being constructed for
template<typename /*Container*/>
struct construction_tag {};

} // namespace container::detail

CTL_END_NAMESPACE

#endif // CTL_ADT_CONTAINER_VIEW_BASE_HPP
