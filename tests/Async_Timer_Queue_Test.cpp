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

typedef ACE_Event_Handler *AEH;

// This is our main timer list.  Right now, it doesn't know anything
// about signals. 
class Async_Timer_List : public ACE_Event_Handler
{
public:
  Async_Timer_List (void);
  // Register the SIGALRM handler.

  long schedule (ACE_Event_Handler *type,
		 const void *act, 
		 const ACE_Time_Value &delay,
		 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule the timer according to the semantics of the
  // <ACE_Timer_List>.  However, this timer gets dispatched via a
  // signal, rather than by a user calling <expire>.

  int cancel (long timer_id, const void **);
  // Cancel the <timer_id>.

  operator ACE_Timer_List &();
  // Conversion operator.

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Called back by SIGALRM handler.

  ACE_Sig_Handler sig_handler_;
  // Handler for the SIGALRM signal, so that we can access our state
  // without requiring global variables.

  ACE_Timer_List timer_list_;
};

Async_Timer_List::operator ACE_Timer_List &()
{
  return this->timer_list_;
}

int
Async_Timer_List::cancel (long timer_id,
			  const void **act)
{
  // Block all signals.
  ACE_Sig_Guard sg;
  ACE_UNUSED_ARG (sg);

  return this->timer_list_.cancel (timer_id, act);
}

long 
Async_Timer_List::schedule (ACE_Event_Handler *eh,
			    const void *act, 
			    const ACE_Time_Value &delay,
			    const ACE_Time_Value &interval)
{
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (interval);

  // Block all signals.
  ACE_Sig_Guard sg;
  ACE_UNUSED_ARG (sg);

  long tid = this->timer_list_.schedule (eh, 0, delay);

  if (tid == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  ACE_Time_Value tv = this->timer_list_.earliest_time () 
    - ACE_OS::gettimeofday ();

  ACE_DEBUG ((LM_DEBUG,
	      "scheduling timer %d for (%d, %d)\n",
	      tid, tv.sec (), tv.usec ()));

  // Beware of negative times and zero times (which cause problems for
  // ualarm()).
  if (tv < ACE_Time_Value::zero)
    tv = ACE_Time_Value (0, 1);

  // Schedule a new timer.
  ACE_OS::ualarm (tv);
  return 0;
}

Async_Timer_List::Async_Timer_List (void)
{
  // The following code is necessary to selectively "block" all
  // signals when SIGALRM is running.  Also, we always restart system
  // calls that are interrupted by the signals.

  // Block *all* signals when the SIGARLM handler is running!
  ACE_Sig_Set ss (1); 

  ACE_Sig_Action sa ((ACE_SignalHandler) 0,
		     ss,
		     SA_RESTART);

  if (this->sig_handler_.register_handler (SIGALRM, this, &sa) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

// This is the signal handler function for the asynchronous timer
// list.  It gets invoked asynchronously when the SIGALRM signal
// occurs.
int
Async_Timer_List::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "handling signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      {
	int expired_timers;

	// Expire the pending timers.
	expired_timers = this->timer_list_.expire ();

	if (expired_timers > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "time = %d, timers expired = %d\n", 
		      ACE_OS::time (),
		      expired_timers));

	// Only schedule a new timer if there is one in the list.
	if (this->timer_list_.is_empty () == 0)
	  ACE_OS::ualarm (this->timer_list_.earliest_time () 
			  - ACE_OS::gettimeofday ());
	
	return 0;
	/* NOTREACHED */
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "unexpected signal %S\n", signum), -1);
      /* NOTREACHED */
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

// Static instance.
Async_Timer_List timer_list;

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

	long tid = timer_list.schedule (eh, 0,
					ACE_OS::gettimeofday () + tv);

	if (tid == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);
	
	break;
	/* NOTREACHED */
      }
    case 2: // Cancel a timer.
      {
	const void *act;

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
#endif /* ACE_HAS_UALARM */

int
main (int, char *[])
{
  // ACE_START_TEST ("Timer_Queue_Test");
#if defined (ACE_HAS_UALARM)
  register_signal_handlers ();

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "%s", menu));

      char buf[BUFSIZ];

      if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
	break;

      parse_commands (buf);
    }
#else
  ACE_ERROR_RETURN ((LM_ERROR, "platform doesn't support ualarm\n"), -1);
#endif /* ACE_HAS_UALARM */
  // ACE_END_TEST;
  return 0;
}
