/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ProxyPushSupplier_Set_T
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
// ============================================================================

#ifndef TAO_EC_PROXYPUSHSUPPLIER_SET_T_H
#define TAO_EC_PROXYPUSHSUPPLIER_SET_T_H

#include "EC_ProxyPushSupplier_Set.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class ACE_LOCK>
class TAO_EC_ProxyPushSupplier_Set_Immediate : public TAO_EC_ProxyPushSupplier_Set
{
  // = TITLE
  //   ProxyPushSupplier_Set_Immediate
  //
  // = DESCRIPTION
  //   A concrete implementation of EC_ProxyPushSupplier_Set that
  //   propagate changes to the set immediately.
  //
  // = LOCKING
  //   Uses parametric types to provide locking.
  //
  // = TODO
  //
public:
  TAO_EC_ProxyPushSupplier_Set_Immediate (void);
  // Constructor.

  virtual int busy (void);
  virtual int idle (void);
  // The implementation of this methods is provided by derived
  // classes, that provide appropiate locking.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

private:
  ACE_LOCK lock_;
  // The lock.
};

// ****************************************************************

template<ACE_SYNCH_DECL>
class TAO_EC_ProxyPushSupplier_Set_Delayed : public TAO_EC_ProxyPushSupplier_Set
{
  // = TITLE
  //   ProxyPushSupplier_Set_Delayed
  //
  // = DESCRIPTION
  //   A concrete implementation of EC_ProxyPushSupplier_Set that
  //   delays changes to the set until no threads are using the set.
  //
  // = LOCKING
  //   Uses parametric types to provide locking.
  //
  // = TODO
  //
public:
  TAO_EC_ProxyPushSupplier_Set_Delayed (void);
  // Constructor.

  // = Read the descriptions in EC_ProxyPushSupplier_Set
  virtual int busy (void);
  virtual int idle (void);
  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);

protected:
  virtual void execute_delayed_operations (void);

private:
  ACE_SYNCH_MUTEX_T lock_;
  // The lock.

  ACE_SYNCH_CONDITION_T busy_cond_;
  // A condition variable to wait while the object is too busy.

  CORBA::ULong busy_count_;
  // Keep track of the number of threads using the set

  CORBA::ULong write_delay_;
  // Keep track of the number of threads that have used the set since
  // the last change was posted.

  ACE_Unbounded_Queue<ACE_Command_Base*> command_queue_;
  // The commands that carry the delayed operations are enqueued
  // here.
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyPushSupplier_Set_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_ProxyPushSupplier_Set_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_ProxyPushSupplier_Set_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_EC_PROXYPUSHSUPPLIER_SET_H */
