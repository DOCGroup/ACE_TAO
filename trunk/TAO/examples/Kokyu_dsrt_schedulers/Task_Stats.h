/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Task_Stats.h
 *
 *  $Id$
 *
 *  Utility Stats class that maintains the readings.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef TASK_STATS_H
#define TASK_STATS_H

#include "ace/OS_NS_time.h"
#include "ace/Singleton.h"
#include "tao/orbconf.h"
#include "tao/debug.h"
#include "Kokyu_dsrt_schedulers_export.h"

/**
 * @class Base_Time
 *
 * @brief maintains readings recorded by tasks.
 *
 */
class Kokyu_DSRT_Schedulers_Export Base_Time
{
 public:
  Base_Time (void);
  ACE_hrtime_t base_time_;
};

typedef ACE_Singleton<Base_Time, TAO_SYNCH_MUTEX> BASE_TIME;

/**
 * @class Task_Stats
 *
 * @brief maintains readings recorded by tasks.
 *
 */
class Kokyu_DSRT_Schedulers_Export Task_Stats
{
 public:
  /// Constructor
  Task_Stats (void);

  /// Destructor
  ~Task_Stats (void);

  /// Init
  int init (size_t max_samples);

  /// Set the base time value.
  void base_time (ACE_hrtime_t time);

  /// Set the end time value.
  void end_time (ACE_hrtime_t time);

  /// Record a sample
  int sample (ACE_UINT32 thr_run_time, int thr_count);

  void dump_samples (const ACE_TCHAR *file_name, const ACE_TCHAR *msg);
 protected:
  void dump_latency_stats (ACE_TCHAR *out_msg, ACE_UINT32 sf);

  /// Base and end times
  ACE_hrtime_t base_time_;
  ACE_hrtime_t end_time_;

  /// The maximum number of samples
  ACE_UINT32 max_samples_;

  /// The number of samples
  ACE_UINT32 samples_count_;

  /// The samples : the time of execution. and the recorded exec. time .
  ACE_UINT32 *thr_run_time_;
  int *thr_count_;

  /// The minimum value
  ACE_UINT64 exec_time_min_;

  /// The number of the sample that had the minimum value
  ACE_UINT32 exec_time_min_at_;

  /// The maximum value
  ACE_UINT64 exec_time_max_;

  /// The number of the sample that had the maximum value
  ACE_UINT32 exec_time_max_at_;

  /// The sum of all the values
  ACE_UINT64 sum_;

  /// The sum of the square of all the values
  ACE_UINT64 sum2_;
};

typedef ACE_Singleton<Task_Stats, TAO_SYNCH_MUTEX> TASK_STATS;

#if defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

#endif /* TASK_STATS_H */
