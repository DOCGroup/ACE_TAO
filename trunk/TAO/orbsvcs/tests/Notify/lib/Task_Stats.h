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
#include "notify_test_export.h"

/**
 * @class Base_Time
 *
 * @brief maintains readings recorded by tasks.
 *
 */
class TAO_NOTIFY_TEST_Export Base_Time
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
class TAO_NOTIFY_TEST_Export Task_Stats
{
 public:
  /// Constructor
  Task_Stats (void);

  /// Destructor
  ~Task_Stats (void);

  /// Useful Global Scale Factor
  static ACE_High_Res_Timer::global_scale_factor_type gsf_;

  /// Init
  int init (size_t max_samples);

  /// Useful helper to calculate diff in sec.
  static double diff_sec (ACE_UINT64 before, ACE_UINT64 after);

  /// Useful helper to calculate diff in usec.
  static ACE_UINT32 diff_usec (ACE_UINT64 before, ACE_UINT64 after);

  /// Set the base time value.
  void base_time (ACE_UINT64 time);

  /// Set the end time value.
  void end_time (ACE_UINT64 time);

  /// Record a sample
  int sample (ACE_UINT64 inv_start_time, ACE_UINT64 inv_end_time);

  void dump_samples (const ACE_TCHAR *file_name, const ACE_TCHAR *msg, int dump_samples);

 protected:
  /// Base and end times
  ACE_UINT64 base_time_;
  ACE_UINT64 end_time_;

  /// The maximum number of samples
  size_t max_samples_;

  /// The number of samples
  size_t samples_count_;

  /// The samples : the time of invocation. and the recorded exec. time .
  ACE_UINT64 *time_inv_;
  ACE_UINT64 *time_exec_;

  /// The mean of the data set.
  ACE_UINT64 mean_;

  /// The Var^2 of the data set.
  ACE_UINT64 var_2_;
};

#if defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

#endif /* TASK_STATS_H */
