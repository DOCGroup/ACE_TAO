// $Id$

# include "Collocation_Tester.h"

ACE_RCSID (tests, client, "$Id$")

int main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // ORB Initialization
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object;

      // Get The IOR from a file
      object = orb->string_to_object ("file://s.ior" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Invalid IOR file\n"));
          return 1;
        }

      Collocation_Tester tester (object.in ());
      tester.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Exception Raised");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
