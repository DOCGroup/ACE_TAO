/**
 * @file Bug_2610_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2610:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2610
 *
 * @author Milan Cvetkovic <milan.cvetkovic@mpathix.com>
 */

#include "test_config.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Trace.h"
#include "ace/Thread_Semaphore.h"

#if defined (ACE_HAS_THREADS)

# define TEST_TRACE(X) ACE_Trace ____ (ACE_TEXT (X), __LINE__, ACE_TEXT (__FILE__))

int g_svc_handlers_leaked = 0;
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
    ++g_svc_handlers_leaked;
    reference_counting_policy().value(
      Reference_Counting_Policy::ENABLED);
  }

  ~My_Svc_Handler()
  {
    TEST_TRACE ("My_Svc_Handler::~My_Svc_Handler");
    --g_svc_handlers_leaked;
  }

  int handle_close (ACE_HANDLE /*fd*/, ACE_Reactor_Mask /*mask*/)
  {
    TEST_TRACE ("handle_close");
    g_semaphore.release();
    return 0;
  }
};

bool g_acceptor_accept_fails;

class My_Acceptor : public ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> super;
  My_Acceptor () : super (ACE_INET_Addr(9000)) {}

protected:
  int accept_svc_handler (My_Svc_Handler *svc_handler)
  {
    TEST_TRACE ("accept_svc_handler");
    int rv = super::accept_svc_handler(svc_handler);
    if (g_acceptor_accept_fails)
      {
        g_semaphore.release();
        return -1;
      }
    return rv;
  }
  int activate_svc_handler (My_Svc_Handler* /*svc_handler*/)
  {
    TEST_TRACE ("My_Acceptor::activate_svc_handler");
    g_semaphore.release();
    return -1;
  }
};

class My_Connector : public ACE_Connector<My_Svc_Handler, ACE_SOCK_CONNECTOR>
{
public:
  typedef ACE_Connector<My_Svc_Handler, ACE_SOCK_CONNECTOR> super;
protected:
  int activate_svc_handler (My_Svc_Handler* /*svc_handler*/)
  {
    TEST_TRACE ("My_Connector::activate_svc_handler");
    g_semaphore.release();
    return -1;
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
       ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("%p\n"),
                   ACE_TEXT ("Cannot run reactor event loop")));
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

#endif

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2610_Regression_Test"));

#if defined (ACE_HAS_THREADS)

  My_Acceptor acceptor;
  Timer_Handler timer_handler;
  ACE_Reactor::instance()->schedule_timer(
    &timer_handler, 0, ACE_Time_Value(0));

  My_Task task;
  int activated = task.activate();
  if (activated < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Could not activate task")),
                        -1);
    }

  // Don't assume addr family of the listener - check and adapt when needed.
  ACE_INET_Addr listen_addr;
  acceptor.acceptor ().get_local_addr (listen_addr);
#if defined (ACE_HAS_IPV6)
  const ACE_TCHAR *me =
    listen_addr.get_type () == PF_INET ? ACE_LOCALHOST : ACE_IPV6_LOCALHOST;
#else
  const ACE_TCHAR *me = ACE_LOCALHOST;
#endif /* ACE_HAS_IPV6 */
  ACE_INET_Addr a1 (listen_addr.get_port_number (),
                    me,
                    listen_addr.get_type ());
  ACE_SOCK_Connector c1;
  g_semaphore.acquire();// wait for reactor to start

  {
    g_acceptor_accept_fails = true;
    ACE_SOCK_Stream s1;
    if (-1 == c1.connect (s1, a1))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Could not connect")),
                          -1);
      }
    g_semaphore.acquire(); // wait for accept_svc_handler() to start
  }
  {
    g_acceptor_accept_fails = false;
    ACE_SOCK_Stream s1;
    if (-1 == c1.connect (s1, a1))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Could not connect")),
                          -1);
      }
    g_semaphore.acquire(); // wait for activate_svc_handler to complete
  }

  {
    My_Svc_Handler* handler=0;
    My_Connector connector;
    connector.connect (handler, a1);
    ACE_Event_Handler_var safe_handler (handler);
    g_semaphore.acquire(); // wait for connect to complete
  }
  ACE_Reactor::end_event_loop();
  task.wait();

  if (g_svc_handlers_leaked != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Svc_Handler leakage detected, ")
                         ACE_TEXT ("%d objects remain\n"),
                         g_svc_handlers_leaked),
                        1);
    }
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif

  ACE_END_TEST;

  return 0;
}
