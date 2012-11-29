// $Id$

#include "Naming_Context.h"
#include "Temperature_Monitor2.h"
#include "Temperature_Monitor_Options.h"

// Listing 1 code/ch21
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Temperature_Monitor_Options opt (argc, argv);
  Naming_Context process_context;
  {
    ACE_Name_Options *name_options =
      process_context.name_options ();
    name_options->context (ACE_Naming_Context::PROC_LOCAL);
    ACE_TCHAR *nargv[] = { argv[0] };
    name_options->parse_args (sizeof(nargv) / sizeof(ACE_TCHAR*) ,
                              nargv);
    process_context.open (name_options->context ());
  }

  Naming_Context shared_context;
  {
    ACE_Name_Options *name_options =
      shared_context.name_options ();
    name_options->process_name (argv[0]);
    name_options->context (ACE_Naming_Context::NODE_LOCAL);
    shared_context.open (name_options->context ());
  }

  Temperature_Monitor2 temperature_monitor (opt,
                                            process_context,
                                            shared_context);
  temperature_monitor.monitor ();

  process_context.close ();
  shared_context.close ();

  return 0;
}
// Listing 1


