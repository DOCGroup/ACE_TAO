// $Id$

#include "ace/Signal.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Async_Timer_Queue_Test.h"

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

  for (ACE_Timer_Heap_Iterator iter (this->tq_.timer_queue ());
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

int
Async_Timer_Queue::schedule_timer (void *argument)
{
  u_long useconds = *(int *)argument;

  // Schedule a timer.
  Async_Timer_Queue::instance ()->schedule (useconds);

  return 0;
}

int
Async_Timer_Queue::cancel_timer (void *argument)
{
  u_long id = *(int *)argument;
    
  // Cancel a timer.
  Async_Timer_Queue::instance ()->cancel (id);

  return 0;
}

int
Async_Timer_Queue::list_timer (void *argument)
{
  // Display an error message.
  ACE_ERROR_RETURN ((LM_ERROR, "invalid input\n"), 0);
}

int
Async_Timer_Queue::shutdown_timer (void *argument)
{
  // Display an error message.
  ACE_ERROR_RETURN ((LM_ERROR, "invalid input\n"), 0);
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

Async_Timer_Queue_Test_Driver::Async_Timer_Queue_Test_Driver (void)
    {
      timer_queue_ = Async_Timer_Queue::instance ();
    }

int 
Async_Timer_Queue_Test_Driver::display_menu (void)
{ 
  // The menu of options provided to the user.
  static char menu[] = 
    "****\n"
    "1) schedule timer <usecs> \n"
    "2) cancel timer <timer_id>\n"
    "^C list timers\n"
    "^\\ exit program\n";

  ACE_DEBUG ((LM_DEBUG, "%s", menu)); 

  return 0; 
}

int 
Async_Timer_Queue_Test_Driver::init (void)
{
  // initialize commands with their corresponding input_task methods.
  ACE_NEW_RETURN (schedule_cmd_, 
		  Command<Async_Timer_Queue> (*Async_Timer_Queue::instance (),
					      Async_Timer_Queue::instance ()->schedule_timer),
		  -1);
  
  ACE_NEW_RETURN (cancel_cmd_,
		  Command<Async_Timer_Queue> (*Async_Timer_Queue::instance (),
				       Async_Timer_Queue::instance ()->cancel_timer),
		  -1);

  ACE_NEW_RETURN (list_cmd_,
		  Command<Async_Timer_Queue> (*Async_Timer_Queue::instance (),
				       Async_Timer_Queue::instance ()->list_timer),
		  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
		  Command<Async_Timer_Queue> (*Async_Timer_Queue::instance (),
				       Async_Timer_Queue::instance ()->shutdown_timer),
		  -1);

  register_signal_handlers ();

  return 0;
} 
    
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Async_Timer_Queue_Adapter<ACE_Timer_Heap>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Async_Timer_Queue_Adapter<ACE_Timer_Heap>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
