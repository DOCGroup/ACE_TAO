// $Id$

#ifndef METRICS_CACHE_T_H
#define METRICS_CACHE_T_H

#include "ace/OS.h"

// helpful macro definitions
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_COMPILE_TIMEPROBES)

#include "ace/Timeprobe.h"
#include "ace/Timeprobe_T.h"

// Defaults for initializing timeprobes and timeprobe arays.
#define METRICS_MIN_TIMEPROBE_TABLE_SIZE 256 * 4
#define METRICS_MAX_TIMEPROBE_TABLE_SIZE 256 * 256
#define METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE METRICS_MIN_TIMEPROBE_TABLE_SIZE
#define METRICS_DEFAULT_TIMEPROBE_COUNT 6

template <class ACE_LOCK, class ALLOCATOR>
class ACE_Metrics_Timeprobe :
  public ACE_Timeprobe_Ex<ACE_LOCK, ALLOCATOR>
  {
  // = TITLE
  //   This class implements a timeprobe for use in a Metrics framework.
  //
  // = DESCRIPTION
  //   This class provides a probe for specific thread and method call
  //   metrics timing points.
public:

  typedef ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>
          ACE_METRICS_TIMEPROBE_TYPE;

  typedef ACE_timeprobe_t ACE_METRICS_TIMEPROBE_DATA_TYPE;
  typedef ACE_METRICS_TIMEPROBE_TYPE* ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE;
  typedef char* ACE_METRICS_NAME_BASED_PTR_TYPE;

  enum event_id
  {
    WORK_START = 0,
    WORK_STOP = 1,
    WORK_SUSPEND = 2,
    WORK_RESUME = 3
  };
  // Enumerated timeprobe event types.

  ACE_Metrics_Timeprobe (u_int id = 0,
                         const char *name = 0,
                         u_long size = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE);
  // Default constructor: plugs in the above event descriptions.

  ACE_Metrics_Timeprobe (ALLOCATOR *allocatorPtr,
                         u_int id = 0,
                         const char *name = 0,
                         u_long size = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE);
  // Constructor with allocator: plugs in the above event descriptions.

  virtual ~ACE_Metrics_Timeprobe ();
  // Destructor.

  int is_event (const ACE_METRICS_TIMEPROBE_DATA_TYPE &t,
                ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::event_id id);
  // Returns true if a timeprobe event matches the passed id.

  const char * probe_name (void);
  void probe_name (char * name);
  // Accessor and mutator for probe name.

  u_int probe_id (void);
  // Accessor for probe id.

  void probe_id (u_int id);
  // Mutator for probe id.

  void flush_ACE_Metrics_Timeprobe ();
  // Flush the ACE metrics timeprobe into shared memory.

protected:

  u_int id_;
  // Identifier for the timeprobe.

  char* name_;
  // Name of the timeprobe.

private:

  // Declare but do not define.
  ACE_Metrics_Timeprobe (const ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> &);
  void operator =(const ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> &);
};


template <class ACE_LOCK, class ALLOCATOR>
class ACE_Metrics_Cache
{
  // = TITLE
  //   This class implements a cache for metrics timeprobe data.
  //
  // = DESCRIPTION
  //   This class allows probes to be recorded into a single cache that
  //   monitors and other higher level metrics classes can query.
public:

  typedef ACE_Metrics_Cache <ACE_LOCK, ALLOCATOR> ACE_METRICS_CACHE_TYPE;

  ACE_Metrics_Cache (u_long table_size
                       = METRICS_DEFAULT_TIMEPROBE_TABLE_SIZE,
                     u_long number_of_probes
                       = METRICS_DEFAULT_TIMEPROBE_COUNT,
                     ALLOCATOR * allocatorPtr = (ALLOCATOR*)ALLOCATOR::instance());
  // Default constructor.

  ~ACE_Metrics_Cache ();
  // Destructor.

  // = Dispatching metrics.

  void report_enqueue_start (u_int i);
  void report_enqueue_stop (u_int i);
  void report_enqueue_suspend (u_int i);
  void report_enqueue_resume (u_int i);
  // Report start, stop, suspend, and resume times of a dispatch
  // enqueue: stores data metrics on the supplier side.

  void report_dequeue_start (u_int i);
  void report_dequeue_stop (u_int i);
  void report_dequeue_suspend (u_int i);
  void report_dequeue_resume (u_int i);
  // Report start, stop, suspend, and resume times of a dispatch
  // dequeue: stores data metrics on the supplier side..

  void reset_base_statistics ();
  // Reset the metrics data on the consumer side.

  void flip_supplier_and_consumer ();
  // Flips the supplier and consumer sides.

  void flush_ACE_Metrics_Cache ();
  // Flush the ACE metrics cache into shared memory.

  void metrics_enabled(int enabled);
  // Set the enable state for metrics collection.

  int metrics_enabled(void) const;
  // Return the enable state for metrics collection.

protected:

  ALLOCATOR * allocator (void);
  // Obtain an allocator pointer correctly thunked for the current
  // address space.  If there is no allocator stored in the instance,
  // the singleton allocator in the current process is used.

  // = Implementation members.

  u_long probe_set_size_;
  // Number of probes in each supplier/consumer set.

  u_long * enqueue_count_ [2];
  u_long * dequeue_count_ [2];
  // Probe data counts for each supplier/consumer set.

  ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> ** enqueue_probes_ [2];
  ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR> ** dequeue_probes_ [2];
  // Probes for each supplier/consumer set.

  char ** enqueue_names_;
  char ** dequeue_names_;
  // Names for the probes.

  int consumer_index_;
  // Index from which probe events are being consumed.
  // for WSOA, it's the data being sent to the logger

  int supplier_index_;
  // Index to which probe events are being supplied.
  // for WSOA, it's the data being recorded from the probes

  u_long table_size_;
  // Size of the timestamp table in each probe.

  ACE_Time_Value interval_start_;
  // Interval start and stop timestamps.

  ACE_Time_Value interval_end_;
  // Interval start and stop timestamps.

  int interval_initialized_;
  // Flag to indicate whether or not start time of interval has been
  // initialized since the last reset.

  int metrics_enabled_;
  // Indicator of whether metrics is enabled.

private:

  ALLOCATOR* allocator_;
  // Allocation strategy object.

  // Declare but do not define.
  ACE_Metrics_Cache (const ACE_Metrics_Cache<ACE_LOCK, ALLOCATOR> &);
  void operator = (const ACE_Metrics_Cache<ACE_LOCK, ALLOCATOR> &);
};

#if defined (__ACE_INLINE__)
#include "Metrics_Cache_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Metrics_Cache_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Metrics_Cache_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* defined (ACE_COMPILE_TIMEPROBES) */

#endif /* METRICS_CACHE_T_H */
