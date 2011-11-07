/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RedGreen_Test.h
 *
 *  $Id$
 *
 * Performance test to show improvement in Notify performance by picking the
 * correcting configuration.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_RedGreen_Test_CLIENT_H
#define NOTIFY_RedGreen_Test_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class RedGreen_Test_StructuredPushConsumer;
class RedGreen_Test_StructuredPushSupplier;

/**
 * @class Worker
 *
 * @brief Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  /// Constructor.
  Worker (void);

  void orb (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);

  // Shutdown ORB
  void done (void);

private:
  /// The orb
  CORBA::ORB_var orb_;
};

/**
 * @class RedGreen_Test
 *
 * @brief RedGreen_Test
 *
 * Shows how consumers RedGreen_Test for events.
 */
class RedGreen_Test
{

 public:
  // = Initialization
  RedGreen_Test (void);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  void dump_results (void);

  /// Initialize the Client.
  void init (int argc,
             ACE_TCHAR *argv []);

  /// Run the demo.
  void run (void);

  /// Called when all events we are waiting for have occured.
  void done (void);

  /// Destroy from the EC
  void destroy_ec (void);

  int burst_size_;
  Worker worker_;
  int nthreads_;

 protected:
  /// Initializes the ORB.
  void init_ORB (int argc,
                 ACE_TCHAR *argv []);

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

  // = Data Members.
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

  RedGreen_Test_StructuredPushConsumer* normal_consumer_;
  RedGreen_Test_StructuredPushConsumer* slow_consumer_;

  RedGreen_Test_StructuredPushSupplier* supplier_;
};

/*****************************************************************/
class RedGreen_Test_StructuredPushConsumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
  // = TITLE
  //   RedGreen_Test_StructuredPushConsumer
  //
  // = DESCRIPTION
  //   Consumer for the RedGreen_Test example.
  //

 public:
  // = Initialization and Termination code
  /// Constructor.
  RedGreen_Test_StructuredPushConsumer (RedGreen_Test* RedGreen_Test);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  virtual void disconnect (void);

  /// Accessor for the Proxy that we're connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (
      void
    );

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats &throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (const ACE_TCHAR* msg, ACE_UINT32 gsf);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  /// callback <done>
  RedGreen_Test* RedGreen_Test_;

  /// Protect internal state
  TAO_SYNCH_MUTEX lock_;

  int push_count_;

  /// Used for reporting stats
  ACE_Throughput_Stats throughput_;

  // = Methods
  /// Destructor
  virtual ~RedGreen_Test_StructuredPushConsumer (void);

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

class SlowConsumer : public RedGreen_Test_StructuredPushConsumer
{
public:
  SlowConsumer (RedGreen_Test* RedGreen_Test);

  virtual void push_structured_event (
      const CosNotification::StructuredEvent & notification
    );
};


/*****************************************************************/
class RedGreen_Test_StructuredPushSupplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
  // = TITLE
  //   RedGreen_Test_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Supplier for the RedGreen_Test example.
  //
 public:
  // = Initialization and Termination code
  /// Constructor.
  RedGreen_Test_StructuredPushSupplier (void);

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect (void);

  /// Send one event.
  virtual void send_event (CosNotification::StructuredEvent& event);

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats &throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (const ACE_TCHAR* msg, ACE_UINT32 gsf);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

  /// Measure the elapsed time spent while sending the events.
  ACE_Throughput_Stats throughput_;

  // = Protected Methods
  /// Destructor
  virtual ~RedGreen_Test_StructuredPushSupplier ();

  // = NotifyRedGreen_Test
  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

  // = StructuredPushSupplier method
  virtual void disconnect_structured_push_supplier (
    );
};

/*****************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_RedGreen_Test_CLIENT_H */
