// $Id$
#include "../Simple_util.h"
#include "Server_i.h"

// This is the main driver program for the time and date server.

int
main (int argc, char *argv[])
{
  Server<Checkpoint_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tCheckpoint Event_Types server\n\n"));

  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      if (server.init ("Event_Types_Checkpoint",
                       argc,
                       argv
                       TAO_ENV_ARG_PARAMETER) == -1)
        return 1;
      else
        {
          server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Server<Checkpoint_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Server<Checkpoint_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
