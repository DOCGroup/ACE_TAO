// $Id$

#include "ace/Log_Msg.h"

void foo (void);

// Listing 1 code/ch03
int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  // open() requires the name of the application
  // (e.g. -- argv[0]) because the underlying
  // implementation may use it in the log output.
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR);
  // Listing 1

  /* Alternatively, you can use the set_flags() method to do the same
     thing after the singleton has been created:
  */

  ACE_TRACE ("main");

  // Listing 2 code/ch03
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IHi Mom\n")));
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
  foo ();
  // Listing 2

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight\n")));

  return 0;
}

void foo (void)
{
  ACE_TRACE ("foo");

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHowdy Pardner\n")));
}
