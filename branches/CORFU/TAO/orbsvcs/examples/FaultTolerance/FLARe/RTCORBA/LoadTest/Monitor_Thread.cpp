/**
 *  @file  C++ Implementation: Monitor_Thread
 *
 *  @brief Defines implementation of Monitor_Thread.
 *
 */

#include "Monitor_Thread.h"

#include "ace/TP_Reactor.h"

Monitor_Thread::Monitor_Thread ()
  : reactor_ (new ACE_TP_Reactor())
{
}

int Monitor_Thread::svc ()
{
  if (reactor_.run_reactor_event_loop() == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"run_reactor_event_loop failed\n"), -1);

  return 0;
}
/*
int Monitor_Thread::register_handler (ACE_Event_Handler *event_handler, ACE_Reactor_Mask mask)
{
  return reactor_.register_handler (event_handler, mask);
}
*/
ACE_Reactor * Monitor_Thread::get_reactor ()
{
  return &reactor_;
}
