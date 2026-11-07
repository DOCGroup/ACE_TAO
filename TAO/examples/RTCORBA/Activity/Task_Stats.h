/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Task_Stats.h
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
#include "ace/High_Res_Timer.h"

#include <memory>

/**
 * @class Base_Time
 *
 * @brief maintains readings recorded by tasks.
 */
class Base_Time
{
 public:
  Base_Time ();
  ACE_hrtime_t base_time_;
};

using BASE_TIME = ACE_Singleton<Base_Time, TAO_SYNCH_MUTEX>;

/**
 * @class Task_Stats
 *
 * @brief maintains readings recorded by tasks.
 */
class Task_Stats
{
 public:
  /// Constructor
  explicit Task_Stats (size_t max_samples);

  /// Destructor
  ~Task_Stats () = default;

  Task_Stats (Task_Stats const &) = delete;
  Task_Stats (Task_Stats &&) = delete;
  Task_Stats &operator= (Task_Stats const &) = delete;
  Task_Stats &operator= (Task_Stats &&) = delete;

  /// Init
  int init ();

  /// Set the base time value.
  void base_time (ACE_hrtime_t time);

  /// Set the end time value.
  void end_time (ACE_hrtime_t time);

  /// Record a sample
  int sample (ACE_UINT64 inv_start_time, ACE_UINT64 inv_end_time);

  void dump_samples (ACE_TCHAR const *file_name, ACE_TCHAR const *msg,
                     ACE_High_Res_Timer::global_scale_factor_type scale_factor);
 protected:
  void dump_latency_stats (ACE_TCHAR *out_msg,
                           ACE_High_Res_Timer::global_scale_factor_type sf);

  /// Base and end times
  ACE_hrtime_t base_time_;
  ACE_hrtime_t end_time_;

  /// The maximum number of samples
  ACE_UINT32 max_samples_;

  /// The number of samples
  ACE_UINT32 samples_count_;

  /// The samples : the time of invocation. and the recorded exec. time .
  std::unique_ptr<ACE_UINT64[]> time_inv_;
  std::unique_ptr<ACE_UINT64[]> time_exec_;

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

#if defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

#endif /* TASK_STATS_H */
