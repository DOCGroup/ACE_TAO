// $Id$

#include "ace/Process_Semaphore.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Create locked
  ACE_Process_Semaphore s (0, ACE_TEXT ("AceTest"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker is going to acquire semaphore\n")));

  s.acquire ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker has acquired semaphore\n")));

  // Do some init stuff. Simulated by a sleep...
  ACE_OS::sleep(10);

  s.release();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker released semaphore\n")));

  // Do the work...
  ACE_OS::sleep(5);

  return 0;
}
