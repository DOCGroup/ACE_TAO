/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   RedGreen_Test.h
//
// = DESCRIPTION
//   Performance test to show improvement in Notify performance by picking the
//   correcting configuration.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_RedGreen_Test_CLIENT_H
#define NOTIFY_RedGreen_Test_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class RedGreen_Test_StructuredPushConsumer;
class RedGreen_Test_StructuredPushSupplier;

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (void);
  // ctor

  void orb (CORBA::ORB_ptr orb);

  virtual int svc (void);
  // The thread entry point.

        void done (void);
        // set done flag.

private:
  CORBA::ORB_var orb_;
  // The orb

        CORBA::Boolean done_;
        // Flag to tell that we're done servicing.
};

class RedGreen_Test
{
  // = TITLE
  //   RedGreen_Test
  // = DESCRIPTION
  //   Shows how consumers RedGreen_Test for events.

 public:
  // = Initialization and Termination
  RedGreen_Test (void);
  ~RedGreen_Test ();

  int parse_args(int argc, char *argv[]);

  void dump_results (void);

  void init (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // Init the Client.

  void run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the demo.

  void done (void);
  // Called when all events we are waiting for have occured.

  int burst_size_;

  Worker worker_;
  int nthreads_;

 protected:
  void init_ORB (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // Initializes the ORB.

  void resolve_naming_service (TAO_ENV_SINGLE_ARG_DECL);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (TAO_ENV_SINGLE_ARG_DECL);
  // Try to resolve the Notify factory from the Naming service.

  void create_EC (TAO_ENV_SINGLE_ARG_DECL);
  // Create an EC.

  void create_supplieradmin(TAO_ENV_SINGLE_ARG_DECL);
  // Create the Supplier Admin.

  void create_consumeradmin (TAO_ENV_SINGLE_ARG_DECL);
  // Create the Consumer Admin.

  void create_consumers (TAO_ENV_SINGLE_ARG_DECL);
  // Create and initialize the consumers.

  void create_suppliers (TAO_ENV_SINGLE_ARG_DECL);
  // create and initialize the suppliers.

  void send_events (TAO_ENV_SINGLE_ARG_DECL);
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

  RedGreen_Test_StructuredPushConsumer* normal_consumer_;
  RedGreen_Test_StructuredPushConsumer* slow_consumer_;

  RedGreen_Test_StructuredPushSupplier* supplier_;
};

/*****************************************************************/
class RedGreen_Test_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   RedGreen_Test_StructuredPushConsumer
  //
  // = DESCRIPTION
  //   Consumer for the RedGreen_Test example.
  //

 public:
  // = Initialization and Termination code
  RedGreen_Test_StructuredPushConsumer (RedGreen_Test* RedGreen_Test);
  // Constructor.

  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin TAO_ENV_ARG_DECL);
  // Connect the Consumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier (void);
  // Accessor for the Proxy that we're connected to.

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

protected:
  // = Data members
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The proxy_supplier id.

  RedGreen_Test* RedGreen_Test_;
  // callback <done>

  TAO_SYNCH_MUTEX lock_;
  // Protect internal state

  int push_count_;

  ACE_Throughput_Stats throughput_;
  // Used for reporting stats

  // = Methods
  virtual ~RedGreen_Test_StructuredPushConsumer (void);
  // Destructor

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  virtual void disconnect_structured_push_consumer (
        TAO_ENV_SINGLE_ARG_DECL
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

/*****************************************************************/

class SlowConsumer : public RedGreen_Test_StructuredPushConsumer
{
public:
  SlowConsumer (RedGreen_Test* RedGreen_Test);

  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));
};


/*****************************************************************/
class RedGreen_Test_StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   RedGreen_Test_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Supplier for the RedGreen_Test example.
  //
 public:
  // = Initialization and Termination code
  RedGreen_Test_StructuredPushSupplier (void);
  // Constructor.

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
                TAO_ENV_ARG_DECL);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  virtual void send_event (CosNotification::StructuredEvent& event
                           TAO_ENV_ARG_DECL);
  // Send one event.

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

protected:
  // = Data members
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This supplier's id.

  ACE_Throughput_Stats throughput_;
  // Measure the elapsed time spent while sending the events.

  // = Protected Methods
  virtual ~RedGreen_Test_StructuredPushSupplier ();
  // Destructor

  // = NotifyRedGreen_Test
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        TAO_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};

/*****************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_RedGreen_Test_CLIENT_H */
