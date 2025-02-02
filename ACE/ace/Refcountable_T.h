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
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCK>
class ACE_Refcountable_T
{
public:
  /// Destructor.
  virtual ~ACE_Refcountable_T () = default;

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
#include "ace/Refcountable_T.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Refcountable_T.cpp"

#include /**/ "ace/post.h"
#endif /*ACE_REFCOUNTABLE_T_H*/
