// -*- C++ -*-

/**
 *  @file Refcountable.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_REFCOUNTABLE_H
#define TAO_Notify_REFCOUNTABLE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Refcountable_Guard_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"

// Debugging macros
#ifndef TAO_NOTIFY_REFCOUNT_DIAGNOSTICS
#define TAO_NOTIFY_REFCOUNT_DIAGNOSTICS 0
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Refcountable
 *
 * @brief Thread-safe refounting, calls the <release> method when refcount falls to 0.
 *
 *        The refcount is initialized to 0. When an instance of a
 *        derived class becomes owned by a managed pointer
 *        (Refcountable_Guard_T) the reference count becomes non-zero.
 *
 *        Instances declared on the stack should always have a
 *        refcount of zero.
 *
 *        A method that creates or simply returns an instance of
 *        Refcountable should not increment the reference count. It is
 *        the responsibility of the client to increment the reference
 *        count (take ownership or guard against deletion). The client
 *        cannot know if the method will or will not incr the refcount
 *        on its behalf.
 *
 * Use Refcountable_Guard_T or similar service to guarantee the
 * exception safe direct pairing of increments and decrements. Avoid
 * calling _incr_refcnt and _decr_refcnt.
 */

class TAO_Notify_Serv_Export TAO_Notify_Refcountable
{
public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Refcountable > Ptr;

  /// Constructor
  TAO_Notify_Refcountable (void);

  /// Destructor
  /// public for stack allocated instances
  virtual ~TAO_Notify_Refcountable ();

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  /// Public for bridge implementations and various guard classes
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )
  static void diagnostic_dump( const char* title = 0 );
#endif

private:
  /// The release method is called when the refcount reaches 0.
  virtual void release (void) = 0;

  /// Use a signed counter so that we can more easily detect
  /// boundary conditions such as too many _decr_refcnt() calls.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::Long> refcount_;

#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )
  friend class TAO_Notify_Tracker;
  int ref_id_;
#endif
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_REFCOUNTABLE_H */
