// $Id$

#include "Clerk_i.h"

ACE_RCSID(TimeService, Time_Service_Clerk, "$Id$")

// This is the main driver program for the Time Service clerk.

int
main (int argc, char *argv[])
{
  Clerk_i clerk;

  ACE_DEBUG ((LM_DEBUG,
              "[SERVER] Process/Thread Id : (%P/%t) Time Service clerk\n"));

  TAO_TRY
    {
      if (clerk.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          clerk.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
