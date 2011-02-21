/**
 * @file Bug_1576_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 1576:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1576
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "test_config.h"
#include "ace/DLL.h"

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_1576_Regression_Test"));

  ACE_DLL dll;

  const ACE_TCHAR * dll_name = ACE_TEXT ("NOT_A_DLL") ACE_DLL_SUFFIX;

  // Normally applications should check the return value, but if they
  // ignore it...
  int result = dll.open (dll_name);

  if(result == -1)
  {
    // Use dll.error() is you want to get the error text, but we don't this in
    // this test because else the error is shown on the scoreboard
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Load failed, as expected\n")));
  }
  else
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT ("Success loading %s ? It should have failed!\n"),
               dll_name));
  }

  // ... and then use the DLL library, the program crashes (instead of
  // just getting an error ...
  void * symbol = dll.symbol (ACE_TEXT ("SHOULD_CRASH"));

  if(symbol == 0)
  {
    // Use dll.error() is you want to get the error text, but we don't this in
    // this test because else the error is shown on the scoreboard
    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("Symbol lookup failed, as expected\n")));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Found symbol ? It should have failed!\n")));
  }

  ACE_END_TEST;

  return 0;
}
