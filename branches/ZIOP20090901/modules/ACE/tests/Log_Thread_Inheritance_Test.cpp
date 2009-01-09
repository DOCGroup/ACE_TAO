// $Id$

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Task.h"
#include <fstream>

ACE_RCSID (tests, Log_Thread_Inheritance_Test, "$Id$")

#if !defined (ACE_HAS_THREADS) || defined (ACE_LACKS_IOSTREAM_TOTALLY)
int run_main (int, ACE_TCHAR *[])
{
  return 0;
}
#else

struct MyThread : ACE_Task_Base
{

  enum { THREAD_DEFAULTS = THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED };

  explicit MyThread (bool openfile = false)
    : openfile_ (openfile) {}

  bool openfile_;
  static MyThread childthread_;

  int svc ()
  {
    if (openfile_)
      {
        ACE_LOG_MSG->msg_ostream (
          new std::ofstream (
            ACE_TEXT_ALWAYS_CHAR (
              ACE_LOG_DIRECTORY
              ACE_TEXT ("Log_Thread_Inheritance_Ostream")
              ACE_LOG_FILE_EXT_NAME
            )
          ), true);
        ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
        ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
        MyThread ends_first_thread;
        ends_first_thread.activate (THREAD_DEFAULTS, 10);
        ends_first_thread.wait ();
        childthread_.activate (THREAD_DEFAULTS, 10);
      }
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) - in svc() openfile_ is %C\n",
                (openfile_ ? "true" : "false")));
    if (!openfile_)
      {
        for (int i = 0; i < 100; ++i)
          {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) - loop %d\n", i));
            if (!(i % 10)) ACE_OS::thr_yield ();
          }
      }
    return 0;
  }
};

MyThread MyThread::childthread_;

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Log_Thread_Inheritance_Test"));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - in run_main()\n"));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - this test will crash ACE if it does not "
              "have the fix for bug #3480.\n"));
  ACE_OSTREAM_TYPE *initial_stream = ACE_LOG_MSG->msg_ostream ();
  MyThread mt (true);
  mt.activate ();
  mt.wait ();
  MyThread::childthread_.wait ();
  ACE_LOG_MSG->msg_ostream (initial_stream, 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Test passed.\n"));
  ACE_END_TEST;
  return 0;
}

#endif
