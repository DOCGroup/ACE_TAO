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
//      This test exercises <ACE_Timer_List> in the context of
//      signal-based timer handling.  
//
// = AUTHORS
//    Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Signal.h"
#include "ace/Timer_List.h"
#include "test_config.h"

#if defined (ACE_HAS_UALARM)
// Number of lines of input read.
static int lines = 0;

// Were we interrupted by a SIGINT?
sig_atomic_t sigint = 0;

// This is our main timer list.  Right now, it doesn't know anything
// about signals. 
static ACE_Timer_List timer_list;

// This is our signal handler function.  It gets invoked
// asynchronously when SIGINT, SIGALRM, or SIGQUIT gets called.
static void 
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "handling signal %S\n", signum));
  sigint = signum == SIGINT;

  switch (signum)
    {
    case SIGALRM:
      {
	int expired_timers;

	ACE_Sig_Set ss;
	ss.sig_add (SIGINT);

	// Prevent SIGINT from occurring while we're handling SIGALRM.
	ACE_Sig_Guard sg (&ss);

	// Expire the pending timers.
	expired_timers = timer_list.expire ();

	if (expired_timers > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "you've entered %d lines, time = %d, timers expired = %d\n", 
		      lines,
		      ACE_OS::time (),
		      expired_timers));

	// Only schedule a new timer if there is one in the list.
	if (timer_list.is_empty () == 0)
	  ACE_OS::ualarm (timer_list.earliest_time () - timer_list.gettimeofday ());
	
	break;
      }
    /* NOTREACHED */
    case SIGINT:
      {
	ACE_DEBUG ((LM_DEBUG, "begin dumping timer queue\n"));

	ACE_Sig_Set ss;
	ss.sig_add (SIGALRM);

	// Prevent SIGALRM from occurring while we're handling
	// SIGINT.
	ACE_Sig_Guard sg (&ss);

	for (ACE_Timer_List_Iterator iter (timer_list);
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

class Timer_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Callback hook invoked by the <Timer_List>.

  ~Timer_Handler (void);
  // Destructor
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

Timer_Handler::~Timer_Handler (void)
{
  // ACE_DEBUG ((LM_DEBUG, "deleting %x\n", this));
}

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

	ACE_Sig_Set ss;
	ss.sig_add (SIGALRM);
	ss.sig_add (SIGINT);

	{
	  // Prevent SIGALRM and SIGINT from occurring while we're
	  // scheduling a timer.
	  ACE_Sig_Guard sg (&ss);

	  long tid = timer_list.schedule (eh, 0,
					  timer_list.gettimeofday () + tv);

	  if (tid == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

	  tv = timer_list.earliest_time () - timer_list.gettimeofday ();

	  ACE_DEBUG ((LM_DEBUG,
		      "scheduling timer %d for (%d, %d)\n",
		      tid, tv.sec (), tv.usec ()));
	}

	// Beware of negative times and zero times (which cause
	// problems for ualarm()).
	if (tv < ACE_Time_Value::zero)
	  tv = ACE_Time_Value (0, 1);

	// Schedule a new timer.
	ACE_OS::ualarm (tv);

	break;
	/* NOTREACHED */
      }
    case 2: // Cancel a timer.
      {
	const void *act;

	ACE_Sig_Set ss;
	ss.sig_add (SIGALRM);
	ss.sig_add (SIGINT);

	// Prevent SIGALRM and SIGINT from occurring while we're
	// scheduling a timer.
	ACE_Sig_Guard sg (&ss);

	if (timer_list.cancel (value, &act) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cancel_timer"), -1);

	delete (ACE_Event_Handler *) act;

	ACE_DEBUG ((LM_DEBUG, "canceling %d\n", value));
	break;
	/* NOTREACHED */
      }
    }

  return 0;
}

static char menu[] = 
"****\n"
"1) schedule_timer <usecs> \n"
"2) cancel_timer <timer_id>\n"
"^C list_timers\n"
"please enter your choice: ";
#endif /* ACE_HAS_UALARM */

int
main (int, char *[])
{
  // ACE_START_TEST ("Timer_Queue_Test");
#if defined (ACE_HAS_UALARM)
  ACE_Sig_Action sa ((ACE_SignalHandler) handler);
  ACE_UNUSED_ARG (sa);

  // Register the signal handlers.
  sa.register_action (SIGINT);
  sa.register_action (SIGALRM);
  sa.register_action (SIGQUIT);

  ACE_DEBUG ((LM_DEBUG, "%s", menu));

  for (;;)
    {
      char buf[BUFSIZ];

      errno = 0;
      if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
	{
	  if (errno != EINTR)
	    break;
	  else if (sigint)
	    ACE_DEBUG ((LM_DEBUG, "%s", menu));
	}
      else
	{
	  lines++;
	  parse_commands (buf);
	  ACE_DEBUG ((LM_DEBUG, "%s", menu));
	}
    }
#else
  ACE_ERROR_RETURN ((LM_ERROR, "platform doesn't support ualarm\n"), -1);
#endif /* ACE_HAS_UALARM */
  // ACE_END_TEST;
  return 0;
}
