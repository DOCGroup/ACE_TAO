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

#if defined (ACE_WIN32) && defined (_MSC_VER) && defined (_DEBUG)
# define OBJ_SUFFIX ACE_TEXT ("d") ACE_DLL_SUFFIX
#else /* ACE_WIN32 && _MSC_VER && _DEBUG */
# define OBJ_SUFFIX ACE_DLL_SUFFIX
#endif /* ACE_WIN32 && && _MSC_VER && _DEBUG */

#if defined (ACE_WIN32)
#  define OBJ_PREFIX ACE_DLL_PREFIX
#else
#  define OBJ_PREFIX ACE_TEXT("./") ACE_DLL_PREFIX
#endif /* ACE_WIN32 */

// Declare the type of the symbol:
typedef Hello *(*Hello_Factory)(void);

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("DLL_Test"));

// Protection against this test being run on platforms not supporting Dlls.
#if defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
    defined (__hpux)

  ACE_DLL dll;

#if defined (__KCC)
  /* With KCC, turning on close-on-destruction will cause problems
     when libKCC tries to call dtors. */
  int retval = dll.open (ACE_TEXT (OBJ_PREFIX)
                         ACE_TEXT ("DLL_Test")
                         ACE_TEXT (OBJ_SUFFIX),
                         ACE_DEFAULT_SHLIB_MODE,
                         0);
#else
  int retval = dll.open (OBJ_PREFIX
                         ACE_TEXT ("DLL_Test")
                         OBJ_SUFFIX);
#endif /* __KCC */

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error in DLL Open\n")),
                      -1);

  // Just because the ANSI C++ spec says you can no longer cast a
  // void* to a function pointer. Doesn't allow:
  // TC f = (Hello_Factory) dll.symbol ("get_hello");
  void *foo;

  ACE_TCHAR *cdecl_str = ACE::ldname (ACE_TEXT ("get_hello"));
  foo = dll.symbol (cdecl_str);
  delete[] cdecl_str;

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
