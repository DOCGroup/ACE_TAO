// -*- C++ -*-
//=============================================================================
/**
 * @file Auto_Functor.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef ACE_AUTO_FUNCTOR_H
#define ACE_AUTO_FUNCTOR_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_Utils
{
/**
 * @class Auto_Functor_Ref
 *
 * @brief Helper class to implement assignment and copy-construction
 *        as expected
 */
template<typename X, typename Functor>
struct Auto_Functor_Ref
{
  X * p_;
  Functor f_;

  Auto_Functor_Ref(X * p, Functor f);
};

/**
 * @class Auto_Functor
 *
 * @brief Helper template to implement auto_ptr<>-like classes, but
 *    executing a functor in the destructor, instead of always
 *    deleting things.
 *
 * The functor is called in the destructor, and it must implement:
 *
 * Functor() noexcept;<BR>
 * Functor(Functor const &) noexcept;<BR>
 * Functor & operator=(Functor const &) noexcept;<BR>
 * void operator()(X * p) noexcept;<BR>
 */
template<typename X, typename Functor>
class Auto_Functor
{
public:
  typedef X element_type;
  typedef Functor functor_type;

  /// Constructor
  explicit Auto_Functor (X * p = nullptr,
      Functor functor = Functor()); // noexcept

  Auto_Functor (Auto_Functor & rhs); // noexcept

  Auto_Functor<X,Functor>& operator= (Auto_Functor & rhs); // noexcept

  template<typename Y>
  Auto_Functor(Auto_Functor<Y,Functor>& rhs); // noexcept

  template<typename Y>
  Auto_Functor<X,Functor>& operator= (Auto_Functor<Y,Functor>& rhs); // noexcept

  ~Auto_Functor(); // noexcept

  X & operator*() const; // noexcept

  X * operator->() const; // noexcept

  X * get(); // noexcept

  X * release(); // noexcept

  void reset (X * p = nullptr); // noexcept

  void reset (X * p, Functor f); // noexcept

  Functor const & functor() const; // noexcept

  Auto_Functor(Auto_Functor_Ref<X,Functor> rhs); // noexcept

  Auto_Functor<X,Functor> & operator=(Auto_Functor_Ref<X,Functor> rhs); // noexcept

  template<typename Y> operator Auto_Functor_Ref<Y,Functor>(); // noexcept

  template<typename Y> operator Auto_Functor<Y,Functor>(); // noexcept

private:
  X * p_;
  Functor f_;
};
} // namespace ACE_Utils

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
# include "ace/Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Auto_Functor.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_AUTO_FUNCTOR_H*/
