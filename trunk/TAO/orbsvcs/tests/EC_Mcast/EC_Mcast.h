/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test attempts to communicate several Event Channels UDP
//   using multicast.
//   The test hardcodes all the objects involved (consumers,
//   suppliers, proxies, etc.); the objective is to gain understanding
//   on the use of multicast events, not to provide a complete and
//   clean implementation.
//
// ============================================================================

#if !defined (EC_MCAST_H)
#define EC_MCAST_H

#include "ace/SString.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"

class ECM_Driver;

class ECM_Supplier : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Helper class to implement the different tests within ECM_Driver.
  //
  // = DESCRIPTION
  //   ECM_Driver can be configured to have a single or mcast
  //   suppliers, to use the EC or short-circuit it, to use the
  //   Gateway or not; this class connects as a consumer for timeouts
  //   in the EC, at each timeout it delegates on the ECM_Driver class
  //   to execute the proper test.
public:
  ECM_Supplier (ECM_Driver* test, void* cookie);

  void open (const char* name,
	     int event_a, int event_b,
	     int event_count,
	     const RtecScheduler::Period& rate,
	     RtecEventChannelAdmin::EventChannel_ptr ec,
	     CORBA::Environment& _env);
  // This method connects the supplier to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  void activate (const char* name,
		 const RtecScheduler::Period& rate,
		 RtecEventChannelAdmin::EventChannel_ptr ec,
		 CORBA::Environment& _env);

  void push (const RtecEventComm::EventSet& events,
	     CORBA::Environment &_env);
  void disconnect_push_consumer (CORBA::Environment &);
  // Implement the callbacks for our consumer personality.


  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The methods in the skeleton.

  RtecEventComm::EventSourceID supplier_id (void) const;
  // The supplier ID.

private:
  ECM_Driver* test_;

  void *cookie_;
  // The test provide us a cookie so we can give back our identity.

  RtecEventComm::EventSourceID supplier_id_;
  // We generate an id based on the name....

  int event_a_;
  int event_b_;
  // The two types of events we may generate...

  int event_count_;
  // The number of events sent by this supplier.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_; 
  // We talk to the EC (as a supplier) using this proxy.

  ACE_PushConsumer_Adapter<ECM_Supplier> consumer_;
  // We also connect to the EC as a consumer so we can receive the
  // timeout events.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_; 
  // We talk to the EC (as a supplier) using this proxy.

};

class ECM_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Helper class to implement the different tests within ECM_Driver.
  //
  // = DESCRIPTION
  //   ECM_Driver must collect events destined to many consumers, but
  //   needs to distinguish through which consumer it is receiving the
  //   event. The easiest way is to create a shallow class that
  //   forwards the events to the EC, but passing back some cookie to
  //   identify the consumer.
public:
  ECM_Consumer (ECM_Driver* test, void *cookie);

  void open (const char* name,
	     int event_a, int event_b,
	     RtecEventChannelAdmin::EventChannel_ptr ec,
	     CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  virtual void push (const RtecEventComm::EventSet& events,
		     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The skeleton methods.

private:
  ECM_Driver* test_;
  // The test class.

  void *cookie_;
  // The magic cookie that serves as our ID.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.
};

class ECM_Driver
{
  //
  // = TITLE
  //   Test and demonstrate the use of TAO_EC_Gateway.
  //
  // = DESCRIPTION
  //   This class is design to exercise several features of the EC_Gateway
  //   class and the mcast EC architecture.
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
  ECM_Driver (void);

  enum {
    MAX_EVENTS = 1024,
    // Maximum number of events to send...

    MAX_CONSUMERS = 16,
    // Maximum number of consumers.

    MAX_SUPPLIERS = 16
    // Maximum number of suppliers.
  };

  int run (int argc, char* argv[]);
  // Execute the test.

  void push_supplier (void* supplier_cookie,
		      RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
		      const RtecEventComm::EventSet &events,
		      CORBA::Environment &);
  // Callback method for suppliers, we push for them to their
  // consumers and take statistics on the way.
  // It is possible that we ignore the <consumer> parameter when
  // testing the short-circuit case.

  void push_consumer (void* consumer_cookie,
		      ACE_hrtime_t arrival,
		      const RtecEventComm::EventSet& events,
		      CORBA::Environment&);
  // Callback method for consumers, if any of our consumers has
  // received events it will invoke this method.

  void shutdown_supplier (void* supplier_cookie,
			  RtecEventComm::PushConsumer_ptr consumer,
			  CORBA::Environment& _env);
  // One of the suppliers has completed its work.
			  
private:
  RtecEventChannelAdmin::EventChannel_ptr
    get_ec (CosNaming::NamingContext_ptr naming_context,
	    const char* ec_name,
	    CORBA::Environment &_env);
  // Helper routine to obtain an EC given its name.

  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			  CORBA::Environment &_env);
  void disconnect_suppliers (CORBA::Environment &_env);
  // Connect the suppliers.

  void activate_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			   CORBA::Environment &_env);
  // Activate the suppliers, i.e. they start generating events.

  void connect_ecg (RtecEventChannelAdmin::EventChannel_ptr local_ec,
		    CORBA::Environment &_env);
  // Connect the EC gateway, it builds the Subscriptions and the
  // Publications list.

  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			  CORBA::Environment &_env);
  void disconnect_consumers (CORBA::Environment &_env);
  // Connect and disconnect the consumers.

  int shutdown (CORBA::Environment&);
  // Called when the main thread (i.e. not the scavenger thread) is
  // shutting down.

  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void dump_results (void);
  // Dump the results to the standard output.

  void wait_until_ready (void);
  // Block event delivery until all the consumers are ready.

  struct Stats;
  void dump_results (const char* name, Stats& stats);
  // Dump the results for a particular consumer.

  int local_source (RtecEventComm::EventSourceID id) const;
  // Check if <id> correspond to a local supplier.

  void shutdown_consumer (int id);
  // One of the consumers has completed its work.
			  
private:
  char* lcl_name_;
  // The name of the "local" EC.

  int short_circuit_;
  // Send events directly to local consumers.

  int n_suppliers_;
  // The number of suppliers in this test.

  int n_consumers_;
  // The number of consumers.

  int workload_;
  // The number of iterations of ACE::is_prime() to execute.

  int event_period_;
  // The events are generated using this interval.

  int event_count_;
  // How many events will the suppliers send

  int s_event_a_;
  int s_event_b_;
  int c_event_a_;
  int c_event_b_;
  int r_event_a_;
  int r_event_b_;
  // Each supplier send two types of events, each consumer receives
  // two other types. The remote suppliers send other two types of
  // events.

  const char* schedule_file_;
  // Ask the schedule to compute and dump its schedule after the test
  // execution.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  ACE_INET_Addr send_mcast_group_;
  // The multicast group to send the consumer events.

  TAO_ECG_UDP_Sender sender_;
  // This consumer sends the local events through <send_mcast_group>

  ACE_INET_Addr recv_mcast_group_;
  // The multicast group to receive the consumer events.

  TAO_ECG_UDP_Receiver receiver_;
  // This supplier pushes the remote events received throug
  // <recv_mcast_group> 

  TAO_ECG_Mcast_EH mcast_eh_;
  // The event handler to receive the mcast events.

  ECM_Supplier* suppliers_[ECM_Driver::MAX_SUPPLIERS];
  ECM_Consumer* consumers_[ECM_Driver::MAX_CONSUMERS];
  // The suppliers and consumer arrays, the sizes are controlled using
  // {lp,hp}_{suppliers,consumers}_

  // @@ TODO it looks like the HP and LP data could be encapsulated.

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
  Stats stats_[ECM_Driver::MAX_CONSUMERS];
  // Store the measurements for local and remote events..

  int ready_;
  ACE_SYNCH_MUTEX ready_mtx_;
  ACE_SYNCH_CONDITION ready_cnd_;
  // Before accepting any events the suppliers must wait for the test
  // to setup all the consumers.
  // The suppliers wait on the condition variable.

  ACE_Atomic_Op<ACE_SYNCH_MUTEX,int> running_suppliers_;
  // keep track of how many suppliers are still running so we shutdown
  // at the right moment.

  ACE_Atomic_Op<ACE_SYNCH_MUTEX,int> running_consumers_;
  // keep track of how many consumers are still running so we shutdown
  // at the right moment.

  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_stop_;
  // Measure the test elapsed time as well as mark the beginning of
  // the frames.
};

#endif /* EC_MCAST_H */
