
//=============================================================================
/**
 *  @file    Async_Timer_Queue_Test.cpp
 *
 *  $Id$
 *
 *    This test exercises the <ACE_Asynch_Timer_Queue_Adapter>
 *    using an <ACE_Timer_Heap>.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "ace/Signal.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Async_Timer_Queue_Test.h"



// Hook method that is called to handle the expiration of a timer.
int
Async_Timer_Handler::handle_timeout (const ACE_Time_Value &tv,
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

      ACE_NEW_RETURN (Async_Timer_Queue::instance_, Async_Timer_Queue (&ss), 0);
    }
  return Async_Timer_Queue::instance_;
}

// Sets the signal set to mask, for the timer queue.

Async_Timer_Queue::Async_Timer_Queue (ACE_Sig_Set *ss) :
  tq_ (ss)
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
  ACE_NEW (eh, Async_Timer_Handler);

  // Schedule the timer to run in the future.
  long tid = this->tq_.schedule(eh,
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

  const void *act = 0;

  if (this->tq_.cancel (timer_id, &act) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "cancel_timer"));

  // In this case, the act will be 0, but it could be a real pointer
  // in other cases.
  delete (ACE_Event_Handler *) act;
}

// Schedule timer hook method.  This method is called from the driver.

int
Async_Timer_Queue::schedule_timer (void *argument)
{
  u_long useconds = *(int *)argument;

  // Schedule a timer.
  Async_Timer_Queue::instance ()->schedule (useconds);

  return 0;
}

// Cancel timer hook method.   Is called from the driver class.

int
Async_Timer_Queue::cancel_timer (void *argument)
{
  u_long id = *(int *)argument;

  // Cancel a timer.
  Async_Timer_Queue::instance ()->cancel (id);

  return 0;
}

// Dummy list timer hook method.  The listing of timers is done from a
// signal handler using SIGINT, not from the driver.

int
Async_Timer_Queue::list_timer (void *)
{
  // Display an error message.
  ACE_ERROR_RETURN ((LM_ERROR, "invalid input\n"), 0);
}

// Dummy shutdown timer hook method.  The shutdown of the timer queue
// is done with a signal handler using SIGQUIT, not from the driver.

int
Async_Timer_Queue::shutdown_timer (void *)
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

#if !defined (ACE_LACKS_UNIX_SIGNALS)
    case SIGQUIT:
      ACE_ERROR ((LM_ERROR, "shutting down on SIGQUIT%a\n", 1));
      /* NOTREACHED */
      break;
#endif
    }
}

// Register the signal handlers for SIGQUIT and SIGINT.  We must
// ensure that the SIGINT handler isn't interrupted by SIGALRM.
// However, SIGQUIT is never blocked...

static void
register_signal_handlers (void)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // Register SIGQUIT (never blocked).
  ACE_Sig_Action sigquit ((ACE_SignalHandler) signal_handler, SIGQUIT);
  ACE_UNUSED_ARG (sigquit);
#endif

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

// constructor

Async_Timer_Queue_Test_Driver::Async_Timer_Queue_Test_Driver (void)
{
}

// displays the menu of options.

int
Async_Timer_Queue_Test_Driver::display_menu (void)
{
  // The menu of options provided to the user.
  static char menu[] =
    "****\n"
    "1) schedule timer <usecs>\n"
    "2) cancel timer <timer_id>\n"
    "^C list timers\n"
    "^\\ exit program\n";

  ACE_DEBUG ((LM_DEBUG, "%s", menu));
  return 0;
}

// Initializes the test driver.

int
Async_Timer_Queue_Test_Driver::init (void)
{
  typedef Command<Async_Timer_Queue, Async_Timer_Queue::ACTION> CMD;

  // Initialize <Command> objects with their corresponding <Input_Task> methods.
  ACE_NEW_RETURN (schedule_cmd_,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::schedule_timer),
                  -1);

  ACE_NEW_RETURN (cancel_cmd_,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::cancel_timer),
                  -1);

  ACE_NEW_RETURN (list_cmd_,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::list_timer),
                  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::shutdown_timer),
                  -1);

  register_signal_handlers ();

  return 0;
}
