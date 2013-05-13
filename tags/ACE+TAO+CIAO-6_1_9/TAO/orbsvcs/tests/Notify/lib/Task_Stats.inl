//$Id$

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"

ACE_INLINE double
Task_Stats::diff_sec (ACE_UINT64 before, ACE_UINT64 after)
{
  double seconds =
    static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER((after - before) / gsf_));
  seconds /= ACE_HR_SCALE_CONVERSION;

  return seconds;
}

ACE_INLINE ACE_UINT32
Task_Stats::diff_usec (ACE_UINT64 before, ACE_UINT64 after)
{
  // convert to microseconds
   ACE_UINT32 elapsed_microseconds = ACE_UINT32((after - before) / gsf_);

  #if defined (ACE_WIN32)
    elapsed_microseconds*=1000; // convert to uSec on Win32
  #endif /* ACE_WIN32 */

  return elapsed_microseconds;
}

ACE_INLINE int
Task_Stats::sample (ACE_UINT64 inv_start_time, ACE_UINT64 inv_end_time)
{
  if (this->samples_count_ >= this->max_samples_)
  {
    ACE_DEBUG ((LM_DEBUG, "Task_Stats::sample ret -1\n"));
    return -1;
   }

  this->time_inv_[this->samples_count_] = inv_start_time;
  this->time_exec_[this->samples_count_] = inv_end_time;

  this->samples_count_++;

  return 0;
}
