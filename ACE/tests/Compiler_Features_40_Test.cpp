/**
 * This program checks if the compiler/RTL does have correct support
 * for stack unwinding 
 */

#include "test_config.h"
#include <vector>
#include <string>
//#include <vcl.h>
#include "ace/SString.h"

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
			//Generate a sequence of numbers (0 - 255)
			for (int i = 0; i < 256; i++)
			{
				data[i] = i;
			}
SomeData.push_back ("Foo");
	//		ACE_DEBUG((LM_DEBUG, "Constructor called with %@\n", this));
		}

		~TTestClass()
		{
//			ACE_DEBUG((LM_DEBUG, "Destructor called with %@\n", this));

//if (this != this_pointer)
//ACE_ERROR ((LM_ERROR, "ERROR: Invalid this\n"));
			//Check if number sequence is still the same
			
			for (int i = 0; i < 256; i++)
			{
				if (data[i] != i)
				{
					isDataCorrupt = true;
					break;
				}
			}

			if (isDataCorrupt)
			{
MessageBoxW(0, L"Object data is corrupt!", L"Fatal error", 0x30);
				ACE_ERROR((LM_ERROR, "ERROR: Invalid data\n"));
			}
else 
ACE_DEBUG ((LM_DEBUG, "Correct data\n"));
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

	if (std::strcmp(str.c_str(), "branch_a") == 0)
	{
		SomeFunction("");
	}
	else if (str == "branch_b")
	{
		TTestClass test;
		throw std::exception("Some exception");
	}

}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_40_Test"));

 try 
  {
     Test();
  }
  catch (...)
  {
  } 

  ACE_END_TEST;

  return 0;
}

