/**
 * @file
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

ACE_RCSID (tests,
           Bug_1576_Regression_Test,
           "$Id$")

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_1576_Regression_Test"));

  ACE_DLL dll;

  char const dll_name[] = "NOT_A_DLL.so";

  // Normally applications should check the return value, but if they
  // ignore it...
  int result = dll.open (dll_name);

  if(result == -1)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Load failed, as expected (%s)\n",
                dll.error ()));
  }
  else
  {
    ACE_ERROR((LM_ERROR,
               "Success loading %s ? It should have failed!\n",
               dll_name));
  }

  // ... and then use the DLL library, the program crashes (instead of
  // just getting an error ...
  void * symbol = dll.symbol ("SHOULD_CRASH");

  if(symbol == 0)
  {
    ACE_DEBUG((LM_DEBUG,
               "Symbol lookup failed, as expected (%s)\n",
               dll.error ()));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                "Found symbol ? It should have failed!\n"));
  }

  ACE_END_TEST;

  return 0;
}
