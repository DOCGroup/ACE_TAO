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

#define ACE_BUILD_SVC_DLL

#include "test_config.h" 
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(tests, DLL_Test, "$Id$")

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// Considering UNIX OS to be default. On Win32 platforms, the symbols
// are got form the .exe as one cant have .exe and .dll for the same
// .cpp. Also, on Win32 platforms one cant use the .obj to obtain
// symbols dynamically at runtime.

#if defined (ACE_WIN32)
#  define OBJ_SUFFIX ".exe"
#  define OBJ_PREFIX ""
#else
#  define OBJ_SUFFIX ACE_DLL_SUFFIX
#  define OBJ_PREFIX "./" ACE_DLL_PREFIX
#endif /*ACE_WIN32*/

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

extern "C" ACE_Svc_Export Hello *get_hello (void);

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

// Protection against this test being run on platforms not supporting Dlls.
#if defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
    defined (__hpux) 
      
      ACE_DLL dll;
    
    int retval = dll.open (OBJ_PREFIX "DLL_Test" OBJ_SUFFIX);
  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
		       dll.error ()),
                      -1);
  
  TC f = (TC) dll.symbol ("get_hello");
  
  if (f == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
		       dll.error ()),
                      -1);
  
  auto_ptr<Hello> my_hello (f ());
  
  // Make the method calls, as the object pointer is available.
  my_hello->say_hello ();
  my_hello->say_next ();
  
#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("Dynamically Linkable Libraries not supported on this platform\n")));
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
