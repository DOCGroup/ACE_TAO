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

#include "test_config.h"
#include "DLL_Test.h"
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(tests, DLL_Test, "$Id$")

#define OBJ_SUFFIX ACE_DLL_SUFFIX
#define OBJ_PREFIX "./" ACE_DLL_PREFIX

ACE_TCHAR const *
cdecl_decoration (ACE_TCHAR const *func_name)
{
#if defined (__BORLANDC__)
  static ACE_TCHAR decorated_func_name[10*1024];
  ACE_OS::sprintf (decorated_func_name,
                   ACE_TEXT ("_%s"),
                   func_name);
  return decorated_func_name;
#else
  return func_name;
#endif /* __BORLANDC__ */
}

// Declare the type of the symbol:
typedef Hello *(*Hello_Factory)(void);

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("DLL_Test"));

// Protection against this test being run on platforms not supporting Dlls.
#if defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
    defined (__hpux)

  ACE_DLL dll;
  int retval = dll.open (OBJ_PREFIX
                         ACE_TEXT ("DLL_Testd")
                         OBJ_SUFFIX);
  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error in DLL Open\n")),
                      -1);

  // Just because the ANSI C++ spec says you can no longer cast a
  // void* to a function pointer. Doesn't allow:
  // TC f = (Hello_Factory) dll.symbol ("get_hello");
  void *foo;

  ACE_TCHAR const *cdecl_str = cdecl_decoration (ACE_TEXT ("get_hello"));
  foo = dll.symbol (cdecl_str);

  // Cast the void* to long first.
  long tmp = ACE_reinterpret_cast (long, foo);
  Hello_Factory factory = ACE_reinterpret_cast (Hello_Factory, tmp);
  if (factory == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
		       dll.error ()),
                      -1);

  auto_ptr<Hello> my_hello (factory ());

  // Make the method calls, as the object pointer is available.
  my_hello->say_hello ();
  my_hello->say_next ();

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dynamically Linkable Libraries not supported on this platform\n")));
#endif /* ACE_WIN32 || ACE_HAS_SVR4_DYNAMIC_LINKING || __hpux */

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
