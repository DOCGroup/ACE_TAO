#include "Server_i.h"
#include "INS_i.h"

// This is the main driver program for the INS test server.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_i server;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\tINS test server\n\n")));

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
