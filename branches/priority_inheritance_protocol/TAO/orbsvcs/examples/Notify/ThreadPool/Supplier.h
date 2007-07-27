/* -*- C++ -*- */
/**
 *  @file Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SUPPLIER_H
#define TAO_Notify_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/Condition_Thread_Mutex.h"

#define TEST_MAX_CONSUMERS 10

/**
 * @class TAO_Notify_ThreadPool_Supplier
 *
 * @brief Implement a Structured Supplier.
 *
 */
class TAO_Notify_ThreadPool_Supplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
  // = Initialization and Termination code

  /// Constructor.
  TAO_Notify_ThreadPool_Supplier (TAO_Notify_ORB_Objects& orb_objects);

  /// Init
  void init (CosNotifyChannelAdmin::SupplierAdmin_var& admin, int expected_consumer_count, int max_events,
             int proxy_consumer_thread_count);

  /// Run
  void run (void);

protected:
  // = Protected Methods

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy consumer and connects to it.
  void connect (void);

  /// Disconnect the supplier.
  void disconnect (void);

  /// Deactivate.
  void deactivate (void);

  /// Send one event.
  virtual void send_event (const CosNotification::StructuredEvent& event);

  /// Destructor
  virtual ~TAO_Notify_ThreadPool_Supplier ();

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );

  // = StructuredPushSupplier method
  virtual void disconnect_structured_push_supplier (void);
  /// = Data members

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

  /// Number of Consumers expected to connect.
  int expected_consumer_count_;

  // The ORB that we use.
  CORBA::ORB_var orb_;

  // The Supplier Admin
  CosNotifyChannelAdmin::SupplierAdmin_var admin_;

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Condition that consumers are connected.
  TAO_SYNCH_CONDITION consumers_connected_;

  /// Number of consumers connected.
  int consumer_count_;

  /// Number of events to send.
  int max_events_;

  /// Number of Threads at the ProxyConsumer.
  int proxy_consumer_thread_count_;

  /// Array of events to send.
  CosNotification::StructuredEvent event_[TEST_MAX_CONSUMERS];
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_H */
