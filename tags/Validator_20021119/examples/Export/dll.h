// $Id$

// To use the export macros with a DLL, a file will need to be
// created (see ACE_wrapper/bin/generate_export_file.pl) and
// included.  This file defines Test_Export (and the
// TEST_SINGLETON_* macros).
#include "test_export.h"

#include "ace/Singleton.h"

#define RETVAL 42

// To expose a function outside of a DLL, use the *_Export
// at the beginning of the function declaration.

Test_Export int test_function ();

// To expose data, put use the *Export at the beginning
// of the variable declaration.

Test_Export int test_variable;

// To expose a class, put the *_Export between "class"
// and the class name.

class Test_Export test_class
{
public:
  int method ();
};

// ACE_Singleton and its relatives are special cases.  The problem is
// that ACE_Singleton is a template.  If the singleton is used in both
// the DLL and the executable linking the DLL, then two instances of
// the singleton will be used (which defeats the purpose of a Singleton).
//
// This occurs because the ACE_Singleton template is expanded in both
// places because Visual C++ and Borland C++ do this automatically by
// including the template source.  This in turn creates two copies of
// the static member variable.
//
// So to get around this problem, the *_SINGLETON_DECLARE macro is 
// used to instruct the compiler to not create the second copy in the
// program.  This macro solution does not work for Borland C++, so for
// this compiler you must explicitly disable the template instantiation
// using a #pragma (or use the other workaround below).
//
// Another workaround for this is to not to expose the Singleton itself
// to the outside world, but to instead supply a function or static
// member function that returns the singleton to the executable
// (like get_dll_singleton () does below).

#if defined (__BORLANDC__)
# if !defined (TEST_BUILD_DLL)
#   pragma option push -Jgx
# endif
#endif
typedef ACE_Singleton<test_class, ACE_Null_Mutex> TEST_SINGLETON;
TEST_SINGLETON_DECLARE (ACE_Singleton, test_class, ACE_Null_Mutex);
#if defined (__BORLANDC__)
# if !defined (TEST_BUILD_DLL)
#   pragma option pop
# endif
#endif

Test_Export test_class *get_dll_singleton ();
