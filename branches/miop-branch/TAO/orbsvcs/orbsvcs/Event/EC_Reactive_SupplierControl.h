/* -*- C++ -*- */
/**
 *  @file   EC_Reactive_SupplierControl.h
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

#ifndef TAO_EC_REACTIVE_SUPPLIERCONTROL_H
#define TAO_EC_REACTIVE_SUPPLIERCONTROL_H
#include "ace/pre.h"

#include "EC_SupplierControl.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_EC_Event_Channel;

class TAO_EC_Reactive_SupplierControl;

/**
 * @class TAO_EC_SupplierControl_Adapter
 *
 * @brief Forwards timeout events to the Reactive SupplierControl
 *
 * The Reactive SupplierControl strategy uses the reactor to
 * periodically wakeup and verify the state of the suppliers
 * registered with the Event Channel.
 */
class TAO_RTEvent_Export TAO_EC_SupplierControl_Adapter : public ACE_Event_Handler
{
public:
  /// Constructor
  TAO_EC_SupplierControl_Adapter (TAO_EC_Reactive_SupplierControl *adaptee);

  // = Documented in ACE_Event_Handler.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

private:
  /// The adapted object
  TAO_EC_Reactive_SupplierControl *adaptee_;
};

/**
 * @class TAO_EC_Reactive_SupplierControl
 *
 * @brief SupplierControl
 *
 * Defines the interface for the supplier control strategy.
 * This strategy handles misbehaving or failing suppliers.
 */
class TAO_RTEvent_Export TAO_EC_Reactive_SupplierControl : public TAO_EC_SupplierControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_EC_Reactive_SupplierControl (const ACE_Time_Value &rate,
                                   TAO_EC_Event_Channel *event_channel,
                                   CORBA::ORB_ptr orb);

  /// destructor...
  virtual ~TAO_EC_Reactive_SupplierControl (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_EC_SupplierControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void supplier_not_exist (TAO_EC_ProxyPushConsumer *proxy
                                   TAO_ENV_ARG_DECL_NOT_USED);
  virtual void system_exception (TAO_EC_ProxyPushConsumer *proxy,
                                 CORBA::SystemException &
                                 TAO_ENV_ARG_DECL_NOT_USED);

private:
  /// Check if the suppliers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_suppliers (TAO_ENV_SINGLE_ARG_DECL);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The Adapter for the reactor events
  TAO_EC_SupplierControl_Adapter adapter_;

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

class TAO_EC_Ping_Supplier : public TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
{
public:
  TAO_EC_Ping_Supplier (TAO_EC_SupplierControl *control);

  virtual void work (TAO_EC_ProxyPushConsumer *consumer
                     TAO_ENV_ARG_DECL);

private:
  TAO_EC_SupplierControl *control_;
};

#if defined (__ACE_INLINE__)
#include "EC_Reactive_SupplierControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_SUPPLIERCONTROL_H */
