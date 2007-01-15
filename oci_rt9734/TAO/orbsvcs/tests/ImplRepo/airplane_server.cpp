// $Id$

#include "airplane_server_i.h"

ACE_RCSID (ImplRepo,
           airplane_server,
           "$Id$")

int
main (int argc, char *argv[])
{
  Airplane_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Server\n\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int retval = server.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval == -1)
        return -1;
 
 
      retval = server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Paper Airplane Server says goodnight\n"));

      return retval;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
    }
  ACE_ENDTRY;

  return 1;
}
