// $Id$
#include "../Simple_util.h"
#include "Time_i.h"

// This is the main driver program for the time and date server.

int
main (int argc, char *argv[])
{
  Server<Time_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tTime and Date server\n\n"));
  
  ACE_DECLARE_NEW_CORBA_ENV;
 
  ACE_TRY
    {
      if (server.init ("Time",
                       argc,
                       argv,
                       ACE_TRY_ENV) == -1)
        return 1;
      else
        {
          server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Server<Time_i>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Server<Time_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

