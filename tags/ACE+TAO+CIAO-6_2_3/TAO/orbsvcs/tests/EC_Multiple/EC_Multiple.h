/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test attempts to communicate several Event Channels.
//   The test hardcodes all the objects involved (consumers,
//   suppliers, proxies, etc.); the objective is to gain understanding
//   on the architecture needed to exploit locality in the Event
//   cycle, not to provide a definite solution.
//
// ============================================================================

#ifndef EC_MULTIPLE_H
#define EC_MULTIPLE_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "ace/Condition_Thread_Mutex.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/CosNamingC.h"

class Test_ECG;

class Test_Supplier : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Helper class to implement the different tests within Test_ECG.
  //
  // = DESCRIPTION
  //   Test_ECG can be configured to have a single or multiple
  //   suppliers, to use the EC or short-circuit it, to use the
  //   Gateway or not; this class connects as a consumer for timeouts
  //   in the EC, at each timeout it delegates on the Test_ECG class
  //   to execute the proper test.
public:
  Test_Supplier (Test_ECG* test, void* cookie);

  void open (const char* name,
             int event_a, int event_b,
             int message_count,
             const RtecScheduler::Period_t& rate,
             RtecEventChannelAdmin::EventChannel_ptr ec);
  // This method connects the supplier to the EC.

  void close (void);
  // Disconnect from the EC.

  void activate (const char* name,
                 const RtecScheduler::Period_t& rate,
                 RtecEventChannelAdmin::EventChannel_ptr ec);

  void push (const RtecEventComm::EventSet& events);
  void disconnect_push_consumer (void);
  // Implement the callbacks for our consumer personality.


  virtual void disconnect_push_supplier (void);
  // The methods in the skeleton.

  RtecEventComm::EventSourceID supplier_id (void) const;
  // The supplier ID.

private:
  Test_ECG* test_;

  void *cookie_;
  // The test provide us a cookie so we can give back our identity.

  RtecEventComm::EventSourceID supplier_id_;
  // We generate an id based on the name....

  int event_a_;
  int event_b_;
  // The two types of events we may generate...

  int message_count_;
  // The number of events sent by this supplier.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a supplier) using this proxy.

  ACE_PushConsumer_Adapter<Test_Supplier> consumer_;
  // We also connect to the EC as a consumer so we can receive the
  // timeout events.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a supplier) using this proxy.

};

class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Helper class to implement the different tests within Test_ECG.
  //
  // = DESCRIPTION
  //   Test_ECG must collect events destined to many consumers, but
  //   needs to distinguish through which consumer it is receiving the
  //   event. The easiest way is to create a shallow class that
  //   forwards the events to the EC, but passing back some cookie to
  //   identify the consumer.
public:
  Test_Consumer (Test_ECG* test, void *cookie);

  void open (const char* name,
             int event_a, int event_b,
             RtecEventChannelAdmin::EventChannel_ptr ec);
  // This method connects the consumer to the EC.

  void close (void);
  // Disconnect from the EC.

  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);
  // The skeleton methods.

private:
  Test_ECG* test_;
  // The test class.

  void *cookie_;
  // The magic cookie that serves as our ID.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.
};

class Test_ECG
{
  //
  // = TITLE
  //   Test and demonstrate the use of TAO_EC_Gateway.
  //
  // = DESCRIPTION
  //   This class is design to exercise several features of the EC_Gateway
  //   class and the multiple EC architecture.
  //   We want to create two EC, each one having a single supplier and a
  //   single consumer.
  //    + To test the remote facilities the consumer register for both a
  //    local event and a remote one.
  //    + To test the remote filtering features the remote consumer only
  //    wants one of the local events, and this event is generated less
  //    frequently.
  //
  //   This class creates the local EC_Gateway a consumer and a
  //   supplier, it uses the command line to figure the subscriptions
  //   and publications list.
  //
public:
  Test_ECG (void);

  enum {
    MAX_EVENTS = 1024,
    // Maximum number of events to send...

    MAX_CONSUMERS = 1024,
    // Maximum number of consumers.

    MAX_SUPPLIERS = 1024
    // Maximum number of suppliers.
  };

  int run (int argc, ACE_TCHAR* argv[]);
  // Execute the test.

  void push_supplier (void* supplier_cookie,
                      RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
                      const RtecEventComm::EventSet &events);
  // Callback method for suppliers, we push for them to their
  // consumers and take statistics on the way.
  // It is possible that we ignore the <consumer> parameter when
  // testing the short-circuit case.

  void push_consumer (void* consumer_cookie,
                      ACE_hrtime_t arrival,
                      const RtecEventComm::EventSet& events);
  // Callback method for consumers, if any of our consumers has
  // received events it will invoke this method.

  void shutdown_supplier (void* supplier_cookie,
                          RtecEventComm::PushConsumer_ptr consumer);
  // One of the suppliers has completed its work.

private:
  RtecEventChannelAdmin::EventChannel_ptr
    get_ec (CosNaming::NamingContext_ptr naming_context,
            const char* ec_name);
  // Helper routine to obtain an EC given its name.

  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers (void);
  // Connect the suppliers.

  void activate_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  // Activate the suppliers, i.e. they start generating events.

  void connect_ecg (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                    RtecEventChannelAdmin::EventChannel_ptr remote_ec,
                    RtecScheduler::Scheduler_ptr remote_sch);
  // Connect the EC gateway, it builds the Subscriptions and the
  // Publications list.

  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers (void);
  // Connect and disconnect the consumers.

  int shutdown (void);
  // Called when the main thread (i.e. not the scavenger thread) is
  // shutting down.

  int parse_args (int argc, ACE_TCHAR* argv[]);
  // parse the command line args

  void dump_results (void);
  // Dump the results to the standard output.

  void wait_until_ready (void);
  // Block event delivery until all the consumers are ready.

  struct Stats;
  void dump_results (const ACE_TCHAR* name, Stats& stats);
  // Dump the results for a particular consumer.

  int local_source (RtecEventComm::EventSourceID id) const;
  // Check if <id> correspond to a local supplier.

  void shutdown_consumer (int id);
  // One of the consumers has completed its work.

private:
  ACE_CString lcl_name_;
  // The name of the "local" EC.

  ACE_CString rmt_name_;
  // The name of the "remote" EC.

  TAO_EC_Gateway_Sched ecg_;
  // The proxy used to connect both event channels.

  enum {
    ss_global,
    ss_local,
    ss_runtime
  };
  int scheduling_type_;
  // The type of scheduling service to use:
  // "global" is a remote scheduling service, usually to perform
  // simultaneous scheduling across all the processes.
  // "local" instantiate a local config time scheduling service.
  // "runtime" instantiates a local rumtime scheduling service.

  int consumer_disconnects_;
  int supplier_disconnects_;
  // How many times to disconnect the consumers (and suppliers) before
  // the final connection. This is useful to test the disconnection in
  // the EC.

  int short_circuit_;
  // Don't send the messages through the EC. This is needed to measure
  // the overhead introduced by the EC.

  Test_Supplier* suppliers_[Test_ECG::MAX_SUPPLIERS];
  Test_Consumer* consumers_[Test_ECG::MAX_CONSUMERS];
  // The suppliers and consumer arrays, the sizes are controlled using
  // {lp,hp}_{suppliers,consumers}_

  // @@ TODO it looks like the HP and LP data could be encapsulated.

  int hp_suppliers_;
  // The number of high priority suppliers in this test.

  int hp_consumers_;
  // The number of high priority consumers.

  int hp_workload_;
  // The number of iterations of ACE::is_prime() to execute in high
  // priority consumers.

  int hp_interval_;
  // The high priority events are generated using this interval.

  int hp_message_count_;
  // How many messages we will send in the HP suppliers

  int hps_event_a_;
  int hps_event_b_;
  int hpc_event_a_;
  int hpc_event_b_;
  // Each supplier send two types of events, each consumer receives
  // two other types. The types for high-priority clients can be
  // different from the types to low priority clients.

  int lp_suppliers_;
  // The number of low priority suppliers in this test.

  int lp_consumers_;
  // The number of low priority consumers.

  int lp_workload_;
  // The number of iterations of ACE::is_prime() to execute in low
  // priority consumers.

  int lp_interval_;
  // The low priority events are generated using this interval.

  int lp_message_count_;
  // How many messages we will send in the LP suppliers

  int lps_event_a_;
  int lps_event_b_;
  int lpc_event_a_;
  int lpc_event_b_;
  // Each supplier send two types of events, each consumer receives
  // two other types. The types for high-priority clients can be
  // different from the types to low priority clients.

  const ACE_TCHAR* schedule_file_;
  // Ask the schedule to compute and dump its schedule after the test
  // execution.

  const ACE_TCHAR* pid_file_name_;
  // The name of a file where the process stores its pid

  struct Stats {
    ACE_hrtime_t total_time_;
    float laxity_[MAX_EVENTS];
    ACE_hrtime_t lcl_latency_[MAX_EVENTS];
    ACE_hrtime_t end_[MAX_EVENTS];
    int lcl_count_;
    // We keep laxity and total_time stats only for the local events.

    ACE_hrtime_t rmt_latency_[MAX_EVENTS];
    int rmt_count_;
  };
  Stats stats_[Test_ECG::MAX_CONSUMERS];
  // Store the measurements for local and remote events..

  int ready_;
  TAO_SYNCH_MUTEX ready_mtx_;
  TAO_SYNCH_CONDITION ready_cnd_;
  // Before accepting any events the suppliers must wait for the test
  // to setup all the consumers.
  // The suppliers wait on the condition variable.

  ACE_Atomic_Op<TAO_SYNCH_MUTEX,int> running_suppliers_;
  // keep track of how many suppliers are still running so we shutdown
  // at the right moment.

  ACE_Atomic_Op<TAO_SYNCH_MUTEX,int> running_consumers_;
  // keep track of how many consumers are still running so we shutdown
  // at the right moment.

  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_stop_;
  // Measure the test elapsed time as well as mark the beginning of
  // the frames.
};

#endif /* EC_MULTIPLE_H */
