/* -*- C++ -*- */
/**
 *  @file Refcountable.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_REFCOUNTABLE_H
#define TAO_Notify_REFCOUNTABLE_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"
#include "tao/orbconf.h"
#include "tao/corbafwd.h"

/**
 * @class TAO_Notify_Refcountable
 *
 * @brief Thread-safe refounting, calls the <release> method when refcount falls to 0.
 *
 */
class TAO_Notify_Export TAO_Notify_Refcountable
{
public:
  /// Constuctor
  TAO_Notify_Refcountable (void);

  /// Destructor
  virtual ~TAO_Notify_Refcountable ();

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  /// The release method is called when the refcount reaches 0.
  virtual void release (void) = 0;

protected:
  /// The reference count.
  CORBA::ULong refcount_;

  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;
};

#if defined (__ACE_INLINE__)
#include "Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_REFCOUNTABLE_H */
