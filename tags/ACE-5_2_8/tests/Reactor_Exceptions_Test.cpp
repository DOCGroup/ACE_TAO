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
//    Luca Priorelli <lucapri@mbox.vol.it> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

ACE_RCSID(tests, Reactor_Exceptions_Test, "$Id$")

#if defined (ACE_HAS_EXCEPTIONS)

#if defined (ACE_WIN32) && defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
static void
throw_exception (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) throw exception\n")));

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
              ACE_TEXT ("(%P|%t) throw exception\n")));
  throw Except ();
}
#endif /* ACE_WIN32 && ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

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
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Activity occurred on handle %d!\n"),
              ACE_SOCK_Dgram::get_handle ()));

  ssize_t n = ACE_SOCK_Dgram::recv (buf,
                                    sizeof buf,
                                    from_addr);
  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("handle_input")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("got buf = %s\n"),
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
#       if defined (__BORLANDC__)
        // BCB does not catch structured exceptions with catch (...).
        // Actually, the ANSI spec says that system exceptions are not
        // supposed to be caught with catch.  Borland may add this, and
        // make it "switchable" in the future...
        try
        {
#       endif /* defined (__BORLANDC__) */

        ret = ACE_Reactor::handle_events (max_wait_time);

#       if defined (__BORLANDC__)
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
          // Probably should handle the details of the exception
          // and throw something that represents the structured exception
          throw "Win32 Structured Exception";
        }
#       endif /* defined (__BORLANDC__) */
      }
    catch (...)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT (" (%t) catch exception\n")));
        ret = -1;
        // do your thing, etc.
      }
//}
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
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) exception return\n")));
        break;
      }

  return 0;
}
#endif /* ACE_HAS_EXCEPTIONS */

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Exceptions_Test"));

#if defined (ACE_HAS_EXCEPTIONS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Starting tracing\n")));

  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  ACE_INET_Addr local_addr (port);
  ACE_INET_Addr remote_addr (port,
                             ACE_DEFAULT_SERVER_HOST);
  My_Reactor reactor;

  ACE_Reactor::instance (&reactor);
  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  {
    // Make sure handler gets cleaned up before reactor by putting it in its
    // own scope
    My_Handler handler (local_addr);


    if (ACE_Reactor::instance ()->register_handler
        (&handler,
         ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("register_handler")),
                        -1);

#if defined (ACE_HAS_THREADS)
    thr_mgr->spawn (ACE_THR_FUNC (worker));
#else
    // Need to figure out how to implement this test.
    ACE_ERROR ((LM_INFO,
                ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

    ACE_SOCK_Dgram dgram (ACE_sap_any_cast (ACE_INET_Addr &));

    for (size_t i = 0; i < ACE_MAX_ITERATIONS; i++)
      dgram.send (ACE_TEXT ("Hello"),
                  sizeof (ACE_TEXT ("Hello")),
                  remote_addr);
    // Barrier to wait for the other thread to return.
    thr_mgr->wait ();

    handler.close ();
    dgram.close ();
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) exiting main\n")));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("C++ exception support not enabled on this platform\n")));
#endif /* ACE_HAS_EXCEPTIONS */

  ACE_END_TEST;
  return 0;
}
