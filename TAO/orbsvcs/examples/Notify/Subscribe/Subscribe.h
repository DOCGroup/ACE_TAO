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

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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

  void init (int argc, char *argv [] ACE_ENV_ARG_DECL);
  // Init the Client.

  void run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the demo.

  void done (void);
  // Called when all events we are waiting for have occured.

 protected:
  void init_ORB (int argc, char *argv [] ACE_ENV_ARG_DECL);
  // Initializes the ORB.

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (ACE_ENV_SINGLE_ARG_DECL);
  // Try to resolve the Notify factory from the Naming service.

  void create_EC (ACE_ENV_SINGLE_ARG_DECL);
  // Create an EC.

  void create_supplieradmin(ACE_ENV_SINGLE_ARG_DECL);
  // Create the Supplier Admin.

  void create_consumeradmin (ACE_ENV_SINGLE_ARG_DECL);
  // Create the Consumer Admin.

  void create_consumers (ACE_ENV_SINGLE_ARG_DECL);
  // Create and initialize the consumers.

  void create_suppliers (ACE_ENV_SINGLE_ARG_DECL);
  // create and initialize the suppliers.

  void send_events (ACE_ENV_SINGLE_ARG_DECL);
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
class Subscribe_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
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

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL);
  // Connect the Consumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (void);
  // Accessor for the Proxy that we're connected to.

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
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  virtual void disconnect_structured_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

/*****************************************************************/

class Subscribe_StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
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

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
                ACE_ENV_ARG_DECL);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event
                           ACE_ENV_ARG_DECL);
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
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

/*****************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_SUBSCRIBE_CLIENT_H */
