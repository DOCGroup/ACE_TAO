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

#if !defined (EC_MULTIPLE_H)
#define EC_MULTIPLE_H

#include "ace/SString.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/Event/EC_Gateway.h"

class Test_ECG : public ACE_Task_Base
//
// = TITLE
//   A simple test for the EC_Proxy class.
//
// = DESCRIPTION
//   This class is design to exercise several features of the EC_Proxy
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
{
public:
  Test_ECG (void);

  enum {
    DEFAULT_EVENT_COUNT = 128,
    // The default event count.

    MAX_EVENTS = 256000
    // Maximum number of events to send...
  };

  int run (int argc, char* argv[]);
  // Execute the test.

  void disconnect_push_supplier (CORBA::Environment &);
  void disconnect_push_consumer (CORBA::Environment &);
  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
  // Implement the consumer and supplier upcalls.

  virtual int svc (void);
  // Run the scavenger thread for the utilization test.

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg = 0);
  // Used when short circuiting the EC, this is the Event_Handler
  // callback.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  int run_short_circuit (CORBA::ORB_ptr orb,
			 PortableServer::POA_ptr root_poa,
			 PortableServer::POAManager_ptr poa_manager,
			 CORBA::Environment& _env);
  // Run the short-circuit version of the test, i.e. we don't use the
  // EC to pass the events.
  // To simulate the periodic nature of the EC event generation we run
  // the ORB and connect to its Reactor for the timeouts.

  int run_ec (CORBA::ORB_ptr orb,
	      PortableServer::POA_ptr root_poa,
	      PortableServer::POAManager_ptr poa_manager,
	      CORBA::Environment& _env);
  // Run the test using the EC.

  void dump_results (void);
  // Dumpt the results to the standard output.

  RtecEventChannelAdmin::EventChannel_ptr
    get_ec (CosNaming::NamingContext_ptr naming_context,
	    const char* ec_name,
	    CORBA::Environment &_env);
  // Helper routine to obtain an EC given its name.

  int connect_supplier (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			CORBA::Environment &_env);
  // Connect the supplier, it builds the proper Publication list based
  // on the command line args.

  int connect_consumer (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			CORBA::Environment &_env);
  // Connect the consumer, it builds the proper Subscription list
  // based on the command line args.

  int connect_ecg (RtecEventChannelAdmin::EventChannel_ptr local_ec,
		   RtecEventChannelAdmin::EventChannel_ptr remote_ec,
		   RtecScheduler::Scheduler_ptr remote_sch,
		   CORBA::Environment &_env);
  // Connect the EC gateway, it builds the Subscriptions and the
  // Publications list.

  int shutdown (CORBA::Environment&);
  // Called when the main thread (i.e. not the scavenger thread) is
  // shutting down.

private:
  ACE_PushConsumer_Adapter<Test_ECG> consumer_; 
  // Our consumer personality....

  ACE_PushSupplier_Adapter<Test_ECG> supplier_;
  // Our supplier personality....

  TAO_EC_Gateway ecg_;
  // The proxy used to connect both event channels.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_; 
  // We talk to the EC (as a supplier) using this proxy.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a consumer) using this proxy.

  RtecEventComm::EventSourceID supplier_id_;
  // Our ID as a supplier.

  char* lcl_ec_name_;
  // The name of the "local" EC.

  char* rmt_ec_name_;
  // The name of the "remote" EC.

  char* lcl_sch_name_;
  // The name of the local scheduling service.

  char* rmt_sch_name_;
  // The name of the remote scheduling service.

  int global_scheduler_;
  // Don't create a local scheduler.

  int short_circuit_;
  // Don't send the messages through the EC. This is needed to measure
  // the overhead introduced by the EC.

  int interval_;
  // The interval between the messages.

  int message_count_;
  // How many messages we will send

  int event_a_;
  int event_b_;
  int event_c_;
  // We generate events <a> and <b> and receive events <a> and <c>,
  // this allows for a lot of configurations (making a == c or
  // different, etc.)

  ACE_hrtime_t lcl_time_[Test_ECG::MAX_EVENTS];
  int lcl_count_;
  ACE_hrtime_t rmt_time_[Test_ECG::MAX_EVENTS];
  int rmt_count_;
  // Store the measurements for local and remote events..

  ACE_High_Res_Timer scavenger_timer_;
  // Measure the time it takes to run the scavenger thread.

  ACE_Barrier scavenger_barrier_;
  // The scavenger thread should not start until the EC is running.

  int scavenger_count_;
  // The number of iterations to run in the scavenger thread, if 0
  // then there utilization thread is not started (this is good to
  // measure just latency).

  int scavenger_priority_;
  // The priority at which the scavenger thread runs, must be lower
  // that all the other threads.

  ACE_High_Res_Timer push_timer_;
  // Measure the time spent in pushes...

  int push_count_;
  // Number of iterations of ACE::is_prime() in the push() method.

  const char* schedule_file_;
  // Ask the schedule to compute and dump its schedule after the test
  // execution.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid
};

#endif /* EC_MULTIPLE_H */
