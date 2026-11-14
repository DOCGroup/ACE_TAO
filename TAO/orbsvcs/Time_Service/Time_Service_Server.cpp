#include "orbsvcs/Log_Macros.h"
#include "Server_i.h"
#include "ace/OS_main.h"

// This is the main driver program for the Time Service server.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_i server;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "[SERVER] Process/Thread Id : (%P/%t) Time Service server\n"));

  try
    {
      int const result = server.init (argc, argv);
      if (result == -1)
        return 1;

      server.run ();
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }

  return 0;
}
