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
#include "ace/ARGV.h"
#include "ace/Log_Msg.h"
#include "Timer_Service.h"

ACE_RCSID(Misc, main, "$Id$")

// Create an object that will insert the <Timer_Service> into the list
// of statically linked services that the <ACE_Service_Config> will
// process at run-time.
ACE_STATIC_SVC_REQUIRE (Timer_Service_1)

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  // Set up an argument vector that we can add entries to!
  ACE_ARGV args;

  // Manufacture a "fake" svc.conf entry to demonstrate the -S option
  // that allows us to pass these entries via the "command-line"
  // rather than the svc.conf file.
  args.add (argv[0]);
  args.add (ACE_TEXT ("-y"));
  args.add (ACE_TEXT ("-d"));
  args.add (ACE_TEXT ("-S"));
  args.add (ACE_TEXT ("\"static Timer_Service_1 'timer 1 10 $TRACE'\""));
  args.add (ACE_TEXT ("-S"));
  args.add (ACE_TEXT ("\"dynamic Timer_Service_2 Service_Object * ./Timer:_make_Timer_Service_2() 'timer 2 10 $TRACE'\""));
  // Test the -f option!
  args.add (ACE_TEXT ("-fsvc.conf1"));
  args.add (ACE_TEXT ("-fsvc.conf2"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("argc = %d\n"),
              ACE_TEXT (args.argc ())));

  // Print the contents of the combined <ACE_ARGV>.
  for (int i = 0; i < args.argc (); i++)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%d) %s\n"),
                i,
                args.argv ()[i]));

  int result = ACE_Service_Config::open (args.argc (),
                                         args.argv (),
                                         ACE_DEFAULT_LOGGER_KEY,
                                         0);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      1);

  // Run forever, performing the configured services until we
  // shutdown.

  ACE_Reactor::run_event_loop ();
  return 0;
}
