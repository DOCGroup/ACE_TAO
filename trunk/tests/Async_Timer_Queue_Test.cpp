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
//      This test exercises the <ACE_Asynch_Timer_Queue_Adapter> 
//      using an <ACE_Timer_List>.
//
// = AUTHORS
//    Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Signal.h"
#include "ace/Timer_List.h"
#include "test_config.h"

class Timer_Handler : public ACE_Event_Handler
  // = TITLE
  //     Target of the asynchronous timeout operation.
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Callback hook invoked by the <Timer_Queue>.
};

int 
Timer_Handler::handle_timeout (const ACE_Time_Value &tv,
			       const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
	      "handle_timeout() = (%d, %d) %d\n",
	      tv.sec (),
	      tv.usec (),
	      arg));
  delete this;
  return 0;
}

// Create a helpful typedef.
typedef ACE_Async_Timer_Queue_Adapter<ACE_Timer_List> ASYNC_TIMER_LIST;
typedef ACE_Timer_List_Iterator TIMER_LIST_ITERATOR;

// Static instance, instantiated by an <ACE_Timer_List>.

ASYNC_TIMER_LIST timer_queue;

// Command-line API.

static int
parse_commands (char *buf)
{
  u_int choice;
  long value;

  if (sscanf (buf, "%u %ld", &choice, &value) != 2)
    ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s", buf), -1);

  switch (choice)
    {
    case 1: // Schedule a timer.
      {
	ACE_Time_Value tv (0, value);
	ACE_Event_Handler *eh;
	ACE_NEW_RETURN (eh, Timer_Handler, -1);

	long tid = timer_queue.schedule (eh, 0,
					ACE_OS::gettimeofday () + tv);

	if (tid == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);
	
	break;
	/* NOTREACHED */
      }
    case 2: // Cancel a timer.
      {
	const void *act;

	if (timer_queue.cancel (value, &act) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cancel_timer"), -1);

	delete (ACE_Event_Handler *) act;

	ACE_DEBUG ((LM_DEBUG, "canceling %d\n", value));
	break;
	/* NOTREACHED */
      }
    }

  return 0;
}

static void 
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "handling signal %S\n", signum));

  switch (signum)
    {
    /* NOTREACHED */
    case SIGINT:
      {
	ACE_DEBUG ((LM_DEBUG, "begin dumping timer queue\n"));

	for (TIMER_LIST_ITERATOR iter (timer_queue.timer_queue ());
	     iter.item () != 0;
	     iter.next ())
	  iter.item ()->dump ();

	ACE_DEBUG ((LM_DEBUG, "end dumping timer queue\n"));

	break;
	/* NOTREACHED */
      }
    case SIGQUIT:
      ACE_DEBUG ((LM_DEBUG, "shutting down on SIGQUIT%a\n", 1));
      /* NOTREACHED */
      break;
    }
}

static void
register_signal_handlers (void)
{
  // Register SIGQUIT (never blocked).
  ACE_Sig_Action sigquit ((ACE_SignalHandler) handler, SIGQUIT);
  ACE_UNUSED_ARG (sigquit);

  // Don't let the SIGALRM interrupt the SIGINT handler!
  ACE_Sig_Set ss;
  ss.sig_add (SIGALRM);

  ACE_Sig_Action sigint ((ACE_SignalHandler) handler,
			 SIGINT,
			 ss,
			 SA_RESTART);
  ACE_UNUSED_ARG (sigint);
}

static char menu[] = 
"****\n"
"1) schedule_timer <usecs> \n"
"2) cancel_timer <timer_id>\n"
"^C list_timers\n"
"please enter your choice: ";

int
main (int, char *[])
{
  // ACE_START_TEST ("Timer_Queue_Test");

  register_signal_handlers ();

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "%s", menu));

      char buf[BUFSIZ];

      if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
	break;

      parse_commands (buf);
    }

  // ACE_END_TEST;
  return 0;
}
