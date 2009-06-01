/**
 * @file Bug_2609_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2609:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2609
 *
 * @author Milan Cvetkovic <milan.cvetkovic@mpathix.com>
 */

#include "test_config.h"

ACE_RCSID (tests,
           Bug_2609_Regression_Test,
           "$Id$")

#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Trace.h"
#include "ace/SOCK_Connector.h"
#include "ace/Thread_Semaphore.h"

#if defined (ACE_HAS_THREADS)

# define TEST_TRACE(X) ACE_Trace ____ (ACE_TEXT (X), __LINE__, ACE_TEXT (__FILE__))

bool g_handler_deleted = false;
// use semaphore for synchronization
//
ACE_Thread_Semaphore g_semaphore (0);

class My_Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH>
{
public:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH> super;
  My_Svc_Handler()
  {
    TEST_TRACE ("My_Svc_Handler:My_Svc_Handler");
    g_handler_deleted = false;
    reference_counting_policy().value(
      Reference_Counting_Policy::ENABLED);
  }

  ~My_Svc_Handler()
  {
    TEST_TRACE ("My_Svc_Handler::~My_Svc_Handler");
    g_handler_deleted = true;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void* pv)
  {
    TEST_TRACE ("open");
    g_semaphore.release(); // signal open completed
    return super::open (pv);
  }
  //FUZZ: enable check_for_lack_ACE_OS

  int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask mask)
  {
    TEST_TRACE ("handle_close");
    super::handle_close (fd, mask);
    if (g_handler_deleted)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Handler deleted in base class' handle_close()\n")));
    }
    // signal handle_close() completed
    g_semaphore.release();
    return 0;
  }
};

struct My_Task : public ACE_Task_Base
{
   int svc()
   {
     TEST_TRACE ("My_Task::svc");
     ACE_Reactor::instance()->owner(ACE_OS::thr_self());
     int rv = ACE_Reactor::instance()->run_reactor_event_loop();
     if (rv < 0)
     {
       ACE_ERROR ((LM_ERROR, ACE_TEXT("Cannot run reactor event loop\n")));
     }
     return 0;
   }
};

// event handler used to signal when the reactor started
struct Timer_Handler : public ACE_Event_Handler
{
   int handle_timeout (const ACE_Time_Value&, const void*)
   {
     g_semaphore.release(); // signal reactor started
     return 0;
   }
};

typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> My_Acceptor;

#endif

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2609_Regression_Test"));

#if defined (ACE_HAS_THREADS)

  My_Acceptor acceptor (ACE_INET_Addr(9876));
  Timer_Handler timer_handler;
  ACE_Reactor::instance()->schedule_timer(
    &timer_handler, 0, ACE_Time_Value(0));

  My_Task task;
  int activated = task.activate();
  if (activated < 0)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could not activate task\n")));
  }

  g_semaphore.acquire();// wait for reactor to start
  {
    ACE_SOCK_Connector c1;
    ACE_SOCK_Stream s1;
    ACE_INET_Addr a1(9876, "localhost");
    if (-1 == c1.connect (s1, a1))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("Could not connect\n")), -1);
    }
    g_semaphore.acquire(); // wait for open to complete
    s1.close();
    g_semaphore.acquire(); // wait for handle_close to complete
  }
  ACE_Reactor::end_event_loop();
  task.wait();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif
  ACE_END_TEST;

  return 0;
}
