// $Id$

// Listing 1 code/ch19
#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config::open (argc, argv);

  // We use this version of the event loop so that reconfigurations
  // are triggered properly.
  ACE_Reactor::instance ()->run_reactor_event_loop (ACE_Reactor::check_reconfiguration);
  return 0;
}
// Listing 1
