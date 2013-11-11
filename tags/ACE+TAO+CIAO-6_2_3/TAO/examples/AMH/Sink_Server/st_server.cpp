// $Id$

#include "AMH_Servant.h"
#include "Base_Server.h"

void
usage (const char *message)
{
  // @@ Mayur, what use is there in placing a space before a newline
  //    character?
  //
  // Mayur: It makes the newline stand out

  static const char * usage =
    "invoke as: st_server -o <ior_output_file>\n"
    "-s <sleep_time (in microseconds)>\n";

  ACE_ERROR ((LM_ERROR, "%s : %s", message, usage));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Base_Server amh_server (argc, argv);
  amh_server.try_RT_scheduling();
  amh_server.start_orb_and_poa ();

  if (amh_server.parse_args () != 1)
    {
      usage ("IOR file missing\n");
      ACE_OS::exit (1);
    }

  AMH_Servant servant (amh_server.orb ());

  if (servant.parse_args (argc, argv) != 1)
    {
      usage ("Sleep time unspecified\n");
      ACE_OS::exit (1);
    }

  amh_server.register_servant (&servant);
  amh_server.run_event_loop ();
  amh_server.shutdown_orb_and_poa ();

  return 0;
}
