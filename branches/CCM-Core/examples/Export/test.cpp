// $Id$

#include "dll.h"
#include <iostream>

int 
main (int, char *[])
{
  int failure_count = 0;
  test_class my_test_class;

  // Tet out the export of a class.  I don't see
  // How this can fail at runtime (rather it would
  // probably give link errors), but just in case...

  std::cout << "Method Test: ";
  if (my_test_class.method () != RETVAL)
    {
      std::cout << "Failed" << std::endl;
      ++failure_count;
    }
  else
    std::cout << "Succeeded" << std::endl;

  // Test out the export of a function.  Like above,
  // I don't know how this can fail at runtime.
  
  std::cout << "Function Test: ";
  if (test_function () != RETVAL)
    {
      std::cout << "Failed" << std::endl;
      ++failure_count;
    }
  else
    std::cout << "Succeeded" << std::endl;

  // Also test out the export of data.

  std::cout << "Variable Test: ";
  if (test_variable != RETVAL)
    {
      std::cout << "Failed" << std::endl;
      ++failure_count;
    }
  else
    std::cout << "Succeeded" << std::endl;


  // Test out the ACE_Singleton export by checking to see
  // that we have the same instance pointer as the DLL does.
  // This can fail at runtime.

  std::cout << "Singleton Test: ";
  if (TEST_SINGLETON::instance () != get_dll_singleton ())
    {
      std::cout << "Failed" << std::endl;
      ++failure_count;
    }
  else
    std::cout << "Succeeded" << std::endl;

  // Return the number of failures
  return failure_count;
}
