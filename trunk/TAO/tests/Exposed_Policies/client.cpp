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
  int status = 0;
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Policy_Verifier policy_verifier;

      bool retval = policy_verifier.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval)
        {
          policy_verifier.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          status++;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Exception Raised");
      status++;
    }

  ACE_ENDTRY;

  return status;
}
