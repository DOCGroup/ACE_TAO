/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test to measure how many events per minute can the EC
//   process, it also serves as an example how how to encode complex
//   data types in a octet sequence.
//
// ============================================================================

#if !defined (ECT_CONSUMER_H)
#define ECT_CONSUMER_H

#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Channel_Clients_T.h"

class Driver;

class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Receive the events.
  //
  // = DESCRIPTION
public:
  Test_Consumer (Driver* driver, void* cookie,
                 int n_suppliers);

  void connect (const char* name,
		int event_a,
		int event_b,
		RtecEventChannelAdmin::EventChannel_ptr ec,
		CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void disconnect (CORBA::Environment &_env);
  // Disconnect from the EC.

  void dump_results (const char* name);
  // Print out the results

  virtual void push (const RtecEventComm::EventSet& events,
		     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The skeleton methods.

private:
  Driver* driver_;
  // The main driver for the test.

  void* cookie_;
  // A magic cookie passed by the driver that we pass back in our
  // callbacks.

  int n_suppliers_;
  // The number of suppliers that are feeding this consumer, we
  // terminate once we receive a shutdown event from each supplier.
  
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  ACE_SYNCH_MUTEX lock_;
  int recv_count_;
  ACE_High_Res_Timer timer_;
  // How many events we have received.

  int shutdown_count_;
  // How many shutdown events we have received.
};

class Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  Driver (void);

  enum {
    MAX_CONSUMERS = 16
    // Maximum number of consumers.
  };

  int run (int argc, char* argv[]);
  // Execute the test.

  void shutdown_consumer (void* consumer_cookie,
                          CORBA::Environment&);
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			  CORBA::Environment &_env);
  void disconnect_consumers (CORBA::Environment &_env);
  // Connect and disconnect the consumers.

  void dump_results (void);
  // Print out the results

private:
  Test_Consumer* consumers_[Driver::MAX_CONSUMERS];
  // The consumer array.

  int n_consumers_;
  // The number of consumers.

  int n_suppliers_;
  // How many suppliers are sending events, used for shutdown, each
  // supplier sends a shutdown message after it finishes, the consumer
  // finishes when all the suppliers do.

  int event_a_;
  int event_b_;
  // We send two types of events, with different contents.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  CORBA::ORB_var orb_;
  // A reference to the ORB, to shut it down properly.

  ACE_SYNCH_MUTEX lock_;
  int active_count_;
  // How many consumers are still receiving events.
};

#endif /* ECT_CONSUMER_H */
