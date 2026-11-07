
//=============================================================================
/**
 *  @file    Async_Timer_Queue_Test.cpp
 *
 *    This test exercises the <ACE_Asynch_Timer_Queue_Adapter>
 *    using an <ACE_Timer_Heap>.
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "ace/Signal.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Async_Timer_Queue_Test.h"
#include <memory>
#include <new>


// Hook method that is called to handle the expiration of a timer.
int
Async_Timer_Handler::handle_timeout (ACE_Time_Value const &tv,
                                     void const *arg)
{
  // Print some information here (note that this is not strictly
  // signal-safe since the ACE logging mechanism uses functions that
  // aren't guaranteed to work in all signal handlers).
  ACE_DEBUG ((LM_DEBUG,
              "handle_timeout() = (%d, %d) %d\n",
              tv.sec (),
              tv.usec (),
              arg));

  // Timer callbacks own and delete their one-shot handler.
  delete this;
  return 0;
}

// Implement the Singleton logic.
Async_Timer_Queue *
Async_Timer_Queue::instance ()
{
  static ACE_Sig_Set signal_mask = [] ()
    {
      ACE_Sig_Set result (1);
      result.sig_del (SIGQUIT);
      return result;
    } ();
  static Async_Timer_Queue instance (&signal_mask);
  return &instance;
}

// Sets the signal set to mask, for the timer queue.

Async_Timer_Queue::Async_Timer_Queue (ACE_Sig_Set *ss)
  :
  tq_ (ss)
{
}

// Dump the contents of the queue when we receive ^C.

void
Async_Timer_Queue::dump ()
{
  ACE_DEBUG ((LM_DEBUG, "begin dumping timer queue\n"));

  // This iterator is implicitly protected since SIGINT and SIGALRM
  // signals cannot occur while it is running.

  for (ACE_Timer_Heap_Iterator iter (this->tq_.timer_queue ());
       iter.item () != nullptr;
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

  std::unique_ptr<ACE_Event_Handler> eh (new (std::nothrow) Async_Timer_Handler);
  if (!eh)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "new Async_Timer_Handler"));
      return;
    }

  // Transfer ownership before scheduling because a timer can expire immediately.
  ACE_Event_Handler *const raw_handler = eh.release ();
  long const tid = this->tq_.schedule (raw_handler,
                                       raw_handler,
                                       ACE_OS::gettimeofday () + tv);

  if (tid == -1)
    {
      delete raw_handler;
      ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));
    }
}

// Cancel a timer.

void
Async_Timer_Queue::cancel (long timer_id)
{
  ACE_DEBUG ((LM_DEBUG,
              "canceling %q\n",
              static_cast<ACE_INT64> (timer_id)));

  void const *act {};

  if (this->tq_.cancel (timer_id, &act) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "cancel_timer"));
      return;
    }

  std::unique_ptr<ACE_Event_Handler> canceled_handler (
    static_cast<ACE_Event_Handler *> (const_cast<void *> (act)));
}

// Schedule timer hook method.  This method is called from the driver.

int
Async_Timer_Queue::schedule_timer (void *argument)
{
  u_int const useconds = *static_cast<u_int const *> (argument);
  Async_Timer_Queue::instance ()->schedule (useconds);

  return 0;
}

// Cancel timer hook method.   Is called from the driver class.

int
Async_Timer_Queue::cancel_timer (void *argument)
{
  long const id = *static_cast<long const *> (argument);
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
register_signal_handlers ()
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // Register SIGQUIT (never blocked).
  ACE_Sig_Action sigquit (&signal_handler, SIGQUIT);
  ACE_UNUSED_ARG (sigquit);
#endif

  // Don't let the SIGALRM interrupt the SIGINT handler!
  ACE_Sig_Set ss;
  ss.sig_add (SIGALRM);

  // Register SIGINT (note that system calls will be restarted
  // automatically).
  ACE_Sig_Action sigint (&signal_handler,
                         SIGINT,
                         ss,
                         SA_RESTART);
  ACE_UNUSED_ARG (sigint);
}

// displays the menu of options.

int
Async_Timer_Queue_Test_Driver::display_menu ()
{
  // The menu of options provided to the user.
  static char const menu[] =
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
Async_Timer_Queue_Test_Driver::init ()
{
  using CMD = Command<Async_Timer_Queue, Async_Timer_Queue::ACTION>;

  // Initialize <Command> objects with their corresponding <Input_Task> methods.
  CMD *raw_schedule_cmd {};
  ACE_NEW_RETURN (raw_schedule_cmd,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::schedule_timer),
                  -1);
  schedule_cmd_.reset (raw_schedule_cmd);

  CMD *raw_cancel_cmd {};
  ACE_NEW_RETURN (raw_cancel_cmd,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::cancel_timer),
                  -1);
  cancel_cmd_.reset (raw_cancel_cmd);

  CMD *raw_list_cmd {};
  ACE_NEW_RETURN (raw_list_cmd,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::list_timer),
                  -1);
  list_cmd_.reset (raw_list_cmd);

  CMD *raw_shutdown_cmd {};
  ACE_NEW_RETURN (raw_shutdown_cmd,
                  CMD (*Async_Timer_Queue::instance (),
                       &Async_Timer_Queue::shutdown_timer),
                  -1);
  shutdown_cmd_.reset (raw_shutdown_cmd);

  register_signal_handlers ();

  return 0;
}
