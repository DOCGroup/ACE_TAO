// $Id$

#include "Naming_Context.h"
#include "Temperature_Monitor.h"
#include "Temperature_Monitor_Options.h"

// Listing 1 code/ch21
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Temperature_Monitor_Options opt (argc, argv);
  // Listing 1

  // Listing 2 code/ch21
  Naming_Context naming_context;

  ACE_Name_Options *name_options = naming_context.name_options();
  // Listing 2

  // Listing 3 code/ch21
  ACE_TCHAR *naming_options_argv[] = { argv[0] };
  name_options->parse_args
    (sizeof(naming_options_argv) / sizeof(ACE_TCHAR*),
     naming_options_argv);
  name_options->context (ACE_Naming_Context::PROC_LOCAL);
  naming_context.open (name_options->context ());
  // Listing 3

  // Listing 4 code/ch21
  Temperature_Monitor temperature_monitor (opt, naming_context);
  temperature_monitor.monitor ();
  // Listing 4

  // Listing 5 code/ch21
  naming_context.close ();
  return 0;
  // Listing 5
}
