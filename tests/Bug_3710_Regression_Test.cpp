/**
 * @file Bug_3710_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3710
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3710
 */

#include "test_config.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_Memory.h"



// Main function.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3710_Regression_Test"));

  ACE_TCHAR out_string[64*1024+1];

  ACE_OS::memset(out_string,ACE_TEXT('@'),sizeof(out_string));
  //I only use ACE_MAXLOGMSGLEN char .
  out_string[ACE_MAXLOGMSGLEN] = ACE_TEXT('\0');
  ACE_DEBUG((LM_ALERT, ACE_TEXT("%s out_string: I love beijing."),out_string));

  ACE_END_TEST;
  return 0;
}

