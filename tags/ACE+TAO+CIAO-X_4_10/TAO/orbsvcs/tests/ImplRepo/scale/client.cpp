// $Id$
// This is a simple test of an ImR using the corba interfaces
// Start with -orbinitref Test=...

#include "testC.h"
#include "ace/Log_Msg.h"

using namespace CORBA;

int main(int argc, char* argv[]) {

  ACE_TRY_NEW_ENV
  {
 
    ORB_var orb = ORB_init(argc, argv, 0 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    Object_var obj = orb->resolve_initial_references("Test" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    test_var test = test::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! is_nil(test.in()));

    Long n = test->get(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    Long m = test->get(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (m == n + 1)
      ACE_DEBUG((LM_DEBUG, "Client: All tests ran successfully.\n"));
    else
      ACE_DEBUG((LM_DEBUG, "Error: Client Expected %d = %d + 1.\n", m, n));

    return 0;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client:");
  }
  ACE_ENDTRY;
  return -1;
}
