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
  ACE_TRY_NEW_ENV
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
