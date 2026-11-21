#include "Notifier_Input_Handler.h"

// This is the main driver program for the Callback Quoter Notifier.
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Notifier_Input_Handler notifier;

  ACE_DEBUG ((LM_DEBUG, "\n\tNotifier\n\n"));

  try
    {
      int rc = notifier.init (argc, argv);

      if (rc == -1)
        return 1;
      else
        {
          notifier.run ();
        }
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }

  return 0;
}
