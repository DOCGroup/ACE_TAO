/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Filter.h
 *
 *  $Id$
 *
 * Class to demo structured event filtering.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_FILTER_CLIENT_H
#define NOTIFY_FILTER_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SString.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  // = Initialization and Termination
  /// Constructor
  FilterClient (void);

  /// Destructor
  ~FilterClient ();

  /// Init the Client.
  void init_supplier (int argc, ACE_TCHAR *argv []);
  void init_consumer (int argc, ACE_TCHAR *argv []);

  /// Run the demo.
  void run_supplier ();
  void run_consumer ();

  /// Consumer calls done, We're done.
  void done (void);

 protected:

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Initializes the ORB.
  void init_ORB (int& argc, ACE_TCHAR **& argv);

  /// Try to get hold of a running naming service.
  void resolve_naming_service ();

  /// Try to resolve the Notify factory from the Naming service.
  void resolve_Notify_factory ();

  /// Create an EC.
  void create_EC ();
  void get_EC ();

  /// Create the Supplier Admin.
  void create_supplieradmin();
  void get_supplieradmin();

  /// Create the Consumer Admin.
  void create_consumeradmin ();
  void get_consumeradmin ();

  /// Create and initialize the consumers.
  void create_consumers ();

  /// create and initialize the suppliers.
  void create_suppliers ();

  /// send the events.
  void send_events ();

  void wait_ready ();

  void wait_consumer_complete ();

  void verify_filter (CosNotifyFilter::FilterAdmin_var& admin,
                      const char* constraint_expr,
                      const char* mod_constraint_expr);

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
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_1_;
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_2_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumer #1
  Filter_StructuredPushConsumer* consumer_1;

  /// Consumer #2
  Filter_StructuredPushConsumer* consumer_2;

  /// Supplier #1
  Filter_StructuredPushSupplier* supplier_1;

  /// Supplier #2
  Filter_StructuredPushSupplier* supplier_2;

  CosNotifyChannelAdmin::AdminID adminid_1_id_;
  CosNotifyChannelAdmin::AdminID adminid_2_id_;

  CosNotifyChannelAdmin::ChannelID channel_id_;

  bool use_persistent_;

  bool modify_constraint_;

  // Set this flag to exit the run loop.
  CORBA::Boolean done_;
};

/*****************************************************************/
/**
 * @class Filter_StructuredPushConsumer
 *
 * @brief Filter_StructuredPushConsumer
 *
 * Consumer for the Filter example.
 */
class Filter_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer
{

 public:
  // = Initialization and Termination code
  /// Constructor.
  Filter_StructuredPushConsumer (FilterClient* filter, const char *my_name);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  virtual void disconnect ();

protected:
  // = Data members

  /// The callback for <done>
  FilterClient* filter_;

  /// The name of this consumer.
  ACE_CString my_name_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  // = Methods
  /// Destructor
  virtual ~Filter_StructuredPushConsumer (void);

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
                               );

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
                                      );

  virtual void disconnect_structured_push_consumer ();
};

/*****************************************************************/

 /**
  * @class Filter_StructuredPushSupplier
  *
  * @brief Filter_StructuredPushSupplier
  *
  * Supplier for the filter example.
  */
class Filter_StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier
{
 public:
  // = Initialization and Termination code
  /// Constructor.
  Filter_StructuredPushSupplier (const char* my_name);

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Send one event.
  virtual void send_event (const CosNotification::StructuredEvent& event);

protected:
  // = Data members
  /// The name of this consumer.
  ACE_CString my_name_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

  // = Protected Methods
  /// Destructor
  virtual ~Filter_StructuredPushSupplier ();

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
                                    );

  // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier ();
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_FILTER_CLIENT_H */
