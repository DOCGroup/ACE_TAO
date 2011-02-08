/* $Id$ */

#include "Timer.h"
#include "Task_Client.h"

MT_Cubit_Timer::MT_Cubit_Timer (u_int granularity)
  :granularity_ (granularity)
{
}

void
MT_Cubit_Timer::start (void)
{
  this->timer_.start ();
}

void
MT_Cubit_Timer::stop (void)
{
  this->timer_.stop ();
  this->timer_.elapsed_time (this->delta_);
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
