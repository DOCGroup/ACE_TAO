/* -*- C++ -*- */
/**
 *  @file Supplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SUPPLIER_H
#define TAO_Notify_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/Condition_Thread_Mutex.h"

/**
 * @class TAO_Notify_Lanes_Supplier
 *
 * @brief Implement a Structured Supplier.
 */
class TAO_Notify_Lanes_Supplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
  /// Constructor.
  explicit TAO_Notify_Lanes_Supplier (TAO_Notify_ORB_Objects& orb_objects);
  TAO_Notify_Lanes_Supplier (TAO_Notify_Lanes_Supplier const&) = delete;
  TAO_Notify_Lanes_Supplier& operator= (TAO_Notify_Lanes_Supplier const&) = delete;
  TAO_Notify_Lanes_Supplier (TAO_Notify_Lanes_Supplier&&) = delete;
  TAO_Notify_Lanes_Supplier& operator= (TAO_Notify_Lanes_Supplier&&) = delete;

  /// Init
  void init (CosNotifyChannelAdmin::SupplierAdmin_ptr admin, int count);

  /// Run
  void run ();

protected:
  // = Protected Methods

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy consumer and connects to it.
  void connect ();

  /// Disconnect the supplier.
  void disconnect ();

  /// Deactivate.
  void deactivate ();

  /// Send one event.
  void send_event (CosNotification::StructuredEvent const& event);

  /// Destructor
  ~TAO_Notify_Lanes_Supplier () override = default;

  // = NotifySubscribe
  void subscription_change (CosNotification::EventTypeSeq const& added,
                            CosNotification::EventTypeSeq const& removed) override;

  // = StructuredPushSupplier method
  void disconnect_structured_push_supplier () override;
  /// = Data members

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_ {};

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_ {};

  /// Number of Consumers expected to connect.
  int expected_consumer_count_ {2};

  // The Supplier Admin
  CosNotifyChannelAdmin::SupplierAdmin_var admin_ {};

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Condition that consumers are connected.
  TAO_SYNCH_CONDITION consumers_connected_;

  /// Number of consumers connected.
  int consumer_count_ {};
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_H */
