// $Id$

#include "Notifier_Input_Handler.h"

ACE_RCSID(notifier, Callback_Quoter, "$Id$")

// This is the main driver program for the Callback Quoter Notifier.

int
main (int argc, char *argv[])
{
  Notifier_Input_Handler notifier;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tNotifier\n\n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      int rc = notifier.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (rc == -1)
        return 1;
      else
        {
          notifier.run (TAO_ENV_SINGLE_ARG_PARAMETER);
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
