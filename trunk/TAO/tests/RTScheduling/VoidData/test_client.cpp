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
      ACE_CHECK_RETURN (-1);
  
      test_ptr foo_i;
  
      ACE_NEW_RETURN (foo_i,
                      test_impl,
                      -1);

      int something =  28;
      foo_i->bar ((CORBA::VoidData)something);
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

