// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Exceptions_Test.cpp
//
// = DESCRIPTION
//      This is a test that makes sure the <ACE_Reactor> works
//      correctly in the face of C++ exceptions and threads.
//
// = AUTHOR
//    Luca Priorelli <lucapri@mbox.vol.it> and Douglas C. Schmidt
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

ACE_RCSID(tests, Reactor_Exceptions_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_EXCEPTIONS)

#if defined (ACE_WIN32)
static void
throw_exception (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) throw exception\n")));

  // Cause a Win32 structured exception.
  *(char *) 0 = 0;
}
#else
// Just need a simple exception class.
class Except {};

static void
throw_exception (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) throw exception\n")));
  throw Except ();
}
#endif /* ACE_WIN32 */

class My_Handler : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
public:
  My_Handler (const ACE_INET_Addr &local_addr);

  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
};

My_Handler::My_Handler (const ACE_INET_Addr &local_addr)
  : ACE_SOCK_Dgram (local_addr)
{
}

ACE_HANDLE
My_Handler::get_handle (void) const
{
  return ACE_SOCK_Dgram::get_handle ();
}

int
My_Handler::handle_input (ACE_HANDLE)
{
  ASYS_TCHAR buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Activity occurred on handle %d!\n"),
              ACE_SOCK_Dgram::get_handle ()));

  ssize_t n = ACE_SOCK_Dgram::recv (buf,
                                    sizeof buf,
                                    from_addr);
  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("handle_input")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("got buf = %s\n"),
                buf));

  throw_exception ();
  return 0;
}

class My_Reactor : public ACE_Reactor
{
public:
  virtual int handle_events (ACE_Time_Value *max_wait_time)
  {
    int ret = 0;
    try
      {
        ret = ACE_Reactor::handle_events (max_wait_time);
      }
    catch (...)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT (" (%t) catch exception\n")));
        ret = -1;
        // do your thing, etc.
      }
    return ret;
  }

  virtual int handle_events (ACE_Time_Value &max_wait_time)
  {
    return this->handle_events (&max_wait_time);
  }
};

static int
worker (void)
{
  ACE_Reactor::instance ()->owner (ACE_OS::thr_self ());

  for (;;)
    if (ACE_Reactor::instance ()->handle_events () == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT (" (%t) exception return\n")),
                        0);

  ACE_NOTREACHED (return 0);
}
#endif /* ACE_HAS_EXCEPTIONS */

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Exceptions_Test"));

#if defined (ACE_HAS_EXCEPTIONS)
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Starting tracing\n")));

  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  ACE_INET_Addr local_addr (port);
  ACE_INET_Addr remote_addr (port,
                             ACE_DEFAULT_SERVER_HOST);
  // Put the <handler> before the <reactor> so that they'll be cleaned
  // up in the proper order.
  My_Handler handler (local_addr);

  My_Reactor reactor;

  ACE_Reactor::instance (&reactor);
  ACE_Thread_Manager *thr_mgr =
    ACE_Thread_Manager::instance ();

  if (ACE_Reactor::instance ()->register_handler
      (&handler,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("register_handler")),
                      -1);

#if defined (ACE_HAS_THREADS)
  thr_mgr->spawn (ACE_THR_FUNC (worker));
#else
  // Need to figure out how to implement this test.
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_SOCK_Dgram dgram (ACE_sap_any_cast (ACE_INET_Addr &));

  for (size_t i = 0; i < ACE_MAX_ITERATIONS; i++)
    dgram.send (ASYS_TEXT ("Hello"),
                sizeof (ASYS_TEXT ("Hello")),
                remote_addr);
  // Barrier to wait for the other thread to return.
  thr_mgr->wait ();

  handler.close ();
  dgram.close ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT (" (%t) exiting main\n")));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("C++ exception support not enabled on this platform\n")));
#endif /* ACE_HAS_EXCEPTIONS */

  ACE_END_TEST;
  return 0;
}
