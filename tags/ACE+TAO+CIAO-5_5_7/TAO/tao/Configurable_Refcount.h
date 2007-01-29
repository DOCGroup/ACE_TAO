// This may look like C, but it's really
// -*- C++ -*-

//=============================================================================
/**
 *  @file Configurable_Refcount.h
 *
 *  $Id$
 *
 *  Definition for a synchronised refcountable interface.
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONFIGURABLE_REFCOUNT_H
#define TAO_CONFIGURABLE_REFCOUNT_H

#include /**/ "ace/pre.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Configurable_Refcount
 *
 * @brief Definition for a synchronised refcountable interface.
 */
class TAO_Export TAO_Configurable_Refcount
{
public:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  TAO_Configurable_Refcount (Lock_Type type = TAO_THREAD_LOCK,
                             unsigned long value = 1);

  unsigned long increment (void);
  unsigned long decrement (void);

  unsigned long value (void) const;

private:
  Lock_Type type_;
  ACE_Atomic_Op<ACE_SYNCH_NULL_MUTEX, unsigned long> null_refcount_;
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> mutex_refcount_;
};


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "Configurable_Refcount.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CONFIGURABLE_REFCOUNT */
