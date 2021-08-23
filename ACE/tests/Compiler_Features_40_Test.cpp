/**
 * This program checks if the compiler/RTL does have correct support
 * for stack unwinding
 */

#include "test_config.h"
#include <vector>
#include <string>
#include <stdexcept>

class TTestClass;

static TTestClass* this_pointer {};
static bool isDataCorrupt = false;

class TTestClass
{
  unsigned char data[256];

public:
  TTestClass()
  {
    this_pointer = this;

    // Generate a sequence of numbers (0 - 255)
    for (int i = 0; i < 256; i++)
    {
      data[i] = i;
    }
  }

  ~TTestClass()
  {
    if (this != this_pointer)
    {
      isDataCorrupt = true;
      ACE_ERROR ((LM_ERROR, "ERROR: Invalid this %@ != %@\n", this, this_pointer));
    }

    // Check if number sequence is still the same
    for (int i = 0; i < 256; i++)
    {
      if (data[i] != i)
      {
        isDataCorrupt = true;
        break;
      }
    }
  }

private:
  std::vector<std::string> SomeData;
};

void SomeFunction(std::string)
{
  // Unused function
}

void Test()
{
  std::string str = "branch_b";

  if (str == "branch_a")
  {
    SomeFunction("");
  }
  else if (str == "branch_b")
  {
    TTestClass test;
    throw std::runtime_error("Some exception");
  }
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_40_Test"));

  int retval = 0;
  try
  {
     Test();
  }
  catch (...)
  {
  }

  if (isDataCorrupt)
  {
    ACE_ERROR((LM_ERROR, "ERROR: Invalid data\n"));
    ++retval;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "Correct data\n"));
  }

  ACE_END_TEST;

  return retval;
}

