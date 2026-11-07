/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RedGreen_Test.h
 *
 * Performance test to show improvement in Notify performance by picking the
 * correcting configuration.
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
#include <atomic>

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
  Worker () = default;

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;

  void orb (CORBA::ORB_ptr orb);

  /// The thread entry point.
  int svc () override;

private:
  /// The orb
  CORBA::ORB_var orb_ {};
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
  RedGreen_Test () = default;

  RedGreen_Test (RedGreen_Test const&) = delete;
  RedGreen_Test (RedGreen_Test&&) = delete;
  RedGreen_Test& operator= (RedGreen_Test const&) = delete;
  RedGreen_Test& operator= (RedGreen_Test&&) = delete;

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  void dump_results ();

  /// Initialize the Client.
  void init (int argc,
             ACE_TCHAR *argv []);

  /// Run the demo.
  void run ();

  /// Called when all events we are waiting for have occurred.
  void done ();

  /// Destroy from the EC
  void destroy_ec ();

protected:
  int burst_size_ {10};
  Worker worker_;
  std::atomic<int> result_count_ {};
  std::atomic<bool> consumer_is_done_ {};
  std::atomic<ACE_hrtime_t> throughput_start_ {};

  /// Initializes the ORB.
  void init_ORB (int argc,
                 ACE_TCHAR *argv []);

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

  // = Data Members.
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

  /// Non-owning pointers; their references are held by the POA.
  RedGreen_Test_StructuredPushConsumer* normal_consumer_ {};
  RedGreen_Test_StructuredPushConsumer* slow_consumer_ {};

  /// Non-owning pointer; its reference is held by the POA.
  RedGreen_Test_StructuredPushSupplier* supplier_ {};

private:
  friend class RedGreen_Test_StructuredPushConsumer;
  friend class RedGreen_Test_StructuredPushSupplier;
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
  /// Constructor.
  explicit RedGreen_Test_StructuredPushConsumer (RedGreen_Test& test_client);

  RedGreen_Test_StructuredPushConsumer (RedGreen_Test_StructuredPushConsumer const&) = delete;
  RedGreen_Test_StructuredPushConsumer (RedGreen_Test_StructuredPushConsumer&&) = delete;
  RedGreen_Test_StructuredPushConsumer& operator= (RedGreen_Test_StructuredPushConsumer const&) = delete;
  RedGreen_Test_StructuredPushConsumer& operator= (RedGreen_Test_StructuredPushConsumer&&) = delete;

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Accessor for the Proxy that we're connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier ();

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats& throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (ACE_TCHAR const* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_ {};

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  /// callback <done>
  RedGreen_Test& test_client_;

  /// Protect internal state
  TAO_SYNCH_MUTEX lock_;

  int push_count_ {};

  /// Used for reporting stats
  ACE_Throughput_Stats throughput_;

  // = Methods
  /// Destructor
  ~RedGreen_Test_StructuredPushConsumer () override = default;

  // = NotifyPublish method
  void offer_change (
      CosNotification::EventTypeSeq const& added,
      CosNotification::EventTypeSeq const& removed) override;

  // = StructuredPushSupplier methods
  void push_structured_event (
      CosNotification::StructuredEvent const& notification) override;

  void disconnect_structured_push_consumer () override;
};

/*****************************************************************/

class SlowConsumer : public RedGreen_Test_StructuredPushConsumer
{
public:
  explicit SlowConsumer (RedGreen_Test& test_client);

  ~SlowConsumer () override = default;
  SlowConsumer (SlowConsumer const&) = delete;
  SlowConsumer (SlowConsumer&&) = delete;
  SlowConsumer& operator= (SlowConsumer const&) = delete;
  SlowConsumer& operator= (SlowConsumer&&) = delete;

  void push_structured_event (
      CosNotification::StructuredEvent const& notification) override;
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
  /// Constructor.
  explicit RedGreen_Test_StructuredPushSupplier (RedGreen_Test& test_client);

  RedGreen_Test_StructuredPushSupplier (RedGreen_Test_StructuredPushSupplier const&) = delete;
  RedGreen_Test_StructuredPushSupplier (RedGreen_Test_StructuredPushSupplier&&) = delete;
  RedGreen_Test_StructuredPushSupplier& operator= (RedGreen_Test_StructuredPushSupplier const&) = delete;
  RedGreen_Test_StructuredPushSupplier& operator= (RedGreen_Test_StructuredPushSupplier&&) = delete;

  /// Connect the Supplier to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the supplier.
  void disconnect ();

  /// Send one event.
  void send_event (CosNotification::StructuredEvent& event);

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats& throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (ACE_TCHAR const* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  // = Data members
  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_ {};

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_ {};

  RedGreen_Test& test_client_;

  /// Measure the elapsed time spent while sending the events.
  ACE_Throughput_Stats throughput_;

  // = Protected Methods
  /// Destructor
  ~RedGreen_Test_StructuredPushSupplier () override = default;

  // = NotifyRedGreen_Test
  void subscription_change (
      CosNotification::EventTypeSeq const& added,
      CosNotification::EventTypeSeq const& removed) override;

  // = StructuredPushSupplier method
  void disconnect_structured_push_supplier () override;
};

/*****************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_RedGreen_Test_CLIENT_H */
