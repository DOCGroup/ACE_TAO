// $Id$

#include "MT_AMH_Server.h"
#include "AMH_Servant.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  MT_AMH_Server amh_server (argc, argv);
  amh_server.try_RT_scheduling();
  amh_server.start_orb_and_poa ();

  if (amh_server.parse_args () != 1)
    {
        amh_server.usage ("");
        ACE_OS::exit (1);
    }

  AMH_Servant servant (amh_server.orb ());

  if (servant.parse_args (argc, argv) != 1)
    {
      amh_server.usage ("sleep time unspecified");
      ACE_OS::exit (1);
    }

  amh_server.register_servant (&servant);
  amh_server.start_threads ();
  amh_server.shutdown_orb_and_poa ();

  return 1;
}
