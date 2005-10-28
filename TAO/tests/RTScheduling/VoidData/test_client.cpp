//$Id$
#include "tao/RTScheduling/RTScheduler.h"
#include "testC.h"
#include "test.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR* argv [])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  CORBA::ORB_var orb;
  ACE_TRY_NEW_ENV
    {
      orb = CORBA::ORB_init (convert.get_argc(),
                             convert.get_ASCII_argv(),
                             ""
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_ptr foo_i;

      ACE_NEW_RETURN (foo_i,
                      test_impl,
                      -1);

      int something = 28;
      foo_i->bar ((CORBA::VoidData) &something);
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
