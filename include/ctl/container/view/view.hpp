//===- ctl/container/view/view.hpp - Combining container views --*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Provides a facility to combine views of containers.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_CONTAINER_VIEW_VIEW_HPP
#define CTL_CONTAINER_VIEW_VIEW_HPP

#include "ctl/config.h"
#include "ctl/container/view/push_back.hpp"

#include <type_traits>

CTL_BEGIN_NAMESPACE

/// \brief Bitmask enumeration which represents each method that can be viewed
/// for a container. This is used with \c view_of to customize which methods to
/// support.
enum class cvt {
  none      = 0,
  push_back = 1 << 1,
  // TODO: continue adding values as views are supported
};

#define CTL_BITMASK_ENUM cvt
#include "ctl/object/bitmask_enum.def"

namespace container {

namespace detail {

/// \brief Defers base construction for \c view_of to determine which methods to
/// support. This case includes the method and pointer in the base.
///
/// \tparam individual Enum for method which will be selected
/// \tparam collection Bitmask for all methods to support
/// \tparam base_with CRTP class which implements the method and pointer
template<
    cvt individual,
    cvt collection,
    template<typename, typename>
    class base_with,
    template<typename, typename>
    class /*base_delete*/,
    bool = (individual & collection) != cvt::none>
struct with_view {
  /// \brief Instantiates the base class which is chosen.
  template<typename Base, typename T>
  using type = base_with<Base, T>;
};

/// \brief Defers base construction for \c view_of to determine which methods to
/// support. This case deletes the given method.
///
/// \tparam individual Enum for method which will be selected
/// \tparam collection Bitmask for all methods to support
/// \tparam base_with CRTP class which implements the method and pointer
/// \tparam base_delete CRTP class which deletes the method
template<
    cvt individual,
    cvt collection,
    template<typename, typename>
    class base_with,
    template<typename, typename>
    class base_delete>
struct with_view<individual, collection, base_with, base_delete, false> {
  /// \brief Instantiates the base class which is chosen.
  template<typename Base, typename T>
  using type = base_delete<Base, T>;
};

} // namespace detail

/// \brief Private convenience macro for specifying base types for \c view_of.
#define CTL_VBT(_enum_, _impl_, _delete_)                                      \
  detail::with_view<(_enum_), view_types, detail::_impl_, detail::_delete_>::  \
      template type<view_of<T, view_types>, T>

/// \brief View of containers which can customize which methods it supports.
/// This should only be used with a few methods or else the size will become
/// large.
///
/// This is meant for usage in function parameters as the view should never
/// outlive the container being viewed.
///
/// Example usage:
/// \code
/// void find_empty_blocks(
///     const BlockSet& blocks,
///     ctl::container::
///         view_of<Block*, ctl::cvt::push_back | ctl::cvt::maybe_reserve> out
/// );
/// \endcode
///
/// \tparam T Value type of any container which should be viewed
/// \tparam view_types Bitmask of all the methods which should be viewed
template<typename T, cvt view_types>
class view_of
    : private CTL_VBT(cvt::push_back, pbv_copy_impl, pbv_copy_delete)
    , private CTL_VBT(cvt::push_back, pbv_move_impl, pbv_move_delete) {
  using push_back_copy_base =
      typename CTL_VBT(cvt::push_back, pbv_copy_impl, pbv_copy_delete);
  friend push_back_copy_base;

  using push_back_move_base =
      typename CTL_VBT(cvt::push_back, pbv_move_impl, pbv_move_delete);
  friend push_back_move_base;

 public:
  /// \brief Main constructor for taking a view of a container.
  ///
  /// Any constraint needed for the specific type of view is enforced in the
  /// inherited classes.
  ///
  /// \tparam Container Type of container which will be erased
  /// \param container Reference to container object which is being viewed
  template<typename Container>
  view_of(Container& container)
      : push_back_copy_base(detail::construction_tag<Container>{})
      , push_back_move_base(detail::construction_tag<Container>{})
      , container_handle(&container) {}

  /// \brief Deferring constructor that supports \c ctl::out_var.
  ///
  /// \tparam Container The type of container which will be viewed
  /// \param container An \c out_var wrapper of the container to be viewed
  template<typename Container>
  view_of(::ctl::out_var<Container> container) : view_of(*container.variable) {}

  /// \brief Inheriting either copy push back or it is deleted.
  using push_back_copy_base::push_back;
  /// \brief Inheriting either move push back or it is deleted.
  using push_back_move_base::push_back;

 private:
  /// \brief Type erased pointer to the container being viewed. Methods must
  /// use call backs that cast the handle back to the correct type.
  void* container_handle;
};

#undef CTL_VBT

/// \brief Utility for constructing a \c view_of containers. The \c view
/// template specifies the value type while the \c of template specifies all
/// methods to use.
///
/// Example usage:
/// \code
/// void find_empty_blocks(
///     const BlockSet& blocks,
///     ctl::container::view<
///         Block*>::of<ctl::cvt::push_back, ctl::cvt::maybe_reserve> out
/// );
/// \endcode
///
/// \tparam T Value type of any container which should be viewed
template<typename T>
struct view {
  /// \brief Instantiates the \c view_of with the value type and each view
  /// method it should include. These methods are represented in \c cvt.
  ///
  /// \tparam view_types Enums for each method that should be included
  template<cvt... view_types>
  using of = view_of<T, (cvt::none | ... | view_types)>;
};

} // namespace container

CTL_END_NAMESPACE

#endif // CTL_CONTAINER_VIEW_VIEW_HPP
