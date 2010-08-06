/**
 * @file Bug_3500_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3500:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3500
 *
 * @author Bill Rizzi <rizzi@softserv.com>
 */

#include "ace/OS_NS_sys_mman.h"
#include "ace/SString.h"
#include "tests/test_config.h"

ACE_RCSID (tests,
           Bug_3500_Regression_Test,
           "$Id$")

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3500_Regression_Test"));

  int ret = 0;

#if defined(ACE_WIN32)
  ACE_HANDLE  handle = ACE_INVALID_HANDLE;
  ACE_TString name(ACE_TEXT ("Bug3500"));

  void *mmap =
    ACE_OS::mmap(0,                   // addr
                 28,                 // len
                 PAGE_READWRITE,      // prot
                 MAP_SHARED,          // flags
                 ACE_INVALID_HANDLE,  // file_handle
                 0,                   // off
                 &handle,             // file_mapping
                 0,                   // sa
                 name.c_str());       // shared memory name

  if (mmap == MAP_FAILED)
    {
      ret = -1;
    }
#endif

  if (0 != ret)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ACE_OS::mmap() failed\n")));
  }

  ACE_END_TEST;

  return 0;
}
