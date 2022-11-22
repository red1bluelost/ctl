//===- view.hpp - Combining views of containers -----------------*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Provides a facility to combine views of containers.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_ADT_CONTAINER_VIEW_HPP
#define CTL_ADT_CONTAINER_VIEW_HPP

#include "ctl/config.h"

#include <type_traits>

CTL_BEGIN_NAMESPACE

enum class cvt {
  none      = 0,
  push_back = 1 << 1,
};

#define CTL_BITMASK_ENUM cvt
#include "ctl/object/bitmask_enum.def"

namespace container {

namespace detail {

template<
    cvt individual,
    cvt collection,
    template<typename, typename>
    class base_with,
    template<typename, typename>
    class /*base_delete*/,
    bool = (individual & collection) != cvt::none>
struct with_view {
  template<typename Base, typename T>
  using type = base_with<Base, T>;
};

template<
    cvt individual,
    cvt collection,
    template<typename, typename>
    class base_with,
    template<typename, typename>
    class base_delete>
struct with_view<individual, collection, base_with, base_delete, false> {
  template<typename Base, typename T>
  using type = base_delete<Base, T>;
};

template<typename T, cvt view_types>
class view_impl
    : private with_view<
          cvt::push_back,
          view_types,
          pbv_copy_impl,
          pbv_copy_delete>::template type<view_impl<T, view_types>, T>
    , private with_view<
          cvt::push_back,
          view_types,
          pbv_move_impl,
          pbv_move_delete>::template type<view_impl<T, view_types>, T> {
  using Self = view_impl<T, view_types>;

  using push_back_copy_base = typename with_view<
      view_types,
      cvt::push_back,
      pbv_copy_impl,
      pbv_copy_delete>::template type<Self, T>;
  friend push_back_copy_base;

  using push_back_move_base = typename with_view<
      view_types,
      cvt::push_back,
      pbv_move_impl,
      pbv_move_delete>::template type<Self, T>;
  friend push_back_move_base;

 public:
  template<typename Container>
  view_impl(Container& container)
      : push_back_copy_base(detail::construction_tag<Container>{})
      , push_back_move_base(detail::construction_tag<Container>{})
      , container_handle(&container) {}

  /// \brief Deferring constructor that supports \c ctl::out_var.
  ///
  /// \tparam Container The type of container which will be viewed
  /// \param container An \c out_var wrapper of the container to be viewed
  template<typename Container>
  view_impl(::ctl::out_var<Container> container)
      : view_impl(*container.variable) {}

  using push_back_copy_base::push_back;
  using push_back_move_base::push_back;

 private:
  /// \brief Type erased pointer to the container being viewed. Methods must
  /// use call backs that cast the handle back to the correct type.
  void* container_handle;
};

} // namespace detail

template<typename T>
struct view {
  template<cvt... view_types>
  using of = detail::view_impl<T, (cvt::none | ... | view_types)>;
};

} // namespace container

CTL_END_NAMESPACE

#endif // CTL_ADT_CONTAINER_VIEW_HPP
