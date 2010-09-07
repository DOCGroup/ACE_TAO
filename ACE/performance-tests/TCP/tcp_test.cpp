// $Id$

// ============================================================================
//
// = LIBRARY
//    performance-tests/TCP
//
// = FILENAME
//   tcp_test.cpp
//
// = DESCRIPTION
//   Measures TCP round-trip performance.
//
// = AUTHORS
//   Based on udp_test by Fred Kuhns and David L. Levine
//   Modified by Carlos O'Ryan and Nanbor Wang.
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Thread_Manager.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

// FUZZ: disable check_for_math_include
#include <math.h>

ACE_RCSID(TCP, tcp_test, "$Id$")

// Global variables (evil).
static const u_short DEFPORT = 5050;
static const int MAXPKTSZ = 65536;
static const int DEFPKTSZ = 64;
static const int DEFITERATIONS = 1000;
static const int DEFINTERVAL = 0;
static const int DEFAULT_THRNO = 10;

static char sbuf[MAXPKTSZ];
static char rbuf[MAXPKTSZ];

static int usdelay = DEFINTERVAL;
static int bufsz = DEFPKTSZ;
static int VERBOSE = 0;
static int dump_history = 0;
static int svr_thrno = DEFAULT_THRNO;
static int server = 0;
static int client = 0;
static int nsamples = DEFITERATIONS;
static int so_bufsz = 0;
static u_int use_reactor = 0;
static int usecs = 0;

enum {
  SELECT = 1,
    TP,
    WFMO
};


static void
usage (void)
{
  ACE_ERROR ((LM_ERROR,
              "tcp_test\n"
              "  [-v]          (Verbose)\n"
              "  [-h] (dump all the samples)\n"
              "  [-m message size]\n"
              "  [-i iterations]\n"
              "  [-I usdelay]\n"
              "  [-b socket bufsz]\n"
              "  [-p port]\n"
              "  [-s]\n"
              "  [-c]\n"
              //              "  [-x max_sample_allowed]\n"
              "  [-t number of threads]\n"
              "  [-a to use the ACE Select reactor]\n"
              "  [-x to use the ACE TP reactor]\n"
              "  [-w to use the ACE WFMO reactor]\n"
              "  targethost\n"));
}

// ****************************************************************

class Client : public ACE_Event_Handler
{
public:
  Client (const ACE_INET_Addr &remote_addr);

  virtual ~Client (void);

  // = Override <ACE_Event_Handler> methods.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  //FUZZ: disable check_for_lack_ACE_OS
  int send (const char *buf, size_t len);
  // Send the <buf> to the server.
  //FUZZ: enable check_for_lack_ACE_OS

  int get_response (char *buf, size_t len);
  // Wait for the response.

  int run (void);
  // Send messages to server and record statistics.

  //FUZZ: disable check_for_lack_ACE_OS
  int shutdown (void);
  // Send shutdown message to server.
  //FUZZ: enable check_for_lack_ACE_OS

private:
  ACE_SOCK_Stream endpoint_;
  // To send messages and receive responses.

  ACE_INET_Addr remote_addr_;
  // The address to send messages to.

  ACE_UNIMPLEMENTED_FUNC (Client (void))
  ACE_UNIMPLEMENTED_FUNC (Client (const Client &))
  ACE_UNIMPLEMENTED_FUNC (Client &operator= (const Client &))
};

Client::Client (const ACE_INET_Addr &remote_addr)
  : remote_addr_ (remote_addr)
{
  ACE_SOCK_Connector connector;
  if (connector.connect (this->endpoint_, remote_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Client - %p\n",
                  "connect failed"));
    }

  if (use_reactor)
    {
      if (ACE_Reactor::instance ()->register_handler
          (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    "ACE_Reactor::register_handler: Client\n"));
    }
}

Client::~Client (void)
{
}

ACE_HANDLE
Client::get_handle (void) const
{
  return this->endpoint_.get_handle ();
}

int
Client::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ssize_t n = this->endpoint_.recv (buf, sizeof buf);

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) buf of size %d = %*s\n",
                n,
                n,
                buf));

  return 0;
}

int
Client::handle_close (ACE_HANDLE,
                      ACE_Reactor_Mask)
{
  this->endpoint_.close ();
  return 0;
}

int
Client::send (const char *buf, size_t len)
{
  return this->endpoint_.send (buf, len);
}

int
Client::get_response (char *buf, size_t len)
{
  return this->endpoint_.recv (buf, len);
}

int
Client::run (void)
{
  ACE_OS::memset (sbuf, 0, bufsz);
  ACE_OS::memset (rbuf, 0, bufsz);

  for (int j = 0; j != 100; ++j)
    {
      if (this->send (sbuf, bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "send"), -1);

      //      ssize_t n;
      if ((this->get_response (rbuf, bufsz)) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "get_response"), -1);
    }

  ACE_Sample_History history (nsamples);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i != nsamples; ++i)
    {
      if (usecs != 0)
        {
          ACE_Time_Value tv (0, usecs);
          ACE_OS::sleep (tv);
        }

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      if (this->send (sbuf, bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "send"), -1);

      //      ssize_t n;
      if ((this->get_response (rbuf, bufsz)) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "get_response"), -1);

      ACE_hrtime_t end = ACE_OS::gethrtime ();

      history.sample (end - start);

      if (VERBOSE && i % 500 == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Send %d / %d events\n", i, nsamples));
        }
    }
  ACE_hrtime_t test_end = ACE_OS::gethrtime ();

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  if (dump_history)
    {
      history.dump_samples (ACE_TEXT("HISTORY"), gsf);
    }

  ACE_Basic_Stats latency;
  history.collect_basic_stats (latency);
  latency.dump_results (ACE_TEXT("Client"), gsf);
  ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Client"),
                                         gsf,
                                         test_end - test_start,
                                         latency.samples_count ());


  return 0;
}

int
Client::shutdown (void)
{
  const char buf = 'S';
  int n = this->endpoint_.send (&buf, 1u);

  if (use_reactor)
    {
      if (ACE_Reactor::instance ()->remove_handler
          (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_Reactor::remove_handler: Client\n"),
                          -1);
    }

  return n;
}

// ****************************************************************

class Server : public ACE_Event_Handler
{
public:
  Server (const ACE_INET_Addr &addr);

  virtual ~Server (void);

  // = Override <ACE_Event_Handler> methods.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:
  ACE_SOCK_Stream endpoint_;
  // Receives datagrams.

  ACE_UNIMPLEMENTED_FUNC (Server (void))
  ACE_UNIMPLEMENTED_FUNC (Server (const Server &))
  ACE_UNIMPLEMENTED_FUNC (Server &operator= (const Server &))
};

Server::Server (const ACE_INET_Addr &addr)
{
  ACE_SOCK_Acceptor acceptor;

  if (acceptor.open (addr, 1) == -1)
    ACE_DEBUG ((LM_DEBUG, "%p\n", "open"));

  ACE_DEBUG ((LM_DEBUG, "Listening on %s:%d\n",
              addr.get_host_name (),
              addr.get_port_number ()));
  if (acceptor.accept (this->endpoint_) == -1)
    ACE_ERROR ((LM_ERROR, "Server::Server %p\n",
                "accept failed"));

  if (use_reactor)
    {
      if (ACE_Reactor::instance ()->register_handler
          (this,
           ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    "ACE_Reactor::register_handler: Server\n"));
    }

#if !defined (ACE_LACKS_SO_SNDBUF)
  if (so_bufsz != 0)
    {
      if (this->endpoint_.set_option (SOL_SOCKET,
                                      SO_SNDBUF,
                                      (void *) &so_bufsz,
                                      sizeof (so_bufsz)) == -1
          && errno != ENOTSUP)
        {
          ACE_ERROR ((LM_ERROR, "Server::Server: SO_SNDBUF %p\n",
                      "set_option failed"));
        }
    }
#endif /* ACE_LACKS_SO_SNDBUF */
#if !defined (ACE_LACKS_SO_RCVBUF)
  if (so_bufsz != 0)
    {
      if (this->endpoint_.set_option (SOL_SOCKET,
                                      SO_RCVBUF,
                                      (void *) &so_bufsz,
                                      sizeof (so_bufsz)) == -1
               && errno != ENOTSUP)
        {
          ACE_ERROR ((LM_ERROR, "Server::Server: SO_RCVBUF %p\n",
                      "set_option failed"));
        }
    }
#endif /* !ACE_LACKS_SO_RCVBUF */
  if (acceptor.close () == -1)
    ACE_ERROR ((LM_ERROR, "Server::Server %p\n",
                "close failed"));
}

Server::~Server (void)
{
  this->endpoint_.close ();
}

ACE_HANDLE
Server::get_handle (void) const
{
  return this->endpoint_.get_handle ();
}

int
Server::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ssize_t n = this->endpoint_.recv (buf, bufsz);

  if (n == -1)
    ACE_DEBUG ((LM_ERROR,
                "%p\n",
                "handle_input: recv"));

  // Send the message back as the response.
  if (this->endpoint_.send (buf, n) == n)
    {
      if (n == 1 && buf[0] == 'S')
        {
          if (!use_reactor)
            {
              // Indicate done by returning 1.
              return 1;
            }

          if (ACE_Reactor::instance ()->remove_handler
              (this, ACE_Event_Handler::READ_MASK) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "ACE_Reactor::remove_handler: server\n"),
                              -1);

          ACE_Reactor::end_event_loop ();
        }

      return 0;
    }

  ACE_DEBUG ((LM_ERROR,
              "%p\n",
              "handle_input: send"));
  return -1;
}

int
Server::handle_close (ACE_HANDLE,
                      ACE_Reactor_Mask)
{
  this->endpoint_.close ();

  return 0;
}

static ACE_THR_FUNC_RETURN
thread_pool_worker (void *)
{
  // Server thread function.

  while (!ACE_Reactor::event_loop_done ())
    {
      if (ACE_Reactor::instance ()->handle_events () == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("Error handling events")));
    }

  return 0;
}

int
run_server (ACE_INET_Addr &addr)
{
  if (use_reactor)
    {
      ACE_Reactor *new_reactor = 0;

      switch (use_reactor)
        {
        case SELECT:
          {
            ACE_Select_Reactor *sr = new ACE_Select_Reactor ();
            new_reactor = new ACE_Reactor (sr, 1);
          }
          break;
        case TP:
          {
            ACE_TP_Reactor *sr = new ACE_TP_Reactor ();
            new_reactor = new ACE_Reactor (sr, 1);
          }
          break;
        case WFMO:
#if defined (ACE_WIN32)

#else

#endif /* ACE_WIN32 */
        default:
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid reactor type selected\n"), -1);
        }
      ACE_Reactor::instance (new_reactor, 1);
    }

  Server server (addr);

  if (!use_reactor)
    {
      // Handle input in the current thread.
      // This is actually equivalent to thread-per-connection model.
      while (server.handle_input (0) != 1)
        continue;
    }
  else
    {
      switch (use_reactor)
        {
        case SELECT:
          // Run the reactor event loop.
          ACE_Reactor::run_event_loop ();
          break;
        case TP:
          ACE_Thread_Manager::instance ()->spawn_n (svr_thrno,
                                                    thread_pool_worker);
          ACE_Thread_Manager::instance ()->wait ();
          break;
        case WFMO:
          break;
        default:
          break; // won't happen here.
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int c, dstport = DEFPORT;
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("hxwvb:I:p:sci:m:at:"));

  while ((c = get_opt ()) != -1)
    {
      switch ((char) c)
        {
        case 'v':
          VERBOSE = 1;
          break;

        case 'h':
          dump_history = 1;
          break;

        case 'm':
          bufsz = ACE_OS::atoi (get_opt.opt_arg ());

          if (bufsz <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nMessage size must be greater than 0!\n\n"),
                              1);
          else if (bufsz > BUFSIZ)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nbufsz must be <= %d\n",
                               BUFSIZ),
                              1);

        case 'i':
          nsamples = ACE_OS::atoi (get_opt.opt_arg ());
          if (nsamples <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nIterations must be greater than 0!\n\n"),
                              1);
          break;

        case 'a':
          use_reactor = SELECT;
          break;

        case 'x':
          use_reactor = TP;
          break;

        case 'w':
#if defined (ACE_WIN32)
          use_reactor = WFMO;
          break;
#else
          ACE_ERROR_RETURN ((LM_ERROR, "WFMO_Reactor is not supported\n"), -1);
#endif /* ACE_WIN32 */

        case 'b':
          so_bufsz = ACE_OS::atoi (get_opt.opt_arg ());

          if (so_bufsz <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nInvalid socket buffer size!\n\n"),
                              1);
          break;

        case 'I':
          usdelay = ACE_OS::atoi (get_opt.opt_arg ());

          if (usdelay < 0)
            {
              usdelay = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s: bad usdelay: %s\n",
                                 argv[0],
                                 get_opt.opt_arg ()),
                                1);
            }
          break;

        case 'p':
          dstport = ACE_OS::atoi (get_opt.opt_arg ());
          if (dstport <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nInvalid port number!\n\n"),
                              1);
          break;
        case 't':
          svr_thrno = ACE_OS::atoi (get_opt.opt_arg ());

          if (svr_thrno <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nInvalid server thread number!\n\n"),
                              1);
          break;

        case 'c':
          server = 0;
          client = 1;
          break;
        case 's':
          client = 0;
          server = 1;
          break;
        default:
          usage ();
          return 1;
        }
    }

  if ((get_opt.opt_ind () >= argc && client != 0) || argc == 1)
    {
      usage ();
      return 1;
    }

  ACE_INET_Addr addr (dstport);

  if (server)
    {
      return run_server (addr);
    }

  if ((u_int) bufsz < sizeof (ACE_hrtime_t))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "\nbufsz must be >= %d\n",
                         sizeof (ACE_hrtime_t)),
                        1);
    }

  ACE_INET_Addr remote_addr;

  if (ACE_OS::ace_isdigit(argv[get_opt.opt_ind ()][0]))
    {
      if (remote_addr.set (dstport,
                           (ACE_UINT32) ACE_OS::inet_addr
                           (ACE_TEXT_ALWAYS_CHAR(argv[get_opt.opt_ind ()]))) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid IP address: %s\n",
                           argv[get_opt.opt_ind ()]),
                          1);
    }
  else
    {
      if (remote_addr.set (dstport, argv[get_opt.opt_ind ()]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid IP address: %s\n",
                           argv[get_opt.opt_ind ()]),
                          1);
    }
  get_opt.opt_ind ()++;

  ACE_DEBUG ((LM_DEBUG, "Connecting to %s:%d\n",
              remote_addr.get_host_name (),
              remote_addr.get_port_number ()));

  Client client (remote_addr);

  ACE_DEBUG ((LM_DEBUG,
              "\nSending %d byte packets to %s:%d "
              "with so_bufsz = %d\n\n",
              bufsz,
              addr.get_host_name (),
              dstport,
              so_bufsz));

  client.run ();
  client.shutdown ();

  return 0;
}
