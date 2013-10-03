
//=============================================================================
/**
 *  @file    MEM_Stream_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the <ACE_MEM_Acceptor> and
 *   <ACE_MEM_Connector> classes.
 *
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#include "test_config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/MEM_Connector.h"
#include "ace/MEM_Acceptor.h"
#include "ace/Select_Reactor.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Singleton.h"
#include "ace/Atomic_Op.h"

#if (defined (ACE_HAS_THREADS) || defined (ACE_HAS_PROCESS_SPAWN)) && \
    (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#if !defined (ACE_HAS_PROCESS_SPAWN) && defined (ACE_HAS_THREADS)
#  define _TEST_USES_THREADS
#else
#  define _TEST_USES_PROCESSES
#  include "ace/Process.h"
#  include "ace/Process_Manager.h"
#endif

#include "MEM_Stream_Test.h"         // Defines Echo_Handler

#define NUMBER_OF_REACTIVE_CONNECTIONS 3
#if defined (ACE_WIN32) || !defined (_ACE_USE_SV_SEM)
# define NUMBER_OF_MT_CONNECTIONS 3
#else
  // We will use SysV Semaphore in this case which is not very scalable
  // and can only handle one connection.
# define NUMBER_OF_MT_CONNECTIONS 1
#endif /* ACE_WIN32 || !_ACE_USE_SV_SEM */

#define NUMBER_OF_ITERATIONS 100

// If we don't have winsock2 we can't use WFMO_Reactor.
#if defined (ACE_WIN32) \
    && defined (ACE_HAS_WINSOCK2) \
    && ACE_HAS_WINSOCK2 != 0
# define TEST_CAN_USE_WFMO_REACTOR
#endif

#if defined (TEST_CAN_USE_WFMO_REACTOR)
static const int opt_wfmo_reactor = 1;
#endif /* TEST_CAN_USE_WFMO_REACTOR */

static int opt_select_reactor = 1;
static ACE_MEM_IO::Signal_Strategy client_strategy = ACE_MEM_IO::Reactive;

typedef ACE_Atomic_Op <ACE_SYNCH_MUTEX, u_short> WaitingCounter;
typedef ACE_Singleton <WaitingCounter, ACE_SYNCH_RECURSIVE_MUTEX> Waiting;

// Number of connections that are currently open
static u_short connection_count = 0;

typedef ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> ACCEPTOR;
typedef ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> S_ACCEPTOR;

static void reset_handler (int conn)
{
  // Reset the number of connection the test should perform.
  *Waiting::instance () = conn;
  connection_count = 0;
}

int
Echo_Handler::open (void *)
{
  return 0;
}

Echo_Handler::Echo_Handler (ACE_Thread_Manager *thr_mgr)
  : ACE_Svc_Handler<ACE_MEM_STREAM, ACE_SYNCH> (thr_mgr),
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
  else if (len == 0)            // Connection closed.
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Connection %d closed\n"),
                  this->connection_));
      return -1;
    }

  ACE_TCHAR return_buf[MAXPATHLEN];
  ACE_OS::strcpy (return_buf, this->name_);
  ACE_OS::strcat (return_buf, buf);
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

  if (client_strategy != ACE_MEM_IO::Reactive)
    this->reactor ()->remove_handler (this,
                                      mask | ACE_Event_Handler::DONT_CALL);

  // If no connections are open.
  if (*Waiting::instance () == 0)
    ACE_Reactor::instance ()->end_reactor_event_loop ();

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
    continue;
  return 0;
}

static int
run_client (u_short port,
            ACE_MEM_IO::Signal_Strategy strategy)
{
  int status = 0;
  ACE_MEM_Addr to_server (port);
  ACE_MEM_Connector connector;
  connector.preferred_strategy (strategy);
  ACE_MEM_Stream stream;

  //  connector.preferred_strategy (ACE_MEM_IO::MT);

  if (connector.connect (stream, to_server.get_remote_addr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to connect to <%C> %p\n"),
                       to_server.get_host_name (),
                       ACE_TEXT ("connector.connect()")),
                      -1);

  ACE_TCHAR buf[MAXPATHLEN];

  for (size_t cntr = 0; cntr < NUMBER_OF_ITERATIONS; cntr ++)
    {
      ACE_OS::sprintf (buf, ACE_TEXT ("Iteration ")ACE_SIZE_T_FORMAT_SPECIFIER,
                       cntr);

      ssize_t slen = (ACE_OS::strlen (buf) + 1) * sizeof (ACE_TCHAR);

      if (stream.send (buf, slen) < slen)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                      ACE_TEXT ("In stream.send()")));
          status = -1;
          break;
        }

      if (stream.recv (buf, MAXPATHLEN) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                      ACE_TEXT ("stream.recv()")));
          status = -1;
          break;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("run_client(), got echo %s\n"),
                  buf));
    }

  status = stream.close () == -1 ? -1 : status;
  return status;
}

#if defined (_TEST_USES_THREADS)
static ACE_THR_FUNC_RETURN
connect_client (void *arg)
{
  u_short *sport =  reinterpret_cast <u_short *> (arg);
  run_client (*sport, client_strategy);
  return 0;
}
#endif

static void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

#if defined (TEST_CAN_USE_WFMO_REACTOR)
  if (opt_wfmo_reactor)
    ACE_NEW (impl,
             ACE_WFMO_Reactor);
#endif /* TEST_CAN_USE_WFMO_REACTOR */

  if (impl == 0 && opt_select_reactor)
    ACE_NEW (impl,
             ACE_Select_Reactor);

  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor,
           ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

static int
test_reactive (const ACE_TCHAR *prog,
               ACE_MEM_Addr &server_addr)
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
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                      1);

  u_short sport = local_addr.get_port_number ();

#if defined (_TEST_USES_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (NUMBER_OF_REACTIVE_CONNECTIONS,
                                                connect_client,
                                                &sport) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n ()")));
#else
  ACE_Process_Options opts;
#  if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%s -p%d -r");
#  else
  const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%ls -p%d -r");
#  endif /* ACE_WIN32 || !ACE_USES_WCHAR */
  opts.command_line (cmdline_fmt, prog, sport);
  if (ACE_Process_Manager::instance ()->spawn_n (NUMBER_OF_REACTIVE_CONNECTIONS,
                                                 opts) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n ()")));
#endif /* _TEST_USES_THREADS */

  ACE_Time_Value tv (60, 0);
  ACE_Reactor::instance ()->run_reactor_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Reactor::run_event_loop timeout\n")));
      status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

#if defined (_TEST_USES_THREADS)
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("wait ()")));
#else
  if (ACE_Process_Manager::instance ()->wait () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("wait ()")));
#endif /* _TEST_USES_THREADS */

  if (acceptor.close () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("MEM_Acceptor::close\n")));
      status = 1;
    }

  ACE_UNUSED_ARG (prog);
  return status;
}

static int
test_concurrent (const ACE_TCHAR *prog,
                 ACE_MEM_Addr &server_addr)
{
  ACE_DEBUG ((LM_DEBUG, "Testing Multithreaded MEM_Stream\n\n"));

  int status = 0;
  client_strategy = ACE_MEM_IO::MT;     // Echo_Handler uses this.

  ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR> accept_strategy;
  ACE_Creation_Strategy<Echo_Handler> create_strategy;
#if defined (ACE_HAS_THREADS)
  ACE_Thread_Strategy<Echo_Handler> act_strategy;
#else
  ACE_Reactive_Strategy<Echo_Handler> act_strategy (ACE_Reactor::instance ());
#endif /* ACE_HAS_THREADS */
  S_ACCEPTOR acceptor;

  if (acceptor.open (server_addr,
                     ACE_Reactor::instance (),
                     &create_strategy,
                     &accept_strategy,
                     &act_strategy) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);

  // Make sure the MEM_Stream created by the underlying MEM_Acceptor
  // is capable of passing messages of 1MB.
  acceptor.acceptor ().init_buffer_size (1024 * 1024);
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));
  acceptor.acceptor ().preferred_strategy (ACE_MEM_IO::MT);

  ACE_MEM_Addr local_addr;
  if (acceptor.acceptor ().get_local_addr (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                      1);

  u_short sport = local_addr.get_port_number ();

#if defined (_TEST_USES_THREADS)
  ACE_UNUSED_ARG (prog);

  if (ACE_Thread_Manager::instance ()->spawn_n (NUMBER_OF_MT_CONNECTIONS,
                                                connect_client,
                                                &sport) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n()")));
#else
  ACE_Process_Options opts;
#  if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%s -p%d -m");
#  else
  const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%ls -p%d -m");
#  endif /* ACE_WIN32 || !ACE_USES_WCHAR */
  opts.command_line (cmdline_fmt, prog, sport);
  if (ACE_Process_Manager::instance ()->spawn_n (NUMBER_OF_MT_CONNECTIONS,
                                                 opts) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n()")));
#endif /* _TEST_USES_THREADS */

  ACE_Time_Value tv (60, 0);
  ACE_Reactor::instance ()->run_reactor_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Reactor::run_event_loop timeout\n")));
      status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

#if defined (_TEST_USES_THREADS)
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("wait ()")));
#else
  if (ACE_Process_Manager::instance ()->wait () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("wait ()")));
#endif /* _TEST_USES_THREADS */

  if (acceptor.close () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("MEM_Acceptor::close")));
      status = 1;
    }

  return status;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  u_short port = 0;

  if (argc == 1)
    {
      // This is the "master" process.

      ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));
      create_reactor ();
      ACE_MEM_Addr server_addr (port);

      reset_handler (NUMBER_OF_REACTIVE_CONNECTIONS);

      test_reactive (argc > 0 ? argv[0] : ACE_TEXT ("MEM_Stream_Test"), server_addr);

      ACE_Reactor::instance ()->reset_reactor_event_loop ();

#if !defined (ACE_WIN32) && defined (_ACE_USE_SV_SEM)
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("\n *** Platform only supports non-scalable SysV semaphores ***\n\n")));
#endif /* !ACE_WIN32 && _ACE_USE_SV_SEM */
      reset_handler (NUMBER_OF_MT_CONNECTIONS);

      test_concurrent (argc > 0 ? argv[0] : ACE_TEXT ("MEM_Stream_Test"), server_addr);

      ACE_END_TEST;
      return 0;
    }
  else
    {
      // We end up here if this is a child process spawned for one of
      // the test passes.  command line is: -p <port> -r (reactive) |
      // -m (multithreaded)

      ACE_TCHAR lognm[MAXPATHLEN];
      int mypid (ACE_OS::getpid ());
      ACE_OS::sprintf(lognm, ACE_TEXT ("MEM_Stream_Test-%d"), mypid);
      ACE_START_TEST (lognm);

      ACE_Get_Opt opts (argc, argv, ACE_TEXT ("p:rm"));
      int opt, iport, status;
      ACE_MEM_IO::Signal_Strategy model = ACE_MEM_IO::Reactive;

      while ((opt = opts()) != -1)
        {
          switch (opt)
            {
            case 'p':
              iport = ACE_OS::atoi (opts.opt_arg ());
              port = static_cast <u_short> (iport);
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
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)

template ACE_Singleton<ACE_Atomic_Op<ACE_SYNCH_MUTEX, u_short>,
                       ACE_SYNCH_RECURSIVE_MUTEX> *
  ACE_Singleton<ACE_Atomic_Op<ACE_SYNCH_MUTEX, u_short>,
                ACE_SYNCH_RECURSIVE_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("position independent pointers ")
              ACE_TEXT ("not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* (ACE_HAS_THREADS || ACE_HAS_PROCESS_SPAWN) && ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
