// $Id$

#include "Naming_Context.h"
#include "Temperature_Grapher.h"
#include "Temperature_Grapher_Options.h"

// Listing 1 code/ch21
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Temperature_Grapher_Options opt (argc, argv);

  Naming_Context naming_context;
  ACE_Name_Options *name_options = naming_context.name_options ();
  name_options->process_name (argv[0]);
  name_options->context (ACE_Naming_Context::NODE_LOCAL);
  naming_context.open (name_options->context ());

  Temperature_Grapher grapher (opt, naming_context);
  grapher.monitor ();
  naming_context.close ();
  return 0;
}
// Listing 1
