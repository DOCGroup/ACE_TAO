// $Id$

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "Coll_Tester.h"

ACE_RCSID(Collocation, main, "$Id$")

int main (int argc, char *argv[])
{
  Collocation_Test coll_test;

  ACE_TRY_NEW_ENV
    {
      coll_test.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      coll_test.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Uncaught exception: ");
    }
  ACE_ENDTRY;

  return 0;
}
