// $Id$

#include "Coll_Tester.h"
#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(Collocation, main, "$Id$")

int main (int argc, char *argv[])
{
  Collocation_Test coll_test;

  ACE_TRY_NEW_ENV
    {
      coll_test.init (argc, argv ACE_ENV_ARG_PARAMETER);

      coll_test.run ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Uncaught exception: ");
    }
  ACE_ENDTRY;

  return 0;
}
