
//=============================================================================
/**
 *  @file    Console_Input.cpp
 *
 *  $Id$
 *
 *
 *  This application tests the working of WFMO_Reactor when users
 *  are interested in console input.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Reactor.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_main.h"
#include "ace/Log_Msg.h"



class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_Reactor &reactor);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);
};

Event_Handler::Event_Handler (ACE_Reactor &reactor)
{
  this->reactor (&reactor);

  if (this->reactor ()->register_handler (this,
                                          ACE_STDIN) != 0)
    ACE_ERROR ((LM_ERROR,
                "Registration with Reactor could not be done\n"));
}

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TCHAR buffer[BUFSIZ];
  int result = ACE_OS::read (ACE_STDIN, buffer, sizeof buffer);
  buffer[result] = '\0';

  if (result <= 0)
    {
      this->reactor ()->close ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::read"), -1);
    }

  if (ACE_OS::strcmp (ACE_TEXT("quit\r\n"), buffer) == 0)
    this->reactor ()->close ();

  ACE_DEBUG ((LM_DEBUG, "User input: %s", buffer));

  return 0;
}

int
Event_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from Reactor\n"));
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Reactor reactor;
  Event_Handler handler (reactor);

  int result = 0;
  while (result != -1)
    result = reactor.handle_events ();

  return 0;
}
