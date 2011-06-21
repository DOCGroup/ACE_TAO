// $Id$

#include "airplane_server_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Airplane_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Server\n\n"));

  try
    {
      int retval = server.init (argc, argv);

      if (retval == -1)
        return -1;


      retval = server.run ();

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
