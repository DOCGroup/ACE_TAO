// $Id$

#include "Clerk_i.h"

ACE_RCSID(TimeService, Time_Service_Clerk, "$Id$")

// This is the main driver program for the Time Service clerk.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  Clerk_i clerk;

  ACE_DEBUG ((LM_DEBUG,
              "[SERVER] Process/Thread Id : (%P/%t) Time Service clerk\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int r = clerk.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (r == -1)
        return 1;
      else
        {
          clerk.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
