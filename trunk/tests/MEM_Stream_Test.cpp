// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MEM_Stream_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_MEM_Acceptor> and
//     <ACE_MEM_Connector> classes.
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/MEM_Connector.h"
#include "ace/MEM_Acceptor.h"
#include "ace/Select_Reactor.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Singleton.h"

ACE_RCSID(tests, MEM_Stream_Test, "$Id$")

#if (defined (ACE_HAS_THREADS) || !defined (ACE_LACKS_FORK)) && \
    (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#if defined (ACE_LACKS_FORK)     // Win32, et al
#  define _TEST_USES_THREADS
#else
#  define _TEST_USES_PROCESSES
#  include "ace/Process.h"
#  include "ace/Process_Manager.h"
#endif /* ACE_HAS_FORK */

#include "MEM_Stream_Test.h"         // Defines Echo_Handler

#define NO_OF_REACTIVE_CONNECTION 3
#if defined (ACE_WIN32) || !defined (_ACE_USE_SV_SEM)
# define NO_OF_MT_CONNECTION 3
#else
  // We will use SysV Semaphore in this case which is not very scalable
  // and can only handle one connection.
# define NO_OF_MT_CONNECTION 1
#endif /* ACE_WIN32 || !_ACE_USE_SV_SEM */

#define NO_OF_ITERATION 100

// If we don't have winsock2 we can't use WFMO_Reactor.
#if defined (ACE_WIN32) \
    && !defined (ACE_HAS_WINCE) \
    && defined (ACE_HAS_WINSOCK2) \
    && ACE_HAS_WINSOCK2 != 0
# define TEST_CAN_USE_WFMO_REACTOR
#endif

#if defined(TEST_CAN_USE_WFMO_REACTOR)
static const int opt_wfmo_reactor = 1;
#endif

static int opt_select_reactor = 1;
static ACE_MEM_IO::Signal_Strategy client_strategy = ACE_MEM_IO::Reactive;

typedef ACE_Atomic_Op <ACE_Thread_Mutex, u_short> WaitingCounter;
typedef ACE_Singleton <WaitingCounter, ACE_Thread_Mutex> Waiting;

// Number of connections that are currently open
static u_short connection_count = 0;

typedef ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> ACCEPTOR;
typedef ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> S_ACCEPTOR;

static void reset_handler (int conn)
{
  // Reset the number of connection the test should perform.
  (*Waiting::instance ()) = conn;
  connection_count = 0;
}

int
Echo_Handler::open (void *)
{
  // @@ Nanbor, this method doesn't anything?
  return 0;
}

Echo_Handler::Echo_Handler (ACE_Thread_Manager *thr_mgr)
  : ACE_Svc_Handler<ACE_MEM_STREAM, ACE_MT_SYNCH> (thr_mgr),
    connection_ (++connection_count)
{
  ACE_OS::sprintf (this->name_, ACE_TEXT ("Connection %d --> "),
                   this->connection_);
}

int
Echo_Handler::handle_input (ACE_HANDLE)
{
  ACE_TCHAR buf[MAXPATHLEN];
  ssize_t len;

  len = this->peer ().recv (buf, MAXPATHLEN * sizeof (ACE_TCHAR));

  if (len == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error receiving from MEM_Stream\n")),
                      -1);
  else if (len == 0)
    return -1;

  ACE_TCHAR return_buf[MAXPATHLEN];
  ACE_OS::strcpy (return_buf, this->name_);
  ACE_OS_String::strcat (return_buf, buf);
  len = (ACE_OS::strlen (return_buf) + 1) * sizeof (ACE_TCHAR);

  if (this->peer ().send (return_buf, len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error sending from MEM_Stream\n")),
                      -1);

  return 0;
}

int
Echo_Handler::handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask mask)
{
  // Reduce count.
  (*Waiting::instance ())--;

#if 1
  if (client_strategy != ACE_MEM_IO::Reactive)
    this->reactor ()->remove_handler (this,
                                      mask | ACE_Event_Handler::DONT_CALL);
#endif /* tests */

  // If no connections are open.
  if ((*Waiting::instance ()) == 0)
    ACE_Reactor::instance ()->end_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Echo_Handler %d::handle_close closing down\n"),
              this->connection_));

  // Shutdown
  this->destroy ();
  return 0;
}

int
Echo_Handler::svc (void)
{
  while (this->handle_input (this->get_handle ()) >= 0)
    ;
  return 0;
}

static int
run_client (unsigned short port, ACE_MEM_IO::Signal_Strategy strategy)
{
  int status = 0;
  ACE_MEM_Addr to_server (port);
  ACE_MEM_Connector connector;
  connector.preferred_strategy (strategy);
  ACE_MEM_Stream stream;

  //  connector.preferred_strategy (ACE_MEM_IO::MT);

  if (connector.connect (stream, to_server.get_remote_addr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"), ACE_TEXT ("connect_client")),
                      -1);

  ACE_TCHAR buf[MAXPATHLEN];

  for (ssize_t cntr = 0; cntr < NO_OF_ITERATION; cntr ++)
    {
      ACE_OS::sprintf (buf, ACE_TEXT ("Iteration ")ACE_SSIZE_T_FORMAT_SPECIFIER,
                       cntr);

      ssize_t slen = (ACE_OS::strlen (buf) + 1) * sizeof (ACE_TCHAR);

      if (stream.send (buf, slen) < slen)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                      ACE_TEXT ("In connect_client, send")));
          status = -1;
          break;
        }

      if (stream.recv (buf, MAXPATHLEN) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                      ACE_TEXT ("In connect_client, recv")));
          status = -1;
          break;
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("connect_client, got echo %s\n"), buf));
    }

  status = stream.close () == -1 ? -1 : status;
  return status;
}


void *
connect_client (void *arg)
{
  u_short sport =  (*ACE_reinterpret_cast (u_short *, arg));
  run_client (sport, client_strategy);
  return 0;
}


void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

#if defined (TEST_CAN_USE_WFMO_REACTOR)
  if (opt_wfmo_reactor)
    {
      ACE_NEW (impl,
               ACE_WFMO_Reactor);
    }
#endif /* TEST_CAN_USE_WFMO_REACTOR */

  if (impl == 0 && opt_select_reactor)
    ACE_NEW (impl,
             ACE_Select_Reactor);

  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor,
           ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

int test_reactive (const ACE_TCHAR *prog, ACE_MEM_Addr &server_addr)
{
  ACE_DEBUG ((LM_DEBUG, "Testing Reactive MEM_Stream\n\n"));

  int status = 0;

  client_strategy = ACE_MEM_IO::Reactive;   // Echo_Handler uses this.

  ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR> accept_strategy;
  ACE_Creation_Strategy<Echo_Handler> create_strategy;
  ACE_Reactive_Strategy<Echo_Handler> reactive_strategy (ACE_Reactor::instance ());
  S_ACCEPTOR acceptor;
  if (acceptor.open (server_addr,
                     ACE_Reactor::instance (),
                     &create_strategy,
                     &accept_strategy,
                     &reactive_strategy) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));

  ACE_MEM_Addr local_addr;
  if (acceptor.acceptor ().get_local_addr (local_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                        1);
    }

  u_short sport = local_addr.get_port_number ();

#if defined (_TEST_USES_THREADS)
  ACE_Thread_Manager::instance ()->spawn_n (NO_OF_REACTIVE_CONNECTION,
                                            connect_client,
                                            &sport);
#else
  ACE_Process_Options opts;
  opts.command_line (ACE_TEXT ("%s -p%d -r"), prog, sport);
  if (-1==ACE_Process_Manager::instance ()->spawn_n (NO_OF_REACTIVE_CONNECTION,
                                                     opts))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")));
#endif

  ACE_Time_Value tv(60, 0);
  ACE_Reactor::instance ()->run_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Reactor::run_event_loop timeout\n")));
      status = 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

#if defined (_TEST_USES_THREADS)
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_Process_Manager::instance ()->wait ();
#endif

  if (acceptor.close () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("MEM_Acceptor::close\n")));
      status = 1;
    }

  ACE_UNUSED_ARG (prog);
  return status;
}

int test_multithreaded (const ACE_TCHAR *prog, ACE_MEM_Addr &server_addr)
{
  ACE_DEBUG ((LM_DEBUG, "Testing Multithreaded MEM_Stream\n\n"));

  int status = 0;

  client_strategy = ACE_MEM_IO::MT;     // Echo_Handler uses this.

  ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR> accept_strategy;
  ACE_Creation_Strategy<Echo_Handler> create_strategy;
  ACE_Thread_Strategy<Echo_Handler> thr_strategy;
  S_ACCEPTOR acceptor;


  if (acceptor.open (server_addr,
                     ACE_Reactor::instance (),
                     &create_strategy,
                     &accept_strategy,
                     &thr_strategy) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);

  acceptor.acceptor ().malloc_options ().minimum_bytes_ = 1024 * 1024 ;
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));
  acceptor.acceptor ().preferred_strategy (ACE_MEM_IO::MT);

  ACE_MEM_Addr local_addr;
  if (acceptor.acceptor ().get_local_addr (local_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                        1);
    }

  u_short sport = local_addr.get_port_number ();

#if defined (_TEST_USES_THREADS)
  ACE_Thread_Manager::instance ()->spawn_n (NO_OF_REACTIVE_CONNECTION,
                                            connect_client,
                                            &sport);
#else
  ACE_Process_Options opts;
  opts.command_line (ACE_TEXT ("%s -p%d -m"), prog, sport);
  if (-1==ACE_Process_Manager::instance ()->spawn_n (NO_OF_REACTIVE_CONNECTION,
                                                     opts))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")));
#endif

  ACE_Time_Value tv(60, 0);
  ACE_Reactor::instance ()->run_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Reactor::run_event_loop timeout\n")));
      status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

#if defined (_TEST_USES_THREADS)
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_Process_Manager::instance ()->wait ();
#endif

  if (acceptor.close () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("MEM_Acceptor::close")));
      status = 1;
    }

  ACE_UNUSED_ARG (prog);
  return status;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  unsigned short port = 0;

  if (argc == 1)
    {
      ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

      create_reactor ();

      ACE_MEM_Addr server_addr (port);

      reset_handler (NO_OF_REACTIVE_CONNECTION);

      test_reactive (argv[0], server_addr);

      ACE_Reactor::instance ()->reset_event_loop ();


#if !defined (ACE_WIN32) && defined (_ACE_USE_SV_SEM)
      ACE_ERROR ((LM_DEBUG,
              ACE_TEXT ("\n *** Platform only support non-scalable SysV semaphores ***\n\n")));
#endif /* !ACE_WIN32 && _ACE_USE_SV_SEM */

      reset_handler (NO_OF_MT_CONNECTION);

      test_multithreaded (argv[0], server_addr);

      ACE_END_TEST;
      return 0;
    }

  // Here if this is a child process spawned for one of the test passes.
  // command line is:   -p <port> -r (reactive) | -m (multithreaded)

  ACE_TCHAR lognm[MAXPATHLEN];
  ACE_OS::sprintf(lognm, ACE_TEXT ("MEM_Stream_Test-%d"), ACE_OS::getpid());
  ACE_START_TEST (lognm);

  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("p:rm"));
  int opt, iport, status;
  ACE_MEM_IO::Signal_Strategy model = ACE_MEM_IO::Reactive;

  while ((opt = opts()) != -1)
    {
      switch (opt)
        {
        case 'p':
          iport = ACE_OS::atoi (opts.optarg);
          port = ACE_static_cast (unsigned short, iport);
          break;

        case 'r':
          model = ACE_MEM_IO::Reactive;
          break;

        case 'm':
          model = ACE_MEM_IO::MT;
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("Invalid option (-p <port> -r | -m)\n")),
                            1);
        }
    }
  status = run_client (port, model);
  ACE_END_TEST;
  return status;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>;
template class ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Atomic_Op<ACE_Thread_Mutex, u_short>;
template class ACE_Singleton<ACE_Atomic_Op<ACE_Thread_Mutex, u_short>,ACE_Thread_Mutex>;
template class ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Creation_Strategy<Echo_Handler>;
template class ACE_Reactive_Strategy<Echo_Handler>;
template class ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Concurrency_Strategy<Echo_Handler>;
template class ACE_Scheduling_Strategy<Echo_Handler>;
template class ACE_Thread_Strategy<Echo_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>
#pragma instantiate ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, u_short>
#pragma instantiate ACE_Singleton<ACE_Atomic_Op<ACE_Thread_Mutex, u_short>,ACE_Thread_Mutex>
#pragma instantiate ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<Echo_Handler>
#pragma instantiate ACE_Reactive_Strategy<Echo_Handler>
#pragma instantiate ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<Echo_Handler>
#pragma instantiate ACE_Scheduling_Strategy<Echo_Handler>
#pragma instantiate ACE_Thread_Strategy<Echo_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("position independent pointers ")
              ACE_TEXT ("not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* (ACE_HAS_THREADS || ACE_HAS_FORK) && ACE_HAS_POSITION_INDENDENT_POINTERS == 1 */
