#include "ace/Log_Msg.h"

ACE_INLINE int
Task_Stats::sample (ACE_UINT32 thr_run_time, int thr_count)
{
  if (this->samples_count_ >= this->samples_.size ())
    {
      ACE_DEBUG ((LM_DEBUG, "Task_Stats::sample ret -1\n"));
      return -1;
    }

  this->samples_[this->samples_count_] = {thr_run_time, thr_count};

  ++this->samples_count_;
  return 0;
}
