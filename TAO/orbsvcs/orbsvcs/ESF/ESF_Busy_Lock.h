/* -*- C++ -*- */
/**
 *  @file   ESF_Busy_Lock.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_BUSY_LOCK_H
#define TAO_ESF_BUSY_LOCK_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Adapt classes that implement the "busy lock" protocol, to conform
/// to the ACE Lock interface.
/**
 * The ACE Lock interface allows us to use any mutex-like object with
 * the ACE synchronization components (such as guards, Lock_Adapter,
 * etc.).
 * One of the synchronization protocols used in the event services
 * uses the following protocol:
 * <UL>
 * <LI> To acquire the lock the @c busy method of the object is invoked.
 * <LI> To release the lock the @c idle method of the object is invoked.
 * <LI> Any changes performed while the lock is held are delayed until
 *      all the locks are released, i.e. the last call to @c idle
 *      executes the changes.
 * </UL>
 */
template<class Adaptee>
class TAO_ESF_Busy_Lock_Adapter
{
public:
  /// Constructor
  TAO_ESF_Busy_Lock_Adapter (Adaptee* adaptee);

  /**
   * @name The ACE_Lock methods
   *
   * Check the documentation in ace/Synch.h for details.
   */
  //@{
  int remove ();
  int acquire ();
  int tryacquire ();
  int release ();
  int acquire_read ();
  int acquire_write ();
  int tryacquire_read ();
  int tryacquire_write ();
  //@}

private:
  Adaptee* adaptee_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Busy_Lock.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Busy_Lock.cpp"

#endif /* TAO_ESF_BUSY_LOCK_H */
