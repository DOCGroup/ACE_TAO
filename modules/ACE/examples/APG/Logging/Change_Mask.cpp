// $Id$

#include "ace/Log_Msg.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("main");

  // Listing 1 code/ch03
  ACE_LOG_MSG->priority_mask (0, ACE_Log_Msg::PROCESS);
  ACE_LOG_MSG->priority_mask (LM_DEBUG | LM_NOTICE,
                              ACE_Log_Msg::THREAD);
  // Listing 1

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHi Mom\n")));
  foo ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IGoodnight\n")));

  return 0;
}

void foo (void)
{
  ACE_TRACE ("foo");

  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("%IHowdy Pardner\n")));
}
