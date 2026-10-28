#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/os_include/os_pthread.h"

int result = 0;

ACE_THR_FUNC_RETURN
main_orb (void *)
{
  try
    {
      int argc = 0;
      ACE_TCHAR **argv = nullptr;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      orb->destroy ();
    }
  catch (...)
    {
      result = 1;
      ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) Unknown error\n")));
    }

  return ACE_THR_FUNC_RETURN_NULL;
}

int
ACE_TMAIN (int, ACE_TCHAR * [])
{
  ACE_hthread_t thread1;
# if defined (ACE_HAS_PTHREADS)
  pthread_create (&thread1, nullptr, main_orb, nullptr);
#else
  ACE_Thread::spawn (main_orb, nullptr, THR_NEW_LWP | THR_JOINABLE, nullptr, &thread1);
#endif
  ACE_Thread::join (thread1);

  return result;
}
