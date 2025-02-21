#include "server_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_i server;

      int retval = server.init (argc, argv);
      if (retval == -1)
        return -1;
      if (retval == 1)
        return 0;

      server.run ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("Server: System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("Server: User Exception");
      return -1;
    }

  return 0;
}
