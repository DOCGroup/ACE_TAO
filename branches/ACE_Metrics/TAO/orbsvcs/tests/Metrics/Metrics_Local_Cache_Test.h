// $Id$

#ifndef METRICS_LOGGER_TEST_H
#define METRICS_LOGGER_TEST_H

#include "orbsvcs/Metrics/Metrics_LocalCache.h"
#include "orbsvcs/Metrics/Metrics_Utils.h"
#include "orbsvcs/Metrics/Metrics_UpcallMonitor.h"
#include "orbsvcs/Metrics/Metrics_FrameManager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Metrics_Logger;
class TAO_Rate_Group_Info;

class TAO_Metrics_Local_Cache_Test_PushConsumer : public POA_RtecEventComm::PushConsumer
{
public:

  TAO_Metrics_Local_Cache_Test_PushConsumer (int state = 0) : state_variable_ (state) {}

  virtual void push (const RtecEventComm::EventSet &,
                     CORBA_Environment &);

  virtual void disconnect_push_consumer (CORBA_Environment &);

private:

  int state_variable_;
};

class TAO_Metrics_Local_Cache_Test
{
  // = TITLE
  //   Defines a class that implements a test driver for
  //   the testing the Metrics Cache and sending data to the
  //   Metrics Logger.
  //
public:
  TAO_Metrics_Local_Cache_Test (int local_monitor = 1, // Default is to create a local monitor
                                int local_logger = 0);  // Default is not to create a local logger
  // Default Constructor.

  TAO_Metrics_Local_Cache_Test (int argc, 
                                char *argv[], 
                                int local_monitor = 1, 
                                int local_logger = 0);
  // Constructor taking command line arguments.

  virtual ~TAO_Metrics_Local_Cache_Test (void);
  // Destructor.

  int init (int argc, 
            char *argv[]);
  // Initialize the Scheduling Service with the arguments.

  int run (CORBA_Environment& ACE_TRY_ENV);
  // Run the TAO_Metrics_Local_Cache_Test.

private:

  typedef TAO_Metrics_ReportingUpcallMonitorAdapter<ACE_SYNCH_MUTEX, ACE_New_Allocator> METRICS_UPCALL_ADAPTER_TYPE;

  void generate_orb_args (char *& orb_init_str);
  // parses the arguments.

  int parse_args (int argc, char *argv[]);
  // parses the arguments.

  static void * run_orb (void *);
  // Run the orb event loop (can be spawned as a separate thread).

  // ORB reference.
  CORBA::ORB_ptr orb_;

  // Root POA reference.
  PortableServer::POA_var root_poa_;

  // Child (persistent) POA reference.
  PortableServer::POA_var persistent_poa_;

  // POA manager.
  PortableServer::POAManager_var poa_manager_;

  TAO_Metrics_Local_Cache_Test_PushConsumer H40_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer H20_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer H10_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer H05_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer H01_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer S40_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer S20_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer S10_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer S05_push_consumer_;
  TAO_Metrics_Local_Cache_Test_PushConsumer S01_push_consumer_;
  // Push consumers with varying execution time state sequence
  // (0, 0.025, 0.05, 0.1, 0.2, 1, 2 seconds) for push call.

  TAO_Metrics_Logger * logger_impl_;
  // Logger instance.

  Metrics::QoSLogger_var logger_ref_;
  // Logger reference.

  TAO_Metrics_ReportingUpcallMonitor* upcall_monitor_impl_;
  // Upcall monitor instance.

  Metrics::ReportingQoSMonitor_var upcall_monitor_ref_;
  // Upcall monitor instance.

  TAO_Metrics_ReportingLocalCache<ACE_SYNCH_MUTEX, ACE_New_Allocator> cache_;
  // Upcall metrics cache.

  TAO_Metrics_FrameManager<ACE_SYNCH_MUTEX> frame_manager_;
  // The time frame manager.

  RtecEventComm::EventSet event_set_;
  // A dummy event set for pushing to the adapters (who push it to
  // their respective consumers).

  METRICS_UPCALL_ADAPTER_TYPE * H40_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * H20_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * H10_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * H05_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * H01_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * S40_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * S20_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * S10_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * S05_push_consumer_adapter_;
  METRICS_UPCALL_ADAPTER_TYPE * S01_push_consumer_adapter_;
  // Upcall adapters for the push consumers. This is where all
  // the real action happens.

  const char * logger_ior_file_name_;
  // Name of the logger IOR output file.  Used when the logger is remote.

  const char * monitor_ior_file_name_;
  // Name of the monitor IOR output file.  Used when the monitor is remote.

  const char * log_file_name_;
  // Name of the process id output file.  Used when the logger is local.

  int log_output_;
  // Whether or not to log output to file.  

  int visualize_;
  // Whether or not visualization is activated.

  int local_monitor_;
  int local_logger_; 

  static const unsigned int number_of_hrt_rates_;
  static const unsigned int number_of_srt_rates_;
  
  static const unsigned long the_hrt_Rates_in_microseconds_[];
  
  static const int highest_rate_group_priority_;
  
  // Pointer to our local rate group info
  TAO_Rate_Group_Info ** rate_group_info_;

  char* master_host_name_;
  char* master_host_ip_;
};


#endif /* METRICS_LOGGER_TEST_H */






