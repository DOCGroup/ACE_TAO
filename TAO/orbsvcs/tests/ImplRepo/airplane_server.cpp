#include "airplane_server_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Airplane_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Server\n\n"));

  try
    {
      if (server.init (argc, argv) == -1)
        return -1;

      int const retval = server.run ();

      ACE_DEBUG ((LM_DEBUG, "Paper Airplane Server says goodnight\n"));

      return retval;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
    }

  return 1;
}
