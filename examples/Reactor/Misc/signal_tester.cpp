// Perform an extensive test of the ACE_Reactor's event dispatching
// $Id$

// mechanisms.  These mechanisms illustrate how signals, I/O, and
// timeout events can all be handled within the same framework.  In
// addition, this example illustrates how to use the ACE_Reactor for
// devices that perform I/O via signals (such as SVR4 message queues).

#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"

// Used to shut down the event loop.
static sig_atomic_t done = 0;

// This class illustrates how to handle signal-driven I/O using the
// ACE_Reactor framework.  Note that signals may be caught and
// processed without requiring the use of global signal handler
// functions or global signal handler data.

class Sig_Handler : public ACE_Event_Handler
{
public:
  Sig_Handler (void);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int shutdown (ACE_HANDLE, ACE_Reactor_Mask);
  virtual int handle_signal (ACE_HANDLE signum, siginfo_t * = 0, 
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
  ACE_ASSERT (this->handle_ != -1);

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.
  if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "register_handler", 1));

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);  

  // Register the signal handler object to catch the signals.
  if (ACE_Service_Config::reactor ()->register_handler (sig_set, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "register_handler", 1));
}

// Called by the ACE_Reactor to extract the fd.

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
  ACE_DEBUG ((LM_DEBUG, "handling asynchonrous input...\n"));
  return 0;
}

// In a real application, this method would do any cleanup activities
// required when shutting down the I/O device.

int 
Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First, 
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's 
// signal handling mechanism eliminates the need to use global signal 
// handler functions and data. 

int
Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      // Rearm the alarm.
      ACE_OS::alarm (4);
      break;
    case SIGINT:
      // Tell the ACE_Reactor to enable the ready bit for
      // this->handle_.  The ACE_Reactor will subsequently call the
      // Sig_Handler::handle_input method from within its event loop.
      return ACE_Service_Config::reactor ()->ready_ops 
	(this->handle_, ACE_Event_Handler::READ_MASK, ACE_Reactor::ADD_MASK);
    case SIGQUIT:
      ACE_DEBUG ((LM_DEBUG, "%S: shutting down signal tester\n", signum));
      ACE_Service_Config::end_reactor_event_loop ();
      break;
    default: 
      ACE_DEBUG ((LM_DEBUG, 
		  "%S: not handled, returning to program\n", signum));
      break;
    }
  return 0;
}

// This class illustrates that the ACE_Reactor can handle signals,
// STDIO, and timeouts using the same mechanisms.

class STDIN_Handler : public ACE_Event_Handler
{
public:
  STDIN_Handler (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &, 
			      const void *arg);
};

STDIN_Handler::STDIN_Handler (void)
{
  if (ACE::register_stdin_handler (this,
				   ACE_Service_Config::reactor (),
				   ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));

  // Register the STDIN_Handler to be dispatched once every second.
  else if (ACE_Service_Config::reactor ()->schedule_timer
	   (this, 0, ACE_Time_Value (1), ACE_Time_Value (1)) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "schedule_timer", 1));
}

int 
STDIN_Handler::handle_timeout (const ACE_Time_Value &tv, 
			       const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "timeout occurred at %d sec, %d usec\n",
	      tv.sec (), tv.usec ()));
  return 0;
}

// Read from input descriptor and write to stdout descriptor.

int 
STDIN_Handler::handle_input (ACE_HANDLE handle)
{
  ssize_t n;
  char buf[BUFSIZ];

  switch (n = ACE_OS::read (handle, buf, sizeof buf))
    {
    case -1:
      if (errno == EINTR)
	return 0;
        /* NOTREACHED */
      else
	ACE_ERROR ((LM_ERROR, "%p\n", "read"));
      /* FALLTHROUGH */
    case 0:
      ACE_Service_Config::end_reactor_event_loop ();
      break;
    default:
      {
	ssize_t result = ACE::write_n (ACE_STDOUT, buf, n);

	if (result != n)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write"), 
			    result == -1 && errno == EINTR ? 0 : -1);
      }
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon (argv [0]);

  // Signal handler.
  Sig_Handler sh;

  // Define an I/O handler object.
  STDIN_Handler ioh;

  // Optionally start the alarm.
  if (argc > 1)
    ACE_OS::alarm (4);

  // Loop handling signals and I/O events until SIGQUIT occurs.

  while (daemon.reactor_event_loop_done () == 0)
    daemon.run_reactor_event_loop ();

  return 0;
}
