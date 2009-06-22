// -*- C++ -*-

/**
 *  @file Refcountable_Guard_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_REFCOUNTABLE_GUARD_T_H
#define TAO_Notify_REFCOUNTABLE_GUARD_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Refcountable_Guard_T
 *
 * @brief Increments the reference count in the constructor, the count is decremented in the destructor.
 *        See Refcountable.h for further notes on usage.
 *
 */
template <class T>
class TAO_Notify_Refcountable_Guard_T
{
public:
  /// Constructor
  explicit TAO_Notify_Refcountable_Guard_T (T* t = 0);

  /// Copy constructor
  TAO_Notify_Refcountable_Guard_T (const TAO_Notify_Refcountable_Guard_T<T>& rhs);

  /// Destructor
  ~TAO_Notify_Refcountable_Guard_T ();

  /// Redirection operator
  T* get (void) const;

  /// Boolean test
  bool isSet (void) const;

  /// Redirection operator
  T* operator -> (void) const;

  /// Dereference operator
  T& operator * () const;

  /// Reassignment
  void reset (T* t = 0);

  // There is no logical release. Release could return an invalid object.

  /// Assignment
  TAO_Notify_Refcountable_Guard_T<T>& operator = (
    const TAO_Notify_Refcountable_Guard_T<T>& rhs);

private:
  void swap( TAO_Notify_Refcountable_Guard_T<T>& rhs );

  T* t_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Refcountable_Guard_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Refcountable_Guard_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Refcountable_Guard_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_REFCOUNTABLE_GUARD_T_H */
