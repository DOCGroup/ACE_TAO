// $Id$

#include "Naming_Service.h"

// Driver function for the TAO Naming Service.

int
main (int argc, char *argv[])
{
  TAO_Naming_Service naming_service;

  if (naming_service.init (argc, argv) == -1)
    return 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      naming_service.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "NamingService");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
