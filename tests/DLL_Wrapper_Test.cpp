// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
// @@ Kirthika, please rename this test to be DLL_Test.cpp to be
// consistent with out other naming conventions in this directory.
//    DLL_Wrapper_Test.cpp
//
// = DESCRIPTION
//    This test illustrates the use of <ACE_DLL> wrapper class.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "ace/DLL.h"

class Hello 
{
  // = TITLE
  //    The Hello class in the dynamically linkable library.
  //
  // = DESCRIPTION
  //    This class is used in this example to show how a library can be loaded
  //    on demand and its methods called on getting the symbols from the library.
public:

  Hello (void)
    { }

  ~Hello (void)
    { }

  // @@ Kirthika, please follow the ACE programming 
  // guidelines when naming methods, i.e., say_hello (void) or just
  // hello (void).
  void say_hello (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Hello\t"));
    }

  // @@ Same here.
  void say_next (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "How are you?\n"));
    }
};

// This function returns the Hello object pointer.

extern "C"
// @@ *done*Kirthika, please following ACE programming guidelines to name
// this function...
Hello *get_hello (void)
{
  Hello *hello;
  //*done* @@ Kirthika, please always use ACE_NEW_RETURN for dynamic
  // allocation.

  ACE_NEW_RETURN (hello, Hello (), -1);
  // hello = new Hello;
  return hello;
}

typedef Hello *(*TC)(void);

int 
main (void)
{ 
  ACE_START_TEST ("DLL_Test");

  // @@*done* Kirthika, please follow the conventions for creating tests,
  // i.e., use the ACE_START_TEST() and ACE_END_TEST() macros.

  // @@ Kirthika, I suspect that the use of ACE_DL_TYPE (0) may not be
  // portable across NT and UNIX.  Therefore, can you please use the
  // name of the *.o file, i.e., ".shobj/DLL_Test.so" or whatevery it
  // is?  Note that you'll need to figure out how to use various ACE
  // macros to get this portable...  First get an DLL object. Since
  // the dllname is empty, the symbols are loaded from a.out.
  ACE_DLL ace_dll_obj ("./DLL_Test.o");// ***check where it is

  // @@ Kirthika, I'm not sure, but you may need to prefix a "_"
  // before the "getHello" name, i.e., "_getHello" to make this work
  // on some platforms.  Please use the ACE macros for portable here.
  TC f = (TC) ace_dll_obj.symbol ("get_hello");

  auto_ptr <Hello> my_hello = f ();
  // Hello *my_hello = f ();

  // Make the method calls, now that the object pointer is available.
  my_hello->say_hello ();
  my_hello->say_next ();

  //*done* Please use an auto_ptr<> to make sure that my_hello is properly
  // deleted when the scope of main() is exited.
  // delete my_hello;

  ACE_END_TEST;
  return 0;
}
