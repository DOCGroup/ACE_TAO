// $Id$

#include "dll.h"
#include <ace/streams.h>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int failure_count = 0;
  test_class my_test_class;

  // Tet out the export of a class.  I don't see
  // How this can fail at runtime (rather it would
  // probably give link errors), but just in case...

  cout << "Method Test: ";
  if (my_test_class.method () != RETVAL)
    {
      cout << "Failed" << endl;
      ++failure_count;
    }
  else
    cout << "Succeeded" << endl;

  // Test out the export of a function.  Like above,
  // I don't know how this can fail at runtime.

  cout << "Function Test: ";
  if (test_function () != RETVAL)
    {
      cout << "Failed" << endl;
      ++failure_count;
    }
  else
    cout << "Succeeded" << endl;

  // Also test out the export of data.

  cout << "Variable Test: ";
  if (test_variable != RETVAL)
    {
      cout << "Failed" << endl;
      ++failure_count;
    }
  else
    cout << "Succeeded" << endl;


  // Test out the ACE_Singleton export by checking to see
  // that we have the same instance pointer as the DLL does.
  // This can fail at runtime.

  cout << "Singleton Test: ";
  if (TEST_SINGLETON::instance () != get_dll_singleton ())
    {
      cout << "Failed" << endl;
      ++failure_count;
    }
  else
    cout << "Succeeded" << endl;

  // Return the number of failures
  return failure_count;
}
