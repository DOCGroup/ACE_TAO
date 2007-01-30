// $Id$
// This is a simple test of an ImR using the corba interfaces
// Start with -orbinitref Test=...

#include "testC.h"
#include "ace/Log_Msg.h"

using namespace CORBA;

int main(int argc, char* argv[]) {

  try
  {

    ORB_var orb = ORB_init(argc, argv, 0);

    Object_var obj = orb->resolve_initial_references("Test");
    test_var test = test::_narrow(obj.in());
    ACE_ASSERT(! is_nil(test.in()));

    Long n = test->get();
    Long m = test->get();
    if (m == n + 1)
      ACE_DEBUG((LM_DEBUG, "Client: All tests ran successfully.\n"));
    else
      ACE_DEBUG((LM_DEBUG, "Error: Client Expected %d = %d + 1.\n", m, n));

    return 0;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("client:");
  }
  return -1;
}
