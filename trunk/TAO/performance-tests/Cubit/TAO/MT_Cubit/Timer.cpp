/* $Id$ */

#include "Timer.h"
#include "Task_Client.h"

MT_Cubit_Timer::MT_Cubit_Timer (u_int granularity)
  :granularity_ (granularity)
#if defined (CHORUS)
  ,pstartTime_ (0)
   pstopTime_ (0)
#endif
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
          pstopTime = pccTime1Get ();
#else /* CHORUS */
          // if CHORUS is not defined just use plain timer_.stop ().
          this->timer_.stop ();
          this->timer_.elapsed_time (this->delta_);
#endif /* !CHORUS */
}

ACE_timer_t
MT_Cubit_Timer::get_elapsed (void)
{
  ACE_timer_t real_time;
#if defined (ACE_LACKS_FLOATING_POINT)
#   if defined (CHORUS)
  real_time = (this->pstopTime_ - this->pstartTime_) / this->granularity_;
#   else /* CHORUS */
  // Store the time in usecs.
  real_time = (this->delta_.sec () * ACE_ONE_SECOND_IN_USECS  +
               this->delta_.usec ()) / this->granularity_;
#endif /* !CHORUS */
#else /* !ACE_LACKS_FLOATING_POINT */
  
  // Store the time in secs.

#if defined (VXWORKS)
          // @@ David, these comments are to temporarily fix what
          // seems a bug in the ACE_Long_Long class that is used to
          // calc the elapsed time.  It seems that subtraction of two
          // ACE_Long_Long are not done correctly when the least
          // significant value has wrapped around.  For example to
          // subtract these values: 00ff1001:00000001 minus
          // 00ff1000:ffffffff would give a huge number, instead of
          // giving 2.

          // This is only occuring in VxWorks.
          // I'll leave these here to debug it later.
          ACE_timer_t tmp = (ACE_timer_t)delta_t.sec ();
          ACE_timer_t tmp2 = (ACE_timer_t)delta_t.usec ();
          if (tmp > 100000)
            {
              tmp = 0.0;
              tmp2 = 2000.0;
              ACE_DEBUG ((LM_DEBUG, "tmp > 100000!, this->delta_.usec ()=%ld\n",
                         this->delta_.usec ()));
            }

          real_time = tmp + tmp2 / (ACE_timer_t)ACE_ONE_SECOND_IN_USECS;
#else
          real_time = ((ACE_timer_t) this->delta_.sec () +
                       (ACE_timer_t) this->delta_.usec () / (ACE_timer_t) ACE_ONE_SECOND_IN_USECS);
#endif /* VXWORKS */

          real_time /= this->granularity_;
#endif /* !ACE_LACKS_FLOATING_POINT */
          return real_time;
}
   
