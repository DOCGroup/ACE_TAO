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

#include /**/ "ace/pre.h"
#include "ace/Refcountable_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/utils_export.h"
#include "tao/Versioned_Namespace.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Lock;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Synch_Refcountable
 *
 * @brief Definition for a synchronised refcountable interface.
 */
class TAO_UTILS_Export TAO_Synch_Refcountable : private ACE_Refcountable_T<ACE_Null_Mutex>
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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Utils/Synch_Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_SYNCH_REFCOUNTABLE*/
