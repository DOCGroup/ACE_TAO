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
#include "ace/Timer_List.h"
#include "test_config.h"

static int lines = 0;

static ACE_Timer_List timer_list;

static void 
handler (int signum)
{
  switch (signum)
    {
    case SIGALRM:
      ACE_DEBUG ((LM_DEBUG, "you've entered %d lines, time = %d\n", lines, ACE_OS::time ()));
      break;
    case SIGINT:
      ACE_DEBUG ((LM_DEBUG, "dumping timer queue\n"));
      {
	for (ACE_Timer_List_Iterator iter (timer_list);
	     iter.isdone () == 0;
	     iter.next ())
	  iter.item ()->dump ();
      }
      break;
    }
}

static char menu[] = 
"****\n"
"1) schedule_timer usecs \n"
"2) cancel_timer timer_id\n"
"^C list_timers\n"
"please enter your choice: ";

static void
parse_buf (char *buf)
{
  ACE_DEBUG ((LM_DEBUG, "===== %s", buf));
}

/* sigprocmask (SIG_BLOCK, &ss, 0);
   sigprocmask (SIG_UNBLOCK, &ss, 0); */

static int
run (u_int delay)
{
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGALRM);
  ACE_UNUSED_ARG (sa);

  ACE_OS::ualarm (delay, delay);

  ACE_DEBUG ((LM_DEBUG, "%s", menu));

  for (;;)
    {
      char buf[BUFSIZ];

      errno = 0;
      if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
	{
	  if (errno != EINTR)
	    break;
	}
      else
	{
	  lines++;
	  parse_buf (buf);
	  ACE_DEBUG ((LM_DEBUG, "%s", menu));
	}
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  // ACE_START_TEST ("Timer_Queue_Test");

  u_int delay = argc > 1 ? atoi (argv[1]) : ACE_DEFAULT_USECS;

  run (delay);

  // ACE_END_TEST;
  return 0;
}
