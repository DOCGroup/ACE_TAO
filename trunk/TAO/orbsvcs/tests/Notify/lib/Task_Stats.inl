//$Id$

#include "ace/High_Res_Timer.h"

ACE_INLINE double
Task_Stats::diff_sec (ACE_hrtime_t& before, ACE_hrtime_t& after)
{
  double seconds =
#if defined ACE_LACKS_LONGLONG_T
    (after - before) / gsf_;
#else  /* ! ACE_LACKS_LONGLONG_T */
    ACE_static_cast (double,
                     ACE_UINT64_DBLCAST_ADAPTER((after - before) / gsf_));
#endif /* ! ACE_LACKS_LONGLONG_T */
  seconds /= ACE_HR_SCALE_CONVERSION;

  return seconds;
}

ACE_INLINE ACE_UINT32
Task_Stats::diff_usec (ACE_hrtime_t& before, ACE_hrtime_t& after)
{
  // convert to microseconds
  #if !defined ACE_LACKS_LONGLONG_T
   ACE_UINT32 elapsed_microseconds = ACE_UINT32((after - before) / gsf_);
  #else  /* ! ACE_LACKS_LONGLONG_T */

  ACE_UINT32 elapsed_microseconds = (after - before) / gsf_;

  #endif /* ! ACE_LACKS_LONGLONG_T */

  #if defined (ACE_WIN32)
    elapsed_microseconds*=1000; // convert to uSec on Win32
  #endif /* ACE_WIN32 */

  return elapsed_microseconds;
}

ACE_INLINE int
Task_Stats::sample (ACE_UINT64& inv_start_time, ACE_UINT64& inv_end_time)
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
