/* $Id$ */

#include "Timer.h"
#include "Task_Client.h"

MT_Cubit_Timer::MT_Cubit_Timer (u_int granularity)
  :granularity_ (granularity)
#if defined (CHORUS)
  , pstartTime_ (0)
  , pstopTime_ (0)
#endif /* CHORUS */
{
}

void
MT_Cubit_Timer::start (void)
{
#if defined (CHORUS)
  this->pstartTime_ = pccTime1Get();
#else /* CHORUS */
  this->timer_.start ();
#endif /* !CHORUS */
}

void
MT_Cubit_Timer::stop (void)
{
#if defined (CHORUS)
  this->pstopTime_ = pccTime1Get ();
#else /* CHORUS */
  // If CHORUS is not defined just use plain timer_.stop ().
  this->timer_.stop ();
  this->timer_.elapsed_time (this->delta_);
#endif /* !CHORUS */
}

ACE_timer_t
MT_Cubit_Timer::get_elapsed (void)
{
  ACE_timer_t real_time;
#if defined (ACE_LACKS_FLOATING_POINT)
# if defined (CHORUS)
  real_time = (this->pstopTime_ - this->pstartTime_) / this->granularity_;
# else /* CHORUS */
  // Store the time in usecs.
  real_time = (this->delta_.sec () * ACE_ONE_SECOND_IN_USECS  +
               this->delta_.usec ()) / this->granularity_;
# endif /* !CHORUS */
#else  /* !ACE_LACKS_FLOATING_POINT */
  // Store the time in usecs.
  real_time = (ACE_timer_t) this->delta_.sec () * ACE_ONE_SECOND_IN_USECS  +
              (ACE_timer_t) this->delta_.usec ();
  real_time = real_time/this->granularity_;
#endif /* !ACE_LACKS_FLOATING_POINT */
  return real_time; // in usecs.
}
