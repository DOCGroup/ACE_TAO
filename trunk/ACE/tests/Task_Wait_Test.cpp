//=============================================================================
/**
 *  @file    Task_Wait_Test.cpp
 *
 *  $Id$
 *
 *    This program tests the ACE_Thread_Manager::wait() from a called-back
 *    ACE_Task::close() on thread exit.
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Event.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

// On Windows, this test will deadlock because a thread tries to join with
// itself. Pthreads, et al, have deadlock protection in join; Windows Vista
// and Server 2003 have API to access the info needed to check for deadlock
// in ACE_OS::thr_join() - don't run this test on earlier Windows versions
// because it will hang/time out and there's little to be done about it.
#if defined (ACE_HAS_THREADS) && ((defined (_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502)) || !defined (WIN32))
#  define RUN_THIS_TEST
#endif

#ifdef RUN_THIS_TEST

static ACE_Event TaskDone;

// Define a ACE_Task that does little except exit and try to clean up.

class Do_Nothing_Task : public ACE_Task_Base {
public:
  virtual int close (u_long flags = 0);

  virtual int svc (void);
};

// close tries to wait for other threads. There aren't any, but as long as
// we don't deadlock, it's good.
int
Do_Nothing_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Task 0x%x, thread %t closing\n"),
              this));
  this->wait ();
  TaskDone.signal ();
  return 0;
}


// svc just waits a second then exits.
int
Do_Nothing_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Task 0x%x, thread %t waiting to exit\n"),
              this));
  ACE_OS::sleep (1);

  return 0;
}

#endif /* RUN_THIS_TEST */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Task_Wait_Test"));
  int status = 0;

#if defined (RUN_THIS_TEST)

  Do_Nothing_Task t;
  status = t.activate ();
  TaskDone.wait();

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("inadequate thread support on this platform\n")));
#endif /* RUN_THIS_TEST */

  ACE_END_TEST;
  return status;
}
