// $Id$

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "HTTP_Server.h"

ACE_STATIC_SVC_REQUIRE(HTTP_Server)

extern "C"
{

  // call exit() so that static destructors get called 
static void
handler (int) 
{ 
  ACE_OS::exit (0); 
}

}

// This is the driver entry point into JAWS.  It is possible to use
// JAWS as an ACE Service, as well.

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_OS::signal (SIGCHLD, SIG_IGN);
  ACE_UNUSED_ARG (sa);

  if (daemon.open (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Run forever, performing the configured services until we receive
  // a SIGINT.

  ACE_Reactor::run_event_loop ();
  return 0;
}
