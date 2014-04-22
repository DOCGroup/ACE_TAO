// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_Reactive_SupplierControl.h
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

#ifndef TAO_CEC_REACTIVE_SUPPLIERCONTROL_H
#define TAO_CEC_REACTIVE_SUPPLIERCONTROL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_SupplierControl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"

#include "tao/ORB.h"

#include "ace/Event_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_Reactive_SupplierControl;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
class TAO_CEC_TypedEventChannel;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

/**
 * @class TAO_CEC_SupplierControl_Adapter
 *
 * @brief Forwards timeout events to the Reactive SupplierControl
 *
 * The Reactive SupplierControl strategy uses the reactor to
 * periodically wakeup and verify the state of the suppliers
 * registered with the Event Channel.
 */
class TAO_Event_Serv_Export TAO_CEC_SupplierControl_Adapter
  : public ACE_Event_Handler
{
public:
  /// Constructor
  TAO_CEC_SupplierControl_Adapter (TAO_CEC_Reactive_SupplierControl *adaptee);

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  /// The adapted object
  TAO_CEC_Reactive_SupplierControl *adaptee_;
};

/**
 * @class TAO_CEC_Reactive_SupplierControl
 *
 * @brief SupplierControl
 *
 * Defines the interface for the supplier control strategy.
 * This strategy handles misbehaving or failing suppliers.
 * = MEMORY MANAGMENT
 * = LOCKING
 * = TODO
 */
class TAO_Event_Serv_Export TAO_CEC_Reactive_SupplierControl
  : public TAO_CEC_SupplierControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_CEC_Reactive_SupplierControl (const ACE_Time_Value &rate,
                                    const ACE_Time_Value &timeout,
                                    unsigned int retries,
                                    TAO_CEC_EventChannel *event_channel,
                                    CORBA::ORB_ptr orb);

  /// Constructor for the typed ec.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_Reactive_SupplierControl (
      const ACE_Time_Value &rate,
      const ACE_Time_Value &timeout,
      unsigned int retries,
      TAO_CEC_TypedEventChannel *typed_event_channel,
      CORBA::ORB_ptr orb
    );
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// destructor...
  virtual ~TAO_CEC_Reactive_SupplierControl (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_CEC_SupplierControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void supplier_not_exist (TAO_CEC_ProxyPushConsumer *proxy);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual void supplier_not_exist (TAO_CEC_TypedProxyPushConsumer *proxy);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
  virtual void supplier_not_exist (TAO_CEC_ProxyPullConsumer *proxy);
  virtual void system_exception (TAO_CEC_ProxyPullConsumer *proxy,
                                 CORBA::SystemException &);

  /// Do we need to disconnect this supplier?  The parameter type for
  /// proxy is PortableServer::ServantBase* due to the fact that this
  /// method will be used for TAO_CEC_ProxyPushSupplier's and
  /// TAO_CEC_ProxyPullSupplier's.
  virtual bool need_to_disconnect (PortableServer::ServantBase* proxy);

  /// Allow others to inform us when a send or receive was successful.
  virtual void successful_transmission (PortableServer::ServantBase* proxy);

private:
  /// Check if the suppliers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_suppliers (void);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The polling timeout
  ACE_Time_Value timeout_;

  /// The number of retries per proxy until it is disconnected
  unsigned int retries_;

  /// The Adapter for the reactor events
  TAO_CEC_SupplierControl_Adapter adapter_;

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

class TAO_CEC_Ping_Push_Supplier : public TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>
{
public:
  TAO_CEC_Ping_Push_Supplier (TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_ProxyPushConsumer *consumer);

private:
  TAO_CEC_SupplierControl *control_;
};

// ****************************************************************

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
class TAO_CEC_Ping_Typed_Push_Supplier : public TAO_ESF_Worker<TAO_CEC_TypedProxyPushConsumer>
{
public:
  TAO_CEC_Ping_Typed_Push_Supplier (TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_TypedProxyPushConsumer *consumer);

private:
  TAO_CEC_SupplierControl *control_;
};
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

// ****************************************************************

class TAO_CEC_Ping_Pull_Supplier : public TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>
{
public:
  TAO_CEC_Ping_Pull_Supplier (TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_ProxyPullConsumer *consumer);

private:
  TAO_CEC_SupplierControl *control_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Reactive_SupplierControl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_SUPPLIERCONTROL_H */
