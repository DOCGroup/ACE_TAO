// $Id$

// Perform an extensive test of all the ACE_Reactor's event handler
// dispatching mechanisms.  These mechanisms illustrate how I/O,
// timeout, and signal events, as well as ACE_Message_Queues, can all
// be handled within the same demultiplexing and dispatching
// framework.  In addition, this example illustrates how to use the
// ACE_Reactor for devices that perform I/O via signals (such as SVR4
// message queues).

#include "ace/ACE.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Task.h"
#include "ace/Reactor_Notification_Strategy.h"
#include "ace/Signal.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(Misc, test_demuxing, "$Id$")

// Default is to have a 2 second timeout.
static int timeout = 2;

class Sig_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   This class illustrates how to handle signal-driven I/O using
  //   the <ACE_Reactor> framework.  Note that signals may be caught
  //   and processed without requiring the use of global signal
  //   handler functions or global signal handler data.
public:
  Sig_Handler (void);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int shutdown (ACE_HANDLE, ACE_Reactor_Mask);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int handle_signal (int signum, siginfo_t * = 0,
                             ucontext_t * = 0);

private:
  ACE_HANDLE handle_;
};

// A dummy_handle is required to reserve a slot in the ACE_Reactor's
// descriptor table.

Sig_Handler::Sig_Handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != ACE_INVALID_HANDLE);

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.  NULL_MASK just reserves a "slot" in the Reactor's
  // internal demuxing table, but doesn't cause it to dispatch the
  // event handler directly.  Instead, we use the signal handler to do
  // this.
  ACE_Reactor_Mask mask = ACE_Event_Handler::NULL_MASK;
  if (ACE_Reactor::instance ()->register_handler (this, mask) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n%a",
                "register_handler",
                1));

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);

  // Register the signal handler object to catch the signals.
  if (ACE_Reactor::instance ()->register_handler
      (sig_set, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n%a",
                "register_handler",
                1));
}

// Called by the ACE_Reactor to extract the handle.

ACE_HANDLE
Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

// In a real application, this method would be where the read on the
// signal-driven I/O device would occur asynchronously.  For now we'll
// just print a greeting to let you know that everything is working
// properly!

int
Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) handling asynchonrous input...\n"));
  return 0;
}

// In a real application, this method would do any cleanup activities
// required when shutting down the I/O device.

int
Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.  Note
// that the ACE_Reactor's signal handling mechanism eliminates the
// need to use global signal handler functions and data.

int
Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  switch (signum)
    {
#if !defined (ACE_WIN32)
    case SIGALRM:
      // Rearm the alarm.
      ACE_OS::alarm (4);
      break;
#endif /* !ACE_WIN32 */
    case SIGINT:
      // Tell the ACE_Reactor to enable the ready bit for
      // this->handle_.  The ACE_Reactor will subsequently call the
      // <Sig_Handler::handle_input> method from within its event
      // loop, i.e., the behavior triggered by the signal is handled
      // in the main event loop, rather than in the signal handler.
      return ACE_Reactor::instance ()->ready_ops
        (this->handle_,
         ACE_Event_Handler::READ_MASK,
         ACE_Reactor::ADD_MASK);
#if defined (SIGTERM) && (SIGTERM != 0)
    case SIGTERM:
      // This is coded thusly to avoid problems if SIGQUIT is a legit
      // value but is not a preprocessor macro.
#elif !defined (SIGQUIT) || (SIGQUIT != 0)
    case SIGQUIT:
#endif /* SIGTERM != 0 */
      ACE_Reactor::end_event_loop ();
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "invalid signal"), -1);
      /* NOTREACHED */
    }

  return 0;
}

class STDIN_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   This class illustrates that the ACE_Reactor can handle signals,
  //   STDIO, and timeouts using the same mechanisms.
public:
  STDIN_Handler (void);
  ~STDIN_Handler (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg);
};

STDIN_Handler::STDIN_Handler (void)
{
  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_stdin_handler"));

  // Register the <STDIN_Handler> to be dispatched once every
  // <timeout> seconds starting in <timeout> seconds.  This example
  // uses the "interval timer" feature of the <ACE_Reactor>'s timer
  // queue.
  else if (ACE_Reactor::instance ()->schedule_timer
           (this,
            0,
            ACE_Time_Value (timeout),
            ACE_Time_Value (timeout)) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n%a",
                "schedule_timer",
                1));
}

STDIN_Handler::~STDIN_Handler (void)
{
  if (ACE_Event_Handler::remove_stdin_handler (ACE_Reactor::instance (),
                                               ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "remove_stdin_handler"));
  else if (ACE_Reactor::instance ()->cancel_timer
           (this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n%a",
                "cancel_timer",
                1));
}

int
STDIN_Handler::handle_timeout (const ACE_Time_Value &tv,
                               const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) timeout occurred at %d sec, %d usec\n",
              tv.sec (),
              tv.usec ()));
  return 0;
}

// Read from input handle and write to stdout handle.

int
STDIN_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

  switch (n)
    {
    case -1:
      if (errno == EINTR)
        return 0;
        /* NOTREACHED */
      else
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "read"));
      /* FALLTHROUGH */
    case 0:
      ACE_Reactor::end_event_loop ();
      break;
    default:
      {
        ssize_t result = ACE::write_n (ACE_STDOUT, buf, n);

        if (result != n)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "write"),
                            result == -1 && errno == EINTR ? 0 : -1);
      }
    }
  return 0;
}

class Message_Handler : public ACE_Task <ACE_SYNCH>
{
public:
  Message_Handler (void);

  virtual int handle_input (ACE_HANDLE);
  // Called back within the context of the <ACE_Reactor> Singleton to
  // dequeue and process the message on the <ACE_Message_Queue>.

  virtual int svc (void);
  // Run the "event-loop" periodically putting messages to our
  // internal <Message_Queue> that we inherit from <ACE_Task>.

private:
  ACE_Reactor_Notification_Strategy notification_strategy_;
  // This strategy will notify the <ACE_Reactor> Singleton when a new
  // message is enqueued.
};

Message_Handler::Message_Handler (void)
  : notification_strategy_ (ACE_Reactor::instance (),
                            this,
                            ACE_Event_Handler::READ_MASK)
{
  // Set this to the Reactor notification strategy.
  this->msg_queue ()->notification_strategy (&this->notification_strategy_);

  if (this->activate ())
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate"));
}

int
Message_Handler::svc (void)
{
  for (int i = 0;; i++)
    {
      ACE_Message_Block *mb = 0;

      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (1),
                      0);

      mb->msg_priority (i);
      ACE_OS::sleep (1);

      // Note that this putq() call with automagically invoke the
      // notify() hook of our ACE_Reactor_Notification_Strategy,
      // thereby informing the <ACE_Reactor> Singleton to call our
      // <handle_input> method.
      if (this->putq (mb) == -1)
        {
          if (errno == ESHUTDOWN)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%t) queue is deactivated"), 0);
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%t) %p\n",
                               "putq"),
                              -1);
        }
    }

  ACE_NOTREACHED (return 0);
}

int
Message_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Message_Handler::handle_input\n"));

  ACE_Message_Block *mb = 0;

  if (this->getq (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "dequeue_head"));
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) priority = %d\n",
                  mb->msg_priority ()));
      mb->release ();
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv [0]);

  // Optionally start the alarm.
  if (argc > 1)
    {
      ACE_OS::alarm (4);
      timeout = ACE_OS::atoi (argv[1]);
    }

  // Signal handler.
  Sig_Handler sh;

  // Define an I/O handler object.
  STDIN_Handler ioh;

  // Define a message handler.
  Message_Handler mh;

  // Loop handling signals and I/O events until SIGQUIT occurs.

  while (ACE_Reactor::instance ()->event_loop_done () == 0)
    ACE_Reactor::instance ()->run_reactor_event_loop ();

  // Deactivate the message queue.
  mh.msg_queue ()->deactivate ();

  // Wait for the thread to exit.
  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) leaving main\n")));
  return 0;
}
