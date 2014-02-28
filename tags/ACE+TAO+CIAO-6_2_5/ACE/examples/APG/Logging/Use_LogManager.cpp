// $Id$

#include "LogManager.h"

// Listing 1 code/ch03
void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  LOG_MANAGER->redirectToStderr ();
  ACE_TRACE ("main");
  LOG_MANAGER->redirectToSyslog ();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHi Mom\n")));
  foo ();
  LOG_MANAGER->redirectToDaemon ();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight\n")));

  return 0;
}
void foo (void)
{
  ACE_TRACE ("foo");
  LOG_MANAGER->redirectToFile ("output.test");
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHowdy Pardner\n")));
}
// Listing 1

// Listing 2 code/ch03
ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, LogManager, ACE_Null_Mutex);

// Listing 2
