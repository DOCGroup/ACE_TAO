/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Driver
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_DRIVER_H
#define EC_DRIVER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"

class TAO_EC_Event_Channel_Attributes;
class TAO_Module_Factory;
class EC_Consumer;
class EC_Supplier;

#define EC_DISABLE_REMOTE_EC
#define EC_DISABLE_OLD_EC

#if !defined(EC_DISABLE_REMOTE_EC)
#include "orbsvcs/CosNamingC.h"
#endif

class EC_Test_Export EC_Driver
{
  //
  // = TITLE
  //   Defines the interface of a test driver
  //
  // = DESCRIPTION
  //   Abstract base class for the test driver, this let us implement a
  //   collocated and a non-collocated test, and to write generic
  //   consumers and suppliers that can call back the driver.
  //
public:
  EC_Driver (void);
  // Constructor

  virtual ~EC_Driver (void);
  // Destructor

  int verbose (void) const;
  // Flag to indicate in the test should be verbose.

  virtual int run (int argc, char* argv[]);
  // Execute the test.

  virtual void run_init (int& argc, char* argv[],
                         CORBA::Environment& ACE_TRY_ENV);
  // The initialization section

  virtual void run_cleanup (CORBA::Environment& ACE_TRY_ENV);
  // The cleanup section

  virtual void initialize_orb_and_poa (int& argc, char* argv[],
                                       CORBA::Environment& env);
  // Initialize the ORB and obtain the RootPOA object

  virtual int parse_args (int& argc, char* argv[]);
  // Parse the common command-line arguments for all tests

  virtual void print_usage (void);
  // Print the usage method

  virtual void print_args (void) const;
  // Print out the arguments

  virtual int move_to_rt_class (void);
  // Run the test in the real-time class, return -1 on error.

  virtual void initialize_ec_impl (CORBA::Environment& env);
  // Construct the EC and its helper objects, also activates the EC in
  // the RootPOA.

  virtual void connect_clients (CORBA::Environment& env);
  // By default connect the consumers and then the suppliers, other
  // orders should work too.

  virtual void disconnect_clients (CORBA::Environment &env);
  // By default disconnect the suppliers and then the consumers, other
  // orders should work too.

  virtual void connect_consumers (CORBA::Environment& env);
  // Connect all the consumers, by default it lets each consumer
  // connect itself.

  virtual void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV);
  // Connect consumer number <i> using the consumer_admin provided.

  virtual void build_consumer_qos (
      int i,
      RtecEventChannelAdmin::ConsumerQOS& qos,
      int& shutdown_event_type,
      CORBA::Environment&);
  // Build the QoS requirements for consumer <i>

  virtual void connect_suppliers (CORBA::Environment& env);
  // Connect all the suppliers, by default it lets each supplier
  // connect itself.

  virtual void connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV);
  // Connect supplier number <i> using the supplier_admin provided.

  virtual void build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type,
      CORBA::Environment&);
  // Build the QoS requirements for supplier <i>

  virtual void execute_test (CORBA::Environment &env);
  // Execute the test, by default simply call activate_suppliers()

  virtual void dump_results (void);
  // Dump the results, i.e. invoke dump_results on all the suppliers
  // and consumers, collect the latency and throughput results for
  // each and print the totals too.

  virtual void disconnect_consumers (CORBA::Environment& env);
  // Disconnect all the consumers.

  virtual void disconnect_suppliers (CORBA::Environment& env);
  // Disconnect all the suppliers.

  virtual void destroy_ec (CORBA::Environment& env);
  // Call EC->destroy

  virtual void deactivate_ec (CORBA::Environment& env);
  // De-activate the EC (and its helper classes).

  virtual void cleanup_ec (void);
  virtual void cleanup_tasks (void);
  virtual void cleanup_consumers (void);
  virtual void cleanup_suppliers (void);
  // Cleanup the resources

  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);
  // Allow modifications of the default EC attributes

  virtual int decode_consumer_cookie (void* cookie) const;
  // Returns the index of the consumer for <cookie>

  virtual int decode_supplier_cookie (void* cookie) const;
  // Returns the index of the supplier for <cookie>

  virtual void consumer_push (void* consumer_cookie,
                              const RtecEventComm::EventSet& event,
                              CORBA::Environment& ACE_TRY_ENV);
  // One of the consumers in the test has received an event
  
  virtual void consumer_shutdown (void* consumer_cookie,
                                  CORBA::Environment& ACE_TRY_ENV);
  // One of the consumers has received a shutdown event

  virtual void consumer_disconnect (void* consumer_cookie,
                                    CORBA::Environment& ACE_TRY_ENV);
  // One of the consumers in the test has been disconnected from the EC

  virtual void supplier_disconnect (void* supplier_cookie,
                                    CORBA::Environment& ACE_TRY_ENV);
  // One of the suppliers in the test has been disconnected from the EC

#if !defined(EC_DISABLE_REMOTE_EC)
  virtual void obtain_remote_ec (CORBA::Environment& ACE_TRY_ENV);
  // Obtain the EC from the Naming service

  virtual CosNaming::NamingContext_var
       get_naming_context (CORBA::Environment &ACE_TRY_ENV);
#endif

#if !defined(EC_DISABLE_OLD_EC)
  virtual void initialize_old_ec (CORBA::Environment& ACE_TRY_ENV);
  // Initialize the EC using the old implementation
#endif

  virtual void initialize_new_ec (CORBA::Environment& ACE_TRY_ENV);
  // Initialize the EC using the old implementation

  virtual int allocate_consumers (void);
  virtual int allocate_suppliers (void);
  // Allocate the suppliers and the consumers

  virtual EC_Consumer* allocate_consumer (int i);
  virtual EC_Supplier* allocate_supplier (int i);
  // Allocate one consumer or supplier

  virtual int allocate_tasks (void);
  virtual ACE_Task_Base* allocate_task (int i);
  // Allocate one task for supplier number <i>

  virtual void activate_tasks (CORBA::Environment& env);
  // Activate all the tasks, by default runs each supplier on its
  // own thread.

  class EC_Test_Export Latency_Stats
  {
    // = TITLE
    //   Maintains latency statistics.
    //
    // = DESCRIPTION
    //   This class is used to keep latency statistics, collected
    //   using the high resolution timers.
    //
    // = TODO
    //   Implement a version that keeps a histogram, with bounded
    //   memory requirements.
    //
  public:
    Latency_Stats (void);

    void dump_results (const char* test_name,
                       const char* sub_test);

    void sample (ACE_hrtime_t sample);

    void accumulate (const Latency_Stats& stats);
    // Useful to merge several Latency_Stats.

  private:
    u_long n_;
    ACE_hrtime_t sum_;
    ACE_hrtime_t sum2_;
    ACE_hrtime_t min_;
    ACE_hrtime_t max_;
  };

  class EC_Test_Export Throughput_Stats
  {
    // = TITLE
    //   Maintains throughput statistics.
    //
    // = DESCRIPTION
    //   This class is used to keep throughput statistics, the data is
    //   collected using the High Resolution Timer.
    //
  public:
    Throughput_Stats (void);

    void dump_results (const char* test_name,
                       const char* sub_test);

    void start (void);
    // Start measuring the time.

    void stop (void);
    // The test has completed

    void sample (void);
    // An event has been received

    void accumulate (const Throughput_Stats& stats);
    // Useful to merge several Throughput_Stats.

  private:
    CORBA::ULong n_;
    int done_;
    ACE_hrtime_t start_;
    ACE_hrtime_t stop_;
  };

protected:
  CORBA::ORB_var orb_;
  // The ORB

  PortableServer::POA_var root_poa_;
  // The Root POA

  int n_consumers_;
  // The number of consumers

  EC_Consumer** consumers_;
  // The consumers

  int n_suppliers_;
  // The number of suppliers

  EC_Supplier** suppliers_;
  // The suppliers

  ACE_Task_Base** tasks_;
  // The tasks for each supplier

  int burst_count_;
  // How many bursts we will send from each supplier.

  int burst_size_;
  // The number of events

  int payload_size_;
  // The size of the payload on each event.

  int burst_pause_;
  // The time between each event burst, in microseconds.

  int consumer_type_start_;
  int consumer_type_count_;
  int consumer_type_shift_;
  // The consumers subscribe to different sets of events, as follows:
  //
  // Consumer0: [start          , start           + count)
  // Consumer1: [start + 1*shift, start + 1*shift + count)
  // Consumer2: [start + 2*shift, start + 2*shift + count)
  // .....
  // ....
  // .
  // Some sub-tests may choose other configurations, but this is good
  // for many cases.

  int supplier_type_start_;
  int supplier_type_count_;
  int supplier_type_shift_;
  // The suppliers generate different sets of events, as follows:
  //
  // Supplier0: [start          , start           + count)
  // Supplier1: [start + 1*shift, start + 1*shift + count)
  // Supplier2: [start + 2*shift, start + 2*shift + count)
  // .....
  // ....
  // .
  // Some sub-tests may choose other configurations, but this is good
  // for many cases.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  int verbose_;
  // Verbosity flag

  int thr_create_flags_;
  // The flags used to create threads, by default we attempt:
  //
  // THR_SCHED_FIFO|THR_BOUND|THR_NEW_LWP
  //
  // but if we don't have enough privileges we fall back to
  //
  // THR_NEW_LWP
  //

  int use_remote_ec_;
  // Use a remote event channel.  If this flag is enabled it
  // superseedes any options for the event channel implementation.

  const char* event_service_name_;
  // The name of the remote event service

  int use_old_ec_;
  // Use the old implementation of the event channel.

#if !defined(TAO_EC_DISABLE_OLD_EC)
  TAO_Module_Factory* module_factory_;
  // The module factory for the OLD ec
#endif

  POA_RtecScheduler::Scheduler *scheduler_impl_;
  // The scheduler implementation

  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler object reference

  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;
  // The event channel implementation

  RtecEventChannelAdmin::EventChannel_var event_channel_;
  // The event channel object reference
};

#if defined (__ACE_INLINE__)
#include "Driver.i"
#endif /* __ACE_INLINE__ */

#endif /* ECT_CONSUMER_H */
