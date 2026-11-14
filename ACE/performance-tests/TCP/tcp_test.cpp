//=============================================================================
/**
 *  @file   tcp_test.cpp
 *
 * Measures TCP round-trip performance.
 *
 *  @author Based on udp_test by Fred Kuhns and David L. LevineModified by Carlos O'Ryan and Nanbor Wang.
 */
//=============================================================================

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

#include <array>
#include <cstring>
#include <memory>

// FUZZ: disable check_for_math_include
#include <math.h>

// Global variables (evil).
static constexpr u_short DEFPORT = 5050;
static constexpr int MAXPKTSZ = 65536;
static constexpr int DEFPKTSZ = 64;
static constexpr int DEFITERATIONS = 1000;
static constexpr int DEFINTERVAL = 0;
static constexpr int DEFAULT_THRNO = 10;

static std::array<char, MAXPKTSZ> sbuf {};
static std::array<char, MAXPKTSZ> rbuf {};

static int bufsz {DEFPKTSZ};
static bool VERBOSE {};
static bool dump_history {};
static int svr_thrno {DEFAULT_THRNO};
static bool server {};
static bool client {};
static int nsamples {DEFITERATIONS};
static int so_bufsz {};
static int usdelay {DEFINTERVAL};
enum class Reactor_Type
{
  NONE,
  SELECT,
  TP,
  WFMO
};
static Reactor_Type use_reactor {Reactor_Type::NONE};

static void
usage ()
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
  explicit Client (ACE_INET_Addr const &remote_addr);

  ~Client () override;

  /// Overrides ACE_Event_Handler methods.
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask) override;

  //FUZZ: disable check_for_lack_ACE_OS
  /// Send the <buf> to the server.
  int send (char const *buf, size_t len);
  //FUZZ: enable check_for_lack_ACE_OS

  /// Wait for the response.
  int get_response (char *buf, size_t len);

  /// Send messages to server and record statistics.
  int run ();

  //FUZZ: disable check_for_lack_ACE_OS
  /// Send shutdown message to server.
  int shutdown ();
  //FUZZ: enable check_for_lack_ACE_OS

private:
  /// To send messages and receive responses.
  ACE_SOCK_Stream endpoint_;

  /// The address to send messages to.
  ACE_INET_Addr remote_addr_;

  Client () = delete;
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;
};

Client::Client (ACE_INET_Addr const &remote_addr)
  : remote_addr_ (remote_addr)
{
  ACE_SOCK_Connector connector;
  if (connector.connect (this->endpoint_, remote_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Client - %p\n",
                  "connect failed"));
    }

  if (use_reactor != Reactor_Type::NONE)
    {
      if (ACE_Reactor::instance ()->register_handler
          (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    "ACE_Reactor::register_handler: Client\n"));
    }
}

Client::~Client ()
{
  this->endpoint_.close ();
}

ACE_HANDLE
Client::get_handle () const
{
  return this->endpoint_.get_handle ();
}

int
Client::handle_input (ACE_HANDLE)
{
  std::array<char, BUFSIZ> buf {};

  ssize_t const n {this->endpoint_.recv (buf.data (), buf.size ())};

  if (n == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) buf of size %d = %*s\n",
                static_cast<int> (n),
                static_cast<int> (n),
                buf.data ()));

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
Client::send (char const *buf, size_t len)
{
  return this->endpoint_.send (buf, len);
}

int
Client::get_response (char *buf, size_t len)
{
  return this->endpoint_.recv (buf, len);
}

int
Client::run ()
{
  std::memset (sbuf.data (), 0, bufsz);
  std::memset (rbuf.data (), 0, bufsz);

  for (int j {}; j != 100; ++j)
    {
      if (this->send (sbuf.data (), bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "send"), -1);

      if (this->get_response (rbuf.data (), bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "get_response"), -1);
    }

  ACE_Sample_History history (nsamples);

  ACE_hrtime_t const test_start {ACE_OS::gethrtime ()};
  for (int i {}; i != nsamples; ++i)
    {
      if (usdelay != 0)
        {
          ACE_Time_Value const tv {0, usdelay};
          ACE_OS::sleep (tv);
        }

      ACE_hrtime_t const start {ACE_OS::gethrtime ()};
      if (this->send (sbuf.data (), bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "send"), -1);

      if ((this->get_response (rbuf.data (), bufsz)) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "get_response"), -1);

      ACE_hrtime_t const end {ACE_OS::gethrtime ()};

      history.sample (end - start);

      if (VERBOSE && i % 500 == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Send %d / %d events\n", i, nsamples));
        }
    }
  ACE_hrtime_t const test_end {ACE_OS::gethrtime ()};

  ACE_High_Res_Timer::global_scale_factor_type const gsf {
    ACE_High_Res_Timer::global_scale_factor ()};

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
Client::shutdown ()
{
  char const buf {'S'};
  int const n {static_cast<int> (this->endpoint_.send (std::addressof (buf), 1u))};

  if (use_reactor != Reactor_Type::NONE)
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
  explicit Server (ACE_INET_Addr const &addr);

  ~Server () override;

  /// Overrides ACE_Event_Handler methods.
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask) override;

private:
  /// Receives datagrams.
  ACE_SOCK_Stream endpoint_;

  Server () = delete;
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;
};

Server::Server (ACE_INET_Addr const &addr)
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

  if (use_reactor != Reactor_Type::NONE)
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
                                      std::addressof (so_bufsz),
                                      static_cast<int> (sizeof so_bufsz)) == -1
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
                                      std::addressof (so_bufsz),
                                      static_cast<int> (sizeof so_bufsz)) == -1
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

Server::~Server ()
{
  this->endpoint_.close ();
}

ACE_HANDLE
Server::get_handle () const
{
  return this->endpoint_.get_handle ();
}

int
Server::handle_input (ACE_HANDLE)
{
  std::array<char, BUFSIZ> buf {};

  ssize_t const n {this->endpoint_.recv (buf.data (), bufsz)};

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "handle_input: recv"),
                      -1);

  // Send the message back as the response.
  if (this->endpoint_.send (buf.data (), n) == n)
    {
      if (n == 1 && buf[0] == 'S')
        {
          if (use_reactor == Reactor_Type::NONE)
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
run_server (ACE_INET_Addr const &addr)
{
  if (use_reactor != Reactor_Type::NONE)
    {
      std::unique_ptr<ACE_Reactor_Impl> implementation {};

      switch (use_reactor)
        {
        case Reactor_Type::SELECT:
          implementation = std::make_unique<ACE_Select_Reactor> ();
          break;
        case Reactor_Type::TP:
          implementation = std::make_unique<ACE_TP_Reactor> ();
          break;
        case Reactor_Type::WFMO:
#if defined (ACE_WIN32)

#else

#endif /* ACE_WIN32 */
        default:
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid reactor type selected\n"), -1);
        }

      std::unique_ptr<ACE_Reactor> new_reactor {
        std::make_unique<ACE_Reactor> (implementation.get (), true)};
      implementation.release ();
      ACE_Reactor::instance (new_reactor.release (), true);
    }

  Server server (addr);

  if (use_reactor == Reactor_Type::NONE)
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
        case Reactor_Type::SELECT:
          // Run the reactor event loop.
          ACE_Reactor::run_event_loop ();
          break;
        case Reactor_Type::TP:
          ACE_Thread_Manager::instance ()->spawn_n (svr_thrno,
                                                    thread_pool_worker);
          ACE_Thread_Manager::instance ()->wait ();
          break;
        case Reactor_Type::WFMO:
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
  int c {};
  int dstport {DEFPORT};
  int const min_priority {ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)};
  int const max_priority {ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)};
  int priority {(min_priority + max_priority) / 2};
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);
  // Enable FIFO scheduling, e.g.
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
          VERBOSE = true;
          break;

        case 'h':
          dump_history = true;
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
          break;

        case 'i':
          nsamples = ACE_OS::atoi (get_opt.opt_arg ());
          if (nsamples <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nIterations must be greater than 0!\n\n"),
                              1);
          break;

        case 'a':
          use_reactor = Reactor_Type::SELECT;
          break;

        case 'x':
          use_reactor = Reactor_Type::TP;
          break;

        case 'w':
#if defined (ACE_WIN32)
          use_reactor = Reactor_Type::WFMO;
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
          server = false;
          client = true;
          break;
        case 's':
          client = false;
          server = true;
          break;
        default:
          usage ();
          return 1;
        }
    }

  if ((get_opt.opt_ind () >= argc && client) || argc == 1)
    {
      usage ();
      return 1;
    }

  ACE_INET_Addr addr (dstport);

  if (server)
    {
      return run_server (addr);
    }

  if (static_cast<size_t> (bufsz) < sizeof (ACE_hrtime_t))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "\nbufsz must be >= %B\n",
                         sizeof (ACE_hrtime_t)),
                        1);
    }

  ACE_INET_Addr remote_addr;

  if (ACE_OS::ace_isdigit(argv[get_opt.opt_ind ()][0]))
    {
      ACE_UINT32 const ip_address {
        static_cast<ACE_UINT32> (
          ACE_OS::inet_addr (
            ACE_TEXT_ALWAYS_CHAR (argv[get_opt.opt_ind ()])))};
      if (remote_addr.set (ACE_HTONS (dstport), ip_address, 0) == -1)
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
  ++get_opt.opt_ind ();

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
