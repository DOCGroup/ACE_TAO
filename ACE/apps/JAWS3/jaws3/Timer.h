/* -*- c++ -*- */
#ifndef JAWS_TIMER_H
#define JAWS_TIMER_H

#include "ace/Singleton.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Queue_Adapters.h"

#include "jaws3/Export.h"
#include "jaws3/Event_Completer.h"

class JAWS_Timer;
class JAWS_Timer_Impl;

class JAWS_Export JAWS_Timer_Impl
// = TITLE
//     Implementation base class for Timers that corresponds to the
//     RHS of the Bridge pattern.
{
public:
  virtual ~JAWS_Timer_Impl () = default;

  virtual void schedule_timer ( long *timer_id
                              , const ACE_Time_Value &delta
                              , JAWS_Event_Completer *completer
                              , void *act = nullptr
                              ) = 0;
  // Schedule a timer to expire at now+delta.

  virtual void schedule_absolute_timer ( long *timer_id
                                       , const ACE_Time_Value &tv
                                       , JAWS_Event_Completer *completer
                                       , void *act = nullptr
                                       ) = 0;
  // Schedule a timer to expire at tv.

  virtual void schedule_interval_timer ( long *timer_id
                                       , const ACE_Time_Value &interval
                                       , JAWS_Event_Completer *completer
                                       , void *act = nullptr
                                       ) = 0;
  // Schedule a timer to expire at now+interval, and every interval following.

  virtual void cancel_timer (long timer_id) = 0;
  // Cancel a timer.
};


class JAWS_Export JAWS_Timer
// = TITLE
//     Abstraction base class for Timers that corresponds to the LHS of the
//     Bridge pattern.
{
public:
  explicit JAWS_Timer (JAWS_Timer_Impl *impl = nullptr);

  static JAWS_Timer * instance ()
  {
    return ACE_Singleton<JAWS_Timer, ACE_SYNCH_MUTEX>::instance ();
  }

  void schedule_timer ( long *timer_id
                      , const ACE_Time_Value &delta
                      , JAWS_Event_Completer *completer
                      , void *act = nullptr
                      );

  void schedule_absolute_timer ( long *timer_id
                               , const ACE_Time_Value &tv
                               , JAWS_Event_Completer *completer
                               , void *act = nullptr
                               );

  void schedule_interval_timer ( long *timer_id
                               , const ACE_Time_Value &interval
                               , JAWS_Event_Completer *completer
                               , void *act = nullptr
                               );

  void cancel_timer (long timer_id);

private:
  JAWS_Timer_Impl *impl_;
};


#endif /* JAWS_TIMER_H */
