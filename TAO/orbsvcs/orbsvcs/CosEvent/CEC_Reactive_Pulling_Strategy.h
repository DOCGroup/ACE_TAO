/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_Reactive_Pulling_Strategy
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//
// ============================================================================

#ifndef TAO_CEC_REACTIVE_PULLING_STRATEGY_H
#include "ace/pre.h"
#define TAO_CEC_REACTIVE_PULLING_STRATEGY_H

#include "CEC_Pulling_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Worker.h"
#include "tao/corba.h"

class TAO_CEC_ProxyPullConsumer;
class TAO_CEC_EventChannel;
class TAO_CEC_ConsumerAdmin;
class TAO_CEC_SupplierControl;
class TAO_CEC_Reactive_Pulling_Strategy;

class TAO_Event_Export TAO_CEC_Pulling_Strategy_Adapter : public ACE_Event_Handler
{
  // = TITLE
  //   Forwards timeout events to the Reactive Pulling Strategy
  //
  // = DESCRIPTION
  //   The Reactive Pulling Strategy strategy uses the reactor to
  //   periodically wakeup and try top pull events from each
  //   PullSupplier connected to the EventChannel.
  //
public:
  TAO_CEC_Pulling_Strategy_Adapter (TAO_CEC_Reactive_Pulling_Strategy *adaptee);
  // Constructor

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  TAO_CEC_Reactive_Pulling_Strategy *adaptee_;
  // The adapted object
};

// ****************************************************************

class TAO_Event_Export TAO_CEC_Reactive_Pulling_Strategy : public TAO_CEC_Pulling_Strategy
{
  // = TITLE
  //   Dispatch using the caller thread.
  //
  // = DESCRIPTION
  //   The events are dispatched in FIFO ordering, using the invoking
  //   thread to push the event to the consumer.
  //
public:
  TAO_CEC_Reactive_Pulling_Strategy (const ACE_Time_Value &rate,
                                     TAO_CEC_EventChannel *event_channel,
                                     CORBA::ORB_ptr orb);
  // The scheduler is used to find the range of priorities and similar
  // info.

  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);
  // Receive the timeout from the adapter

  // = The CEC_Pulling_Strategy methods.
  virtual void activate (void);
  virtual void shutdown (void);

private:
  TAO_CEC_Pulling_Strategy_Adapter adapter_;
  // The Adapter for the reactor events

  ACE_Time_Value rate_;
  // The polling rate

  TAO_CEC_EventChannel *event_channel_;
  // The event channel

  CORBA::ORB_var orb_;
  // The ORB

  CORBA::PolicyCurrent_var policy_current_;
  // To control the timeout policy in the thread

  CORBA::PolicyList policy_list_;
  // Precomputed policy list to the set timeout.

  ACE_Reactor *reactor_;
  // The ORB reactor
};

// ****************************************************************

class TAO_CEC_Pull_Event : public TAO_EC_Worker<TAO_CEC_ProxyPullConsumer>
{
public:
  TAO_CEC_Pull_Event (TAO_CEC_ConsumerAdmin *consumer_admin,
                      TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_ProxyPullConsumer *consumer,
                     CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ConsumerAdmin *consumer_admin_;
  // Used to propagate the events.

  TAO_CEC_SupplierControl *supplier_control_;
  // To report failed or dead suppliers
};

#if defined (__ACE_INLINE__)
#include "CEC_Reactive_Pulling_Strategy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_REACTIVE_PULLING_STRATEGY_H */
