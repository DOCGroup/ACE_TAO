// $Id$

#include "ace/Process_Semaphore.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  // Create locked
  ACE_Process_Semaphore s (0, ACE_TEXT("AceTest"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starter has created and acquired semaphore\n")));

  // Do some init stuff. Simulated by a sleep...
  ACE_OS::sleep(10);

  s.release();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starter has released semaphore\n")));

  return 0;
}
