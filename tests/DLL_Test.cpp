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
#include "ace/SString.h"
#include "DLL_Test.h"



#if defined (ACE_LD_DECORATOR_STR)
# define OBJ_SUFFIX ACE_LD_DECORATOR_STR ACE_DLL_SUFFIX
#else
# define OBJ_SUFFIX ACE_DLL_SUFFIX
#endif /* ACE_LD_DECORATOR_STR */

#if defined (ACE_WIN32) || defined (ACE_OPENVMS)
#  define OBJ_PREFIX ACE_DLL_PREFIX
#else
#  define OBJ_PREFIX ACE_TEXT("./") ACE_DLL_PREFIX
#endif /* ACE_WIN32 */

// Declare the type of the symbol:
typedef Hello *(*Hello_Factory)(void);

typedef int ( *PFN )( Parent* );

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

  ACE_TString dll_file;
  const char *subdir_env = ACE_OS::getenv ("ACE_EXE_SUB_DIR");
  if (subdir_env)
    {
      dll_file = ACE_TEXT_CHAR_TO_TCHAR (subdir_env);
      dll_file += ACE_DIRECTORY_SEPARATOR_STR;
    }

  dll_file += OBJ_PREFIX ACE_TEXT ("DLL_Test_Lib") OBJ_SUFFIX;

  int retval = dll.open (dll_file.c_str());

  if (retval != 0)
    {
      ACE_TCHAR *dll_error = dll.error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error in DLL Open of <%s>: %s\n"),
                         OBJ_PREFIX ACE_TEXT ("DLL_Test_Lib") OBJ_SUFFIX,
                         dll_error ? dll_error : ACE_TEXT ("unknown error")),
                        -1);
    }

  // Just because the ANSI C++ spec says you can no longer cast a
  // void* to a function pointer. Doesn't allow:
  // TC f = (Hello_Factory) dll.symbol ("get_hello");
  void *foo = dll.symbol (ACE_TEXT ("get_hello"));

  // Cast the void* to long first.
  ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (foo);
  Hello_Factory factory = reinterpret_cast<Hello_Factory> (tmp);
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
  ACE_OS::free (malloc_str);

  return 0;
}

int dynamic_cast_test (ACE_DLL &dll)
{
  Child child;
  child.test();

  Parent *parent = &child;

  void * foo = dll.symbol (ACE_TEXT ("dynamic_cast_test"));

  // Cast the void* to long first.
  ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (foo);
  PFN pfnAcquire = reinterpret_cast<PFN> (tmp);
  if (pfnAcquire == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       dll.error ()),
                      -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("before %@ %@\n"),
              &child,  dynamic_cast<Child*> (parent)));

  if (pfnAcquire (&child) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("dynamic_cast failed.\n")), -1);

  return 0;
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("DLL_Test"));

  int retval = 0;

// Protection against this test being run on platforms not supporting Dlls.
#if defined(ACE_HAS_DYNAMIC_LINKING)

  ACE_DLL dll;

  retval += basic_test (dll);

  retval += dynamic_cast_test (dll);

  retval += handle_test (dll);

  // Call close here so that any errors make it into the log.
  dll.close ();

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dynamically Linkable Libraries not supported on this platform\n")));
#endif /* ACE_HAS_DYNAMIC_LINKING */

  ACE_END_TEST;
  return retval == 0 ? 0 : 1;
}

