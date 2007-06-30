// $Id$
#include "Server_i.h"
#include "INS_i.h"

// This is the main driver program for the INS test server.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  Server_i server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tINS test server\n\n"));

  try
    {
      if (server.init (argc,
                       argv) == -1)
        return 1;
      else
        {
          server.run ();
        }
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }

  return 0;
}
