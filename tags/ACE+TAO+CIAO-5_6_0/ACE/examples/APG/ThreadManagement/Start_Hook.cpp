// $Id$

#include "ace/Thread_Hook.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"

#include "SecurityContext.h"

// Listing 1 code/ch13
class HA_ThreadHook : public ACE_Thread_Hook
{
public:
  virtual ACE_THR_FUNC_RETURN start (ACE_THR_FUNC func, void* arg)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%t) New Thread Spawned\n")));

    // Create the context on the thread's own stack.
    ACE_TSS<SecurityContext> secCtx;
    // Special initialization.
    add_sec_context_thr (secCtx);

    return (*func) (arg);
  }

  void add_sec_context_thr (ACE_TSS<SecurityContext> &secCtx);
};
// Listing 1

void
HA_ThreadHook::add_sec_context_thr(ACE_TSS<SecurityContext> &secCtx)
{
  secCtx->user = 0;
}


class HA_CommandHandler : public ACE_Task_Base
{
public:
  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up \n")));

    // Do something.

    return 0;
  }
};
// Listing 2 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_ThreadHook hook;
  ACE_Thread_Hook::thread_hook (&hook);

  HA_CommandHandler handler;
  handler.activate ();
  handler.wait();
  return 0;
}
// Listing 2

