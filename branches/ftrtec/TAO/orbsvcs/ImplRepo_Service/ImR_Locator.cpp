// $Id$

#include "ImR_Locator_i.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Instantiate an instance of ImR_Locator_i
      ImR_Locator_i *imr_locator;
      
      ACE_NEW_THROW_EX (imr_locator,
                        ImR_Locator_i,
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      // Invoke the init method and the ImR_Locator running.
      imr_locator->init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Locator");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
