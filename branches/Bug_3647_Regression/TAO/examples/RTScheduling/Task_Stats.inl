//$Id$

#include "ace/Log_Msg.h"

ACE_INLINE int
Task_Stats::sample (time_t thr_run_time, int thr_count)
{
  if (this->samples_count_ >= this->max_samples_)
  {
    ACE_DEBUG ((LM_DEBUG, "Task_Stats::sample ret -1\n"));
    return -1;
  }

  this->thr_run_time_[this->samples_count_] = thr_run_time;
  this->thr_count_[this->samples_count_] = thr_count;

  this->samples_count_++;
  return 0;
}
