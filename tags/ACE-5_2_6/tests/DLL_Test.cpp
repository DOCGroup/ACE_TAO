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
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"
#include "ace/ACE.h"
#include "ace/DLL_Manager.h"
#include "DLL_Test.h"

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

int handle_test (ACE_DLL &dll)
{
  // Test the get/set_handle methods.
  ACE_DLL local_dll;

  ACE_SHLIB_HANDLE handle = dll.get_handle (1);
  if (handle != ACE_SHLIB_INVALID_HANDLE)
    {
      if (local_dll.set_handle (handle) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Error setting handle.\n")),
                          -1);
      return 0;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Error getting handle.\n")),
                    -1);
}

int basic_test (ACE_DLL &dll)
{
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
    {
      ACE_TCHAR *dll_error = dll.error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error in DLL Open: %s\n"), 
                         dll_error ? dll_error : ACE_TEXT ("unknown error")),
                        -1);
    }

  // Just because the ANSI C++ spec says you can no longer cast a
  // void* to a function pointer. Doesn't allow:
  // TC f = (Hello_Factory) dll.symbol ("get_hello");
  void *foo;

  foo = dll.symbol (ACE_TEXT ("get_hello"));

  // Cast the void* to long first.
  ptrdiff_t tmp = ACE_reinterpret_cast (ptrdiff_t, foo);
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

  // Allocate and delete a string allocated via new in a different dll.
  ACE_TCHAR *new_str = my_hello->new_info ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Result for new_info(): %s\n"), new_str));
  ACE::strdelete (new_str);

  // Allocate and free a string allocated via malloc in a different dll.
  ACE_TCHAR *malloc_str = my_hello->malloc_info ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Result for malloc_info(): %s\n"), malloc_str));
  ACE_OS_Memory::free (malloc_str);

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("DLL_Test"));

  int retval = 0;

// Protection against this test being run on platforms not supporting Dlls.
#if defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
    defined (__hpux)

  ACE_DLL dll;

  retval += basic_test (dll);

  retval += handle_test (dll);

  // Call close here so that any errors make it into the log.
  dll.close ();

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dynamically Linkable Libraries not supported on this platform\n")));
#endif /* ACE_WIN32 || ACE_HAS_SVR4_DYNAMIC_LINKING || __hpux */

  ACE_END_TEST;
  return retval == 0 ? 0 : 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr <Hello>;
template class ACE_Auto_Basic_Ptr <Hello>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <Hello>
#pragma instantiate ACE_Auto_Basic_Ptr <Hello>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
