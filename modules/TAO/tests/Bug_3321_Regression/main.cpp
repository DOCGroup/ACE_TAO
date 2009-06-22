// $Id$

#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/os_include/os_pthread.h"

#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Policy_Manager.h"
#include "tao/TimeBaseC.h"

int result = 0;

#if defined (__SUNPRO_CC)
extern "C"
#endif /* __SUNPRO_CC */
ACE_THR_FUNC_RETURN main_orb(void*)
{
  try
  {
    int argc = 0;
    ACE_TCHAR **argv = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    TimeBase::TimeT CONNECTION_TIMEOUT = 1000000;
    TimeBase::TimeT ROUND_TRIP_TIMEOUT = 1000000;

    if (!CORBA::is_nil(orb.in()))
      {
        ACE_DEBUG((LM_DEBUG, "ORB not nil\n"));

        CORBA::Any connection_timeout_as_any;
        connection_timeout_as_any <<= CONNECTION_TIMEOUT;

        CORBA::Any rt_timeout_as_any;
        rt_timeout_as_any <<= ROUND_TRIP_TIMEOUT;

        orb->destroy ();
        ACE_DEBUG((LM_DEBUG, "Destroyed ORB\n"));
      }
  }
  catch(const ::CORBA::Exception& ex)
  {
    result = 1;
    ex._tao_print_exception ("Exception caught:");
  }
  catch(...)
  {
    result = 1;
    ACE_ERROR ((LM_ERROR, " (%P|%t) Unknown error\n"));
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
