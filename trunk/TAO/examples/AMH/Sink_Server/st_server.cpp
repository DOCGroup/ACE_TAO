
// $Id$

#include "AMH_Servant.h"
#include "Base_Server.h"


char *
usage ()
{
  return "invoke as: st_server -o <ior_output_file> \n -s <sleep_time (in microseconds)> \n";
}

int
main (int argc, char *argv[])
{
  Base_Server amh_server (&argc, argv);
  amh_server.try_RT_scheduling();
  amh_server.start_orb_and_poa ();

  if (amh_server.parse_args () != 1)
    {
        ACE_ERROR ((LM_ERROR, "%s : %s", "IOR file missing", usage ()));
        ACE_OS::exit (1);
    }

  AMH_Servant *servant = new AMH_Servant (amh_server.orb ());

  if (servant->parse_args (&argc, argv) != 1)
    {
        ACE_ERROR ((LM_ERROR, "%s : %s", "sleep time unspecified", usage ()));
        ACE_OS::exit (1);
    }

  // Server takes over memory responsibility for servant,
  // no need to deallocate it at the end
  amh_server.register_servant (servant);
  amh_server.run_event_loop ();
}
