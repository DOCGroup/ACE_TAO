// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
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

#include "test_config.h"
#include "ace/Signal.h"
#include "ace/Timer_List.h"

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
  // Print some information here (note that this is not strictly
  // signal-safe since the ACE logging mechanism uses functions that
  // aren't guaranteed to work in all signal handlers).
  ACE_DEBUG ((LM_DEBUG,
	      "handle_timeout() = (%d, %d) %d\n",
	      tv.sec (),
	      tv.usec (),
	      arg));

  // Commit suicide!
  delete this;
  return 0;
}

class Async_Timer_Queue 
  // = TITLE
  //     Asynchronous Timer Queue Singleton.
  //
  // = DESCRIPTION
  //     We use this class to avoid global variables and to
  //     consolidate all the Timer Queue processing in one central 
  //     place. 
{
public:
  static Async_Timer_Queue *instance (void);
  // Singleton access point.

  void schedule (u_int microsecs);
  // Schedule a timer to expire <microsecs> in the future.

  void cancel (long timer_id);
  // Cancel a timer with <timer_id>.

  void dump (void);
  // Dump the contents of the queue.

private:
  Async_Timer_Queue (ACE_Sig_Set *);
  // Private constructor enforces the Singleton.

  static Async_Timer_Queue *instance_;
  // Pointer to the timer queue.

  ACE_Async_Timer_Queue_Adapter<ACE_Timer_List> tq_;
  // The adapter is instantiated by an <ACE_Timer_List>.
};

// Initialize the Singleton pointer.
Async_Timer_Queue *Async_Timer_Queue::instance_ = 0;

// Implement the Singleton logic.
Async_Timer_Queue *
Async_Timer_Queue::instance (void)
{
  if (Async_Timer_Queue::instance_ == 0)
    {
      // Initialize with all signals enabled.
      ACE_Sig_Set ss (1);

      // But, don't block out SIGQUIT since we always want that
      // signal to interrupt the program.
      ss.sig_del (SIGQUIT);

      ACE_NEW_RETURN (Async_Timer_Queue::instance_,
		      Async_Timer_Queue (&ss),
		      0);
    }
  return Async_Timer_Queue::instance_;
}

Async_Timer_Queue::Async_Timer_Queue (ACE_Sig_Set *ss)
  : tq_ (ss)
{
}

// Dump the contents of the queue when we receive ^C.

void
Async_Timer_Queue::dump (void)
{
  ACE_DEBUG ((LM_DEBUG, "begin dumping timer queue\n"));

  // This iterator is implicitly protected since SIGINT and SIGALRM
  // signals cannot occur while it is running.

  for (ACE_Timer_List_Iterator iter (this->tq_.timer_queue ());
       iter.item () != 0;
       iter.next ())
    iter.item ()->dump ();

  ACE_DEBUG ((LM_DEBUG, "end dumping timer queue\n"));
}

// Schedule a timer.

void
Async_Timer_Queue::schedule (u_int microsecs)
{
  ACE_Time_Value tv (0, microsecs);

  // Create a new Event_Handler for our timer.

  ACE_Event_Handler *eh;
  ACE_NEW (eh, Timer_Handler);

  // Schedule the timer to run in the future.
  long tid = this->tq_.schedule
    (eh, 
     0, // Note that our "magic cookie" ACT is always NULL.
     ACE_OS::gettimeofday () + tv);

  if (tid == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));
}	

// Cancel a timer.

void
Async_Timer_Queue::cancel (long timer_id)
{
  ACE_DEBUG ((LM_DEBUG, "canceling %d\n", timer_id));

  const void *act;

  if (this->tq_.cancel (timer_id, &act) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "cancel_timer"));

  // In this case, the act will be 0, but it could be a real pointer
  // in other cases.
  delete (ACE_Event_Handler *) act;
}

// Command-line API.

static int
parse_commands (const char *buf)
{
  u_int choice;
  long value;

  // @@ Should make sure to use signal-safe logic here...

  if (sscanf (buf, "%u %ld", &choice, &value) != 2)
    ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s", buf), -1);

  switch (choice)
    {
    case 1: // Schedule a timer.
      Async_Timer_Queue::instance ()->schedule (value);

      break;
      /* NOTREACHED */

    case 2: // Cancel a timer.
      Async_Timer_Queue::instance ()->cancel (value);
      break;
      /* NOTREACHED */      
    }

  return 0;
}

// Handler for the SIGINT and SIGQUIT signals.

static void 
signal_handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "handling signal %S\n", signum));

  switch (signum)
    {
    case SIGINT:
      Async_Timer_Queue::instance ()->dump ();
      break;
      /* NOTREACHED */

    case SIGQUIT:
      ACE_ERROR ((LM_ERROR, "shutting down on SIGQUIT%a\n", 1));
      /* NOTREACHED */
      break;
    }
}

// Register the signal handlers for SIGQUIT and SIGINT.  We must
// ensure that the SIGINT handler isn't interrupted by SIGALRM.
// However, SIGQUIT is never blocked...

static void
register_signal_handlers (void)
{
  // Register SIGQUIT (never blocked).
  ACE_Sig_Action sigquit ((ACE_SignalHandler) signal_handler,
			  SIGQUIT);
  ACE_UNUSED_ARG (sigquit);

  // Don't let the SIGALRM interrupt the SIGINT handler!
  ACE_Sig_Set ss;
  ss.sig_add (SIGALRM);

  // Register SIGINT (note that system calls will be restarted
  // automatically).
  ACE_Sig_Action sigint ((ACE_SignalHandler) signal_handler,
			 SIGINT,
			 ss,
			 SA_RESTART);
  ACE_UNUSED_ARG (sigint);
}

// The menu of options provided to the user.

static char menu[] = 
"****\n"
"1) schedule timer <usecs> \n"
"2) cancel timer <timer_id>\n"
"^C list timers\n"
"^\\ exit program\n"
"please enter your choice: ";

int
main (int, char *[])
{
  // ACE_START_TEST ("Timer_Queue_Test");

  register_signal_handlers ();

  // Run until the user types ^\.

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "%s", menu));

      char buf[BUFSIZ];

      // Wait for user to type commands.  This call is automatically
      // restarted when SIGINT or SIGALRM signals occur.
      if (ACE_OS::read (ACE_STDIN, buf, sizeof buf) <= 0)
	break;

      // Run the command.
      parse_commands (buf);
    }

  // ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Async_Timer_Queue_Adapter<ACE_Timer_List>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Async_Timer_Queue_Adapter<ACE_Timer_List>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
