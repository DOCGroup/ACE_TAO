/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Driver.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_DRIVER_H
#define EC_DRIVER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"

#include "ace/Stats.h"
#include "ace/Task.h"
#include "ace/Throughput_Stats.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_EC_Event_Channel_Attributes;
TAO_END_VERSIONED_NAMESPACE_DECL

class TAO_Module_Factory;
class EC_Consumer;
class EC_Supplier;

#define EC_DISABLE_REMOTE_EC
#define EC_DISABLE_OLD_EC

#if !defined(EC_DISABLE_REMOTE_EC)
#include "orbsvcs/CosNamingC.h"
#endif

/**
 * @class EC_Driver
 *
 * @brief Defines the interface of a test driver
 *
 * Abstract base class for the test driver, this let us implement a
 * collocated and a non-collocated test, and to write generic
 * consumers and suppliers that can call back the driver.
 */
class EC_Test_Export EC_Driver
{
public:
  /// Constructor
  EC_Driver (void);

  /// Destructor
  virtual ~EC_Driver (void);

  /// Flag to indicate in the test should be verbose.
  int verbose (void) const;

  /// Execute the test.
  virtual int run (int argc, ACE_TCHAR* argv[]);

  /// The initialization section
  virtual void run_init (int& argc, ACE_TCHAR* argv[]);

  /// The cleanup section
  virtual void run_cleanup (void);

  /// Initialize the ORB and obtain the RootPOA object
  virtual void initialize_orb_and_poa (int& argc, ACE_TCHAR* argv[]);

  /// Parse the common command-line arguments for all tests
  virtual int parse_args (int& argc, ACE_TCHAR* argv[]);

  /// Print the usage method
  virtual void print_usage (void);

  /// Print out the arguments
  virtual void print_args (void) const;

  /// Run the test in the real-time class, return -1 on error.
  virtual int move_to_rt_class (void);

  /// Construct the EC and its helper objects, also activates the EC in
  /// the RootPOA.
  virtual void initialize_ec_impl (void);

  /// By default connect the consumers and then the suppliers, other
  /// orders should work too.
  virtual void connect_clients (void);

  /// By default disconnect the suppliers and then the consumers, other
  /// orders should work too.
  virtual void disconnect_clients (void);

  /// By default deactivate the suppliers and then the consumers, other
  /// orders should work too.
  virtual void shutdown_clients (void);

  /// Connect all the consumers, by default it lets each consumer
  /// connect itself.
  virtual void connect_consumers (void);

  /// Connect consumer number <i> using the consumer_admin provided.
  virtual void connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i);

  /// Build the QoS requirements for consumer <i>
  virtual void build_consumer_qos (
      int i,
      RtecEventChannelAdmin::ConsumerQOS& qos,
      int& shutdown_event_type);

  /// Connect all the suppliers, by default it lets each supplier
  /// connect itself.
  virtual void connect_suppliers (void);

  /// Connect supplier number <i> using the supplier_admin provided.
  virtual void connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i);

  /// Build the QoS requirements for supplier <i>
  virtual void build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type);

  /// Execute the test, by default simply call activate_suppliers()
  virtual void execute_test (void);

  /**
   * Dump the results, i.e. invoke dump_results on all the suppliers
   * and consumers, collect the latency and throughput results for
   * each and print the totals too.
   */
  virtual void dump_results (void);

  /// Disconnect all the consumers.
  virtual void disconnect_consumers (void);

  /// Disconnect all the suppliers.
  virtual void disconnect_suppliers (void);

  /// Deactivate all the consumers.
  virtual void shutdown_consumers (void);

  /// Deactivate all the suppliers.
  virtual void shutdown_suppliers (void);

  /// Call EC->destroy
  virtual void destroy_ec (void);

  /// De-activate the EC (and its helper classes).
  virtual void deactivate_ec (void);

  /// Cleanup the resources
  virtual void cleanup_ec (void);
  virtual void cleanup_tasks (void);
  virtual void cleanup_consumers (void);
  virtual void cleanup_suppliers (void);

  /// Allow modifications of the default EC attributes
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);

  /// Returns the index of the consumer for @a cookie
  virtual int decode_consumer_cookie (void* cookie) const;

  /// Returns the index of the supplier for @a cookie
  virtual int decode_supplier_cookie (void* cookie) const;

  /// One of the consumers in the test has received an event
  virtual void consumer_push (void* consumer_cookie,
                              const RtecEventComm::EventSet& event);

  /// One of the consumers has received a shutdown event
  virtual void consumer_shutdown (void* consumer_cookie);

  /// One of the consumers in the test has been disconnected from the EC
  virtual void consumer_disconnect (void* consumer_cookie);

  /// One of the suppliers in the test has been disconnected from the EC
  virtual void supplier_disconnect (void* supplier_cookie);

#if !defined(EC_DISABLE_REMOTE_EC)
  /// Obtain the EC from the Naming service
  virtual void obtain_remote_ec (void);

  virtual CosNaming::NamingContext_ptr
       get_naming_context (void);
#endif

  /// Initialize the EC using the new implementation
  virtual void initialize_new_ec (void);

  /// Allocate the suppliers and the consumers
  virtual int allocate_consumers (void);
  virtual int allocate_suppliers (void);

  /// Allocate one consumer or supplier
  virtual EC_Consumer* allocate_consumer (int i);
  virtual EC_Supplier* allocate_supplier (int i);

  /// Allocate one task for supplier number <i>
  virtual int allocate_tasks (void);
  virtual ACE_Task_Base* allocate_task (int i);

  /// Activate all the tasks, by default runs each supplier on its
  /// own thread.
  virtual void activate_tasks (void);

protected:
  /// The ORB
  CORBA::ORB_var orb_;

  /// The Root POA
  PortableServer::POA_var root_poa_;

  /// The number of consumers
  int n_consumers_;

  /// The consumers
  EC_Consumer** consumers_;

  /// The number of suppliers
  int n_suppliers_;

  /// The suppliers
  EC_Supplier** suppliers_;

  /// The tasks for each supplier
  ACE_Task_Base** tasks_;

  /// How many bursts we will send from each supplier.
  int burst_count_;

  /// The number of events
  int burst_size_;

  /// The size of the payload on each event.
  int payload_size_;

  /// The time between each event burst, in microseconds.
  int burst_pause_;

  /**
   * The consumers subscribe to different sets of events, as follows:
   *
   * Consumer0: [start          , start           + count)
   * Consumer1: [start + 1*shift, start + 1*shift + count)
   * Consumer2: [start + 2*shift, start + 2*shift + count)
   * .....
   * ....
   * .
   * Some sub-tests may choose other configurations, but this is good
   * for many cases.
   */
  int consumer_type_start_;
  int consumer_type_count_;
  int consumer_type_shift_;

  /**
   * The suppliers generate different sets of events, as follows:
   *
   * Supplier0: [start          , start           + count)
   * Supplier1: [start + 1*shift, start + 1*shift + count)
   * Supplier2: [start + 2*shift, start + 2*shift + count)
   * .....
   * ....
   * .
   * Some sub-tests may choose other configurations, but this is good
   * for many cases.
   */
  int supplier_type_start_;
  int supplier_type_count_;
  int supplier_type_shift_;

  /// The name of a file where the process stores its pid
  const ACE_TCHAR* pid_file_name_;

  /// Verbosity flag
  int verbose_;

  /**
   * The flags used to create threads, by default we attempt:
   *
   * THR_SCHED_FIFO|THR_BOUND|THR_NEW_LWP
   *
   * but if we don't have enough privileges we fall back to
   *
   * THR_NEW_LWP
   *
   */
  int thr_create_flags_;

  /// Use a remote event channel.  If this flag is enabled it
  /// superseedes any options for the event channel implementation.
  int use_remote_ec_;

  /// The name of the remote event service
  const ACE_TCHAR *event_service_name_;

  /// The event channel implementation
  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;

  /// The event channel object reference
  RtecEventChannelAdmin::EventChannel_var event_channel_;

};

#if defined (__ACE_INLINE__)
#include "Driver.inl"
#endif /* __ACE_INLINE__ */

#endif /* ECT_CONSUMER_H */
