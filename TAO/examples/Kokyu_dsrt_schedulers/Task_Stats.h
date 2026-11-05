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

#include "ace/Basic_Types.h"
#include "Kokyu_dsrt_schedulers_export.h"
#include <cstddef>
#include <vector>

/**
 * @class Task_Stats
 *
 * @brief maintains readings recorded by tasks.
 */
class Kokyu_DSRT_Schedulers_Export Task_Stats
{
 public:
  /// Constructor
  Task_Stats () = default;
  Task_Stats (Task_Stats const &) = delete;
  Task_Stats &operator= (Task_Stats const &) = delete;
  Task_Stats (Task_Stats &&) = delete;
  Task_Stats &operator= (Task_Stats &&) = delete;

  /// Destructor
  ~Task_Stats () = default;

  /// Init
  int init (std::size_t max_samples);

  /// Record a sample
  int sample (ACE_UINT32 thr_run_time, int thr_count);

  void dump_samples (ACE_TCHAR const *file_name, ACE_TCHAR const *msg);
 protected:
  struct Sample
  {
    ACE_UINT32 run_time {};
    int count {};
  };

  /// The number of samples
  std::size_t samples_count_ {};

  /// The execution time and recorded execution count for each sample.
  std::vector<Sample> samples_;
};

#if defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

#endif /* TASK_STATS_H */
