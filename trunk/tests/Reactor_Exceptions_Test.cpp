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

#if defined (ACE_HAS_EXCEPTIONS)

#if defined (ACE_WIN32)
static void
raise_exception (void)
{
  // Cause a Win32 structured exception.
  *(char *) 0 = 0;
}
#else
// Just need a simple exception class.
class Except {};

static void
raise_exception (void)
{
  throw Except ();
}
#endif /* ACE_WIN32 */

class Memory_Exception : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
public:
  Memory_Exception (const ACE_INET_Addr &local_addr);

  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE handle);
};

Memory_Exception::Memory_Exception (const ACE_INET_Addr &local_addr)
  : ACE_SOCK_Dgram (local_addr)
{
}

ACE_HANDLE
Memory_Exception::get_handle (void) const
{
  return ACE_SOCK_Dgram::get_handle ();
}

int
Memory_Exception::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  ACE_INET_Addr from_addr;

  ACE_DEBUG ((LM_DEBUG,
              "Activity occurred on handle %d!\n",
              ACE_SOCK_Dgram::get_handle ()));

  ssize_t n = ACE_SOCK_Dgram::recv (buf, sizeof buf, from_addr);

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "got buf = %s\n",
                buf));

  raise_exception ();
  ACE_NOTREACHED (return 0);
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
        ACE_DEBUG ((LM_DEBUG, " (%t) caught exception\n"));
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
      ACE_ERROR_RETURN ((LM_ERROR, " (%t) error return\n"), -1);

  ACE_NOTREACHED (return 0);
}
#endif /* ACE_HAS_EXCEPTIONS */

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Reactor_Exceptions_Test");

#if defined (ACE_HAS_EXCEPTIONS)
  My_Reactor reactor;

  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  ACE_DEBUG ((LM_DEBUG, "Starting tracing\n"));
  ACE_LOG_MSG->start_tracing ();

  ACE_Reactor::instance (&reactor);
  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  ACE_INET_Addr local_addr (port);
  ACE_INET_Addr remote_addr (port,
                             ACE_DEFAULT_SERVER_HOST);
  Memory_Exception ex (local_addr);

  if (ACE_Reactor::instance ()->register_handler
      (&ex, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n%a", "register_handler", 1), -1);

#if defined (ACE_HAS_THREADS)
  thr_mgr->spawn (ACE_THR_FUNC (worker));
#else
  // Need to figure out how to implement this test.
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  ACE_SOCK_Dgram dgram ((ACE_INET_Addr &) ACE_Addr::sap_any);

  for (size_t i = 0; i < ACE_MAX_ITERATIONS; i++)
    dgram.send ("Hello", 6, remote_addr);

  thr_mgr->wait ();
  ex.close ();
  dgram.close ();

  ACE_DEBUG ((LM_DEBUG, " (%t) exiting main\n"));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "C++ exceptions not supported on this platform\n"));
#endif /* ACE_HAS_EXCEPTIONS */

  ACE_END_TEST;
  return 0;
}
