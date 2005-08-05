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

#include "notify_serv_export.h"
#include "Refcountable_Guard_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"

/**
 * @class TAO_Notify_Refcountable
 *
 * @brief Thread-safe refounting, calls the <release> method when refcount falls to 0.
 *
 *        The refcount is initialized to 0. When an instance of a derived class becomes owned
 *        by a managed pointer (Refcountable_Guard_T) the reference count becomes non-zero.
 *        A debug message is printed from the destructor if a refcounted is deleted with a
 *        non-zero refcount.
 *
 *        Instances declared on the stack should always have a refcount of zero.
 *
 *        A method that creates or simply returns an instance of Refcountable should not
 *        increment the reference count. It is the responsibility of the client to increment
 *        the reference count (take ownership or guard against deletion). The client cannot
 *        know if the method will or will not incr the refcount in its behalf.
 *
 *        Use Refcountable_Guard_T or similar service to guarantee the exception safe direct
 *        pairing of increments and decrements. Avoid calling _incr_refcnt and _decr_refcnt.
 */
class TAO_Notify_Serv_Export TAO_Notify_Refcountable
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Refcountable > Ptr;
  typedef long Counter;

  /// Constuctor
  TAO_Notify_Refcountable (void);

  /// Destructor
  /// public for stack allocated instances
  virtual ~TAO_Notify_Refcountable ();

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  /// Public for bridge implementations and various guard classes
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

protected:
  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;

  /// Accessor for debugging.
#ifdef _DEBUG
  Counter refcount (void) const;
#endif

private:
  /// The release method is called when the refcount reaches 0.
  virtual void release (void) = 0;

  ACE_Atomic_Op< TAO_SYNCH_MUTEX, Counter > refcount_;
};

#include /**/ "ace/post.h"

#endif /* TAO_Notify_REFCOUNTABLE_H */
