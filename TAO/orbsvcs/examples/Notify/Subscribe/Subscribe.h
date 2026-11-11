/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Subscribe.h
 *
 * Class to demo structured event subscription.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef NOTIFY_SUBSCRIBE_CLIENT_H
#define NOTIFY_SUBSCRIBE_CLIENT_H

#include <atomic>
#include <memory>

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

class Subscribe_StructuredPushConsumer;
class Subscribe_StructuredPushSupplier;

/**
 * @class Subscribe
 *
 * @brief Subscribe
 *
 * Shows how consumers subscribe for events.
 */
class Subscribe
{
public:
  Subscribe () = default;
  Subscribe (Subscribe const &) = delete;
  Subscribe &operator= (Subscribe const &) = delete;
  Subscribe (Subscribe &&) = delete;
  Subscribe &operator= (Subscribe &&) = delete;
  ~Subscribe ();

  /// Init the Client.
  void init (int argc, ACE_TCHAR *argv []);

  /// Run the demo.
  void run ();

  /// Called when all events we are waiting for have occurred.
  void done ();

 protected:
  /// Initializes the ORB.
  void init_ORB (int argc, ACE_TCHAR *argv []);

  /// Try to get hold of a running naming service.
  void resolve_naming_service ();

  /// Try to resolve the Notify factory from the Naming service.
  void resolve_Notify_factory ();

  /// Create an EC.
  void create_EC ();

  /// Create the Supplier Admin.
  void create_supplieradmin();

  /// Create the Consumer Admin.
  void create_consumeradmin ();

  /// Create and initialize the consumers.
  void create_consumers ();

  /// create and initialize the suppliers.
  void create_suppliers ();

  /// send the events.
  void send_events ();

  // = Data Members
  /// Reference to the root poa.
  PortableServer::POA_var root_poa_ {};

  /// The ORB that we use.
  CORBA::ORB_var orb_ {};

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_ {};

  /// Channel factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_ {};

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_ {};

  /// The group operator between admin-proxy's.
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_ {CosNotifyChannelAdmin::OR_OP};

  /// Initial qos specified to the factory when creating the EC.
  CosNotification::QoSProperties initial_qos_ {};

  /// Initial admin props specified to the factory when creating the EC.
  CosNotification::AdminProperties initial_admin_ {};

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_ {};

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_ {};

  std::unique_ptr<Subscribe_StructuredPushConsumer> consumer_1_ {};
  std::unique_ptr<Subscribe_StructuredPushConsumer> consumer_2_ {};

  std::unique_ptr<Subscribe_StructuredPushSupplier> supplier_1_ {};
  std::unique_ptr<Subscribe_StructuredPushSupplier> supplier_2_ {};

  /// Set this flag to exit the run loop.
  std::atomic_bool done_ {};
};

/*****************************************************************/
class Subscribe_StructuredPushConsumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
  // = TITLE
  //   Subscribe_StructuredPushConsumer
  //
  // = DESCRIPTION
  //   Consumer for the Subscribe example.
  //
public:
  /// Constructor.
  explicit Subscribe_StructuredPushConsumer (Subscribe *subscribe);
  ~Subscribe_StructuredPushConsumer () override = default;
  Subscribe_StructuredPushConsumer (Subscribe_StructuredPushConsumer const &) = delete;
  Subscribe_StructuredPushConsumer &operator= (Subscribe_StructuredPushConsumer const &) = delete;
  Subscribe_StructuredPushConsumer (Subscribe_StructuredPushConsumer &&) = delete;
  Subscribe_StructuredPushConsumer &operator= (Subscribe_StructuredPushConsumer &&) = delete;

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Accessor for the Proxy that we're connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier ();

  // public data member for evaluating the results of subscription.
  CosNotification::EventTypeSeq expected_subscription_ {};
protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_ {};

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  /// callback <done>
  Subscribe *subscribe_ {};

  // = Methods
  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;

  // = StructuredPushSupplier methods
  void push_structured_event (CosNotification::StructuredEvent const &notification) override;

  void disconnect_structured_push_consumer () override;
};

/*****************************************************************/

class Subscribe_StructuredPushSupplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
  // = TITLE
  //   Subscribe_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Supplier for the SUBSCRIBE example.
  //
public:
  /// Constructor.
  Subscribe_StructuredPushSupplier () = default;
  ~Subscribe_StructuredPushSupplier () override = default;
  Subscribe_StructuredPushSupplier (Subscribe_StructuredPushSupplier const &) = delete;
  Subscribe_StructuredPushSupplier &operator= (Subscribe_StructuredPushSupplier const &) = delete;
  Subscribe_StructuredPushSupplier (Subscribe_StructuredPushSupplier &&) = delete;
  Subscribe_StructuredPushSupplier &operator= (Subscribe_StructuredPushSupplier &&) = delete;

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy consumer and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Send one event.
  void send_event (CosNotification::StructuredEvent const &event);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_ {};

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_ {};

  // = Protected Methods
  // = NotifySubscribe
  void subscription_change (CosNotification::EventTypeSeq const &added,
                            CosNotification::EventTypeSeq const &removed) override;

  // = StructuredPushSupplier method
  void disconnect_structured_push_supplier () override;
};

#endif /* NOTIFY_SUBSCRIBE_CLIENT_H */
