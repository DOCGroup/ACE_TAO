//=============================================================================
/**
 *  @file    udp_test.cpp
 *
 *  Measures UDP round-trip performance.
 *
 *  @author Fred Kuhns and David L. Levine
 */
//=============================================================================
#include "ace/OS_main.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <array>
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// Global variables (evil).
static constexpr u_short DEFPORT = 5050;
static constexpr int MAXPKTSZ = 65536;
static constexpr int DEFPKTSZ = 64;
static constexpr int DEFITERATIONS = 1000;
static constexpr int DEFINTERVAL = 1000; // 1000 usecs.
static constexpr int DEFWINDOWSZ = 10; // 10 microsecond.
static std::array<char, MAXPKTSZ> SendBuf {};
static std::array<char, MAXPKTSZ> RxBuf {};
static ACE_TCHAR *const *cmd {};
static std::basic_string<ACE_TCHAR> datafile;

static int nsamples {DEFITERATIONS};
static int usdelay {DEFINTERVAL};
static int bufsz {DEFPKTSZ};
static int window {DEFWINDOWSZ};
static bool VERBOSE {};
static bool logfile {};
static bool server {};
static bool client {};
static bool use_reactor {};
static ACE_hrtime_t max_allow {};

static void
usage ()
{
  ACE_ERROR ((LM_ERROR,
              "%s\n"
              "[-w window_size]\n"
              "  [-f datafile] (creates datafile.samp and datafile.dist)\n"
              "  [-v]          (Verbose)\n"
              "  [-b send_bufsz]\n"
              "  [-n nsamples]\n"
              "  [-I usdelay]\n"
              "  [-s so_bufsz]\n"
              "  [-p port]\n"
              "  [-t]\n"
              "  [-r]\n"
              "  [-x max_sample_allowed]\n"
              "  [-a to use the ACE reactor]\n"
              "  targethost\n",
              *cmd));
}

class Client : public ACE_Event_Handler
{
public:
  Client (ACE_INET_Addr const &addr,
          ACE_INET_Addr const &remote_addr);

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
  ACE_SOCK_Dgram endpoint_;

  /// The address to send messages to.
  ACE_INET_Addr remote_addr_;

  Client () = delete;
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;
};

Client::Client (ACE_INET_Addr const &addr,
                ACE_INET_Addr const &remote_addr)
  : endpoint_ (addr),
    remote_addr_ (remote_addr)
{
  if (use_reactor)
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
  return endpoint_.get_handle ();
}

int
Client::handle_input (ACE_HANDLE)
{
  std::array<char, MAXPKTSZ> buf {};
  ACE_INET_Addr from_addr;

  ssize_t const n = endpoint_.recv (buf.data (), buf.size (), from_addr);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "handle_input"),
                      -1);
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
  return this->endpoint_.send (buf, len, remote_addr_);
}

int
Client::get_response (char *buf, size_t len)
{
  ACE_INET_Addr addr;
  return this->endpoint_.recv (buf, len, addr);
}

int
Client::run ()
{
  int i {};
  int j {};
  int n {};
  int maxindx {};
  int minindx {};
  char *const sbuf = SendBuf.data ();
  char *const rbuf = RxBuf.data ();

  ACE_High_Res_Timer timer;
  ACE_hrtime_t sample {};

  int d {};
  double std_dev {};
  double std_err {};
  double sample_mean {};

  bool tracking_last_over {};
  ACE_High_Res_Timer since_over;
  ACE_hrtime_t psum {};
  ACE_hrtime_t sum {};
  ACE_hrtime_t max {};
  ACE_hrtime_t min {};
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr sumfp {nullptr, ACE_OS::fclose};
  File_Ptr distfp {nullptr, ACE_OS::fclose};
  File_Ptr sampfp {nullptr, ACE_OS::fclose};
  pid_t const pid = ACE_OS::getpid ();
  int seq {};

  std::memset (sbuf, 0, bufsz);
  std::memset (rbuf, 0, bufsz);

  std::memcpy (sbuf, std::addressof (pid), sizeof pid);
  std::memcpy (sbuf + sizeof pid, std::addressof (seq), sizeof seq);

  ACE_DEBUG ((LM_DEBUG,
              "PID = %d, Starting SEQ = %d\n",
              pid,
              seq));

  // Allocate memory to hold samples.
  std::vector<ACE_hrtime_t> samples (nsamples);

  for (i = -1, seq = 0, j = 0;
       i < nsamples;
       ++seq, ++i, ++j, timer.reset ())
    {
      std::memcpy (sbuf + sizeof pid, std::addressof (seq), sizeof seq);
      if (i >= 0 && usdelay > 0)
        {
          ACE_Time_Value const delay {0, usdelay};
          ACE_OS::sleep (delay);
        }
      timer.start ();
      if (this->send (sbuf, bufsz) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "send"), -1);

      if ((n = get_response (rbuf, bufsz)) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P) %p\n", "get_response"), -1);

      timer.stop ();

      if (n <= 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nTrouble receiving from socket!\n\n"),
                          -1);

      timer.elapsed_time (sample);     // in nanoseconds.

      if (i < 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Ignoring first sample of %u usecs\n",
                       static_cast<ACE_UINT32> (sample)));
          continue;
        }
      else if (max_allow > 0  &&  sample > max_allow)
        {
          ACE_DEBUG ((LM_DEBUG, "Sample # %i = "
                      "%u msec is over the limit (%u)!\n",
                      i,
                      static_cast<ACE_UINT32> (sample / 1000000),
                      static_cast<ACE_UINT32> (max_allow / 1000000)));

          if (tracking_last_over)
            {
              since_over.stop ();
              ACE_Time_Value over_time;
              since_over.elapsed_time (over_time);
              ACE_DEBUG ((LM_DEBUG,
                          "\tTime since last over = %u msec!\n",
                          over_time.msec ()));
              since_over.reset ();
            }

          tracking_last_over = true;
          since_over.start ();
          --i;
          continue;
        }

      samples[i] = sample;
      sum += sample;

      if (i == 0)
        {
          max = sample;
          maxindx = i;
          min = sample;
          minindx = i;
        }
      else
        {
          if (sample > max)
            {
              max = sample;
              maxindx = i;
            }
          if (sample < min)
            {
              min = sample;
              minindx = i;
            }
        }

      if (VERBOSE)
        {
          psum += sample;
          if (j == 500)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%i) Partial (running) mean %u usecs\n",
                          i + 1,
                          static_cast<ACE_UINT32> (psum / (1000 * 500))));
              j = 0;
              psum = 0;
            }
        }
    }

  sample_mean = static_cast<double> (sum) / static_cast<double> (nsamples);

  if (logfile)
    {
      std::basic_string<ACE_TCHAR> const sumfile = datafile + ACE_TEXT (".sum");
      std::basic_string<ACE_TCHAR> const distfile = datafile + ACE_TEXT (".dist");
      std::basic_string<ACE_TCHAR> const sampfile = datafile + ACE_TEXT (".samp");

      distfp.reset (ACE_OS::fopen (distfile.c_str (), ACE_TEXT("w")));

      if (!distfp)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Unable to open dist file!\n\n"));
          logfile = false;
        }
      if (logfile)
        {
          sampfp.reset (ACE_OS::fopen (sampfile.c_str (), ACE_TEXT("w")));
          if (!sampfp)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unable to open sample file!\n\n"));
              logfile = false;
            }
        }
      if (logfile)
        {
          sumfp.reset (ACE_OS::fopen (sumfile.c_str (), ACE_TEXT("w")));
          if (!sumfp)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unable to open sample file!\n\n"));
              logfile = false;
            }
        }
    }

  ACE_hrtime_t const window_ns {static_cast<ACE_hrtime_t> (window) * 1000};
  int const ndist {static_cast<int> ((max - min) / window_ns) + 1};
  std::vector<u_int> dist (ndist);

  for (i = 0; i < nsamples; ++i)
    {
      double const difference {static_cast<double> (samples[i]) - sample_mean};
      std_dev += difference * difference;
      d = static_cast<int> ((samples[i] - min) / window_ns);

      if (d < 0 || d >= ndist)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nError indexing into dist array %d (%d)\n\n",
                      d,
                      ndist));
          ACE_OS::exit (1);
        }
      ++dist[d];
      if (logfile)
        ACE_OS::fprintf (sampfp.get (),
                         "%u\n",
                         ACE_U64_TO_U32 (samples[i]));
    }

  if (logfile)
    {
      ACE_hrtime_t tmp;
      tmp = min + (window_ns / 2);

      for (i = 0; i < ndist; ++i)
        {
          ACE_OS::fprintf (distfp.get (),
                           "%u %d\n",
                           static_cast<ACE_UINT32> (tmp / 1000),
                           dist[i]);
          tmp += window_ns;
        }
    }

  std_dev = std::sqrt (std_dev / static_cast<double> (nsamples - 1));
  std_err = std_dev / std::sqrt (static_cast<double> (nsamples));

  ACE_DEBUG ((LM_DEBUG,
              "\nResults for %i samples (usec):\n"
              "\tSample Mean = %f,\n"
              "\tSample Max = %u, Max index = %d,\n"
              "\tSample Min = %u, Min index = %d,\n"
              "\tStandard Deviation = %f,\n"
              "\tStandard Error = %f\n",
              nsamples,
              sample_mean / 1000.0,
              static_cast<u_int> (max / 1000),
              maxindx,
              static_cast<u_int> (min / 1000),
              minindx,
              std_dev / 1000.0,
              std_err / 1000.0));

  if (logfile)
    {
      ACE_OS::fprintf (sumfp.get (),
                       "Command executed:\n");
      for (ACE_TCHAR *const *arg = cmd; *arg; ++arg)
        ACE_OS::fprintf (sumfp.get (),
                         "%s ",
                         ACE_TEXT_ALWAYS_CHAR (*arg));
      ACE_OS::fprintf (sumfp.get (),
                       "\n");

      ACE_OS::fprintf (sumfp.get (),
                       "\nResults for %i samples (usec):"
                       "\tSample Mean = %f,\n"
                       "\tSample Max = %u, Max index = %d,\n"
                       "\tSample Min = %u, Min index = %d,\n"
                       "\tStandard Deviation = %f,\n"
                       "\tStandard Error = %f\n",
                       nsamples,
                       sample_mean / 1000.0,
                       static_cast<ACE_UINT32> (max / 1000),
                       maxindx,
                       static_cast<ACE_UINT32> (min / 1000),
                       minindx,
                       std_dev / 1000.0,
                       std_err / 1000.0);
    }

  return 0;
}

int
Client::shutdown ()
{
  char const buf = 'S';
  int const n = endpoint_.send (std::addressof (buf), 1u, remote_addr_);

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
  ACE_SOCK_Dgram endpoint_;

  Server () = delete;
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;
};

Server::Server (ACE_INET_Addr const &addr)
  : endpoint_ (addr)
{
  if (use_reactor)
    {
      if (ACE_Reactor::instance ()->register_handler
          (this,
           ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    "ACE_Reactor::register_handler: Server\n"));
    }
}

Server::~Server ()
{
  this->endpoint_.close ();
}

ACE_HANDLE
Server::get_handle () const
{
  return endpoint_.get_handle ();
}

int
Server::handle_input (ACE_HANDLE)
{
  std::array<char, MAXPKTSZ> buf {};
  ACE_INET_Addr from_addr;

  ssize_t const n = endpoint_.recv (buf.data (), buf.size (), from_addr);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "handle_input: recv"),
                      -1);

  // Send the message back as the response.
  if (endpoint_.send (buf.data (), n, from_addr) == n)
    {
      if (n == 1 && buf[0] == 'S')
        {
          if (use_reactor)
            {
              if (ACE_Reactor::instance ()->remove_handler
                  (this, ACE_Event_Handler::READ_MASK) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "ACE_Reactor::remove_handler: server\n"),
                                  -1);

              ACE_Reactor::end_event_loop ();
            }
          else
            {
              // Indicate done by returning 1.
              return 1;
            }
        }

      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "%p\n",
                  "handle_input: send"));
      return -1;
    }
}

int
Server::handle_close (ACE_HANDLE,
                      ACE_Reactor_Mask)
{
  endpoint_.close ();

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int c {};
  int dstport {DEFPORT};
  int so_bufsz {};

  cmd = argv;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("x:w:f:vs:I:p:rtn:b:a"));

  while ((c = getopt ()) != -1)
    {
  //FUZZ: enable check_for_lack_ACE_OS
      switch (static_cast<char> (c))
        {
        case 'x':
          max_allow = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'w':
          window = ACE_OS::atoi (getopt.opt_arg ());
          if (window < 2)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Invalid window!\n\n"),
                              1);
          break;
        case 'f':
          datafile = getopt.opt_arg ();
          logfile = true;
          break;
        case 'v':
          VERBOSE = true;
          break;
        case 'b':
          bufsz = ACE_OS::atoi (getopt.opt_arg ());

          if (bufsz <= 0 || bufsz > MAXPKTSZ)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nBuffer size must be between 1 and %d!\n\n",
                               MAXPKTSZ),
                              1);
          break;
        case 'n':
          nsamples = ACE_OS::atoi (getopt.opt_arg ());
          if (nsamples <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nIterations must be greater than 0!\n\n"),
                              1);
          break;
        case 'a':
          use_reactor = true;
          break;
        case 's':
          so_bufsz = ACE_OS::atoi (getopt.opt_arg ());

          if (so_bufsz <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nInvalid socket buffer size!\n\n"),
                              1);
          break;
        case 'I':
          usdelay = ACE_OS::atoi (getopt.opt_arg ());

          if (usdelay <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: bad usdelay: %s\n",
                               argv[0],
                               getopt.opt_arg ()),
                              1);
          break;
        case 'p':
          dstport = ACE_OS::atoi (getopt.opt_arg ());
          if (dstport <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "\nInvalid port number!\n\n"),
                              1);
          break;
        case 't':
          server = false;
          client = true;
          break;
        case 'r':
          client = false;
          server = true;
          break;
        default:
          usage ();
          return 1;
        }
    }

  if ((getopt.opt_ind () >= argc && client) || argc == 1)
    {
      usage ();
      return 1;
    }

  ACE_INET_Addr addr (server ? dstport : dstport + 1);

  if (server)
    {
      Server server (addr);

      if (use_reactor)
        {
          ACE_Reactor::run_event_loop ();
        }
      else
        {
          // Handle input in the current thread.
          int result {};
          while ((result = server.handle_input (0)) == 0)
            {
              continue;
            }
          if (result == -1)
            return 1;
        }
    }
  else
    {
      if (static_cast<size_t> (bufsz) < sizeof (pid_t) + sizeof (int))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nbufsz must be >= %B\n",
                           sizeof (pid_t) + sizeof (int)),
                          1);
      ACE_INET_Addr remote_addr;

      if (remote_addr.set (dstport, argv[getopt.opt_ind ()]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid IP address: %s\n",
                           argv[getopt.opt_ind ()]),
                          1);
      ++getopt.opt_ind ();

      Client client (addr, remote_addr);

      ACE_DEBUG ((LM_DEBUG,
                  "\nSending %d byte packets to %s:%d "
                  "with so_bufsz = %d\n\n",
                  bufsz,
                  addr.get_host_name (),
                  dstport,
                  so_bufsz));

      int const run_status = client.run ();
      int const shutdown_status = client.shutdown ();
      if (run_status == -1 || shutdown_status == -1)
        return 1;
    }

  return 0;
}
