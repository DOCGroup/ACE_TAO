/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Reactive_ConsumerControl
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
//   More details can be found in:
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_EC_REACTIVE_CONSUMERCONTROL_H
#define TAO_EC_REACTIVE_CONSUMERCONTROL_H

#include "EC_ConsumerControl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_EC_Event_Channel;

class TAO_EC_Reactive_ConsumerControl;

class TAO_ORBSVCS_Export TAO_EC_ConsumerControl_Adapter : public ACE_Event_Handler
{
  // = TITLE
  //   Forwards timeout events to the Reactive ConsumerControl
  //
  // = DESCRIPTION
  //   The Reactive ConsumerControl strategy uses the reactor to
  //   periodically wakeup and verify the state of the consumers
  //   registered with the Event Channel.
  //
public:
  TAO_EC_ConsumerControl_Adapter (TAO_EC_Reactive_ConsumerControl *adaptee);
  // Constructor

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  TAO_EC_Reactive_ConsumerControl *adaptee_;
  // The adapted object
};

class TAO_ORBSVCS_Export TAO_EC_Reactive_ConsumerControl : public TAO_EC_ConsumerControl
{
  // = TITLE
  //   ConsumerControl
  //
  // = DESCRIPTION
  //   Defines the interface for the consumer control strategy.
  //   This strategy handles misbehaving or failing consumers.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //
  // = TODO
  //
public:
  TAO_EC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                   TAO_EC_Event_Channel *event_channel,
                                   CORBA::ORB_ptr orb);
  // Constructor.  It does not assume ownership of the <event_channel>
  // parameter.

  virtual ~TAO_EC_Reactive_ConsumerControl (void);
  // destructor...

  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);
  // Receive the timeout from the adapter

  // = Documented in TAO_EC_ConsumerControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void consumer_not_exist (TAO_EC_ProxyPushSupplier *proxy,
                                   CORBA::Environment &);
  virtual void system_exception (TAO_EC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);

private:
  void query_consumers (CORBA::Environment &ACE_TRY_ENV);
  // Check if the consumers still exists.  It is a helper method for
  // handle_timeout() to isolate the exceptions.

private:
  ACE_Time_Value rate_;
  // The polling rate

  TAO_EC_ConsumerControl_Adapter adapter_;
  // The Adapter for the reactor events

  TAO_EC_Event_Channel *event_channel_;
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

#if defined (__ACE_INLINE__)
#include "EC_Reactive_ConsumerControl.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_CONSUMERCONTROL_H */
