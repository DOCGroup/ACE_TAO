// This may look like C, but it's really
// -*- C++ -*-

//=============================================================================
/**
 *  @file Synch_Refcountable.h
 *
 *  $Id$
 *
 *  Definition for a synchronised refcountable interface.
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_SYNCH_REFCOUNTABLE_H
#define TAO_SYNCH_REFCOUNTABLE_H
#include "ace/pre.h"

#include "ace/Refcountable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Export.h"
#include "ace/Synch.h"

class ACE_Lock;

/**
 * @class TAO_Synch_Refcountable
 *
 * @brief Definition for a synchronised refcountable interface.
 */
class TAO_Export TAO_Synch_Refcountable : private ACE_Refcountable
{
public:
  virtual ~TAO_Synch_Refcountable (void);

  int increment (void);
  int decrement (void);

  int refcount (void) const;

protected:
  TAO_Synch_Refcountable (ACE_Lock *lock, int refcount);

  ACE_Lock *refcount_lock_;
};


#if defined (__ACE_INLINE__)
# include "Synch_Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_SYNCH_REFCOUNTABLE*/
