/* $Id$ */

#include "Timer.h"
#include "Task_Client.h"

MT_Cubit_Timer::MT_Cubit_Timer (u_int granularity)
  :granularity_ (granularity)
#if defined (CHORUS_MVME)
  , pstartTime_ (0)
  , pstopTime_ (0)
#endif /* CHORUS_MVME */
{
}

void
MT_Cubit_Timer::start (void)
{
#if defined (CHORUS_MVME)
  this->pstartTime_ = pccTime1Get();
#else /* CHORUS_MVME */
  this->timer_.start ();
#endif /* !CHORUS_MVME */
}

void
MT_Cubit_Timer::stop (void)
{
#if defined (CHORUS_MVME)
  this->pstopTime_ = pccTime1Get ();
#else /* CHORUS_MVME */
  // If CHORUS_MVME is not defined just use plain timer_.stop ().
  this->timer_.stop ();
  this->timer_.elapsed_time (this->delta_);
#endif /* !CHORUS_MVME */
}

ACE_timer_t
MT_Cubit_Timer::get_elapsed (void)
{
  ACE_timer_t real_time;
  // Store the time in usecs.
  real_time = (ACE_timer_t) this->delta_.sec () * ACE_ONE_SECOND_IN_USECS  +
              (ACE_timer_t) this->delta_.usec ();
  real_time = real_time/this->granularity_;
  return real_time; // in usecs.
}
