// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Service_Configurator/Misc
//
// = FILENAME
//    main.cpp
//
// = DESCRIPTION
//   This directory contains an example that illustrates how the ACE
//   Service Configurator can configure static and dynamic services,
//   both from the command-line and from a svc.config file.
//
// = AUTHOR
//    Doug Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "ace/Service_Config.h"
#include "Timer_Service.h"

ACE_RCSID(Misc, main, "$Id$")

// Create an object that will insert the <Timer_Service> into the list
// of statically linked services that the <ACE_Service_Config> will
// process at run-time.
ACE_STATIC_SVC_REQUIRE (Timer_Service_1)

int
main (int, ASYS_TCHAR *argv[])
{
  ASYS_TCHAR *l_argv[7];

  // Manufacture a "fake" svc.conf entry to demonstrate the -S option
  // that allows us to pass these entries via the "command-line"
  // rather than the svc.conf file.
  l_argv[0] = argv[0];
  l_argv[1] = ASYS_TEXT ("-y");
  l_argv[2] = ASYS_TEXT ("-S");
  l_argv[3] = ASYS_TEXT ("static Timer_Service_1 \"timer 1 10 $TRACE\"");
  l_argv[4] = ASYS_TEXT ("-S");
  l_argv[5] = ASYS_TEXT ("dynamic Timer_Service_2 Service_Object * ./Timer:_make_Timer_Service_2() \"timer 2 10 $TRACE\"");
  l_argv[6] = 0;

  if (ACE_Service_Config::open (6, l_argv) == -1 && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("open")),
                      1);

  // Run forever, performing the configured services until we
  // shutdown.

  ACE_Reactor::run_event_loop ();
  return 0;
}
