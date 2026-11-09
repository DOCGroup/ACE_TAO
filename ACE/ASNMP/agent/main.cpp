// This server daemon processes SNMP Version 1 get, get-next, and set
// commands. over the MIB II "System" group only.

#include "ace/config-all.h"
#include "snmp_agent.h"
#include "ace/Init_ACE.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE::init ();
  snmp_agent the_agent;

#ifdef ACE_USES_WCHAR
  ACE_Argv_Type_Converter arg_converter (argc, argv);
  int const result =
    the_agent.set_args (arg_converter.get_argc (), arg_converter.get_ASCII_argv ());
#else
  int const result = the_agent.set_args (argc, argv);
#endif
  if (result != 0)
    return 1;

  if (!the_agent.valid ())
    return 1;

  return the_agent.run (); // main loop
}
