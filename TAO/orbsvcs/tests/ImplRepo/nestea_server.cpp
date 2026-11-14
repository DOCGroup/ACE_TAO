#include "nestea_server_i.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Nestea_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tNestea Bookshelf Server\n\n"));

  try
    {
      int const retval = server.init (argc, argv);

      if (retval != 0)
        return retval;

      return server.run ();
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

  ACE_DEBUG ((LM_DEBUG, "Nestea Bookshelf Server says goodnight\n"));

  return 0;
}
