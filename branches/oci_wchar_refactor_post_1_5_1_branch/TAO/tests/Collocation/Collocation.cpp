// $Id$

#include "Collocation_Tester.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Collocation, main, "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  ACE_TRY_NEW_ENV
    {
      Collocation_Test coll_test;

      coll_test.init (convert.get_argc(), convert.get_ASCII_argv() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      coll_test.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      coll_test.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Uncaught exception: ");
    }
  ACE_ENDTRY;

  return 0;
}
