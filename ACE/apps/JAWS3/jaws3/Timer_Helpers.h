/* -*- c+ -*- */
#ifndef JAWS_TIMER_HELPERS_H
#define JAWS_TIMER_HELPERS_H

#include "ace/Singleton.h"
#include "ace/Event_Handler.h"

#include "jaws3/Timer.h"
#include "jaws3/Event_Completer.h"
#include "jaws3/Event_Result.h"

class JAWS_Timer_EH : public ACE_Event_Handler
{
public:
  JAWS_Timer_EH ( JAWS_Event_Completer *completer
                , void *act = nullptr
                , int interval = 0
                )
    : completer_ (completer)
    , act_ (act)
    , interval_ (interval)
    , expired_ (false)
  {
  }

  int handle_timeout (const ACE_Time_Value &tv, const void *act) override;
  // Called when timer expires.

  int handle_close (ACE_HANDLE h, ACE_Reactor_Mask m) override;
  // Called directly when timer is canceled.

private:
  JAWS_Event_Completer *completer_;
  void *act_;
  const int interval_;
  bool expired_;
};

#endif /* JAWS_TIMER_HELPERS_H */
