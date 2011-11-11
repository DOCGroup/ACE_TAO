
//=============================================================================
/**
 *  @file    Priority_Reactor_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the <ACE_Priority_Reactor>.  The test forks
 *   two processes (for a total of three processes) which connect to
 *   the main process and The clients send data to a connector,
 *   interestingly enough the acceptor will give more priority to
 *   the second connection, which should run always before the first
 *   one.
 *
 *   The test itself is interesting, it shows how to write very
 *   simple <ACE_Svc_Handler>, <ACE_Connectors> and <ACE_Acceptors>.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Priority_Reactor.h"
#include "Priority_Reactor_Test.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"



static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// The number of children to run, it can be changed using the -c
// option.
static int opt_nchildren = 10;

// The number of loops per children, it can be changed using the -l
// option.
static int opt_nloops = 200;

// If not set use the normal reactor, it can be changed using the -d
// option.
static int opt_priority_reactor = 1;

// Maximum time to wait for the test termination (-t)
static int opt_max_duration = 60;

// Maximum number of retries to connect, it can be changed using the
// -m option.
static int max_retries = 5;

typedef ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR>
        CONNECTOR;
typedef ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR>
        ACCEPTOR;

int Read_Handler::waiting_ = 0;
int Read_Handler::started_ = 0;

void
Read_Handler::set_countdown (int nchildren)
{
  Read_Handler::waiting_ = nchildren;
}

int
Read_Handler::get_countdown (void)
{
  return Read_Handler::waiting_;
}

int
Read_Handler::open (void *)
{
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Read_Handler::open, ")
                       ACE_TEXT ("cannot set non blocking mode")),
                      -1);

  if (reactor ()->register_handler (this, READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Read_Handler::open, ")
                       ACE_TEXT ("cannot register handler")),
                      -1);

  // A number larger than the actual number of priorities, so some
  // clients are misbehaved, hence pusnished.
  const int max_priority = 15;

  this->priority (ACE_Event_Handler::LO_PRIORITY + started_ % max_priority);
  started_++;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) created svc_handler for handle %d ")
              ACE_TEXT ("with priority %d\n"),
              get_handle (),
              priority ()));
  return 0;
}

int
Read_Handler::handle_input (ACE_HANDLE h)
{
  // ACE_DEBUG((LM_DEBUG,
  // "(%P|%t) Read_Handler::handle_input (%d)\n", h));
  ACE_UNUSED_ARG (h);

  char buf[BUFSIZ];

  ssize_t result = this->peer ().recv (buf, sizeof (buf));

  if (result <= 0)
    {
      if (result < 0 && errno == EWOULDBLOCK)
        return 0;

      if (result != 0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("Read_Handler::handle_input")));
      waiting_--;

      if (waiting_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Last svc_handler closed, shutting down\n")));
          ACE_Reactor::instance()->end_reactor_event_loop();
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Read_Handler::handle_input closing down\n")));
      return -1;
    }

  // ACE_DEBUG((LM_DEBUG,
  // "(%P|%t) read %d bytes from handle %d, priority %d\n",
  // result, h, priority ()));
  return 0;
}

int
Write_Handler::open (void *)
{
  return 0;
}

int
Write_Handler::svc (void)
{
  // Send several short messages, doing pauses between each message.
  // The number of messages can be controlled from the command line.
  ACE_Time_Value pause (0, 1000);

  for (int i = 0; i < opt_nloops; ++i)
    {
      if (this->peer ().send_n (ACE_ALPHABET,
                                sizeof (ACE_ALPHABET) - 1) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("send_n")));
      ACE_OS::sleep (pause);
    }

  return 0;
}

#if !defined (ACE_LACKS_FORK) || defined (ACE_HAS_THREADS)

// Execute the client tests.
static void *
client (void *arg)
{
  ACE_INET_Addr *connection_addr =
    reinterpret_cast<ACE_INET_Addr *> (arg);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) running client\n")));
  CONNECTOR connector;

  Write_Handler *writer = 0;

  // Do exponential backoff connections
  ACE_Synch_Options options = ACE_Synch_Options::synch;

  // Start with one msec timeouts.
  ACE_Time_Value msec (0, 1000);
  options.timeout (msec);

  // Try up to <max_retries> to connect to the server.
  for (int i = 0; i < max_retries; i++)
    {
      if (connector.connect (writer,
                             *connection_addr,
                             options) == -1)
        {
          // Double the timeout...
          ACE_Time_Value tmp = options.timeout ();
          tmp += options.timeout ();
          options.timeout (tmp);
          writer = 0;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) still trying to connect\n")));
        }
      else
        {
          // Let the new Svc_Handler to its job...
          writer->svc ();

          // then close the connection and release the Svc_Handler.
          writer->destroy ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) finishing client\n")));
          return 0;
        }
    }

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t) failed to connect after %d retries\n"),
              max_retries));
  return 0;
}

#endif

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Priority_Reactor_Test"));

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("dc:l:m:t:"));

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
  //FUZZ: enable check_for_lack_ACE_OS
      case 'd':
        opt_priority_reactor = 0;
        break;
      case 'c':
        opt_nchildren = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'l':
        opt_nloops = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'm':
        max_retries = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 't':
        opt_max_duration = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: Priority_Reactor_Test ")
                           ACE_TEXT ("   [-d] (disable priority reactor)\n")
                           ACE_TEXT ("   [-c nchildren] (number of threads/processes)\n")
                           ACE_TEXT ("   [-l loops] (number of loops per child)\n")
                           ACE_TEXT ("   [-m maxretries] (attempts to connect)\n")
                           ACE_TEXT ("   [-t max_time] (limits test duration)\n")),
                          -1);
        ACE_NOTREACHED (break);
      }

  // Manage Reactor memory automagically.
  // Note:  If opt_priority_reactor is false, the default ACE_Reactor is used
  // and we don't need to set one up.
  ACE_Reactor *orig_reactor = 0;
  auto_ptr<ACE_Reactor> reactor;

  if (opt_priority_reactor)
    {
      ACE_Select_Reactor *impl_ptr;
      ACE_NEW_RETURN (impl_ptr, ACE_Priority_Reactor, -1);
      auto_ptr<ACE_Select_Reactor> auto_impl (impl_ptr);

      ACE_Reactor *reactor_ptr;
      ACE_NEW_RETURN (reactor_ptr, ACE_Reactor (impl_ptr, 1), -1);
      auto_impl.release ();   // ACE_Reactor dtor will take it from here
      auto_ptr<ACE_Reactor> auto_reactor (reactor_ptr);
      reactor = auto_reactor;
      orig_reactor = ACE_Reactor::instance (reactor_ptr);
    }

  Read_Handler::set_countdown (opt_nchildren);

  // Acceptor
  ACCEPTOR acceptor;

  acceptor.priority (ACE_Event_Handler::HI_PRIORITY);
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("open")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting server at port %d\n"),
              server_addr.get_port_number ()));

  ACE_INET_Addr connection_addr (server_addr.get_port_number (),
                                 ACE_DEFAULT_SERVER_HOST);

  int i;

#if defined (ACE_HAS_THREADS)
  for (i = 0; i < opt_nchildren; ++i)
    {
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (client),
           (void *) &connection_addr,
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n%a"),
                    ACE_TEXT ("thread create failed"),
                    1));
    }
#elif !defined (ACE_LACKS_FORK)
  for (i = 0; i < opt_nchildren; ++i)
    {
      switch (ACE_OS::fork ("child"))
        {
        case -1:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n%a"),
                      ACE_TEXT ("fork failed"),
                      1));
          ACE_OS::exit (-1);
          /* NOTREACHED */
        case 0:
          client (&connection_addr);
          ACE_OS::exit (0);
          break;
          /* NOTREACHED */
        default:
          break;
          /* NOTREACHED */
        }
    }
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("(%P|%t) ")
              ACE_TEXT ("only one thread may be run ")
              ACE_TEXT ("in a process on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_Time_Value tv (opt_max_duration);

  ACE_Reactor::instance()->register_handler
    (&acceptor, ACE_Event_Handler::READ_MASK);
  ACE_Reactor::instance()->run_reactor_event_loop (tv);

  if (Read_Handler::get_countdown () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) running out of time, ")
                  ACE_TEXT ("probably due to failed connections.\n")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) waiting for the children...\n")));

#if defined (ACE_HAS_THREADS)
  ACE_Thread_Manager::instance ()->wait ();
#elif !defined (ACE_WIN32) && !defined (VXWORKS)
  for (i = 0; i < opt_nchildren; ++i)
    {
      pid_t pid = ACE_OS::wait();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) child %d terminated\n"),
                  pid));
    }
#else
  /* NOTREACHED */
  // We aborted on the previous #ifdef
#endif /* ACE_HAS_THREADS */

  if (orig_reactor != 0)
    ACE_Reactor::instance (orig_reactor);

  ACE_END_TEST;
  return 0;
}

