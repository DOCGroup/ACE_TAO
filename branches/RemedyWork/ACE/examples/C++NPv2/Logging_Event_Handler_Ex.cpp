/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "Logging_Event_Handler_Ex.h"
#include "ace/Timer_Queue.h"

int Logging_Event_Handler_Ex::handle_input (ACE_HANDLE h) {
  time_of_last_log_record_ =
    reactor ()->timer_queue ()->gettimeofday ();
  return PARENT::handle_input (h);
}

int Logging_Event_Handler_Ex::open () {
  int result = PARENT::open ();
  if (result != -1) {
    ACE_Time_Value reschedule (max_client_timeout_.sec () / 4);
    result =
      reactor ()->schedule_timer
        (this,
         0,
         max_client_timeout_,  // Initial timeout.
         reschedule);          // Subsequent timeouts.
  }
  return result;
}

int Logging_Event_Handler_Ex::handle_timeout
  (const ACE_Time_Value &now, const void *) {
  if (now - time_of_last_log_record_ >= max_client_timeout_)
    reactor ()->remove_handler (this, ACE_Event_Handler::READ_MASK);
  return 0;
}

int Logging_Event_Handler_Ex::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask) {
  reactor ()->cancel_timer (this);
  return PARENT::handle_close ();
}
