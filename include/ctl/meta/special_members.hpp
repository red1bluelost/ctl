//===- ctl/meta/special_members.hpp - Special member utilities --*- C++ -*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Meta types and utilities relating to class special member functions.
///
//===----------------------------------------------------------------------===//

#ifndef CTL_META_SPECIAL_MEMBERS_HPP
#define CTL_META_SPECIAL_MEMBERS_HPP

namespace ctl {

/// \brief A meta template that can be used for disabling certain special member
/// functions. This will typically be used for generic types that conditionally
/// allow special members based on the template type.
///
/// \tparam copy_construct Whether to allow a copy constructor.
/// \tparam move_construct Whether to allow a move constructor.
/// \tparam copy_assign Whether to allow a copy assignment operator.
/// \tparam move_assign Whether to allow a move assignment operator.
/// \tparam tag A tag which can make the specialization unique.
template<
    bool copy_construct,
    bool move_construct,
    bool copy_assign,
    bool move_assign,
    typename tag = void>
struct with_copy_move {};

template<typename tag>
struct with_copy_move<true, true, true, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<true, true, false, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<true, true, false, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<true, false, true, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<true, false, true, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<true, false, false, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<true, false, false, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = default;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<false, true, true, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<false, true, true, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<false, true, false, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<false, true, false, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = default;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<false, false, true, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<false, false, true, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = default;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

template<typename tag>
struct with_copy_move<false, false, false, true, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = default;
};

template<typename tag>
struct with_copy_move<false, false, false, false, tag> {
  constexpr with_copy_move() noexcept                                 = default;
  constexpr with_copy_move(const with_copy_move&) noexcept            = delete;
  constexpr with_copy_move(with_copy_move&&) noexcept                 = delete;
  constexpr with_copy_move& operator=(const with_copy_move&) noexcept = delete;
  constexpr with_copy_move& operator=(with_copy_move&&) noexcept      = delete;
};

/// \brief A meta type that only has move constructors and assignment enabled,
/// no copy. This can be inherited from to easily disable copying.
///
/// \tparam tag A tag which can make the specialization unique.
template<typename tag = void>
struct only_move_members
    : private with_copy_move<false, true, false, true, tag> {};

/// \brief A meta type that can be inherited from to disable a given class,
/// typically in use during template specializations.
///
/// Example usage:
/// \code
/// template<typename T, bool = IsReversible<T>>
/// struct Reverser : ctl::disable_class {};
///
/// template<typename T>
/// struct Reverser<T, true> { T reverse(T); };
/// \endcode
///
/// \tparam tag A tag which can make the specialization unique.
template<typename tag = void>
struct disable_class : with_copy_move<false, false, false, false, tag> {
  disable_class()  = delete;
  ~disable_class() = delete;
};

} // namespace ctl

#endif // CTL_META_SPECIAL_MEMBERS_HPP
