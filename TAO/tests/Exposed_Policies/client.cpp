// $Id$

// -- App. Specific Include --
#include "CounterC.h"
// #include "util.h"

// -- App. Specific Include --
#include "Policy_Verifier.h"

#include "tao/Strategies/advanced_resource.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(tao, client, "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  int status = 0;
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Policy_Verifier policy_verifier;

      bool retval = policy_verifier.init (convert.get_argc(), convert.get_ASCII_argv() ACE_ENV_ARG_PARAMETER);
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
