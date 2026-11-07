#ifndef JAWS_BUILD_DLL
#define JAWS_BUILD_DLL
#endif

#include "ace/OS_NS_sys_time.h"

#include "jaws3/Task_Timer.h"
#include "jaws3/Timer_Helpers.h"
#include <memory>


JAWS_Task_Timer::JAWS_Task_Timer ()
{
  this->timer_queue_.activate ();
}


void
JAWS_Task_Timer::schedule_timer ( long *timer_id
                                , const ACE_Time_Value &delta
                                , JAWS_Event_Completer *completer
                                , void *act
                                )
{
  std::unique_ptr<JAWS_Timer_EH> eh = std::make_unique<JAWS_Timer_EH> (
    completer, act);
  if ((*timer_id =
         this->timer_queue_.schedule (eh.get (), nullptr,
                                      delta + ACE_OS::gettimeofday ())) == -1)
    {
      JAWS_Event_Result timer_result ( 0
                                     , JAWS_Event_Result::JE_ERROR
                                     , JAWS_Event_Result::JE_TIMER_FAIL
                                     );

      if (completer)
        completer->timer_complete (timer_result, act);

    }
  else
    eh.release ();
}


void
JAWS_Task_Timer::schedule_absolute_timer ( long *timer_id
                                         , const ACE_Time_Value &tv
                                         , JAWS_Event_Completer *completer
                                         , void *act
                                         )
{
  std::unique_ptr<JAWS_Timer_EH> eh = std::make_unique<JAWS_Timer_EH> (
    completer, act);
  if ((*timer_id = this->timer_queue_.schedule (eh.get (), nullptr, tv)) == -1)
    {
      JAWS_Event_Result timer_result ( 0
                                     , JAWS_Event_Result::JE_ERROR
                                     , JAWS_Event_Result::JE_TIMER_FAIL
                                     );

      if (completer)
        completer->timer_complete (timer_result, act);

    }
  else
    eh.release ();
}


void
JAWS_Task_Timer::schedule_interval_timer ( long *timer_id
                                         , const ACE_Time_Value &interval
                                         , JAWS_Event_Completer *completer
                                         , void *act
                                         )
{
  std::unique_ptr<JAWS_Timer_EH> eh = std::make_unique<JAWS_Timer_EH> (
    completer, act);
  if ((*timer_id =
         this->timer_queue_.schedule (eh.get (), nullptr,
                                      interval + ACE_OS::gettimeofday (),
                                      interval)) == -1)
    {
      JAWS_Event_Result timer_result ( 0
                                     , JAWS_Event_Result::JE_ERROR
                                     , JAWS_Event_Result::JE_TIMER_FAIL
                                     );

      if (completer)
        completer->timer_complete (timer_result, act);

    }
  else
    eh.release ();
}


void
JAWS_Task_Timer::cancel_timer (long timer_id)
{
  this->timer_queue_.cancel (timer_id);
}

