// $Id$
#include "TestBasicTypes.h"
#include "TestBuilder.h"
#include "TestBasic.h"
#include "TestQoS.h"

#include <tao/corba.h>

//// Uncomment for automatic memory leak detection on Windows.
//#include <stdlib.h>
//#include <crtdbg.h>

int main(int, char**)
{
  //// Uncomment for automatic memory leak detection on Windows.
  //_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  int result = 0;
  ACE_TRY_NEW_ENV
  {
    // To aid debugging, always put new tests first.
    TestBasicTypes::testAll(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    TestBasic::testAll(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    TestBuilder::testAll(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    TestQoS::testAll(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Test failures throw UnitTesterException, so if we get here
    ACE_OS::printf ("All Unit Tests passed successfully.\n");
  }
  ACE_CATCHANY
  {
    ACE_ANY_EXCEPTION._tao_print_exception (ACE_TEXT ("Error caught in main"), stderr);
    result = 1;
  }
  ACE_CATCH(UnitTesterException, ex)
  {
    ACE_OS::fprintf (stderr, "Error : Test Failed %s\n", ex.what());
    result = 1;
  }
  ACE_CATCHALL
  {
    cerr << "Error : Unknown exception running unit test." << endl;
    result = 1;
  }
  ACE_ENDTRY;
  return result;
}
