// $Id$

// -- App. Specific Include --
#include "CounterC.h"
// #include "util.h"

// -- App. Specific Include --
#include "Policy_Verifier.h"

ACE_RCSID(tao, client, "$Id$")

int
main (int argc, char *argv[])
{
#if (TAO_HAS_RT_CORBA == 1)

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Policy_Verifier policy_verifier;

      policy_verifier.init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policy_verifier.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Exception Raised");
      return 1;
    }

  ACE_ENDTRY;

#else

  ACE_UNUSED_ARG(argc);
  ACE_UNUSED_ARG(argv);
  ACE_DEBUG ((LM_DEBUG,
              "Exposed_Policies are disabled on this configuration\n"));

#endif /* (TAO_HAS_RT_CORBA == 1) */

  return 0;
}
