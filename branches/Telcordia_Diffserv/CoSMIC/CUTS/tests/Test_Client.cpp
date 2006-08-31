// $Id$

#include "ace/DLL.h"
#include "ace/Log_Msg.h"
#include "Test.h"

static const char * CUTS_TEST_SYMBOL = "create_cuts_test";

//
// main
//
int main (int argc, char * argv [])
{
  ACE_DLL test_dll;

  if (test_dll.open (argv[1], ACE_DEFAULT_SHLIB_MODE, 0) == 0)
  {
    // Load the export symbol to create the test and create the
    // test using the symbol.
    typedef CUTS_Test * (* CUTS_TEXT_EXPORT_SYMBOL)(void);

    CUTS_TEXT_EXPORT_SYMBOL create_cuts_test =
      (CUTS_TEXT_EXPORT_SYMBOL) test_dll.symbol (CUTS_TEST_SYMBOL);

    CUTS_Test * test = 0;

    if (create_cuts_test != 0)
    {
      test = create_cuts_test ();
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "%M - failed to load symbol %s\n",
                  CUTS_TEST_SYMBOL));
    }

    if (test != 0)
    {
      // Run the test.
      test->run (argc, argv);

      ACE_ERROR ((LM_INFO,
                  "%M - [%s]: %u passed and %u failed\n",
                  test->name (),
                  test->passed (),
                  test->failed ()));

      // Close the test and the DLL module.
      test->close ();
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "%M - failed to run test in %s\n",
                  test_dll.dll_name_));
    }

    // Close the DLL.
    test_dll.close ();
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                "%M - failed to load %s\n",
                argv[1]));
  }
  return 0;
}
