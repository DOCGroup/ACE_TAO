// $Id$

#ifndef METRICS_LOCAL_CACHE_T_H
#define METRICS_LOCAL_CACHE_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Metrics_Cache.h"
#include "ace/Metrics_Cache_T.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/MetricsC.h"
#include "orbsvcs/Metrics/Metrics_Utils.h"

#if defined (ACE_HAS_METRICS_BASE_POINTERS)
  typedef ACE_Based_Pointer<char> ACE_METRICS_STRING_TYPE;
  typedef ACE_Based_Pointer<Metrics::TimeprobeParameter_Set> ACE_METRICS_PARAMETER_SET_TYPE;
#else
  typedef char * ACE_METRICS_STRING_TYPE;
  typedef Metrics::TimeprobeParameter_Set * ACE_METRICS_PARAMETER_SET_TYPE;
#endif

//DCL:Commented out to remove Linux compile error
//#include "orbsvcs/Metrics/Metrics_LocalCache.h"

template <typename ACE_LOCK, typename ALLOCATOR>
class TAO_Metrics_LocalTimeprobe
  : public ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>
{
  // = TITLE
  //   Defines a class that implements a local timeprobe.
  //
  // = DESCRIPTION
  //   This class makes use of the <ACE_Metrics_Timeprobe> class to
  //   implement a timeprobe for use in a local metrics cache.
public:

  typedef TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR> TAO_METRICS_TIMEPROBE_TYPE;

  TAO_Metrics_LocalTimeprobe (u_int id = 0,
                              const char *name = 0,
                              u_long size
                                = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE,
                              ALLOCATOR * alloc = 0);
  // Default constructor.

  void report_intervals (int report_full_interval_times = 0);
  // Reports valid intervals between start and stop events.

  void install_data_set (Metrics::TimeprobeParameter_Set *data_set = 0);
  // Installs or unistalls the data set into which probe will report.

  void mark_as_operation (void) {this->is_operation_ = 1;}
  // Marks the probe as being for an operation.

  void set_hrt (int hrt) {this->is_hrt_ = hrt;}
  // Marks the probe as being SRT/HRT.

  ACE_Time_Value last_recorded_timestamp (void);
  // Returns the last timestamp that was recorded in the probe.
  // If no stamps are recorded, returns zero.

  void flush_TAO_Metrics_LocalTimeprobe ();
  // Flush the TAO metrics local timeprobe into shared memory.

protected:

  int is_operation_;
  // Indicates whether or not the probe is for an operation.

  int is_hrt_;
  // Indicates whether or not the probe is Hard Real-Time.

  ACE_METRICS_PARAMETER_SET_TYPE data_set_;
  // Points to a probe metrics data set.
};

template <class ACE_LOCK, class ALLOCATOR>
class TAO_Metrics_LocalCache
  : public ACE_Metrics_Cache<ACE_LOCK, ALLOCATOR>
{
  // = TITLE
  //   Defines a class that implements a local metrics cache.
  //
  // = DESCRIPTION
  //   This class makes use of the <ACE_Metrics_Cache> class to
  //   implement a cache for local use.
public:

  typedef ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>
          ACE_METRICS_TIMEPROBE_TYPE;

#if defined (ACE_HAS_METRICS_BASE_POINTERS)
  typedef ACE_Based_Pointer<ACE_METRICS_TIMEPROBE_TYPE>
          ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE;
#else
  typedef ACE_METRICS_TIMEPROBE_TYPE * ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE;
#endif

  typedef TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR> TAO_METRICS_TIMEPROBE_TYPE;

  typedef TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> TAO_METRICS_CACHE_TYPE;

  struct TAO_Metrics_Cache_Data_Base
  {
     ACE_METRICS_STRING_TYPE probe_name;
     TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR> probe;
  };

  struct TAO_Metrics_Cache_Data : public TAO_Metrics_Cache_Data_Base
  {
     int is_hrt;
     u_long missed_deadlines;
     u_long made_deadlines;
     u_long cancellations;
  };

#if defined (ACE_HAS_METRICS_BASE_POINTERS)
  typedef ACE_Based_Pointer<TAO_Metrics_Cache_Data_Base> ACE_METRICS_CACHE_DATA_BASE_TYPE;
  typedef ACE_Based_Pointer<TAO_Metrics_Cache_Data> ACE_METRICS_CACHE_DATA_TYPE;
#else
  typedef TAO_Metrics_Cache_Data_Base* ACE_METRICS_CACHE_DATA_BASE_TYPE;
  typedef TAO_Metrics_Cache_Data* ACE_METRICS_CACHE_DATA_TYPE;
#endif

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  ACE_METRICS_CACHE_DATA_BASE_TYPE,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_Null_Mutex> METRICS_BASE_MONITOR_MAP;
  // Type of map used for O(1) lookup of monitored data for operations.

  typedef ACE_Hash_Map_Entry<RtecScheduler::handle_t,
                             ACE_METRICS_CACHE_DATA_BASE_TYPE >
    METRICS_BASE_MONITOR_MAP_ENTRY;
  // Type of entry in monitor map.

  typedef ACE_Hash_Map_Iterator_Ex<RtecScheduler::handle_t,
                                   ACE_METRICS_CACHE_DATA_BASE_TYPE,
                                   ACE_Hash<RtecScheduler::handle_t>,
                                   ACE_Equal_To<RtecScheduler::handle_t>,
                                   ACE_Null_Mutex>
    METRICS_BASE_MONITOR_MAP_ITERATOR;
  // Type of iterator for monitor map.

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  ACE_METRICS_CACHE_DATA_TYPE,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_Null_Mutex> METRICS_MONITOR_MAP;
  // Type of map used for O(1) lookup of monitored data for operations.

  typedef ACE_Hash_Map_Entry<RtecScheduler::handle_t,
                             ACE_METRICS_CACHE_DATA_TYPE >
    METRICS_MONITOR_MAP_ENTRY;
  // Type of entry in monitor map.

  typedef ACE_Hash_Map_Iterator_Ex<RtecScheduler::handle_t,
                                   ACE_METRICS_CACHE_DATA_TYPE,
                                   ACE_Hash<RtecScheduler::handle_t>,
                                   ACE_Equal_To<RtecScheduler::handle_t>,
                                   ACE_Null_Mutex>
    METRICS_MONITOR_MAP_ITERATOR;
  // Type of iterator for monitor map.

  TAO_Metrics_LocalCache (u_long table_size
                            = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE,
                          u_long number_of_probes
                            = METRICS_DEFAULT_TIMEPROBE_COUNT,
                          ALLOCATOR * alloc = 0);
  // Constructor.

  ~TAO_Metrics_LocalCache () {this->fini ();}
  // Destructor.

  void reset_statistics ();
  // Reset the collection of metrics data.

  int init (u_long number_of_probes);
  // Factory method for constructing probes.

  void fini ();
  // Method for destroying probes.

  void register_upcall_adapter (RtecScheduler::handle_t handle,
                                const char *name,
                                int is_hrt,
                                unsigned short incr_upcall_id = 1);
  // Registers an upcall adapter, allocates monitoring data structures
  // for it in hash maps if not already present.

  RtecScheduler::handle_t register_base_metrics (const char *name,
                                                 TAO_Metrics_Utils::Base_Metrics_Type bmt,
                                                 unsigned short incr_base_id = 1);
  // Registers a base metrics probe, allocates monitoring data structures
  // for it in hash maps, returns a handle to the metrics segment.

  void report_upcall_start (RtecScheduler::handle_t handle);
  // Inline: Records start time of an upcall.

  int report_upcall_stop (RtecScheduler::handle_t handle,
                          const ACE_Time_Value & deadline);
  // Inline: Records end time of an upcall, returns 0 if the deadline
  // was made, 1 if the deadline was missed, or -1 if an error occurred.
  // If the passed deadline is equal to ACE_Time_Value::zero, by definition
  // the deadline is made, and the method returns 0.

  void report_upcall_cancellation (RtecScheduler::handle_t handle);
  // Inline: Reports a cancelled upcall.

  void report_base_metrics_start (RtecScheduler::handle_t handle);
  // Inline: Records start time of a base metrics segment.

  void report_base_metrics_stop (RtecScheduler::handle_t handle);
  // Inline: Records stop time of a base metrics segment.

  void report_base_metrics_suspend (RtecScheduler::handle_t handle);
  // Inline: Records start time of a base metrics segment.

  void report_base_metrics_resume (RtecScheduler::handle_t handle);
  // Inline: Records stop time of a base metrics segment.

  void flush_TAO_Metrics_LocalCache ();
  // Flush the TAO metrics local cache into shared memory.

protected:
  void increment_base_handle(TAO_Metrics_Utils::Base_Metrics_Type bmt);
  // Used to increment the probe id counter for a particular metrics type

  void increment_upcall_handle();
  // Used to increment the probe id counter for a upcall probe type

  METRICS_BASE_MONITOR_MAP base_monitor_maps_ [2];
  // These map base metrics ids to base monitor data.

  METRICS_MONITOR_MAP monitor_maps_ [2];
  // These map operation handles to upcall monitor data.

  // Count of the number of registered sched probes.
  u_long sched_count_;

  // Count of the number of registered quo probes.
  u_long quo_count_;

    // Count of the number of registered tns probes.
  u_long tns_count_;

  // Count of the number of registered teec probes.
  u_long teec_count_;

  // Count of the number of registered rtarm probes.
  u_long rtarm_count_;

  // Count of the number of registered upcall adapters.
  u_long upcall_adapter_count_;

  // Holds the next base metrics probe handle to assign
  RtecScheduler::handle_t next_base_metrics_handle_;

};



template <class ACE_LOCK, class ALLOCATOR>
class TAO_Metrics_ReportingLocalCache
  : public TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>
{
  // = TITLE
  //   Defines a class that adds metrics logger reporting to a local cache.
  //
  // = DESCRIPTION
  //   This class makes use of the <TAO_Metrics_LocalCache> as its
  //   a cache for local use, and adds reporting to the logger.
public:

  TAO_Metrics_ReportingLocalCache (u_long table_size
                                     = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE,
                                   u_long number_of_probes
                                     = METRICS_DEFAULT_TIMEPROBE_COUNT,
                                   ALLOCATOR * alloc = 0);
  // Constructor.

  ~TAO_Metrics_ReportingLocalCache () {this->fini ();}
  // Destructor.

  void output_statistics (Metrics::QoSLogger_ptr logger);
  // Output the statistics to the remote logger servant.

  void register_upcall_adapter (RtecScheduler::handle_t handle,
                                const char *name,
                                int is_hrt,
                                Metrics::QoSLogger_ptr logger);
  // Registers an upcall adapter, allocates monitoring data structures
  // for it in hash maps if not already present.

  RtecScheduler::handle_t register_base_metrics (const char *name,
                                                 TAO_Metrics_Utils::Base_Metrics_Type bmt,
                                                 Metrics::QoSLogger_ptr logger);
  // Registers a base metrics probe, allocates monitoring data structures
  // for it in hash maps, returns a handle to the metrics segment.

  int register_probes (Metrics::QoSLogger_ptr logger);
  // Registers probes with the logger.

  void flush_TAO_Metrics_ReportingLocalCache ();
  // Flush the TAO metrics local cache into shared memory.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Metrics_LocalCache_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Metrics_LocalCache_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (__ACE_INLINE__)
#include "Metrics_LocalCache_T.i"
#endif /* __ACE_INLINE__ */

#endif /* METRICS_LOCAL_CACHE_T_H */
