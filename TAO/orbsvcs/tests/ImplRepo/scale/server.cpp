// $Id$

#include "server_i.h"

int
main (int argc, char *argv[])
{
  try
    {
      Server_i server;

      int retval = server.init (argc, argv);

      if (retval == -1)
        return -1;
      else
        {
          server.run ();
        }
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
