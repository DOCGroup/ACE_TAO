// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
 //    DLL_Test.cpp
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

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Considering UNIX OS to be default.
# if defined (ACE_HAS_WIN32)
#   define ACE_OBJ_SUFFIX ".obj"
# else
#   define ACE_OBJ_SUFFIX ".o"
#endif

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

  ACE_NEW_RETURN (hello,
                  Hello,
                  NULL);

  return hello;
}

typedef Hello *(*TC) (void);

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST ("DLL_Test");

  ACE_DLL ace_dll_obj;

  // *done*@@ Kirthika, the following code is incorrect since you're
  // trying to do a strcat() on a string literal...  Make
  // sure you ALWAYS run Purify on your code to find errors
  // list this.
  int retval = ace_dll_obj.open ("./DLL_Test"ACE_OBJ_SUFFIX);
  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ace_dll_obj.error()),
                      -1);

  TC f = (TC) ace_dll_obj.symbol ("get_hello");

  if (f == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ace_dll_obj.error()),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "callling get_hello\n"));

  auto_ptr <Hello> my_hello = f ();

  // Make the method calls, now that the object pointer is available.
  my_hello->say_hello ();
  my_hello->say_next ();

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr <Hello>;
template class ACE_Auto_Basic_Ptr <Hello>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <Hello>
#pragma instantiate ACE_Auto_Basic_Ptr <Hello>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
