// $Id$
// This is a simple test of an ImR using the corba interfaces

#include "testC.h"

#include <stdexcept>
#include <sstream>
#include "ace/Log_Msg.h"

#define assertTrue(CONDITION) \
    if (CONDITION == false) { \
    std::ostringstream oss; \
      oss << "Error : "#CONDITION" "__FILE__":" << __LINE__; \
      throw std::runtime_error(oss.str()); \
    } \

using namespace CORBA;

int main(int argc, char* argv[]) {

  try {
 
    ORB_var orb = ORB_init(argc, argv);

    Object_var obj = orb->resolve_initial_references("Test");
    test_var test = test::_narrow(obj.in());
    assertTrue(! is_nil(test.in()));

    Long n = test->get();
    Long m = test->get();
    assertTrue(m == n + 1);
 
    ACE_DEBUG((LM_DEBUG, "All tests ran successfully.\n"));

    return 0;
  } catch (CORBA::Exception& e) {
    ACE_PRINT_EXCEPTION(e, "test:");
  }
  return 1;
}
