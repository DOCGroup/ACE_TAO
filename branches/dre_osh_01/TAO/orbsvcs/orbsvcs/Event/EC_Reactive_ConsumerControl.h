/* -*- C++ -*- */
/**
 *  @file   EC_Reactive_ConsumerControl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_REACTIVE_CONSUMERCONTROL_H
#define TAO_EC_REACTIVE_CONSUMERCONTROL_H
#include "ace/pre.h"

#include "EC_ConsumerControl.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/Event_Handler.h"

class TAO_EC_Event_Channel;

class TAO_EC_Reactive_ConsumerControl;

/**
 * @class TAO_EC_ConsumerControl_Adapter
 *
 * @brief Forwards timeout events to the Reactive ConsumerControl
 *
 * The Reactive ConsumerControl strategy uses the reactor to
 * periodically wakeup and verify the state of the consumers
 * registered with the Event Channel.
 */
class TAO_RTEvent_Export TAO_EC_ConsumerControl_Adapter : public ACE_Event_Handler
{
public:
  /// Constructor
  TAO_EC_ConsumerControl_Adapter (TAO_EC_Reactive_ConsumerControl *adaptee);

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  /// The adapted object
  TAO_EC_Reactive_ConsumerControl *adaptee_;
};

/**
 * @class TAO_EC_Reactive_ConsumerControl
 *
 * @brief ConsumerControl
 *
 * Defines the interface for the consumer control strategy.
 * This strategy handles misbehaving or failing consumers.
 */
class TAO_RTEvent_Export TAO_EC_Reactive_ConsumerControl : public TAO_EC_ConsumerControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_EC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                   const ACE_Time_Value &timeout,
                                   TAO_EC_Event_Channel *event_channel,
                                   CORBA::ORB_ptr orb);

  /// destructor...
  virtual ~TAO_EC_Reactive_ConsumerControl (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_EC_ConsumerControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void consumer_not_exist (TAO_EC_ProxyPushSupplier *proxy
                                   ACE_ENV_ARG_DECL_NOT_USED);
  virtual void system_exception (TAO_EC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &
                                 ACE_ENV_ARG_DECL_NOT_USED);

private:
  /// Check if the consumers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_consumers (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The polling timeout
  ACE_Time_Value timeout_;

  /// The Adapter for the reactor events
  TAO_EC_ConsumerControl_Adapter adapter_;

  /// The event channel
  TAO_EC_Event_Channel *event_channel_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// To control the timeout policy in the thread
  CORBA::PolicyCurrent_var policy_current_;

  /// Precomputed policy list to the set timeout.
  CORBA::PolicyList policy_list_;

  /// The ORB reactor
  ACE_Reactor *reactor_;
};

// ****************************************************************

class TAO_EC_Ping_Consumer : public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  TAO_EC_Ping_Consumer (TAO_EC_ConsumerControl *control);

  virtual void work (TAO_EC_ProxyPushSupplier *supplier
                     ACE_ENV_ARG_DECL);

private:
  TAO_EC_ConsumerControl *control_;
};

#if defined (__ACE_INLINE__)
#include "EC_Reactive_ConsumerControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_CONSUMERCONTROL_H */
