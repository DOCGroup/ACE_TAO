/* -*- C++ -*- */
// $Id$

#include "ace/Service_Config.h"
#include "Event_Channel.h"
#include "Gateway.h"

class Gateway : public ACE_Service_Object
  // = TITLE
  //     Integrates the whole Gateway application.
  //
  // = DESCRIPTION
  //     This implementation uses the <ACE_Event_Channel> as the basis
  //     for the <Gateway> routing.
{
public:
  // = Service configurator hooks.
  virtual int init (int argc, char *argv[]);
  // Perform initialization.

  virtual int fini (void);
  // Perform termination.

  virtual int info (char **, size_t) const;
  // Return info about this service.

protected:
  int handle_input (ACE_HANDLE);
  // Shut down the Gateway when input comes in from the controlling
  // console.

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Shut down the Gateway when a signal arrives.

  int parse_args (int argc, char *argv[]); 
  // Parse gateway configuration arguments obtained from svc.conf
  // file.

  ACE_Event_Channel<SUPPLIER_HANDLER, CONSUMER_HANDLER> event_channel_;
  // The Event Channel routes events from Supplier(s) to Consumer(s).
};

// Convenient shorthands.
// #define IC SUPPLIER_HANDLER
// #define OC CONSUMER_HANDLER

int
Gateway::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  if (signum > 0)
    ACE_DEBUG ((LM_DEBUG, "(%t) %S\n", signum));

  // Shut down the main event loop.
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

int
Gateway::handle_input (ACE_HANDLE h)
{
  if (ACE_Service_Config::reactor ()->remove_handler 
      (0, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "remove_handler"), -1);

  char buf[BUFSIZ];
  // Consume the input...
  ACE_OS::read (h, buf, sizeof (buf));

  // Shut us down.
  return this->handle_signal (h);
}

int
Gateway::init (int argc, char *argv[])
{
  if (this->event_channel_.open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "open"), -1);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT 
  // so we can shut down gracefully via signals.
  
  if (ACE_Service_Config::reactor ()->register_handler 
      (sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (ACE_Service_Config::reactor ()->register_handler 
      (0, this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
  return 0;
}

// This method is automatically called when the gateway is shutdown.
// It closes down the Event Channel.

int
Gateway::fini (void)
{
  this->event_channel_.close ();
  return 0;
}

// Returns information on the currently active service. 

int
Gateway::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s\t %s", "Gateway daemon", 
	     "# Application-level gateway\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Gateway.

ACE_SVC_FACTORY_DEFINE (Gateway)
