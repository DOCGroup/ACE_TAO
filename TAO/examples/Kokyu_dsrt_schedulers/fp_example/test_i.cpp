// $Id$

#include "test_i.h"
#include "ace/High_Res_Timer.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, test_i, "$Id$")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;
  long server_load = x;

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;
  
  ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));
  
  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
	{
	  ACE_DEBUG((LM_DEBUG,
		     ACE_TEXT ("getprio not supported on this platform\n")
		     ));
	  return 0;
	}
      ACE_ERROR_RETURN ((LM_ERROR,
			 ACE_TEXT ("%p\n"),
			 ACE_TEXT ("getprio failed")),
			-1);
    }

  //  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG, "Request in thread %t, prio = %d, load = %u\n", prio, server_load));

  static CORBA::ULong prime_number = 9619899;

  timer.start ();
  for (long j = 0; j < server_load; ++j)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) loop # = %d, load = %u\n", j, server_load)); 
      ACE::is_prime (prime_number,
		     2,
		     prime_number / 2);
    }
  timer.stop ();
  timer.elapsed_time (elapsed_time);
  
  ACE_DEBUG ((LM_DEBUG, "Request processing in thread %t done, prio = %d, load = %d, elapsed time = %u\n", prio, x, elapsed_time.msec () ));
  
  return x;
}

void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "shutdown request from client\n"));
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
