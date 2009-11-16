// -*- C++ -*-

/**
 *  @file   ECG_Reactive_ConsumerEC_Control.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 */

#ifndef TAO_ECG_REACTIVE_CONSUMEREC_CONTROL_H
#define TAO_ECG_REACTIVE_CONSUMEREC_CONTROL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/ECG_ConsumerEC_Control.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/PolicyC.h"

#include "ace/Event_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ECG_Reactive_ConsumerEC_Control;
class TAO_EC_Gateway_IIOP;

/**
 * @class TAO_ECG_Reactive_ConsumerEC_Control_Adapter
 *
 * @brief Forwards timeout events to the Reactive ConsumerControl
 *
 * The Reactive ConsumerControl strategy uses the reactor to
 * periodically wakeup and verify the state of the consumers
 * registered with the Event Channel.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Reactive_ConsumerEC_Control_Adapter
  : public ACE_Event_Handler
{
public:
  /// Constructor
  TAO_ECG_Reactive_ConsumerEC_Control_Adapter (TAO_ECG_Reactive_ConsumerEC_Control *adaptee);

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  /// The adapted object
  TAO_ECG_Reactive_ConsumerEC_Control *adaptee_;
};

/**
 * @class TAO_ECG_Reactive_ConsumerEC_Control
 *
 * @brief Consumer event channel control
 *
 * Defines the interface for the consumer event channel control strategy.
 * This strategy handles misbehaving or failing consumer event channels.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Reactive_ConsumerEC_Control
  : public TAO_ECG_ConsumerEC_Control
{
public:
  /// Constructor.  It does not assume ownership of the @a event_channel
  /// parameter.
  TAO_ECG_Reactive_ConsumerEC_Control (const ACE_Time_Value &rate,
                                       const ACE_Time_Value &timeout,
                                       TAO_EC_Gateway_IIOP* gateway,
                                       CORBA::ORB_ptr orb);

  /// Destructor.
  virtual ~TAO_ECG_Reactive_ConsumerEC_Control (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_EC_ConsumerControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void event_channel_not_exist (TAO_EC_Gateway_IIOP *gateway);
  virtual void system_exception (TAO_EC_Gateway_IIOP *gateway,
                                 CORBA::SystemException &);

private:
  /// Check if the consumers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_eventchannel (void);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The polling timeout
  ACE_Time_Value timeout_;

  /// The Adapter for the reactor events
  TAO_ECG_Reactive_ConsumerEC_Control_Adapter adapter_;

  /// The gateway
  TAO_EC_Gateway_IIOP* gateway_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// To control the timeout policy in the thread
  CORBA::PolicyCurrent_var policy_current_;

  /// Precomputed policy list to the set timeout.
  CORBA::PolicyList policy_list_;

  /// The ORB reactor
  ACE_Reactor *reactor_;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  /// The timer id
  long timer_id_;
#endif /* TAO_HAS_CORBA_MESSAGING */
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ECG_REACTIVE_CONSUMEREC_CONTROL_H */
