// $Id$

#include "airplane_server_i.h"

ACE_RCSID(ImplRepo, airplane_server, "$Id$")

int
main (int argc, char *argv[])
{
  Airplane_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Server\n\n"));
  
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (server.init (argc, argv, ACE_TRY_ENV) == -1)
        return -1;
      else
        {
          server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
