// $Id$

// -- App. Specific Include --
#include "CounterC.h"
// #include "util.h"

// -- App. Specific Include --
#include "Policy_Verifier.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(tao, client, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Policy_Verifier policy_verifier;

      policy_verifier.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_verifier.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
