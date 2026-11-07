#include "ace/Process_Semaphore.h"
#include "ace/Log_Msg.h"
#include <chrono>
#include <thread>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Process_Semaphore semaphore {0, ACE_TEXT ("AceTest")};
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starter has created and acquired semaphore\n")));
  std::this_thread::sleep_for (std::chrono::seconds {10});
  semaphore.release ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starter has released semaphore\n")));
  return 0;
}
