// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Async_Timer_Queue_Test.cpp
//
// = DESCRIPTION
//      This test exercises <ACE_Timer_Heap> in the context of
//      signal-based timer handling.  
//
// = AUTHORS
//    Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Profile_Timer.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_Queue.h"
#include "test_config.h"

static int lines = 0;

static void 
handler (int i)
{
  fprintf (stderr, "you've entered %d lines, time = %d\n", lines, time (0));
}

/* sigprocmask (SIG_BLOCK, &ss, 0);
   sigprocmask (SIG_UNBLOCK, &ss, 0); */

static int
run (u_int delay)
{
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGALRM);
  ACE_UNUSED_ARG (sa);

  ACE_OS::ualarm (delay, delay);

  ACE_DEBUG ((LM_DEBUG,
	      "****\n"
	      "1) schedule_timer usecs \n"
	      "2) cancel_timer timer_id\n"
	      "^C list_timers\n"
	      "please enter your choice: "));

  for (;;)
    {
      char buf[BUFSIZ];

      errno = 0;
      if (fgets (buf, sizeof buf, stdin) == 0)
	{
	  fprintf (stderr, "errno = %d\n", errno);

	  if (errno != EINTR)
	    break;
	}
      else
	{
	  fprintf (stderr, "buf = %s\n", buf);
	  lines++;
	  fprintf (stderr, "please enter your choice: ");
	}
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  // ACE_START_TEST ("Timer_Queue_Test");

  u_int delay = argc > 1 ? atoi (argv[1]) : 1000;

  run (delay);

  // ACE_END_TEST;
  return 0;
}
