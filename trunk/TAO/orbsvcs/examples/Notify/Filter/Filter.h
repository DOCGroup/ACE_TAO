/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Filter.h
//
// = DESCRIPTION
//   Class to demo structured event filtering.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_FILTER_CLIENT_H
#define NOTIFY_FILTER_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SString.h"

class Filter_StructuredPushConsumer;
class Filter_StructuredPushSupplier;

class FilterClient
{
  // = TITLE
  //   Filter Client
  // = DESCRIPTION
  //   Client example that shows how to do Structured Event filtering
  //   in the Notification Service.

 public:
  // = Initialization and Termination
  FilterClient (void);
  // Constructor

  ~FilterClient ();
  // Destructor

  void init (int argc, ACE_TCHAR *argv []);
  // Init the Client.

  void run (void);
  // Run the demo.

  void done (void);
  // Consumer calls done, We're done.

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

  Filter_StructuredPushConsumer* consumer_1;
  // Consumer #1

  Filter_StructuredPushConsumer* consumer_2;
  // Consumer #2

  Filter_StructuredPushSupplier* supplier_1;
  // Supplier #1

  Filter_StructuredPushSupplier* supplier_2;
  // Supplier #2

  CORBA::Boolean done_;
  // Set this flag to exit the run loop.
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
  // = Initialization and Termination code
  Filter_StructuredPushConsumer (FilterClient* filter, const char *my_name);
  // Constructor.

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  // Connect the Consumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (void);
  // Disconnect from the supplier.

protected:
  // = Data members

  FilterClient* filter_;
  // The callback for <done>

  ACE_CString my_name_;
  // The name of this consumer.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The proxy_supplier id.

  // = Methods
  virtual ~Filter_StructuredPushConsumer (void);
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
  // = Initialization and Termination code
  Filter_StructuredPushSupplier (const char* my_name);
  // Constructor.

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (void);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event);
  // Send one event.

protected:
  // = Data members
  ACE_CString my_name_;
  // The name of this consumer.

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This supplier's id.

  // = Protected Methods
  virtual ~Filter_StructuredPushSupplier ();
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

#endif /* NOTIFY_FILTER_CLIENT_H */
