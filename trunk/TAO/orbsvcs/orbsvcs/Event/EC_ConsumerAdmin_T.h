/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin_T
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Implement concrete versions of the EC_ConsumerAdmin class. This
//   concrete versions provide specific locking policies and
//   strategies to handle delayed vs. immediate connections and
//   disconnections.
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_CONSUMERADMIN_T_H
#define TAO_EC_CONSUMERADMIN_T_H

#include "EC_ConsumerAdmin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<ACE_SYNCH_DECL>
class TAO_EC_ConsumerAdmin_T : public TAO_EC_ConsumerAdmin
{
  // = TITLE
  //   ConsumerAdmin_T
  //
  // = DESCRIPTION
  //   Implements the locking policies for the TAO_EC_ConsumerAdmin
  //   class, we use a parametric class to handle the co-variations
  //   between mutexes and condition variables.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //   The kind of locking is specified as the template argument.
  //   Clients still should follow the locking protocol: call busy()
  //   to use the iterator and idle() once finished, the
  //   Busy_Lock_Adapter can help there.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin_T (TAO_EC_Event_Channel* event_channel);
  // constructor...

  // = The TAO_EC_ConsumerAdmin methods
  virtual int busy (void);
  virtual int idle (void);

protected:
  virtual int busy_i (void);
  // Implements the busy() method, but without locking.

  virtual int idle_i (void);
  // Implements the busy() method, but without locking.

protected:
  int busy_count (void) const;
  // Return the current value of busy_count [derived classes are not
  // allowed to modify this]

  ACE_SYNCH_MUTEX_T lock_;
  // The lock

  ACE_SYNCH_CONDITION_T busy_cond_;
  // A condition variable to wait while the object is too busy.

private:
  int busy_count_;
  // The number of threads iterating on us

  int reached_hwm_;
  // The set was too busy and reached its HWM, now everybody has to
  // wait until we reach the LWM (0)
};

// ****************************************************************

template<ACE_SYNCH_DECL>
class TAO_EC_ConsumerAdmin_Immediate : public TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>
{
  // = TITLE
  //   ConsumerAdmin_Immediate
  //
  // = DESCRIPTION
  //   A concrete version of the EC_ConsumerAdmin class; using the
  //   locking strategy in EC_ConsumerAdmin_T and immediate execution
  //   for the connected() and disconnected() operations.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //   The kind of locking is specified as the template argument.
  //   Clients still should follow the locking protocol: call busy()
  //   to use the iterator and idle() once finished, the
  //   Busy_Lock_Adapter can help there.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin_Immediate (TAO_EC_Event_Channel* event_channel);
  // constructor...

  // = The TAO_EC_ConsumerAdmin methods
  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
};

// ****************************************************************

template<ACE_SYNCH_DECL>
class TAO_EC_ConsumerAdmin_Delayed : public TAO_EC_ConsumerAdmin_T<ACE_SYNCH_USE>
{
  // = TITLE
  //   ConsumerAdmin_Delayed
  //
  // = DESCRIPTION
  //   A concrete version of the EC_ConsumerAdmin class; using the
  //   locking strategy in EC_ConsumerAdmin_T and storing the
  //   execution of connected() and disconnected() operations as
  //   command objects, that are executed once the set is idle.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //   The kind of locking is specified as the template argument.
  //   Clients still should follow the locking protocol: call busy()
  //   to use the iterator and idle() once finished, the
  //   Busy_Lock_Adapter can help there.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin_Delayed (TAO_EC_Event_Channel* event_channel);
  // constructor...

  // = The TAO_EC_ConsumerAdmin methods
  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);

protected:
  virtual void execute_delayed_operations (void);
  // documented in TAO_EC_ConsumerAdmin

private:
  ACE_Unbounded_Queue<ACE_Command_Base*> command_queue_;
  // The commands that carry the delayed operations are enqueued
  // here.
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_Busy_Lock.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Busy_Lock.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_EC_CONSUMERADMIN_T_H */
