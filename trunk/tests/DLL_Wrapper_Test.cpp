// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    DLL_Wrapper_Test.cpp
//
// = DESCRIPTION
//    This test illustrates the use of <ACE_DLL> wrapper.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "ace/DLL.h"

class Hello 
{
public:

  Hello ()
    {
    }

  ~Hello ()
    {
    }

  void sayHello ()
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Hello\t"));
    }

  void sayNext ()
    {
      ACE_DEBUG ((LM_DEBUG,
                  "How are you?\n"));
    }
  
};

// This function returns the Hello object pointer.

extern "C"
Hello* getHello (void)
{
  Hello *hello;
  hello = new Hello;
  return hello;
}

typedef Hello* (*TC)(void);
int main (void)
{
  // First get an DLL object. Since the dllname is empty, the symbols 
  // are loaded from a.out.
  ACE_DLL ace_dll_obj (ACE_DL_TYPE (0));
  TC f = (TC) ace_dll_obj.symbol ("getHello");
  Hello* my_hello = f ();

  // Make the method calls, now that the object pointer is available.  
  my_hello->sayHello ();
  my_hello->sayNext ();

  delete my_hello;
  return 0;
}
