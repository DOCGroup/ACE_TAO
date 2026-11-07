#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/os_include/os_pthread.h"

#include "tao/AnyTypeCode/Any.h"
#include "tao/Policy_Manager.h"
#include "tao/TimeBaseC.h"

ACE_THR_FUNC_RETURN
main_orb (void *arg)
{
  // main owns this result until it joins the thread.
  int *const result = static_cast<int *> (arg);
  try
    {
      int argc {};
      ACE_TCHAR **argv {};
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      TimeBase::TimeT const connection_timeout = 1000000;
      TimeBase::TimeT const round_trip_timeout = 1000000;

      if (!CORBA::is_nil (orb.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "ORB not nil\n"));

      CORBA::Any connection_timeout_as_any;
          connection_timeout_as_any <<= connection_timeout;

          CORBA::Any rt_timeout_as_any;
          rt_timeout_as_any <<= round_trip_timeout;

          orb->destroy ();
          ACE_DEBUG ((LM_DEBUG, "Destroyed ORB\n"));
        }
    }
  catch (::CORBA::Exception const &ex)
    {
      *result = 1;
      ex._tao_print_exception ("Exception caught:");
    }
  catch (...)
    {
      *result = 1;
      ACE_ERROR ((LM_ERROR, " (%P|%t) Unknown error\n"));
    }

  return ACE_THR_FUNC_RETURN_NULL;
}


int
ACE_TMAIN (int, ACE_TCHAR * [])
{
  int result {};
  ACE_hthread_t thread1 {};
  int status {};
# if defined (ACE_HAS_PTHREADS)
  status = pthread_create (&thread1, nullptr, main_orb, &result);
#else
  status = ACE_Thread::spawn (main_orb, &result, THR_NEW_LWP | THR_JOINABLE, nullptr, &thread1);
#endif
  if (status != 0)
    return 1;

  if (ACE_Thread::join (thread1) != 0)
    return 1;

  return result;
}
