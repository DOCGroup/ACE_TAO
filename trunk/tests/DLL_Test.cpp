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

#include "test_config.h" /* Include first to enable ACE_ASSERT. */
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"

class Hello 
{
  // = TITLE
  //    The Hello class in the dynamically linkable library.
  //
  // = DESCRIPTION
  //    This class is used in this example to show how a library can
  //    be loaded on demand and its methods called on getting the
  //    symbols from the library.
public:

  Hello (void)
    { }

  ~Hello (void)
    { }

  void say_hello (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Hello\n"));
    }

  void say_next (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "How are you?\n"));
    }
};

// This function returns the Hello object pointer.

extern "C"
Hello *get_hello (void)
{
  Hello *hello;

  // *done* 
  //@@ Kirthika, please check the return types carefully here...
  ACE_NEW_RETURN (hello, Hello (), NULL);

  return hello;
}

typedef Hello *(*TC)(void);

int 
main (void)
{ 
  ACE_START_TEST (ASYS_TEXT ("DLL_Wrapper_Test"));

  // @@ Kirthika, it turns out that with Windows NT you can't pass a 0
  // in and have it default to the symbols in the executable.
  // Therefore, you'll have to use something like ".obj/DLL_Test.o" on
  // UNIX and whatever the equivalent is on WinNT (I'm not sure what
  // it'll be, so please make sure that you check with Nanbor to find
  // out).
  ACE_DLL ace_dll_obj;

  ACE_DEBUG ((LM_DEBUG,
              "before opening\n"));

  if (0 != open (".obj/DLL_Wrapper_Test.o", 0))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cant open the library\n"),
                       -1);
  ACE_DEBUG ((LM_DEBUG,
              "after opening\n"));

  
  //*done* 
  // @@ Kirthika, I recommend that you don't use the constructor to
  // open the DLL_Test file, but rather use the "open()" method since
  // that way you can test the return value and print an error if it
  // doesn't work.

  TC f = (TC) ace_dll_obj.symbol ("get_hello");

  if ( f == NULL)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cant obtain symbol: get_hello\n"),
                      -1);
  // *done*
  // @@ Kirthika, please make sure to test whether <f> is a NULL
  // pointer or a valid pointer before trying to call it.

  ACE_DEBUG ((LM_DEBUG,
              "callling get_hello\n"));

  auto_ptr <Hello> my_hello = f ();

  // Make the method calls, now that the object pointer is available.
  my_hello->say_hello ();
  my_hello->say_next ();

  ACE_END_TEST;
  return 0;
}

// *done*
//@@ Kirthika, please add the proper explicit template instantiation
// format for GCC and other broken C++ compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr <Hello>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr <Hello>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
