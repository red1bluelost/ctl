//===- ctl/container/view/push_back.hpp - View of push back ------- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// View of a container which can push back values to a container.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_CONTAINER_VIEW_PUSH_BACK_HPP
#define CTL_CONTAINER_VIEW_PUSH_BACK_HPP

#include "ctl/config.h"
#include "ctl/container/view/base.hpp"
#include "ctl/object/parameter.hpp"

#include <type_traits>
#include <utility>

CTL_BEGIN_NAMESPACE

namespace container {

namespace detail {

/// \brief Empty base class for containers which cannot pass \c value_type to \c
/// push_back. It just deletes the \c push_back method to give better error
/// messages if used.
///
/// \tparam Base The base class which will have the \c container_handle pointer
/// \tparam T The value_type of the container being viewed
template<typename Base, typename T>
struct pbv_copy_delete {
  /// \brief Empty constructor since now \c call_back is used.
  ///
  /// \tparam Container The type of the container being viewed
  template<typename Container>
  explicit pbv_copy_delete(construction_tag<Container>) {}

  /// \brief Empty constructor since now \c call_back is used.
  ///
  /// \tparam Container The type of the container being viewed
  void push_back(const T& val) = delete;
};

/// TODO: Migrate to a macro for making these CRTP classes

/// \brief Empty base class for containers which cannot pass \c value_type to \c
/// push_back. It just deletes the \c push_back method to give better error
/// messages if used.
///
/// \tparam Base The base class which will have the \c container_handle pointer
/// \tparam T The value_type of the container being viewed
template<typename Base, typename T>
struct pbv_copy_impl : private pbv_copy_delete<Base, T> {
  using pbv_copy_delete<Base, T>::pbv_copy_delete;
  using pbv_copy_delete<Base, T>::push_back;
};

/// \brief CRTP class that will add \c push_back of const references as a method
/// if the viewed container has that method.
///
/// \tparam Base The class which inherits from CRTP and has \c container_handle
/// \tparam T The \c value_type of the container being viewed
template<typename Base, typename T>
requires std::is_copy_constructible_v<T>
struct pbv_copy_impl<Base, T> {
  template<typename Container>
  explicit pbv_copy_impl(construction_tag<Container>)
      : call_back([](void* container, const T& val) -> void {
        return static_cast<Container*>(container)->push_back(val);
      }) {}

  /// \brief Defers to \c call_back by passing through the container handle and
  /// the parameters.
  ///
  /// \param val The value being pushed back into the container
  void push_back(const T& val) {
    return call_back(static_cast<Base*>(this)->container_handle, val);
  }

 private:
  /// \brief Pointer to a generated static function that calls \c push_back on
  /// the container in the handle.
  void (*call_back)(void*, const T&);
};

/// \brief Empty base class for containers which cannot pass \c value_type to \c
/// push_back. It just deletes the \c push_back method to give better error
/// messages if used.
///
/// \tparam Base The base class which will have the \c container_handle pointer
/// \tparam T The value_type of the container being viewed
template<typename Base, typename T>
struct pbv_move_delete {
  /// \brief Empty constructor since now \c call_back is used.
  ///
  /// \tparam Container The type of the container being viewed
  template<typename Container>
  explicit pbv_move_delete(construction_tag<Container>) {}

  /// \brief Method does not exist for container so this is deleted.
  void push_back(T&& val) = delete;
};

/// \brief Empty base class for containers which cannot pass \c value_type to \c
/// push_back. It just deletes the \c push_back method to give better error
/// messages if used.
///
/// \tparam Base The base class which will have the \c container_handle pointer
/// \tparam T The value_type of the container being viewed
template<typename Base, typename T>
struct pbv_move_impl : private pbv_move_delete<Base, T> {
  using pbv_move_delete<Base, T>::pbv_move_delete;
  using pbv_move_delete<Base, T>::push_back;
};

/// \brief CRTP class that will add \c push_back of rvalue references as a
/// method if the viewed container has that method.
///
/// \tparam Base The class which inherits from CRTP and has \c container_handle
/// \tparam T The \c value_type of the container being viewed
template<typename Base, std::move_constructible T>
struct pbv_move_impl<Base, T> {
  /// \brief Constructs a view of the passed in container. Generates a static
  /// function that is used as the callback.
  ///
  /// \tparam Container The container type which will be viewed
  template<typename Container>
  explicit pbv_move_impl(construction_tag<Container>)
      : call_back([](void* container, T&& val) -> void {
        return static_cast<Container*>(container)->push_back(std::move(val));
      }) {}

  /// \brief Defers to \c call_back by passing through the container handle and
  /// the parameters.
  ///
  /// \param val The value being pushed back into the container
  void push_back(T&& val) {
    return call_back(
        static_cast<Base*>(this)->container_handle, std::move(val)
    );
  }

 private:
  /// \brief Pointer to a generated static function that calls \c push_back on
  /// the container in the handle.
  void (*call_back)(void*, T&&);
};

} // namespace detail

/// \brief View of a container that has push_back as a member function.
///
/// This can be useful for out variables in APIs that let the caller choose
/// what container to use.
///
/// Example usage:
/// \code
/// void find_empty_blocks(
///     const BlockSet& blocks,
///     ctl::container::push_back_view<Block*> out
/// ) {
///   for (Block* b : blocks)
///     if (b->empty()) out.push_back(b);
/// }
/// \endcode
///
/// \tparam T The type to be pushed into the container, corresponding to the
/// viewed containers \c value_type
template<typename T>
class push_back_view
    : private detail::pbv_copy_impl<push_back_view<T>, T>
    , private detail::pbv_move_impl<push_back_view<T>, T> {
  /// \brief Possibly empty type that will implement push_back for const
  /// references to \c value_type.
  using copy_base = detail::pbv_copy_impl<push_back_view<T>, T>;
  friend copy_base;
  /// \brief Possibly empty type that will implement push_back for rvalue
  /// references to \c value_type.
  using move_base = detail::pbv_move_impl<push_back_view<T>, T>;
  friend move_base;

 public:
  /// \brief The type which can be pushed into the container.
  using value_type = T;

  /// \brief Constructs a view for the passed in container that can only push
  /// back values.
  ///
  /// \note The constructor is not explicit so that call sites are more
  /// convenient through implicit construction. This is okay since the view does
  /// not own anything.
  ///
  /// \warning The \c container must outlive the \c push_back_view or else there
  /// will be a dangling reference.
  ///
  /// \tparam Container The type of container that will be viewed
  /// \param container The container object that will be viewed
  // TODO: add Concepts to ensure that push_back exists
  template<typename Container>
  push_back_view(Container& container)
      : copy_base(detail::construction_tag<Container>{})
      , move_base(detail::construction_tag<Container>{})
      , container_handle(&container) {}

  /// \brief Deferring constructor that supports \c ctl::out_var.
  ///
  /// \tparam Container The type of container which will be viewed
  /// \param container An \c out_var wrapper of the container to be viewed
  template<typename Container>
  push_back_view(CTL::out_var<Container> container)
      : push_back_view(*container.variable) {}

  /// \brief Inheriting either copy push back or it is deleted.
  using copy_base::push_back;
  /// \brief Inheriting either move push back or it is deleted.
  using move_base::push_back;

 private:
  /// \brief Type erased pointer to the container being viewed. Methods must use
  /// call backs that cast the handle back to the correct type.
  void* container_handle;
};

} // namespace container

CTL_END_NAMESPACE

#endif // CTL_CONTAINER_VIEW_PUSH_BACK_HPP
