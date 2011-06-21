/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Subscribe.h
 *
 *  $Id$
 *
 * Class to demo structured event subscription.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_SUBSCRIBE_CLIENT_H
#define NOTIFY_SUBSCRIBE_CLIENT_H

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
  // = Initialization and Termination
  Subscribe (void);
  ~Subscribe ();

  /// Init the Client.
  void init (int argc, ACE_TCHAR *argv []);

  /// Run the demo.
  void run (void);

  /// Called when all events we are waiting for have occured.
  void done (void);

 protected:
  /// Initializes the ORB.
  void init_ORB (int argc, ACE_TCHAR *argv []);

  /// Try to get hold of a running naming service.
  void resolve_naming_service (void);

  /// Try to resolve the Notify factory from the Naming service.
  void resolve_Notify_factory (void);

  /// Create an EC.
  void create_EC (void);

  /// Create the Supplier Admin.
  void create_supplieradmin(void);

  /// Create the Consumer Admin.
  void create_consumeradmin (void);

  /// Create and initialize the consumers.
  void create_consumers (void);

  /// create and initialize the suppliers.
  void create_suppliers (void);

  /// send the events.
  void send_events (void);

  // = Data Members
  /// Reference to the root poa.
  PortableServer::POA_var root_poa_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// Channel factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The group operator between admin-proxy's.
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

  /// Initial qos specified to the factory when creating the EC.
  CosNotification::QoSProperties initial_qos_;

  /// Initial admin props specified to the factory when creating the EC.
  CosNotification::AdminProperties initial_admin_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  Subscribe_StructuredPushConsumer* consumer_1_;
  Subscribe_StructuredPushConsumer* consumer_2_;

  Subscribe_StructuredPushSupplier* supplier_1_;
  Subscribe_StructuredPushSupplier* supplier_2_;

  /// Set this flag to exit the run loop.
  CORBA::Boolean done_;
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
  // = Initialization and Termination code
  /// Constructor.
  Subscribe_StructuredPushConsumer (Subscribe* subscribe);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  virtual void disconnect (void);

  /// Accessor for the Proxy that we're connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (void);

  // public data member for evaluating the results of subscription.
  CosNotification::EventTypeSeq expected_subscription_;
protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  /// callback <done>
  Subscribe* subscribe_;

  // = Methods
  /// Destructor
  virtual ~Subscribe_StructuredPushConsumer (void);

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
      );

  virtual void disconnect_structured_push_consumer (
        );
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
  // = Initialization and Termination code
  /// Constructor.
  Subscribe_StructuredPushSupplier (void);

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy consumer and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect (void);

  /// Send one event.
  virtual void send_event (const CosNotification::StructuredEvent& event);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

  // = Protected Methods
  /// Destructor
  virtual ~Subscribe_StructuredPushSupplier ();

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );

  // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
      );
};

#endif /* NOTIFY_SUBSCRIBE_CLIENT_H */
