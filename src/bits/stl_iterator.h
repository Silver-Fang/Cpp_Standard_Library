#pragma once
// Iterators -*- C++ -*-

// Copyright (C) 2001-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/stl_iterator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 *
 *  This file implements reverse_iterator, back_insert_iterator,
 *  front_insert_iterator, insert_iterator, __normal_iterator, and their
 *  supporting functions and overloaded operators.
 */
#ifndef ARDUINO_ARCH_ESP32
#include <bits/stl_iterator_base_types.h> //SAM的此文件依赖此头文件，所以必须前置
#endif
#ifdef ARDUINO_ARCH_AVR
#include <ext/type_traits.h>
#include <bits/move.h>

#if __cplusplus >= 202002L
#include <compare>
#include <new>
#include <bits/exception_defines.h>
#include <bits/stl_construct.h>
#endif

#if __glibcxx_tuple_like  // >= C++23
#include <bits/utility.h> // for tuple_element_t
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup iterators
   * @{
   */

// Ignore warnings about std::iterator.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#if __cplusplus >= 201103L
  // Same as C++14 make_reverse_iterator but used in C++11 mode too.
  template <typename _Iterator>
  inline _GLIBCXX17_CONSTEXPR reverse_iterator<_Iterator>
  __make_reverse_iterator(_Iterator __i)
  {
    return reverse_iterator<_Iterator>(__i);
  }

#ifdef __glibcxx_make_reverse_iterator // C++ >= 14
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 2285. make_reverse_iterator
  /// Generator function for reverse_iterator.
  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR reverse_iterator<_Iterator>
  make_reverse_iterator(_Iterator __i)
  {
    return reverse_iterator<_Iterator>(__i);
  }

#if __cplusplus > 201703L && defined __glibcxx_concepts
  template <typename _Iterator1, typename _Iterator2>
    requires(!sized_sentinel_for<_Iterator1, _Iterator2>)
  inline constexpr bool
      disable_sized_sentinel_for<reverse_iterator<_Iterator1>,
                                 reverse_iterator<_Iterator2>> = true;
#endif // C++20
#endif // __glibcxx_make_reverse_iterator

  template <typename _Iterator>
  _GLIBCXX20_CONSTEXPR auto
  __niter_base(reverse_iterator<_Iterator> __it)
      -> decltype(__make_reverse_iterator(__niter_base(__it.base())))
  {
    return __make_reverse_iterator(__niter_base(__it.base()));
  }

  template <typename _Iterator>
  _GLIBCXX20_CONSTEXPR auto
  __miter_base(reverse_iterator<_Iterator> __it)
      -> decltype(__make_reverse_iterator(__miter_base(__it.base())))
  {
    return __make_reverse_iterator(__miter_base(__it.base()));
  }
#endif // C++11

#pragma GCC diagnostic pop

  /**
   *  @param __x  A container of arbitrary type.
   *  @param __i  An iterator into the container.
   *  @return  An instance of insert_iterator working on @p __x.
   *
   *  This wrapper function helps in creating insert_iterator instances.
   *  Typing the name of the %iterator requires knowing the precise full
   *  type of the container, which can be tedious and impedes generic
   *  programming.  Using this function lets you take advantage of automatic
   *  template parameter deduction, making the compiler match the correct
   *  types for you.
   */
#if __cplusplus > 201703L && defined __glibcxx_concepts
  template <typename _Container>
  [[nodiscard]]
  constexpr insert_iterator<_Container>
  inserter(_Container &__x, std::__detail::__range_iter_t<_Container> __i)
  {
    return insert_iterator<_Container>(__x, __i);
  }
#else
  template <typename _Container>
  _GLIBCXX_NODISCARD inline insert_iterator<_Container>
  inserter(_Container &__x, typename _Container::iterator __i)
  {
    return insert_iterator<_Container>(__x, __i);
  }
#endif

  /// @} group iterators

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  // This iterator adapter is @a normal in the sense that it does not
  // change the semantics of any of the operators of its iterator
  // parameter.  Its primary purpose is to convert an iterator that is
  // not a class, e.g. a pointer, into an iterator that is a class.
  // The _Container parameter exists solely so that different containers
  // using this template can instantiate different types, even if the
  // _Iterator parameter is the same.
  template <typename _Iterator, typename _Container>
  class __normal_iterator
  {
  protected:
    _Iterator _M_current;

    typedef std::iterator_traits<_Iterator> __traits_type;

#if __cplusplus >= 201103L
    template <typename _Iter>
    using __convertible_from = std::__enable_if_t<std::is_convertible<_Iter, _Iterator>::value>;
#endif

  public:
    typedef _Iterator iterator_type;
    typedef typename __traits_type::iterator_category iterator_category;
    typedef typename __traits_type::value_type value_type;
    typedef typename __traits_type::difference_type difference_type;
    typedef typename __traits_type::reference reference;
    typedef typename __traits_type::pointer pointer;

#if __cplusplus > 201703L && __glibcxx_concepts
    using iterator_concept = std::__detail::__iter_concept<_Iterator>;
#endif

    _GLIBCXX_CONSTEXPR __normal_iterator() _GLIBCXX_NOEXCEPT
        : _M_current(_Iterator()) {}

    explicit _GLIBCXX20_CONSTEXPR
    __normal_iterator(const _Iterator &__i) _GLIBCXX_NOEXCEPT
        : _M_current(__i) {}

    // Allow iterator to const_iterator conversion
#if __cplusplus >= 201103L
    template <typename _Iter, typename = __convertible_from<_Iter>>
    _GLIBCXX20_CONSTEXPR
    __normal_iterator(const __normal_iterator<_Iter, _Container> &__i) noexcept
#else
    // N.B. _Container::pointer is not actually in container requirements,
    // but is present in std::vector and std::basic_string.
    template <typename _Iter>
    __normal_iterator(const __normal_iterator<_Iter,
                                              typename __enable_if<
                                                  (std::__are_same<_Iter, typename _Container::pointer>::__value),
                                                  _Container>::__type> &__i)
#endif
        : _M_current(__i.base())
    {
    }

    // Forward iterator requirements
    _GLIBCXX20_CONSTEXPR
    reference
    operator*() const _GLIBCXX_NOEXCEPT
    {
      return *_M_current;
    }

    _GLIBCXX20_CONSTEXPR
    pointer
    operator->() const _GLIBCXX_NOEXCEPT
    {
      return _M_current;
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator &
    operator++() _GLIBCXX_NOEXCEPT
    {
      ++_M_current;
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator
    operator++(int) _GLIBCXX_NOEXCEPT
    {
      return __normal_iterator(_M_current++);
    }

    // Bidirectional iterator requirements
    _GLIBCXX20_CONSTEXPR
    __normal_iterator &
    operator--() _GLIBCXX_NOEXCEPT
    {
      --_M_current;
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator
    operator--(int) _GLIBCXX_NOEXCEPT
    {
      return __normal_iterator(_M_current--);
    }

    // Random access iterator requirements
    _GLIBCXX20_CONSTEXPR
    reference
    operator[](difference_type __n) const _GLIBCXX_NOEXCEPT
    {
      return _M_current[__n];
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator &
    operator+=(difference_type __n) _GLIBCXX_NOEXCEPT
    {
      _M_current += __n;
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator
    operator+(difference_type __n) const _GLIBCXX_NOEXCEPT
    {
      return __normal_iterator(_M_current + __n);
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator &
    operator-=(difference_type __n) _GLIBCXX_NOEXCEPT
    {
      _M_current -= __n;
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    __normal_iterator
    operator-(difference_type __n) const _GLIBCXX_NOEXCEPT
    {
      return __normal_iterator(_M_current - __n);
    }

    _GLIBCXX20_CONSTEXPR
    const _Iterator &
    base() const _GLIBCXX_NOEXCEPT
    {
      return _M_current;
    }
  };

  // Note: In what follows, the left- and right-hand-side iterators are
  // allowed to vary in types (conceptually in cv-qualification) so that
  // comparison between cv-qualified and non-cv-qualified iterators be
  // valid.  However, the greedy and unfriendly operators in std::rel_ops
  // will make overload resolution ambiguous (when in scope) if we don't
  // provide overloads whose operands are of the same type.  Can someone
  // remind me what generic programming is about? -- Gaby

#if __cpp_lib_three_way_comparison
  template <typename _IteratorL, typename _IteratorR, typename _Container>
  [[nodiscard]]
  constexpr bool
  operator==(const __normal_iterator<_IteratorL, _Container> &__lhs,
             const __normal_iterator<_IteratorR, _Container> &__rhs) noexcept(noexcept(__lhs.base() == __rhs.base()))
    requires requires {
      { __lhs.base() == __rhs.base() } -> std::convertible_to<bool>;
    }
  {
    return __lhs.base() == __rhs.base();
  }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  [[nodiscard]]
  constexpr std::__detail::__synth3way_t<_IteratorR, _IteratorL>
  operator<=>(const __normal_iterator<_IteratorL, _Container> &__lhs,
              const __normal_iterator<_IteratorR, _Container> &__rhs) noexcept(noexcept(std::__detail::__synth3way(__lhs.base(), __rhs.base())))
  {
    return std::__detail::__synth3way(__lhs.base(), __rhs.base());
  }

  template <typename _Iterator, typename _Container>
  [[nodiscard]]
  constexpr bool
  operator==(const __normal_iterator<_Iterator, _Container> &__lhs,
             const __normal_iterator<_Iterator, _Container> &__rhs) noexcept(noexcept(__lhs.base() == __rhs.base()))
    requires requires {
      { __lhs.base() == __rhs.base() } -> std::convertible_to<bool>;
    }
  {
    return __lhs.base() == __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  [[nodiscard]]
  constexpr std::__detail::__synth3way_t<_Iterator>
  operator<=>(const __normal_iterator<_Iterator, _Container> &__lhs,
              const __normal_iterator<_Iterator, _Container> &__rhs) noexcept(noexcept(std::__detail::__synth3way(__lhs.base(), __rhs.base())))
  {
    return std::__detail::__synth3way(__lhs.base(), __rhs.base());
  }
#else
  // Forward iterator requirements
  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator==(const __normal_iterator<_IteratorL, _Container> &__lhs,
             const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() == __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator==(const __normal_iterator<_Iterator, _Container> &__lhs,
             const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() == __rhs.base();
  }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator!=(const __normal_iterator<_IteratorL, _Container> &__lhs,
             const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() != __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator!=(const __normal_iterator<_Iterator, _Container> &__lhs,
             const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() != __rhs.base();
  }

  // Random access iterator requirements
  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD inline bool
  operator<(const __normal_iterator<_IteratorL, _Container> &__lhs,
            const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() < __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator<(const __normal_iterator<_Iterator, _Container> &__lhs,
            const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() < __rhs.base();
  }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD inline bool
  operator>(const __normal_iterator<_IteratorL, _Container> &__lhs,
            const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() > __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator>(const __normal_iterator<_Iterator, _Container> &__lhs,
            const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() > __rhs.base();
  }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD inline bool
  operator<=(const __normal_iterator<_IteratorL, _Container> &__lhs,
             const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() <= __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator<=(const __normal_iterator<_Iterator, _Container> &__lhs,
             const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() <= __rhs.base();
  }

  template <typename _IteratorL, typename _IteratorR, typename _Container>
  _GLIBCXX_NODISCARD inline bool
  operator>=(const __normal_iterator<_IteratorL, _Container> &__lhs,
             const __normal_iterator<_IteratorR, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() >= __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline bool
  operator>=(const __normal_iterator<_Iterator, _Container> &__lhs,
             const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() >= __rhs.base();
  }
#endif // three-way comparison

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // According to the resolution of DR179 not only the various comparison
  // operators but also operator- must accept mixed iterator/const_iterator
  // parameters.
  template <typename _IteratorL, typename _IteratorR, typename _Container>
#if __cplusplus >= 201103L
  // DR 685.
  [[__nodiscard__]] _GLIBCXX20_CONSTEXPR inline auto
  operator-(const __normal_iterator<_IteratorL, _Container> &__lhs,
            const __normal_iterator<_IteratorR, _Container> &__rhs) noexcept
      -> decltype(__lhs.base() - __rhs.base())
#else
  inline typename __normal_iterator<_IteratorL, _Container>::difference_type
  operator-(const __normal_iterator<_IteratorL, _Container> &__lhs,
            const __normal_iterator<_IteratorR, _Container> &__rhs)
#endif
  {
    return __lhs.base() - __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline typename __normal_iterator<_Iterator, _Container>::difference_type
  operator-(const __normal_iterator<_Iterator, _Container> &__lhs,
            const __normal_iterator<_Iterator, _Container> &__rhs)
      _GLIBCXX_NOEXCEPT
  {
    return __lhs.base() - __rhs.base();
  }

  template <typename _Iterator, typename _Container>
  _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR inline __normal_iterator<_Iterator, _Container>
  operator+(typename __normal_iterator<_Iterator, _Container>::difference_type
                __n,
            const __normal_iterator<_Iterator, _Container> &__i)
      _GLIBCXX_NOEXCEPT
  {
    return __normal_iterator<_Iterator, _Container>(__i.base() + __n);
  }

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace

namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

#if __cplusplus >= 201103L

  // 24.4.3  Move iterators
  /**
   *  Class template move_iterator is an iterator adapter with the same
   *  behavior as the underlying iterator except that its dereference
   *  operator implicitly converts the value returned by the underlying
   *  iterator's dereference operator to an rvalue reference.  Some
   *  generic algorithms can be called with move iterators to replace
   *  copying with moving.
   */
  template <typename _Iterator>
  class move_iterator
#if __cplusplus > 201703L && __glibcxx_concepts
      : public __detail::__move_iter_cat<_Iterator>
#endif
  {
    _Iterator _M_current;

    using __traits_type = iterator_traits<_Iterator>;
#if !(__cplusplus > 201703L && __glibcxx_concepts)
    using __base_ref = typename __traits_type::reference;
#endif

    template <typename _Iter2>
    friend class move_iterator;

#if __cplusplus > 201703L && __glibcxx_concepts
    static auto
    _S_iter_concept()
    {
      if constexpr (random_access_iterator<_Iterator>)
        return random_access_iterator_tag{};
      else if constexpr (bidirectional_iterator<_Iterator>)
        return bidirectional_iterator_tag{};
      else if constexpr (forward_iterator<_Iterator>)
        return forward_iterator_tag{};
      else
        return input_iterator_tag{};
    }
#endif

  public:
    using iterator_type = _Iterator;

#ifdef __glibcxx_move_iterator_concept // C++ >= 20 && lib_concepts
    using iterator_concept = decltype(_S_iter_concept());

    // iterator_category defined in __move_iter_cat
    using value_type = iter_value_t<_Iterator>;
    using difference_type = iter_difference_t<_Iterator>;
    using pointer = _Iterator;
    using reference = iter_rvalue_reference_t<_Iterator>;
#else
    typedef typename __traits_type::iterator_category iterator_category;
    typedef typename __traits_type::value_type value_type;
    typedef typename __traits_type::difference_type difference_type;
    // NB: DR 680.
    typedef _Iterator pointer;
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 2106. move_iterator wrapping iterators returning prvalues
    using reference = __conditional_t<is_reference<__base_ref>::value,
                                      typename remove_reference<__base_ref>::type &&,
                                      __base_ref>;
#endif

    _GLIBCXX17_CONSTEXPR
    move_iterator()
        : _M_current() {}

    explicit _GLIBCXX17_CONSTEXPR
    move_iterator(iterator_type __i)
        : _M_current(std::move(__i)) {}

    template <typename _Iter>
    _GLIBCXX17_CONSTEXPR
    move_iterator(const move_iterator<_Iter> &__i)
        : _M_current(__i._M_current) {}

    template <typename _Iter>
    _GLIBCXX17_CONSTEXPR move_iterator &operator=(const move_iterator<_Iter> &__i)
    {
      _M_current = __i._M_current;
      return *this;
    }

#if __cplusplus <= 201703L
    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR iterator_type
    base() const
    {
      return _M_current;
    }
#else
    [[nodiscard]]
    constexpr const iterator_type &
    base() const & noexcept
    {
      return _M_current;
    }

    [[nodiscard]]
    constexpr iterator_type
    base() &&
    {
      return std::move(_M_current);
    }
#endif

    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR reference
    operator*() const
#if __cplusplus > 201703L && __glibcxx_concepts
    {
      return ranges::iter_move(_M_current);
    }
#else
    {
      return static_cast<reference>(*_M_current);
    }
#endif

    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR pointer
    operator->() const
    {
      return _M_current;
    }

    _GLIBCXX17_CONSTEXPR move_iterator &
    operator++()
    {
      ++_M_current;
      return *this;
    }

    _GLIBCXX17_CONSTEXPR move_iterator
    operator++(int)
    {
      move_iterator __tmp = *this;
      ++_M_current;
      return __tmp;
    }

    _GLIBCXX17_CONSTEXPR move_iterator &
    operator--()
    {
      --_M_current;
      return *this;
    }

    _GLIBCXX17_CONSTEXPR move_iterator
    operator--(int)
    {
      move_iterator __tmp = *this;
      --_M_current;
      return __tmp;
    }

    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR move_iterator
    operator+(difference_type __n) const
    {
      return move_iterator(_M_current + __n);
    }

    _GLIBCXX17_CONSTEXPR move_iterator &
    operator+=(difference_type __n)
    {
      _M_current += __n;
      return *this;
    }

    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR move_iterator
    operator-(difference_type __n) const
    {
      return move_iterator(_M_current - __n);
    }

    _GLIBCXX17_CONSTEXPR move_iterator &
    operator-=(difference_type __n)
    {
      _M_current -= __n;
      return *this;
    }

    [[__nodiscard__]]
    _GLIBCXX17_CONSTEXPR reference
    operator[](difference_type __n) const
#if __cplusplus > 201703L && __glibcxx_concepts
    {
      return ranges::iter_move(_M_current + __n);
    }
#else
    {
      return std::move(_M_current[__n]);
    }
#endif

#if __cplusplus > 201703L && __glibcxx_concepts
    template <sentinel_for<_Iterator> _Sent>
    [[nodiscard]]
    friend constexpr bool
    operator==(const move_iterator &__x, const move_sentinel<_Sent> &__y)
    {
      return __x.base() == __y.base();
    }

    template <sized_sentinel_for<_Iterator> _Sent>
    [[nodiscard]]
    friend constexpr iter_difference_t<_Iterator>
    operator-(const move_sentinel<_Sent> &__x, const move_iterator &__y)
    {
      return __x.base() - __y.base();
    }

    template <sized_sentinel_for<_Iterator> _Sent>
    [[nodiscard]]
    friend constexpr iter_difference_t<_Iterator>
    operator-(const move_iterator &__x, const move_sentinel<_Sent> &__y)
    {
      return __x.base() - __y.base();
    }

    [[nodiscard]]
    friend constexpr iter_rvalue_reference_t<_Iterator>
    iter_move(const move_iterator &__i) noexcept(noexcept(ranges::iter_move(__i._M_current)))
    {
      return ranges::iter_move(__i._M_current);
    }

    template <indirectly_swappable<_Iterator> _Iter2>
    friend constexpr void
    iter_swap(const move_iterator &__x, const move_iterator<_Iter2> &__y) noexcept(noexcept(ranges::iter_swap(__x._M_current, __y._M_current)))
    {
      return ranges::iter_swap(__x._M_current, __y._M_current);
    }
#endif // C++20
  };

  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator==(const move_iterator<_IteratorL> &__x,
             const move_iterator<_IteratorR> &__y)
#if __cplusplus > 201703L && __glibcxx_concepts
    requires requires { { __x.base() == __y.base() } -> convertible_to<bool>; }
#endif
  {
    return __x.base() == __y.base();
  }

#if __cpp_lib_three_way_comparison
  template <typename _IteratorL,
            three_way_comparable_with<_IteratorL> _IteratorR>
  [[__nodiscard__]]
  constexpr compare_three_way_result_t<_IteratorL, _IteratorR>
  operator<=>(const move_iterator<_IteratorL> &__x,
              const move_iterator<_IteratorR> &__y)
  {
    return __x.base() <=> __y.base();
  }
#else
  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator!=(const move_iterator<_IteratorL> &__x,
             const move_iterator<_IteratorR> &__y)
  {
    return !(__x == __y);
  }
#endif

  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator<(const move_iterator<_IteratorL> &__x,
            const move_iterator<_IteratorR> &__y)
#if __cplusplus > 201703L && __glibcxx_concepts
    requires requires { { __x.base() < __y.base() } -> convertible_to<bool>; }
#endif
  {
    return __x.base() < __y.base();
  }

  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator<=(const move_iterator<_IteratorL> &__x,
             const move_iterator<_IteratorR> &__y)
#if __cplusplus > 201703L && __glibcxx_concepts
    requires requires { { __y.base() < __x.base() } -> convertible_to<bool>; }
#endif
  {
    return !(__y < __x);
  }

  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator>(const move_iterator<_IteratorL> &__x,
            const move_iterator<_IteratorR> &__y)
#if __cplusplus > 201703L && __glibcxx_concepts
    requires requires { { __y.base() < __x.base() } -> convertible_to<bool>; }
#endif
  {
    return __y < __x;
  }

  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator>=(const move_iterator<_IteratorL> &__x,
             const move_iterator<_IteratorR> &__y)
#if __cplusplus > 201703L && __glibcxx_concepts
    requires requires { { __x.base() < __y.base() } -> convertible_to<bool>; }
#endif
  {
    return !(__x < __y);
  }

  // Note: See __normal_iterator operators note from Gaby to understand
  // why we have these extra overloads for some move_iterator operators.

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator==(const move_iterator<_Iterator> &__x,
             const move_iterator<_Iterator> &__y)
  {
    return __x.base() == __y.base();
  }

#if __cpp_lib_three_way_comparison
  template <three_way_comparable _Iterator>
  [[__nodiscard__]]
  constexpr compare_three_way_result_t<_Iterator>
  operator<=>(const move_iterator<_Iterator> &__x,
              const move_iterator<_Iterator> &__y)
  {
    return __x.base() <=> __y.base();
  }
#else
  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator!=(const move_iterator<_Iterator> &__x,
             const move_iterator<_Iterator> &__y)
  {
    return !(__x == __y);
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator<(const move_iterator<_Iterator> &__x,
            const move_iterator<_Iterator> &__y)
  {
    return __x.base() < __y.base();
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator<=(const move_iterator<_Iterator> &__x,
             const move_iterator<_Iterator> &__y)
  {
    return !(__y < __x);
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator>(const move_iterator<_Iterator> &__x,
            const move_iterator<_Iterator> &__y)
  {
    return __y < __x;
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR bool
  operator>=(const move_iterator<_Iterator> &__x,
             const move_iterator<_Iterator> &__y)
  {
    return !(__x < __y);
  }
#endif // ! C++20

  // DR 685.
  template <typename _IteratorL, typename _IteratorR>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR auto
  operator-(const move_iterator<_IteratorL> &__x,
            const move_iterator<_IteratorR> &__y)
      -> decltype(__x.base() - __y.base())
  {
    return __x.base() - __y.base();
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR move_iterator<_Iterator>
  operator+(typename move_iterator<_Iterator>::difference_type __n,
            const move_iterator<_Iterator> &__x)
  {
    return __x + __n;
  }

  template <typename _Iterator>
  [[__nodiscard__]]
  inline _GLIBCXX17_CONSTEXPR move_iterator<_Iterator>
  make_move_iterator(_Iterator __i)
  {
    return move_iterator<_Iterator>(std::move(__i));
  }

  template <typename _Iterator, typename _ReturnType = __conditional_t<__move_if_noexcept_cond<typename iterator_traits<_Iterator>::value_type>::value,
                                                                       _Iterator, move_iterator<_Iterator>>>
  inline _GLIBCXX17_CONSTEXPR _ReturnType
  __make_move_if_noexcept_iterator(_Iterator __i)
  {
    return _ReturnType(__i);
  }

  // Overload for pointers that matches std::move_if_noexcept more closely,
  // returning a constant iterator when we don't want to move.
  template <typename _Tp, typename _ReturnType = __conditional_t<__move_if_noexcept_cond<_Tp>::value,
                                                                 const _Tp *, move_iterator<_Tp *>>>
  inline _GLIBCXX17_CONSTEXPR _ReturnType
  __make_move_if_noexcept_iterator(_Tp *__i)
  {
    return _ReturnType(__i);
  }

#if __cplusplus > 201703L && __glibcxx_concepts
  // [iterators.common] Common iterators

  namespace __detail
  {
    template <typename _It>
    concept __common_iter_has_arrow = indirectly_readable<const _It> && (requires(const _It &__it) { __it.operator->(); } || is_reference_v<iter_reference_t<_It>> || constructible_from<iter_value_t<_It>, iter_reference_t<_It>>);

    template <typename _It>
    concept __common_iter_use_postfix_proxy = (!requires(_It &__i) { { *__i++ } -> __can_reference; }) && constructible_from<iter_value_t<_It>, iter_reference_t<_It>> && move_constructible<iter_value_t<_It>>;
  } // namespace __detail

  /// An iterator/sentinel adaptor for representing a non-common range.
  template <input_or_output_iterator _It, sentinel_for<_It> _Sent>
    requires(!same_as<_It, _Sent>) && copyable<_It>
  class common_iterator
  {
    template <typename _Tp, typename _Up>
    static constexpr bool
    _S_noexcept1()
    {
      if constexpr (is_trivially_default_constructible_v<_Tp>)
        return is_nothrow_assignable_v<_Tp &, _Up>;
      else
        return is_nothrow_constructible_v<_Tp, _Up>;
    }

    template <typename _It2, typename _Sent2>
    static constexpr bool
    _S_noexcept()
    {
      return _S_noexcept1<_It, _It2>() && _S_noexcept1<_Sent, _Sent2>();
    }

    class __arrow_proxy
    {
      iter_value_t<_It> _M_keep;

      constexpr __arrow_proxy(iter_reference_t<_It> &&__x)
          : _M_keep(std::move(__x)) {}

      friend class common_iterator;

    public:
      constexpr const iter_value_t<_It> *
      operator->() const noexcept
      {
        return std::__addressof(_M_keep);
      }
    };

    class __postfix_proxy
    {
      iter_value_t<_It> _M_keep;

      constexpr __postfix_proxy(iter_reference_t<_It> &&__x)
          : _M_keep(std::forward<iter_reference_t<_It>>(__x)) {}

      friend class common_iterator;

    public:
      constexpr const iter_value_t<_It> &
      operator*() const noexcept
      {
        return _M_keep;
      }
    };

  public:
    constexpr common_iterator() noexcept(is_nothrow_default_constructible_v<_It>)
      requires default_initializable<_It>
        : _M_it(), _M_index(0)
    {
    }

    constexpr common_iterator(_It __i) noexcept(is_nothrow_move_constructible_v<_It>)
        : _M_it(std::move(__i)), _M_index(0)
    {
    }

    constexpr common_iterator(_Sent __s) noexcept(is_nothrow_move_constructible_v<_Sent>)
        : _M_sent(std::move(__s)), _M_index(1)
    {
    }

    template <typename _It2, typename _Sent2>
      requires convertible_to<const _It2 &, _It> && convertible_to<const _Sent2 &, _Sent>
    constexpr common_iterator(const common_iterator<_It2, _Sent2> &__x) noexcept(_S_noexcept<const _It2 &, const _Sent2 &>())
        : _M_valueless(), _M_index(__x._M_index)
    {
      __glibcxx_assert(__x._M_has_value());
      if (_M_index == 0)
      {
        if constexpr (is_trivially_default_constructible_v<_It>)
          _M_it = std::move(__x._M_it);
        else
          std::construct_at(std::__addressof(_M_it), __x._M_it);
      }
      else if (_M_index == 1)
      {
        if constexpr (is_trivially_default_constructible_v<_Sent>)
          _M_sent = std::move(__x._M_sent);
        else
          std::construct_at(std::__addressof(_M_sent), __x._M_sent);
      }
    }

    common_iterator(const common_iterator &) = default;

    constexpr common_iterator(const common_iterator &__x) noexcept(_S_noexcept<const _It &, const _Sent &>())
      requires(!is_trivially_copyable_v<_It> || !is_trivially_copyable_v<_Sent>)
        : _M_valueless(), _M_index(__x._M_index)
    {
      if (_M_index == 0)
      {
        if constexpr (is_trivially_default_constructible_v<_It>)
          _M_it = __x._M_it;
        else
          std::construct_at(std::__addressof(_M_it), __x._M_it);
      }
      else if (_M_index == 1)
      {
        if constexpr (is_trivially_default_constructible_v<_Sent>)
          _M_sent = __x._M_sent;
        else
          std::construct_at(std::__addressof(_M_sent), __x._M_sent);
      }
    }

    common_iterator(common_iterator &&) = default;

    constexpr common_iterator(common_iterator &&__x) noexcept(_S_noexcept<_It, _Sent>())
      requires(!is_trivially_copyable_v<_It> || !is_trivially_copyable_v<_Sent>)
        : _M_valueless(), _M_index(__x._M_index)
    {
      if (_M_index == 0)
      {
        if constexpr (is_trivially_default_constructible_v<_It>)
          _M_it = std::move(__x._M_it);
        else
          std::construct_at(std::__addressof(_M_it), std::move(__x._M_it));
      }
      else if (_M_index == 1)
      {
        if constexpr (is_trivially_default_constructible_v<_Sent>)
          _M_sent = std::move(__x._M_sent);
        else
          std::construct_at(std::__addressof(_M_sent),
                            std::move(__x._M_sent));
      }
    }

    constexpr common_iterator &
    operator=(const common_iterator &) = default;

    constexpr common_iterator &
    operator=(const common_iterator &__x) noexcept(is_nothrow_copy_assignable_v<_It> && is_nothrow_copy_assignable_v<_Sent> && is_nothrow_copy_constructible_v<_It> && is_nothrow_copy_constructible_v<_Sent>)
      requires(!is_trivially_copy_assignable_v<_It> || !is_trivially_copy_assignable_v<_Sent>)
    {
      _M_assign(__x);
      return *this;
    }

    constexpr common_iterator &
    operator=(common_iterator &&) = default;

    constexpr common_iterator &
    operator=(common_iterator &&__x) noexcept(is_nothrow_move_assignable_v<_It> && is_nothrow_move_assignable_v<_Sent> && is_nothrow_move_constructible_v<_It> && is_nothrow_move_constructible_v<_Sent>)
      requires(!is_trivially_move_assignable_v<_It> || !is_trivially_move_assignable_v<_Sent>)
    {
      _M_assign(std::move(__x));
      return *this;
    }

    template <typename _It2, typename _Sent2>
      requires convertible_to<const _It2 &, _It> && convertible_to<const _Sent2 &, _Sent> && assignable_from<_It &, const _It2 &> && assignable_from<_Sent &, const _Sent2 &>
    constexpr common_iterator &
    operator=(const common_iterator<_It2, _Sent2> &__x) noexcept(is_nothrow_constructible_v<_It, const _It2 &> && is_nothrow_constructible_v<_Sent, const _Sent2 &> && is_nothrow_assignable_v<_It &, const _It2 &> && is_nothrow_assignable_v<_Sent &, const _Sent2 &>)
    {
      __glibcxx_assert(__x._M_has_value());
      _M_assign(__x);
      return *this;
    }

#if __cpp_concepts >= 202002L // Constrained special member functions
    ~common_iterator() = default;

    constexpr ~common_iterator()
      requires(!is_trivially_destructible_v<_It> || !is_trivially_destructible_v<_Sent>)
#else
    constexpr ~common_iterator()
#endif
    {
      if (_M_index == 0)
        _M_it.~_It();
      else if (_M_index == 1)
        _M_sent.~_Sent();
    }

    [[nodiscard]]
    constexpr decltype(auto)
    operator*()
    {
      __glibcxx_assert(_M_index == 0);
      return *_M_it;
    }

    [[nodiscard]]
    constexpr decltype(auto)
    operator*() const
      requires __detail::__dereferenceable<const _It>
    {
      __glibcxx_assert(_M_index == 0);
      return *_M_it;
    }

    [[nodiscard]]
    constexpr auto
    operator->() const
      requires __detail::__common_iter_has_arrow<_It>
    {
      __glibcxx_assert(_M_index == 0);
      if constexpr (is_pointer_v<_It> || requires { _M_it.operator->(); })
        return _M_it;
      else if constexpr (is_reference_v<iter_reference_t<_It>>)
      {
        auto &&__tmp = *_M_it;
        return std::__addressof(__tmp);
      }
      else
        return __arrow_proxy{*_M_it};
    }

    constexpr common_iterator &
    operator++()
    {
      __glibcxx_assert(_M_index == 0);
      ++_M_it;
      return *this;
    }

    constexpr decltype(auto)
    operator++(int)
    {
      __glibcxx_assert(_M_index == 0);
      if constexpr (forward_iterator<_It>)
      {
        common_iterator __tmp = *this;
        ++*this;
        return __tmp;
      }
      else if constexpr (!__detail::__common_iter_use_postfix_proxy<_It>)
        return _M_it++;
      else
      {
        __postfix_proxy __p(**this);
        ++*this;
        return __p;
      }
    }

    template <typename _It2, sentinel_for<_It> _Sent2>
      requires sentinel_for<_Sent, _It2>
    friend constexpr bool
    operator== [[nodiscard]] (const common_iterator &__x,
                              const common_iterator<_It2, _Sent2> &__y)
    {
      switch (__x._M_index << 2 | __y._M_index)
      {
      case 0b0000:
      case 0b0101:
        return true;
      case 0b0001:
        return __x._M_it == __y._M_sent;
      case 0b0100:
        return __x._M_sent == __y._M_it;
      default:
        __glibcxx_assert(__x._M_has_value());
        __glibcxx_assert(__y._M_has_value());
        __builtin_unreachable();
      }
    }

    template <typename _It2, sentinel_for<_It> _Sent2>
      requires sentinel_for<_Sent, _It2> && equality_comparable_with<_It, _It2>
    friend constexpr bool
    operator== [[nodiscard]] (const common_iterator &__x,
                              const common_iterator<_It2, _Sent2> &__y)
    {
      switch (__x._M_index << 2 | __y._M_index)
      {
      case 0b0101:
        return true;
      case 0b0000:
        return __x._M_it == __y._M_it;
      case 0b0001:
        return __x._M_it == __y._M_sent;
      case 0b0100:
        return __x._M_sent == __y._M_it;
      default:
        __glibcxx_assert(__x._M_has_value());
        __glibcxx_assert(__y._M_has_value());
        __builtin_unreachable();
      }
    }

    template <sized_sentinel_for<_It> _It2, sized_sentinel_for<_It> _Sent2>
      requires sized_sentinel_for<_Sent, _It2>
    friend constexpr iter_difference_t<_It2>
    operator- [[nodiscard]] (const common_iterator & __x,
                             const common_iterator<_It2, _Sent2> &__y)
    {
      switch (__x._M_index << 2 | __y._M_index)
      {
      case 0b0101:
        return 0;
      case 0b0000:
        return __x._M_it - __y._M_it;
      case 0b0001:
        return __x._M_it - __y._M_sent;
      case 0b0100:
        return __x._M_sent - __y._M_it;
      default:
        __glibcxx_assert(__x._M_has_value());
        __glibcxx_assert(__y._M_has_value());
        __builtin_unreachable();
      }
    }

    [[nodiscard]]
    friend constexpr iter_rvalue_reference_t<_It>
    iter_move(const common_iterator &__i) noexcept(noexcept(ranges::iter_move(std::declval<const _It &>())))
      requires input_iterator<_It>
    {
      __glibcxx_assert(__i._M_index == 0);
      return ranges::iter_move(__i._M_it);
    }

    template <indirectly_swappable<_It> _It2, typename _Sent2>
    friend constexpr void
    iter_swap(const common_iterator &__x,
              const common_iterator<_It2, _Sent2> &__y) noexcept(noexcept(ranges::iter_swap(std::declval<const _It &>(),
                                                                                            std::declval<const _It2 &>())))
    {
      __glibcxx_assert(__x._M_index == 0);
      __glibcxx_assert(__y._M_index == 0);
      return ranges::iter_swap(__x._M_it, __y._M_it);
    }

  private:
    template <input_or_output_iterator _It2, sentinel_for<_It2> _Sent2>
      requires(!same_as<_It2, _Sent2>) && copyable<_It2>
    friend class common_iterator;

    constexpr bool
    _M_has_value() const noexcept { return _M_index != _S_valueless; }

    template <typename _CIt>
    constexpr void
    _M_assign(_CIt &&__x)
    {
      if (_M_index == __x._M_index)
      {
        if (_M_index == 0)
          _M_it = std::forward<_CIt>(__x)._M_it;
        else if (_M_index == 1)
          _M_sent = std::forward<_CIt>(__x)._M_sent;
      }
      else
      {
        if (_M_index == 0)
          _M_it.~_It();
        else if (_M_index == 1)
          _M_sent.~_Sent();
        _M_index = _S_valueless;

        if (__x._M_index == 0)
          std::construct_at(std::__addressof(_M_it),
                            std::forward<_CIt>(__x)._M_it);
        else if (__x._M_index == 1)
          std::construct_at(std::__addressof(_M_sent),
                            std::forward<_CIt>(__x)._M_sent);
        _M_index = __x._M_index;
      }
    }

    union
    {
      _It _M_it;
      _Sent _M_sent;
      unsigned char _M_valueless;
    };
    unsigned char _M_index; // 0 == _M_it, 1 == _M_sent, 2 == valueless

    static constexpr unsigned char _S_valueless{2};
  };

  template <typename _It, typename _Sent>
  struct incrementable_traits<common_iterator<_It, _Sent>>
  {
    using difference_type = iter_difference_t<_It>;
  };

  template <input_iterator _It, typename _Sent>
  struct iterator_traits<common_iterator<_It, _Sent>>
  {
  private:
    template <typename _Iter>
    struct __ptr
    {
      using type = void;
    };

    template <typename _Iter>
      requires __detail::__common_iter_has_arrow<_Iter>
    struct __ptr<_Iter>
    {
      using _CIter = common_iterator<_Iter, _Sent>;
      using type = decltype(std::declval<const _CIter &>().operator->());
    };

    static auto
    _S_iter_cat()
    {
      if constexpr (requires { requires derived_from<__iter_category_t<_It>,
                                                     forward_iterator_tag>; })
        return forward_iterator_tag{};
      else
        return input_iterator_tag{};
    }

  public:
    using iterator_concept = __conditional_t<forward_iterator<_It>,
                                             forward_iterator_tag,
                                             input_iterator_tag>;
    using iterator_category = decltype(_S_iter_cat());
    using value_type = iter_value_t<_It>;
    using difference_type = iter_difference_t<_It>;
    using pointer = typename __ptr<_It>::type;
    using reference = iter_reference_t<_It>;
  };

  // [iterators.counted] Counted iterators

  namespace __detail
  {
    template <typename _It>
    struct __counted_iter_value_type
    {
    };

    template <indirectly_readable _It>
    struct __counted_iter_value_type<_It>
    {
      using value_type = iter_value_t<_It>;
    };

    template <typename _It>
    struct __counted_iter_concept
    {
    };

    template <typename _It>
      requires requires { typename _It::iterator_concept; }
    struct __counted_iter_concept<_It>
    {
      using iterator_concept = typename _It::iterator_concept;
    };

    template <typename _It>
    struct __counted_iter_cat
    {
    };

    template <typename _It>
      requires requires { typename _It::iterator_category; }
    struct __counted_iter_cat<_It>
    {
      using iterator_category = typename _It::iterator_category;
    };
  }

  /// An iterator adaptor that keeps track of the distance to the end.
  template <input_or_output_iterator _It>
  class counted_iterator
      : public __detail::__counted_iter_value_type<_It>,
        public __detail::__counted_iter_concept<_It>,
        public __detail::__counted_iter_cat<_It>
  {
  public:
    using iterator_type = _It;
    // value_type defined in __counted_iter_value_type
    using difference_type = iter_difference_t<_It>;
    // iterator_concept defined in __counted_iter_concept
    // iterator_category defined in __counted_iter_cat

    constexpr counted_iterator()
      requires default_initializable<_It>
    = default;

    constexpr counted_iterator(_It __i, iter_difference_t<_It> __n)
        : _M_current(std::move(__i)), _M_length(__n)
    {
      __glibcxx_assert(__n >= 0);
    }

    template <typename _It2>
      requires convertible_to<const _It2 &, _It>
    constexpr counted_iterator(const counted_iterator<_It2> &__x)
        : _M_current(__x._M_current), _M_length(__x._M_length)
    {
    }

    template <typename _It2>
      requires assignable_from<_It &, const _It2 &>
    constexpr counted_iterator &
    operator=(const counted_iterator<_It2> &__x)
    {
      _M_current = __x._M_current;
      _M_length = __x._M_length;
      return *this;
    }

    [[nodiscard]]
    constexpr const _It &
    base() const & noexcept
    {
      return _M_current;
    }

    [[nodiscard]]
    constexpr _It
    base() && noexcept(is_nothrow_move_constructible_v<_It>)
    {
      return std::move(_M_current);
    }

    [[nodiscard]]
    constexpr iter_difference_t<_It>
    count() const noexcept
    {
      return _M_length;
    }

    [[nodiscard]]
    constexpr decltype(auto)
    operator*() noexcept(noexcept(*_M_current))
    {
      __glibcxx_assert(_M_length > 0);
      return *_M_current;
    }

    [[nodiscard]]
    constexpr decltype(auto)
    operator*() const
        noexcept(noexcept(*_M_current))
      requires __detail::__dereferenceable<const _It>
    {
      __glibcxx_assert(_M_length > 0);
      return *_M_current;
    }

    [[nodiscard]]
    constexpr auto
    operator->() const noexcept
      requires contiguous_iterator<_It>
    {
      return std::to_address(_M_current);
    }

    constexpr counted_iterator &
    operator++()
    {
      __glibcxx_assert(_M_length > 0);
      ++_M_current;
      --_M_length;
      return *this;
    }

    constexpr decltype(auto)
    operator++(int)
    {
      __glibcxx_assert(_M_length > 0);
      --_M_length;
      __try
      {
        return _M_current++;
      }
      __catch(...)
      {
        ++_M_length;
        __throw_exception_again;
      }
    }

    constexpr counted_iterator
    operator++(int)
      requires forward_iterator<_It>
    {
      auto __tmp = *this;
      ++*this;
      return __tmp;
    }

    constexpr counted_iterator &
    operator--()
      requires bidirectional_iterator<_It>
    {
      --_M_current;
      ++_M_length;
      return *this;
    }

    constexpr counted_iterator
    operator--(int)
      requires bidirectional_iterator<_It>
    {
      auto __tmp = *this;
      --*this;
      return __tmp;
    }

    [[nodiscard]]
    constexpr counted_iterator
    operator+(iter_difference_t<_It> __n) const
      requires random_access_iterator<_It>
    {
      return counted_iterator(_M_current + __n, _M_length - __n);
    }

    [[nodiscard]]
    friend constexpr counted_iterator
    operator+(iter_difference_t<_It> __n, const counted_iterator &__x)
      requires random_access_iterator<_It>
    {
      return __x + __n;
    }

    constexpr counted_iterator &
    operator+=(iter_difference_t<_It> __n)
      requires random_access_iterator<_It>
    {
      __glibcxx_assert(__n <= _M_length);
      _M_current += __n;
      _M_length -= __n;
      return *this;
    }

    [[nodiscard]]
    constexpr counted_iterator
    operator-(iter_difference_t<_It> __n) const
      requires random_access_iterator<_It>
    {
      return counted_iterator(_M_current - __n, _M_length + __n);
    }

    template <common_with<_It> _It2>
    [[nodiscard]]
    friend constexpr iter_difference_t<_It2>
    operator-(const counted_iterator &__x,
              const counted_iterator<_It2> &__y)
    {
      return __y._M_length - __x._M_length;
    }

    [[nodiscard]]
    friend constexpr iter_difference_t<_It>
    operator-(const counted_iterator &__x, default_sentinel_t)
    {
      return -__x._M_length;
    }

    [[nodiscard]]
    friend constexpr iter_difference_t<_It>
    operator-(default_sentinel_t, const counted_iterator &__y)
    {
      return __y._M_length;
    }

    constexpr counted_iterator &
    operator-=(iter_difference_t<_It> __n)
      requires random_access_iterator<_It>
    {
      __glibcxx_assert(-__n <= _M_length);
      _M_current -= __n;
      _M_length += __n;
      return *this;
    }

    [[nodiscard]]
    constexpr decltype(auto)
    operator[](iter_difference_t<_It> __n) const
        noexcept(noexcept(_M_current[__n]))
      requires random_access_iterator<_It>
    {
      __glibcxx_assert(__n < _M_length);
      return _M_current[__n];
    }

    template <common_with<_It> _It2>
    [[nodiscard]]
    friend constexpr bool
    operator==(const counted_iterator &__x,
               const counted_iterator<_It2> &__y)
    {
      return __x._M_length == __y._M_length;
    }

    [[nodiscard]]
    friend constexpr bool
    operator==(const counted_iterator &__x, default_sentinel_t)
    {
      return __x._M_length == 0;
    }

    template <common_with<_It> _It2>
    [[nodiscard]]
    friend constexpr strong_ordering
    operator<=>(const counted_iterator &__x,
                const counted_iterator<_It2> &__y)
    {
      return __y._M_length <=> __x._M_length;
    }

    [[nodiscard]]
    friend constexpr iter_rvalue_reference_t<_It>
    iter_move(const counted_iterator &__i) noexcept(noexcept(ranges::iter_move(__i._M_current)))
      requires input_iterator<_It>
    {
      __glibcxx_assert(__i._M_length > 0);
      return ranges::iter_move(__i._M_current);
    }

    template <indirectly_swappable<_It> _It2>
    friend constexpr void
    iter_swap(const counted_iterator &__x,
              const counted_iterator<_It2> &__y) noexcept(noexcept(ranges::iter_swap(__x._M_current, __y._M_current)))
    {
      __glibcxx_assert(__x._M_length > 0 && __y._M_length > 0);
      ranges::iter_swap(__x._M_current, __y._M_current);
    }

  private:
    template <input_or_output_iterator _It2>
    friend class counted_iterator;

    _It _M_current = _It();
    iter_difference_t<_It> _M_length = 0;
  };

  template <input_iterator _It>
    requires same_as<__detail::__iter_traits<_It>, iterator_traits<_It>>
  struct iterator_traits<counted_iterator<_It>> : iterator_traits<_It>
  {
    using pointer = __conditional_t<contiguous_iterator<_It>,
                                    add_pointer_t<iter_reference_t<_It>>,
                                    void>;
  };

#if __cplusplus > 202020L
  template <indirectly_readable _It>
  using iter_const_reference_t = common_reference_t<const iter_value_t<_It> &&, iter_reference_t<_It>>;

  template <input_iterator _It>
  class basic_const_iterator;

  namespace __detail
  {
    template <typename _It>
    concept __constant_iterator = input_iterator<_It> && same_as<iter_const_reference_t<_It>, iter_reference_t<_It>>;

    template <typename _Tp>
    inline constexpr bool __is_const_iterator = false;

    template <typename _It>
    inline constexpr bool __is_const_iterator<basic_const_iterator<_It>> = true;

    template <typename _Tp>
    concept __not_a_const_iterator = !__is_const_iterator<_Tp>;

    template <indirectly_readable _It>
    using __iter_const_rvalue_reference_t = common_reference_t<const iter_value_t<_It> &&, iter_rvalue_reference_t<_It>>;

    template <typename _It>
    struct __basic_const_iterator_iter_cat
    {
    };

    template <forward_iterator _It>
    struct __basic_const_iterator_iter_cat<_It>
    {
      using iterator_category = __iter_category_t<_It>;
    };
  } // namespace detail

  template <input_iterator _It>
  using const_iterator = __conditional_t<__detail::__constant_iterator<_It>, _It, basic_const_iterator<_It>>;

  namespace __detail
  {
    template <typename _Sent>
    struct __const_sentinel
    {
      using type = _Sent;
    };

    template <input_iterator _Sent>
    struct __const_sentinel<_Sent>
    {
      using type = const_iterator<_Sent>;
    };
  } // namespace __detail

  template <semiregular _Sent>
  using const_sentinel = typename __detail::__const_sentinel<_Sent>::type;

  template <input_iterator _It>
  class basic_const_iterator
      : public __detail::__basic_const_iterator_iter_cat<_It>
  {
    _It _M_current = _It();
    using __reference = iter_const_reference_t<_It>;
    using __rvalue_reference = __detail::__iter_const_rvalue_reference_t<_It>;

    static auto
    _S_iter_concept()
    {
      if constexpr (contiguous_iterator<_It>)
        return contiguous_iterator_tag{};
      else if constexpr (random_access_iterator<_It>)
        return random_access_iterator_tag{};
      else if constexpr (bidirectional_iterator<_It>)
        return bidirectional_iterator_tag{};
      else if constexpr (forward_iterator<_It>)
        return forward_iterator_tag{};
      else
        return input_iterator_tag{};
    }

    template <input_iterator _It2>
    friend class basic_const_iterator;

  public:
    using iterator_concept = decltype(_S_iter_concept());
    using value_type = iter_value_t<_It>;
    using difference_type = iter_difference_t<_It>;

    basic_const_iterator()
      requires default_initializable<_It>
    = default;

    constexpr basic_const_iterator(_It __current) noexcept(is_nothrow_move_constructible_v<_It>)
        : _M_current(std::move(__current))
    {
    }

    template <convertible_to<_It> _It2>
    constexpr basic_const_iterator(basic_const_iterator<_It2> __current) noexcept(is_nothrow_constructible_v<_It, _It2>)
        : _M_current(std::move(__current._M_current))
    {
    }

    template <__detail::__different_from<basic_const_iterator> _Tp>
      requires convertible_to<_Tp, _It>
    constexpr basic_const_iterator(_Tp &&__current) noexcept(is_nothrow_constructible_v<_It, _Tp>)
        : _M_current(std::forward<_Tp>(__current))
    {
    }

    constexpr const _It &
    base() const & noexcept
    {
      return _M_current;
    }

    constexpr _It
    base() && noexcept(is_nothrow_move_constructible_v<_It>)
    {
      return std::move(_M_current);
    }

    constexpr __reference
    operator*() const
        noexcept(noexcept(static_cast<__reference>(*_M_current)))
    {
      return static_cast<__reference>(*_M_current);
    }

    constexpr const auto *
    operator->() const
        noexcept(contiguous_iterator<_It> || noexcept(*_M_current))
      requires is_lvalue_reference_v<iter_reference_t<_It>> && same_as<remove_cvref_t<iter_reference_t<_It>>, value_type>
    {
      if constexpr (contiguous_iterator<_It>)
        return std::to_address(_M_current);
      else
        return std::__addressof(*_M_current);
    }

    constexpr basic_const_iterator &
    operator++() noexcept(noexcept(++_M_current))
    {
      ++_M_current;
      return *this;
    }

    constexpr void
    operator++(int) noexcept(noexcept(++_M_current))
    {
      ++_M_current;
    }

    constexpr basic_const_iterator
    operator++(int) noexcept(noexcept(++*this) && is_nothrow_copy_constructible_v<basic_const_iterator>)
      requires forward_iterator<_It>
    {
      auto __tmp = *this;
      ++*this;
      return __tmp;
    }

    constexpr basic_const_iterator &
    operator--() noexcept(noexcept(--_M_current))
      requires bidirectional_iterator<_It>
    {
      --_M_current;
      return *this;
    }

    constexpr basic_const_iterator
    operator--(int) noexcept(noexcept(--*this) && is_nothrow_copy_constructible_v<basic_const_iterator>)
      requires bidirectional_iterator<_It>
    {
      auto __tmp = *this;
      --*this;
      return __tmp;
    }

    constexpr basic_const_iterator &
    operator+=(difference_type __n) noexcept(noexcept(_M_current += __n))
      requires random_access_iterator<_It>
    {
      _M_current += __n;
      return *this;
    }

    constexpr basic_const_iterator &
    operator-=(difference_type __n) noexcept(noexcept(_M_current -= __n))
      requires random_access_iterator<_It>
    {
      _M_current -= __n;
      return *this;
    }

    constexpr __reference
    operator[](difference_type __n) const
        noexcept(noexcept(static_cast<__reference>(_M_current[__n])))
      requires random_access_iterator<_It>
    {
      return static_cast<__reference>(_M_current[__n]);
    }

    template <sentinel_for<_It> _Sent>
    constexpr bool
    operator==(const _Sent &__s) const
        noexcept(noexcept(_M_current == __s))
    {
      return _M_current == __s;
    }

    template <__detail::__not_a_const_iterator _CIt>
      requires __detail::__constant_iterator<_CIt> && convertible_to<_It, _CIt>
    constexpr
    operator _CIt() const &
    {
      return _M_current;
    }

    template <__detail::__not_a_const_iterator _CIt>
      requires __detail::__constant_iterator<_CIt> && convertible_to<_It, _CIt>
    constexpr
    operator _CIt() &&
    {
      return std::move(_M_current);
    }

    constexpr bool
    operator<(const basic_const_iterator &__y) const
        noexcept(noexcept(_M_current < __y._M_current))
      requires random_access_iterator<_It>
    {
      return _M_current < __y._M_current;
    }

    constexpr bool
    operator>(const basic_const_iterator &__y) const
        noexcept(noexcept(_M_current > __y._M_current))
      requires random_access_iterator<_It>
    {
      return _M_current > __y._M_current;
    }

    constexpr bool
    operator<=(const basic_const_iterator &__y) const
        noexcept(noexcept(_M_current <= __y._M_current))
      requires random_access_iterator<_It>
    {
      return _M_current <= __y._M_current;
    }

    constexpr bool
    operator>=(const basic_const_iterator &__y) const
        noexcept(noexcept(_M_current >= __y._M_current))
      requires random_access_iterator<_It>
    {
      return _M_current >= __y._M_current;
    }

    constexpr auto
    operator<=>(const basic_const_iterator &__y) const
        noexcept(noexcept(_M_current <=> __y._M_current))
      requires random_access_iterator<_It> && three_way_comparable<_It>
    {
      return _M_current <=> __y._M_current;
    }

    template <__detail::__different_from<basic_const_iterator> _It2>
    constexpr bool
    operator<(const _It2 &__y) const
        noexcept(noexcept(_M_current < __y))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return _M_current < __y;
    }

    template <__detail::__different_from<basic_const_iterator> _It2>
    constexpr bool
    operator>(const _It2 &__y) const
        noexcept(noexcept(_M_current > __y))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return _M_current > __y;
    }

    template <__detail::__different_from<basic_const_iterator> _It2>
    constexpr bool
    operator<=(const _It2 &__y) const
        noexcept(noexcept(_M_current <= __y))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return _M_current <= __y;
    }

    template <__detail::__different_from<basic_const_iterator> _It2>
    constexpr bool
    operator>=(const _It2 &__y) const
        noexcept(noexcept(_M_current >= __y))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return _M_current >= __y;
    }

    template <__detail::__different_from<basic_const_iterator> _It2>
    constexpr auto
    operator<=>(const _It2 &__y) const
        noexcept(noexcept(_M_current <=> __y))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2> && three_way_comparable_with<_It, _It2>
    {
      return _M_current <=> __y;
    }

    template <__detail::__not_a_const_iterator _It2>
    friend constexpr bool
    operator<(const _It2 &__x, const basic_const_iterator &__y) noexcept(noexcept(__x < __y._M_current))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return __x < __y._M_current;
    }

    template <__detail::__not_a_const_iterator _It2>
    friend constexpr bool
    operator>(const _It2 &__x, const basic_const_iterator &__y) noexcept(noexcept(__x > __y._M_current))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return __x > __y._M_current;
    }

    template <__detail::__not_a_const_iterator _It2>
    friend constexpr bool
    operator<=(const _It2 &__x, const basic_const_iterator &__y) noexcept(noexcept(__x <= __y._M_current))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return __x <= __y._M_current;
    }

    template <__detail::__not_a_const_iterator _It2>
    friend constexpr bool
    operator>=(const _It2 &__x, const basic_const_iterator &__y) noexcept(noexcept(__x >= __y._M_current))
      requires random_access_iterator<_It> && totally_ordered_with<_It, _It2>
    {
      return __x >= __y._M_current;
    }

    friend constexpr basic_const_iterator
    operator+(const basic_const_iterator &__i, difference_type __n) noexcept(noexcept(basic_const_iterator(__i._M_current + __n)))
      requires random_access_iterator<_It>
    {
      return basic_const_iterator(__i._M_current + __n);
    }

    friend constexpr basic_const_iterator
    operator+(difference_type __n, const basic_const_iterator &__i) noexcept(noexcept(basic_const_iterator(__i._M_current + __n)))
      requires random_access_iterator<_It>
    {
      return basic_const_iterator(__i._M_current + __n);
    }

    friend constexpr basic_const_iterator
    operator-(const basic_const_iterator &__i, difference_type __n) noexcept(noexcept(basic_const_iterator(__i._M_current - __n)))
      requires random_access_iterator<_It>
    {
      return basic_const_iterator(__i._M_current - __n);
    }

    template <sized_sentinel_for<_It> _Sent>
    constexpr difference_type
    operator-(const _Sent &__y) const
        noexcept(noexcept(_M_current - __y))
    {
      return _M_current - __y;
    }

    template <__detail::__not_a_const_iterator _Sent>
      requires sized_sentinel_for<_Sent, _It>
    friend constexpr difference_type
    operator-(const _Sent &__x, const basic_const_iterator &__y) noexcept(noexcept(__x - __y._M_current))
    {
      return __x - __y._M_current;
    }

    friend constexpr __rvalue_reference
    iter_move(const basic_const_iterator &__i) noexcept(noexcept(static_cast<__rvalue_reference>(ranges::iter_move(__i._M_current))))
    {
      return static_cast<__rvalue_reference>(ranges::iter_move(__i._M_current));
    }
  };

  template <typename _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up>>
  struct common_type<basic_const_iterator<_Tp>, _Up>
  {
    using type = basic_const_iterator<common_type_t<_Tp, _Up>>;
  };

  template <typename _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up>>
  struct common_type<_Up, basic_const_iterator<_Tp>>
  {
    using type = basic_const_iterator<common_type_t<_Tp, _Up>>;
  };

  template <typename _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up>>
  struct common_type<basic_const_iterator<_Tp>, basic_const_iterator<_Up>>
  {
    using type = basic_const_iterator<common_type_t<_Tp, _Up>>;
  };

  template <input_iterator _It>
  constexpr const_iterator<_It>
  make_const_iterator(_It __it) noexcept(is_nothrow_convertible_v<_It, const_iterator<_It>>)
  {
    return __it;
  }

  template <semiregular _Sent>
  constexpr const_sentinel<_Sent>
  make_const_sentinel(_Sent __s) noexcept(is_nothrow_convertible_v<_Sent, const_sentinel<_Sent>>)
  {
    return __s;
  }
#endif // C++23
#endif // C++20

  /// @} group iterators

  template <typename _Iterator>
  _GLIBCXX20_CONSTEXPR auto
  __niter_base(move_iterator<_Iterator> __it)
      -> decltype(make_move_iterator(__niter_base(__it.base())))
  {
    return make_move_iterator(__niter_base(__it.base()));
  }

  template <typename _Iterator>
  _GLIBCXX20_CONSTEXPR auto
  __miter_base(move_iterator<_Iterator> __it)
      -> decltype(__miter_base(__it.base()))
  {
    return __miter_base(__it.base());
  }

#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) std::make_move_iterator(_Iter)
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) \
  std::__make_move_if_noexcept_iterator(_Iter)
#else
#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) (_Iter)
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) (_Iter)
#endif // C++11

#if __cpp_deduction_guides >= 201606
  // These helper traits are used for deduction guides
  // of associative containers.
  template <typename _InputIterator>
  using __iter_key_t = remove_const_t<
#if __glibcxx_tuple_like // >= C++23
      tuple_element_t<0, typename iterator_traits<_InputIterator>::value_type>>;
#else
      typename iterator_traits<_InputIterator>::value_type::first_type>;
#endif

  template <typename _InputIterator>
  using __iter_val_t
#if __glibcxx_tuple_like // >= C++23
      = tuple_element_t<1, typename iterator_traits<_InputIterator>::value_type>;
#else
      = typename iterator_traits<_InputIterator>::value_type::second_type;
#endif

  template <typename _T1, typename _T2>
  struct pair;

  template <typename _InputIterator>
  using __iter_to_alloc_t = pair<const __iter_key_t<_InputIterator>, __iter_val_t<_InputIterator>>;
#endif // __cpp_deduction_guides

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#ifdef _GLIBCXX_DEBUG
#include <debug/stl_iterator.h>
#endif
#else
#include_next <bits/stl_iterator.h>
#endif