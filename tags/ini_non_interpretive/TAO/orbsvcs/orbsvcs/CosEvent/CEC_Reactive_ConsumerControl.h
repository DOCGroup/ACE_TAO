/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_Reactive_ConsumerControl
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
//   More details can be found in:
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_CEC_REACTIVE_CONSUMERCONTROL_H
#define TAO_CEC_REACTIVE_CONSUMERCONTROL_H

#include "CEC_ConsumerControl.h"
#include "orbsvcs/Event/EC_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_CEC_EventChannel;

class TAO_CEC_Reactive_ConsumerControl;

class TAO_ORBSVCS_Export TAO_CEC_ConsumerControl_Adapter : public ACE_Event_Handler
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
  TAO_CEC_ConsumerControl_Adapter (TAO_CEC_Reactive_ConsumerControl *adaptee);
  // Constructor

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  TAO_CEC_Reactive_ConsumerControl *adaptee_;
  // The adapted object
};

class TAO_ORBSVCS_Export TAO_CEC_Reactive_ConsumerControl : public TAO_CEC_ConsumerControl
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
  TAO_CEC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                    TAO_CEC_EventChannel *event_channel,
                                    CORBA::ORB_ptr orb);
  // Constructor.  It does not assume ownership of the <event_channel>
  // parameter.

  virtual ~TAO_CEC_Reactive_ConsumerControl (void);
  // destructor...

  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);
  // Receive the timeout from the adapter

  // = Documented in TAO_CEC_ConsumerControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void consumer_not_exist (TAO_CEC_ProxyPushSupplier *proxy,
                                   CORBA::Environment &);
  virtual void consumer_not_exist (TAO_CEC_ProxyPullSupplier *proxy,
                                   CORBA::Environment &);
  virtual void system_exception (TAO_CEC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);

private:
  void query_consumers (CORBA::Environment &ACE_TRY_ENV);
  // Check if the consumers still exists.  It is a helper method for
  // handle_timeout() to isolate the exceptions.

private:
  ACE_Time_Value rate_;
  // The polling rate

  TAO_CEC_ConsumerControl_Adapter adapter_;
  // The Adapter for the reactor events

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

class TAO_CEC_Ping_Push_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
{
public:
  TAO_CEC_Ping_Push_Consumer (TAO_CEC_ConsumerControl *control);

  virtual void work (TAO_CEC_ProxyPushSupplier *supplier,
                     CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ConsumerControl *control_;
};

// ****************************************************************

class TAO_CEC_Ping_Pull_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPullSupplier>
{
public:
  TAO_CEC_Ping_Pull_Consumer (TAO_CEC_ConsumerControl *control);

  virtual void work (TAO_CEC_ProxyPullSupplier *supplier,
                     CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ConsumerControl *control_;
};

#if defined (__ACE_INLINE__)
#include "CEC_Reactive_ConsumerControl.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_CONSUMERCONTROL_H */
