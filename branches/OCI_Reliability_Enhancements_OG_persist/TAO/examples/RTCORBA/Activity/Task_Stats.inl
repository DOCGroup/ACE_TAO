//$Id$

#include "ace/Log_Msg.h"

ACE_INLINE int
Task_Stats::sample (ACE_UINT64 inv_start_time, ACE_UINT64 inv_end_time)
{
  if (this->samples_count_ >= this->max_samples_)
  {
    ACE_DEBUG ((LM_DEBUG, "Task_Stats::sample ret -1\n"));
    return -1;
   }
  ACE_UINT64 inv_value, exec_value;

  inv_value = inv_start_time - base_time_;
  exec_value  = inv_end_time - inv_start_time;

  this->time_inv_[this->samples_count_] = inv_value;
  this->time_exec_[this->samples_count_] = exec_value;

  this->samples_count_++;

  if (this->samples_count_ == 1u)
    {
      this->exec_time_min_ = exec_value;
      this->exec_time_min_at_ = this->samples_count_;
      this->exec_time_max_ = exec_value;
      this->exec_time_max_at_ = this->samples_count_;
      this->sum_ = exec_value;
      this->sum2_ = exec_value * exec_value;
    }
  else
    {
      if (this->exec_time_min_ > exec_value)
        {
          this->exec_time_min_ = exec_value;
          this->exec_time_min_at_ = this->samples_count_;
        }
      if (this->exec_time_max_ < exec_value)
        {
          this->exec_time_max_ = exec_value;
          this->exec_time_max_at_ = this->samples_count_;
        }

      this->sum_  += exec_value;
      this->sum2_ += exec_value * exec_value;
 }
    return 0;
}
