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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (notifier.init (argc, argv, ACE_TRY_ENV) == -1)
        return 1;
      else
        {
          notifier.run (ACE_TRY_ENV);
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
