//$Id$
#include "tao/RTScheduling/RTScheduler.h"
#include "testC.h"
#include "test.h"

int
main (int argc, char* argv [])
{
  CORBA::ORB_var orb;
  ACE_TRY_NEW_ENV
    {
      orb = CORBA::ORB_init (argc,
                             argv,
                             ""
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_impl foo_i;
      int something = 28;
      foo_i.bar ((CORBA::VoidData) &something);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");

      orb->shutdown ();
      orb->destroy ();
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}
