/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_Reactive_SupplierControl
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)More details can be found in:http://www.cs.wustl.edu/~coryan/EC/index.html
 */
//=============================================================================


#ifndef TAO_CEC_REACTIVE_SUPPLIERCONTROL_H
#define TAO_CEC_REACTIVE_SUPPLIERCONTROL_H
#include "ace/pre.h"

#include "CEC_SupplierControl.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_CEC_EventChannel;

class TAO_CEC_Reactive_SupplierControl;

/**
 * @class TAO_CEC_SupplierControl_Adapter
 *
 * @brief Forwards timeout events to the Reactive SupplierControl
 *
 * The Reactive SupplierControl strategy uses the reactor to
 * periodically wakeup and verify the state of the suppliers
 * registered with the Event Channel.
 */
class TAO_Event_Export TAO_CEC_SupplierControl_Adapter : public ACE_Event_Handler
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
class TAO_Event_Export TAO_CEC_Reactive_SupplierControl : public TAO_CEC_SupplierControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_CEC_Reactive_SupplierControl (const ACE_Time_Value &rate,
                                   TAO_CEC_EventChannel *event_channel,
                                   CORBA::ORB_ptr orb);

  /// destructor...
  virtual ~TAO_CEC_Reactive_SupplierControl (void);

  /// Receive the timeout from the adapter
  void handle_timeout (const ACE_Time_Value &tv,
                       const void* arg);

  // = Documented in TAO_CEC_SupplierControl
  virtual int activate (void);
  virtual int shutdown (void);
  virtual void supplier_not_exist (TAO_CEC_ProxyPushConsumer *proxy,
                                   CORBA::Environment &);
  virtual void supplier_not_exist (TAO_CEC_ProxyPullConsumer *proxy,
                                   CORBA::Environment &);
  virtual void system_exception (TAO_CEC_ProxyPullConsumer *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);

private:
  /// Check if the suppliers still exists.  It is a helper method for
  /// handle_timeout() to isolate the exceptions.
  void query_suppliers (CORBA::Environment &ACE_TRY_ENV);

private:
  /// The polling rate
  ACE_Time_Value rate_;

  /// The Adapter for the reactor events
  TAO_CEC_SupplierControl_Adapter adapter_;

  /// The event channel
  TAO_CEC_EventChannel *event_channel_;

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

class TAO_CEC_Ping_Push_Supplier : public TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>
{
public:
  TAO_CEC_Ping_Push_Supplier (TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_ProxyPushConsumer *consumer,
                     CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_SupplierControl *control_;
};

// ****************************************************************

class TAO_CEC_Ping_Pull_Supplier : public TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>
{
public:
  TAO_CEC_Ping_Pull_Supplier (TAO_CEC_SupplierControl *control);

  virtual void work (TAO_CEC_ProxyPullConsumer *consumer,
                     CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_SupplierControl *control_;
};

#if defined (__ACE_INLINE__)
#include "CEC_Reactive_SupplierControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_SUPPLIERCONTROL_H */
