// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_Reactive_ConsumerControl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/
 */
//=============================================================================


#ifndef TAO_CEC_REACTIVE_CONSUMERCONTROL_H
#define TAO_CEC_REACTIVE_CONSUMERCONTROL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_ConsumerControl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"

#include "tao/ORB.h"

#include "ace/Event_Handler.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;

class TAO_CEC_Reactive_ConsumerControl;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
class TAO_CEC_TypedEventChannel;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

/**
 * @class TAO_CEC_ConsumerControl_Adapter
 *
 * @brief Forwards timeout events to the Reactive ConsumerControl
 *
 * The Reactive ConsumerControl strategy uses the reactor to
 * periodically wakeup and verify the state of the consumers
 * registered with the Event Channel.
 */
class TAO_Event_Serv_Export TAO_CEC_ConsumerControl_Adapter : public ACE_Event_Handler
{
public:
  /// Constructor
  TAO_CEC_ConsumerControl_Adapter (TAO_CEC_Reactive_ConsumerControl *adaptee);

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  /// The adapted object
  TAO_CEC_Reactive_ConsumerControl *adaptee_;
};

/**
 * @class TAO_CEC_Reactive_ConsumerControl
 *
 * @brief ConsumerControl
 *
 * Defines the interface for the consumer control strategy.
 * This strategy handles misbehaving or failing consumers.
 * = MEMORY MANAGMENT
 * = LOCKING
 * = TODO
 */
class TAO_Event_Serv_Export TAO_CEC_Reactive_ConsumerControl
  : public TAO_CEC_ConsumerControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_CEC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                    const ACE_Time_Value &timeout,
                                    unsigned int retries,
                                    TAO_CEC_EventChannel *event_channel,
                                    CORBA::ORB_ptr orb);

  /// Constructor for the typed ec.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_Reactive_ConsumerControl (
      const ACE_Time_Value &rate,
      const ACE_Time_Value &timeout,
      unsigned int retries,
      TAO_CEC_TypedEventChannel *typed_event_channel,
      CORBA::ORB_ptr orb
    );
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// destructor...
  virtual ~TAO_CEC_Reactive_ConsumerControl (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_CEC_ConsumerControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void consumer_not_exist (TAO_CEC_ProxyPushSupplier *proxy);
  virtual void consumer_not_exist (TAO_CEC_ProxyPullSupplier *proxy);
  virtual void system_exception (TAO_CEC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &);

  /// Do we need to disconnect this supplier?  The parameter type for
  /// proxy is PortableServer::ServantBase* due to the fact that this
  /// method will be used for TAO_CEC_ProxyPushSupplier's and
  /// TAO_CEC_ProxyPullSupplier's.
  virtual bool need_to_disconnect (PortableServer::ServantBase* proxy);

  /// Allow others to inform us when a send or receive was successful.
  virtual void successful_transmission (PortableServer::ServantBase* proxy);

private:
  /// Check if the consumers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_consumers (void);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The polling timeout
  ACE_Time_Value timeout_;

  /// The number of retries per proxy until it is disconnected
  unsigned int retries_;

  /// The Adapter for the reactor events
  TAO_CEC_ConsumerControl_Adapter adapter_;

  /// The event channel
  TAO_CEC_EventChannel *event_channel_;

  /// The typed event channel
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_TypedEventChannel *typed_event_channel_;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

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

// ****************************************************************

class TAO_CEC_Ping_Push_Consumer
  : public TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier>
{
public:
  TAO_CEC_Ping_Push_Consumer (TAO_CEC_ConsumerControl *control);

  virtual void work (TAO_CEC_ProxyPushSupplier *supplier);

private:
  TAO_CEC_ConsumerControl *control_;
};

// ****************************************************************

class TAO_CEC_Ping_Pull_Consumer
  : public TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier>
{
public:
  TAO_CEC_Ping_Pull_Consumer (TAO_CEC_ConsumerControl *control);

  virtual void work (TAO_CEC_ProxyPullSupplier *supplier);

private:
  TAO_CEC_ConsumerControl *control_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Reactive_ConsumerControl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_CONSUMERCONTROL_H */
