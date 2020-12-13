// -*- C++ -*-

//=============================================================================
/**
 *  @file   Refcountable_T.h
 *
 * @author Doug Schmidt
 * @author Johnny Willemsen
 */
//=============================================================================
#ifndef ACE_REFCOUNTABLE_T_H
#define ACE_REFCOUNTABLE_T_H
#include /**/ "pre.h"

#include /**/ "ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Atomic_Op.h"
#include "Synch_Traits.h"
#include "Null_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCK>
class ACE_Refcountable_T
{
public:
  /// Destructor.
  virtual ~ACE_Refcountable_T ();

  /// Increment refcount
  long increment ();

  /// Decrement refcount
  long decrement ();

  /// Returns the current refcount.
  long refcount () const;

protected:
  /// Protected constructor.
  ACE_Refcountable_T (long refcount);

  /// Current refcount.
  ACE_Atomic_Op <ACE_LOCK, long> refcount_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "Refcountable_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Refcountable_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Refcountable_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "post.h"
#endif /*ACE_REFCOUNTABLE_T_H*/
