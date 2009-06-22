/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Subscribe.h
//
// = DESCRIPTION
//   Class to demo structured event subscription.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_SUBSCRIBE_CLIENT_H
#define NOTIFY_SUBSCRIBE_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

class Subscribe_StructuredPushConsumer;
class Subscribe_StructuredPushSupplier;

class Subscribe
{
  // = TITLE
  //   Subscribe
  // = DESCRIPTION
  //   Shows how consumers subscribe for events.

 public:
  // = Initialization and Termination
  Subscribe (void);
  ~Subscribe ();

  void init (int argc, ACE_TCHAR *argv []);
  // Init the Client.

  void run (void);
  // Run the demo.

  void done (void);
  // Called when all events we are waiting for have occured.

 protected:
  void init_ORB (int argc, ACE_TCHAR *argv []);
  // Initializes the ORB.

  void resolve_naming_service (void);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (void);
  // Try to resolve the Notify factory from the Naming service.

  void create_EC (void);
  // Create an EC.

  void create_supplieradmin(void);
  // Create the Supplier Admin.

  void create_consumeradmin (void);
  // Create the Consumer Admin.

  void create_consumers (void);
  // Create and initialize the consumers.

  void create_suppliers (void);
  // create and initialize the suppliers.

  void send_events (void);
  // send the events.

  // = Data Members
  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // Channel factory.

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
  // The group operator between admin-proxy's.

  CosNotification::QoSProperties initial_qos_;
  // Initial qos specified to the factory when creating the EC.

  CosNotification::AdminProperties initial_admin_;
  // Initial admin props specified to the factory when creating the EC.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  Subscribe_StructuredPushConsumer* consumer_1_;
  Subscribe_StructuredPushConsumer* consumer_2_;

  Subscribe_StructuredPushSupplier* supplier_1_;
  Subscribe_StructuredPushSupplier* supplier_2_;

  CORBA::Boolean done_;
  // Set this flag to exit the run loop.
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
  Subscribe_StructuredPushConsumer (Subscribe* subscribe);
  // Constructor.

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  // Connect the Consumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (void);
  // Disconnect from the supplier.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (void);
  // Accessor for the Proxy that we're connected to.

  // public data member for evaluating the results of subscription.
  CosNotification::EventTypeSeq expected_subscription_;
protected:
  // = Data members
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The proxy_supplier id.

  Subscribe* subscribe_;
  // callback <done>

  // = Methods
  virtual ~Subscribe_StructuredPushConsumer (void);
  // Destructor

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
  Subscribe_StructuredPushSupplier (void);
  // Constructor.

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy consumer and connects to it.

  void disconnect (void);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event);
  // Send one event.

protected:
  // = Data members
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This supplier's id.

  // = Protected Methods
  virtual ~Subscribe_StructuredPushSupplier ();
  // Destructor

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
