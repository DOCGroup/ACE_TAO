// main.cpp

#include "ace/Service_Config.h"

 #include "HTTP_Server.h"

ACE_STATIC_SVC_REQUIRE(HTTP_Server)

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  if (daemon.open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Run forever, performing the configured services until we receive
  // a SIGINT.

  daemon.run_reactor_event_loop ();
  return 0;
}
