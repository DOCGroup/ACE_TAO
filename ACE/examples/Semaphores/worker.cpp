#include "ace/Process_Semaphore.h"
#include "ace/Log_Msg.h"
#include <chrono>
#include <thread>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Process_Semaphore semaphore {0, ACE_TEXT ("AceTest")};
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker is going to acquire semaphore\n")));
  semaphore.acquire ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker has acquired semaphore\n")));

  // Simulate initialization and work while preserving the semaphore example's timing.
  std::this_thread::sleep_for (std::chrono::seconds {10});
  semaphore.release ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Worker released semaphore\n")));
  std::this_thread::sleep_for (std::chrono::seconds {5});

  return 0;
}
