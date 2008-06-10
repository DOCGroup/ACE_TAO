// $Id$

#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/os_include/os_pthread.h"

int result = 0;

#if defined (__SUNPRO_CC)
extern "C"
#endif /* __SUNPRO_CC */
ACE_THR_FUNC_RETURN main_orb(void*)
{
  try
  {
    int argc = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (argc, 0);

    orb->destroy ();
  }
  catch(...)
  {
    result = 1;
    ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) Unknown error\n")));
  }

  return 0;
}

int ACE_TMAIN(int, ACE_TCHAR*[])
{
  ACE_hthread_t thread1;
# if defined (ACE_HAS_PTHREADS)
  pthread_create(&thread1, 0, main_orb, 0);
#else
  ACE_Thread::spawn (main_orb, 0, THR_NEW_LWP|THR_JOINABLE, 0, &thread1);
#endif
  ACE_Thread::join( thread1);

  return result;
}
