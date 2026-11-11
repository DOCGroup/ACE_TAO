/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Filter.h
 *
 * Class to demo structured event filtering.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef NOTIFY_FILTER_CLIENT_H
#define NOTIFY_FILTER_CLIENT_H

#include <atomic>

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SString.h"

class Filter_StructuredPushConsumer;
class Filter_StructuredPushSupplier;

/**
 * @class FilterClient
 *
 * @brief Filter Client
 *
 * Client example that shows how to do Structured Event filtering
 * in the Notification Service.
 */
class FilterClient
{
 public:
  FilterClient () = default;
  FilterClient (FilterClient const &) = delete;
  FilterClient &operator= (FilterClient const &) = delete;
  FilterClient (FilterClient &&) = delete;
  FilterClient &operator= (FilterClient &&) = delete;

  ~FilterClient ();

  /// Init the Client.
  void init (int argc, ACE_TCHAR *argv []);

  /// Run the demo.
  void run ();

  /// Consumer calls done, We're done.
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
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_ {CosNotifyChannelAdmin::AND_OP};

  /// Initial qos specified to the factory when creating the EC.
  CosNotification::QoSProperties initial_qos_ {};

  /// Initial admin props specified to the factory when creating the EC.
  CosNotification::AdminProperties initial_admin_ {};

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_ {};

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_ {};

  /// Consumer #1
  Filter_StructuredPushConsumer *consumer_1_ {};

  /// Consumer #2
  Filter_StructuredPushConsumer *consumer_2_ {};

  /// Supplier #1
  Filter_StructuredPushSupplier *supplier_1_ {};

  /// Supplier #2
  Filter_StructuredPushSupplier *supplier_2_ {};

  /// Set this flag to exit the run loop.
  std::atomic_bool done_ {};
};

/*****************************************************************/
class Filter_StructuredPushConsumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
  // = TITLE
  //   Filter_StructuredPushConsumer
  //
  // = DESCRIPTION
  //   Consumer for the Filter example.
  //

 public:
  Filter_StructuredPushConsumer (FilterClient *filter, char const *my_name);
  Filter_StructuredPushConsumer (Filter_StructuredPushConsumer const &) = delete;
  Filter_StructuredPushConsumer &operator= (Filter_StructuredPushConsumer const &) = delete;
  Filter_StructuredPushConsumer (Filter_StructuredPushConsumer &&) = delete;
  Filter_StructuredPushConsumer &operator= (Filter_StructuredPushConsumer &&) = delete;

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  void disconnect ();

protected:
  // = Data members

  /// The callback for <done>
  FilterClient *filter_ {};

  /// The name of this consumer.
  ACE_CString my_name_ {};

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_ {};

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  // = Methods
  ~Filter_StructuredPushConsumer () override = default;

  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;

  // = StructuredPushSupplier methods
  void push_structured_event (CosNotification::StructuredEvent const &notification) override;

  void disconnect_structured_push_consumer () override;
};

/*****************************************************************/

class Filter_StructuredPushSupplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
  // = TITLE
  //   Filter_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Supplier for the filter example.
  //
 public:
  explicit Filter_StructuredPushSupplier (char const *my_name);
  Filter_StructuredPushSupplier (Filter_StructuredPushSupplier const &) = delete;
  Filter_StructuredPushSupplier &operator= (Filter_StructuredPushSupplier const &) = delete;
  Filter_StructuredPushSupplier (Filter_StructuredPushSupplier &&) = delete;
  Filter_StructuredPushSupplier &operator= (Filter_StructuredPushSupplier &&) = delete;

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Send one event.
  void send_event (CosNotification::StructuredEvent const &event);

protected:
  // = Data members
  /// The name of this consumer.
  ACE_CString my_name_ {};

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_ {};

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_ {};

  // = Protected Methods
  ~Filter_StructuredPushSupplier () override = default;

  // = NotifySubscribe
  void subscription_change (CosNotification::EventTypeSeq const &added,
                            CosNotification::EventTypeSeq const &removed) override;

  // = StructuredPushSupplier method
  void disconnect_structured_push_supplier () override;
};

#endif /* NOTIFY_FILTER_CLIENT_H */
