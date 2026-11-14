/*
 *    T T C P . C
 *
 * Test TCP connection.  Makes a connection on port 5001
 * and transfers fabricated buffers or data copied from stdin.
 *
 * Usable on 4.2, 4.3, and 4.1a systems by defining one of
 * BSD42 BSD43 (BSD41a)
 * Machines using System V with BSD sockets should define SYSV.
 *
 * Modified for operation under 4.2BSD, 18 Dec 84
 *      T.C. Slattery, USNA
 * Minor improvements, Mike Muuss and Terry Slattery, 16-Oct-85.
 * Modified in 1989 at Silicon Graphics, Inc.
 *      catch SIGPIPE to be able to print stats when receiver has died
 *      for tcp, don't look for sentinel during reads to allow small transfers
 *      increased default buffer size to 8K, nbuf to 2K to transfer 16MB
 *      moved default port to 5001, beyond IPPORT_USERRESERVED
 *      make sinkmode default because it is more popular,
 *              -s now means don't sink/source
 *      count number of read/write system calls to see effects of
 *              blocking from full socket buffers
 *      for tcp, -D option turns off buffered writes (sets TCP_NODELAY sockopt)
 *      buffer alignment options, -A and -O
 *      print stats in a format that's a bit easier to use with grep & awk
 *      for SYSV, mimic BSD routines to use most of the existing timing code
 * Modified by Steve Miller of the University of Maryland, College Park
 *      -b sets the socket buffer size (SO_SNDBUF/SO_RCVBUF)
 * Modified Sept. 1989 at Silicon Graphics, Inc.
 *      restored -s sense at request of tcs@brl
 * Modified Oct. 1991 at Silicon Graphics, Inc.
 *      use getopt(3) for option processing, add -f and -T options.
 *      SGI IRIX 3.3 and 4.0 releases don't need #define SYSV.
 * Modified Aug.1993 at University Paderborn, Germany
 *  some SVR4 changes and time functions changed to itimer() calls
 * Modified by Douglas C. Schmidt September 28, 1994
 *  added support for testing UNIX domain socket performance
 * Modified by Tim Harrison May, 1995
 *  added support for ACE wrappers
 * Distribution Status -
 *      Public Domain.  Distribution Unlimited.
 */

/* #define BSD43 */
/* #define BSD42 */
/* #define BSD41a */
// #define SYSV /* required on SGI IRIX releases before 3.3 */

#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"

#include <cstdio>
#include <csignal>
#include <cctype>
#include <cerrno>
#include <limits>
#include <memory>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h> /* struct itimerval */
#include <climits>
#include <sys/un.h>
#include <unistd.h>

ACE_SOCK_Connector connector_factory;
ACE_SOCK_Acceptor acceptor_factory;
ACE_INET_Addr address;

#if defined(SYSV)
#define bcopy(b1,b2,n)  std::memcpy(b2,b1,n)
#define bzero(b1,n)             std::memset(b1,0,n)
#include <sys/times.h>
#include <sys/param.h>

struct rusage
  {
    struct timeval ru_utime, ru_stime;
  };
#define RUSAGE_SELF 0

#else
#include <sys/resource.h>
#endif

struct Session_Control_Message
{
  long nbuf_;
  // number of buffers that will be sent this round.
  long size_;
  // size of the buffers that will be sent
} session_control_buf;

int domain = PF_INET;           /* Default is to use Internet domain sockets. */
char const *domainname {};

int data_buf_len = 1024 * 1024 * 2;  // length of data portion
long total_msg_len;                  // length of entire message
int nbuf = 2 * 1024;      /* number of buffers to send in sinkmode */

int bufoffset = 0;        /* align buffer to this */
int bufalign = 16 * 1024; /* modulo this */

int udp = 0;              /* 0 = tcp, !0 = udp */
int options = 0;          /* socket options */
int one = 1;              /* for 4.3 BSD style setsockopt() */
unsigned short port {5001}; /* TCP port number */
char const *host {};
int trans {};
int sinkmode = 0;         /* 0=normal I/O, !0=sink/source mode */
int verbose = 0;          /* 0=print basic info, 1=print cpu rate, proc
                           * resource usage. */
int nodelay = 0;          /* set TCP_NODELAY socket option */
int b_flag = 0;           /* use mread() */
int sockbufsize = 0;      /* socket buffer size to use */
char fmt = 'K';           /* output format: k = kilobits, K = kilobytes,
                           * m = megabits, M = megabytes,
                           * g = gigabits, G = gigabytes */
int touchdata = 0;        /* access data after reading */

std::unique_ptr<char, decltype (&std::free)> message_buf {nullptr, &std::free};

static const char Usage[] = "\
Usage: ttcp -t [-options] host [ < in ]\n\
       ttcp -r [-options > out]\n\
Common options:\n\
        -l ##   length of bufs read from or written to network (default 8192)\n\
        -u      use UDP instead of TCP\n\
        -U      use UNIX domain sockets instead of Internet domain sockets\n\
        -p ##   port number to send to or listen at (default 5001)\n\
        -s      -t: source a pattern to network\n\
                -r: sink (discard) all data from network\n\
        -A      align the start of buffers to this modulus (default 16384)\n\
        -O      start buffers at this offset from the modulus (default 0)\n\
        -v      verbose: print more statistics\n\
        -d      set SO_DEBUG socket option\n\
        -b ##   set socket buffer size (if supported)\n\
        -f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
Options specific to -t:\n\
        -n##    number of source bufs written to network (default 2048)\n\
        -D      don't buffer TCP writes (sets TCP_NODELAY socket option)\n\
Options specific to -r:\n\
        -B      for -s, only output full blocks as specified by -l (for TAR)\n\
        -T      \"touch\": access each byte as it's read\n\
";

char stats[128];
unsigned long nbytes;         /* bytes on net */
unsigned long numCalls = 0;   /* # of I/O system calls */
double cput, realt;           /* user, real time (seconds) */

void err (char const *s);
void mes (char const *s);
void pattern (char *cp, int cnt);
char *outfmt (double b);
void prep_timer ();
double read_timer (char *str, int len);
static void prusage (struct rusage const &r0, struct rusage const &r1,
                     struct timeval const &e, struct timeval const &b, char *outp);
static void tvadd (struct timeval &tsum, struct timeval const &t0, struct timeval const &t1);
static void tvsub (struct timeval &tdiff, struct timeval const &t1, struct timeval const &t0);
static void psecs (long l, char *cp);
int Nwrite (ACE_SOCK_Stream &s, void *buf, int count);

static bool
parse_int_option (char const *argument, int &value)
{
  char *end {};
  errno = 0;
  long const parsed = std::strtol (argument, &end, 10);
  if (argument == end || *end != '\0' || errno == ERANGE
      || parsed < std::numeric_limits<int>::min ()
      || parsed > std::numeric_limits<int>::max ())
    return false;

  value = static_cast<int> (parsed);
  return true;
}

void
sigpipe (int signal_number)
{
  std::printf ("Caught signal %d\n", signal_number);
}

char const *title {};
int new_line = 0;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_SOCK_Stream connection_stream;
  int c {};
  long message_payload_len {};

  std::printf("HZ = %d\n", HZ);
  if (argc < 2)
    goto usage;

  while ((c = getopt (argc, argv, "drstU:uvBDTb:f:l:n:p:A:O:L:xh:")) != -1)
    {
      switch (c)
        {
        case 'h':
          host = optarg;
          break;
        case 'x':
          new_line = 1;
          break;
        case 'L':
          title = optarg;
          break;
        case 'B':
          b_flag = 1;
          break;
        case 't':
          trans = 1;
          break;
        case 'r':
          trans = 0;
          break;
        case 'd':
          options |= SO_DEBUG;
          break;
        case 'D':
        #ifdef TCP_NODELAY
          nodelay = 1;
        #else
          std::fprintf (stderr,
                   "ttcp: -D option ignored: TCP_NODELAY socket option not supported\n");
        #endif
          break;
        case 'n':
          if (!parse_int_option (optarg, nbuf) || nbuf <= 0)
            goto usage;
          break;
        case 'l':
          if (!parse_int_option (optarg, data_buf_len) || data_buf_len <= 0)
            goto usage;
          break;
        case 's':
          sinkmode = !sinkmode;
          break;
        case 'p':
          {
            int parsed_port {};
            if (!parse_int_option (optarg, parsed_port)
                || parsed_port <= 0
                || parsed_port > std::numeric_limits<unsigned short>::max ())
              goto usage;
            port = static_cast<unsigned short> (parsed_port);
          }
          break;
        case 'U':
          domain = PF_UNIX;
          domainname = optarg;
          break;
        case 'u':
          udp = 1;
          break;
        case 'v':
          verbose = 1;
          break;
        case 'A':
          if (!parse_int_option (optarg, bufalign) || bufalign <= 0)
            goto usage;
          break;
        case 'O':
          if (!parse_int_option (optarg, bufoffset))
            goto usage;
          break;
        case 'b':
        #if defined(SO_SNDBUF) || defined(SO_RCVBUF)
          if (!parse_int_option (optarg, sockbufsize) || sockbufsize <= 0)
            goto usage;
        #else
          std::fprintf (stderr,
                   "ttcp: -b option ignored: SO_SNDBUF/SO_RCVBUF socket options not supported\n");
        #endif
          break;
        case 'f':
          fmt = *optarg;
          break;
        case 'T':
          touchdata = 1;
          break;

        default:
          goto usage;
      }
    }

  if (trans && host == nullptr && optind < argc)
    host = argv[optind];
  if ((trans && domain == PF_INET && host == nullptr)
      || (domain == PF_UNIX && domainname == nullptr))
    goto usage;

  /* if transmitter, create remote address to transmit to.  */

  if (trans)
    {
      if (address.set (port, host) == -1)
        std::perror ("address.set"), std::exit (1);
    }

  /* else, receiver create address to listen on */
  else
    {
      address.set (port);
    }

  if (data_buf_len > LONG_MAX - static_cast<long> (sizeof (long)))
    goto usage;
  total_msg_len = static_cast<long> (sizeof (long)) + data_buf_len;
  message_payload_len = data_buf_len;

  // allocate the buffer
  message_buf.reset (static_cast<char *> (
                       std::malloc (static_cast<size_t> (total_msg_len))));
  if (!message_buf)
    err ("malloc");

//  if (bufalign != 0)
//    message_buf += (bufalign - ((int) message_buf % bufalign) + bufoffset) % bufalign;

  // let's go ahead and set the control message for every send right now
  std::memcpy (message_buf.get (), &message_payload_len, sizeof (message_payload_len));

  session_control_buf.nbuf_ = nbuf;
  session_control_buf.size_ = data_buf_len;

  //
  // print out option values for trans and receiver
  //

  if (trans)
    {
      std::fprintf (stdout,
               "ttcp-t: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%hu",
               data_buf_len, nbuf, bufalign, bufoffset, port);
      if (sockbufsize)
        std::fprintf (stdout, ", sockbufsize=%d", sockbufsize);
      std::fprintf (stdout, "  %s  -> %s\n",
               domain == PF_INET ? (udp ? "udp" : "tcp") : "unix",
               host == nullptr ? domainname : host);
    }
  else // receiver
    {
      std::fprintf (stdout,
               "ttcp-r: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%hu",
               data_buf_len, nbuf, bufalign, bufoffset, port);
      if (sockbufsize)
        std::fprintf (stdout, ", sockbufsize=%d", sockbufsize);
      std::fprintf (stdout, "  %s\n", domain == PF_INET ? (udp ? "udp" : "tcp") : "unix");
    }

  mes ("socket");

  //
  // connect and accept
  //

  if (!udp)
    {
      std::signal (SIGPIPE, sigpipe);

      /* the transmitter will set options and connect to receiver */
      if (trans)
        {
          // turn off weird ack things
          if (nodelay)
            {
              struct protoent const * const p = getprotobyname ("tcp");

              if (p && connection_stream.set_option (p->p_proto,
                                                    TCP_NODELAY,
                                                    reinterpret_cast<char *> (std::addressof (one)),
                                                    sizeof (one)))
                err ("setsockopt: nodelay");
              mes ("nodelay");
            }
          if (connector_factory.connect (connection_stream, address) == -1)
            std::perror ("connection failed"), std::exit (1);
          std::fprintf (stdout,
                  "ttcp-t: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%hu",
                  data_buf_len, nbuf, bufalign, bufoffset, port);

          if (sockbufsize)
            {
              if (connection_stream.set_option (SOL_SOCKET,
                                                SO_SNDBUF,
                                                reinterpret_cast<char *> (std::addressof (sockbufsize)),
                                                sizeof sockbufsize) == -1)
                err ("acceptor_factory.set_option");
              mes ("sndbuf");
            }
        }

      /* receiver will listen for connections from the transmitter */
      else
        {
          if (acceptor_factory.open (address, 1) == -1)
            std::perror ("acceptor open"), std::exit (1);

          if (sockbufsize)
            {
              if (connection_stream.set_option (SOL_SOCKET,
                                                SO_RCVBUF,
                                                reinterpret_cast<char *> (std::addressof (sockbufsize)),
                                                sizeof sockbufsize) == -1)
                err ("acceptor_factory.set_option");
              mes ("rcvbuf");
            }

          ACE_INET_Addr remote_address;

          if (acceptor_factory.accept (connection_stream,
                                      std::addressof (remote_address)) == -1)
            std::perror ("acceptor accept"), std::exit (1);

          // set the window size

          std::fprintf (stderr,
                  "ttcp-r: accept from %s\n",
                  remote_address.get_host_name());
        }
    }

  //
  // start timer
  //

  errno = 0;
  if (trans)
    {
      pattern (message_buf.get () + sizeof (long), data_buf_len);
      prep_timer ();

      ACE_DEBUG ((LM_DEBUG, "Sending session control message"
                  " nbuf %d, size %d\n", session_control_buf.nbuf_,
                  session_control_buf.size_));
      if (connection_stream.send_n (std::addressof (session_control_buf),
                                    sizeof (Session_Control_Message))
          != sizeof (Session_Control_Message))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p send session control failed\n",
                           "ttcp"),
                          -1);

      long ack {};
      for (int buffer_index {}; buffer_index < nbuf; ++buffer_index)
        {
          ssize_t const send_result =
            connection_stream.send_n (message_buf.get (), total_msg_len);
          if (send_result != total_msg_len)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p only sent %b of %B bytes on call %B\n",
                               "ttcp", send_result,
                               static_cast<size_t> (total_msg_len),
                               static_cast<size_t> (numCalls + 1)),
                              -1);
          ++numCalls;
          nbytes += data_buf_len;

          if (connection_stream.recv_n (std::addressof (ack), sizeof ack) != sizeof ack)
            ACE_ERROR_RETURN ((LM_ERROR, "%p recv of ack failed\n", "ttcp"), -1);

          if (ack != data_buf_len)
            ACE_DEBUG ((LM_DEBUG, "received ack for only %Q bytes\n",
                        static_cast<ACE_INT64> (ack)));
        }
      std::printf("Client finished.\n");
    }
  else
    {
      prep_timer ();

      if (connection_stream.recv_n (std::addressof (session_control_buf),
                                    sizeof (Session_Control_Message))
          != sizeof (Session_Control_Message))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p recv session control failed\n",
                           "ttcp"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "received session control message"
                  " nbuf %d, size %d\n",
                  static_cast<int> (session_control_buf.nbuf_),
                  static_cast<int> (session_control_buf.size_)));

      if (session_control_buf.nbuf_ <= 0
          || session_control_buf.nbuf_ > std::numeric_limits<int>::max ()
          || session_control_buf.size_ <= 0
          || session_control_buf.size_ > data_buf_len)
        ACE_ERROR_RETURN ((LM_ERROR, "invalid session control message\n"), -1);

      nbuf = session_control_buf.nbuf_;

      for (int buffer_index {}; buffer_index < nbuf; ++buffer_index)
        {
          if (connection_stream.recv_n (message_buf.get (), sizeof (long))
              != sizeof (long))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p recv data control failed\n",
                               "ttcp"),
                              -1);

          long payload_length {};
          std::memcpy (&payload_length, message_buf.get (), sizeof (payload_length));
          if (payload_length <= 0 || payload_length > session_control_buf.size_)
            ACE_ERROR_RETURN ((LM_ERROR, "invalid data control message\n"), -1);

          ssize_t const count =
            connection_stream.recv_n (message_buf.get () + sizeof (long),
                                      static_cast<size_t> (payload_length));
          if (count != payload_length)
            ACE_ERROR_RETURN ((LM_ERROR, "recv data failed\n"), -1);

          ++numCalls;
          nbytes += count;

          long const ack = static_cast<long> (count);
          if (connection_stream.send_n (std::addressof (ack), sizeof ack)
              != sizeof ack)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p send ack failed\n",
                               "ttcp"),
                              -1);
        }
      std::printf("Server finished.\n");
    }

  /*  if (errno)
    err ("IO");
    */
  //
  // stop the timer
  //

  (void) read_timer (stats, sizeof (stats));
  if (udp && trans)
    {
      (void) Nwrite (connection_stream, message_buf.get (), 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf.get (), 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf.get (), 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf.get (), 4); /* rcvr end */
    }
  if (cput <= 0.0)
    cput = 0.001;
  if (realt <= 0.0)
    realt = 0.001;

#if defined (LM_RESULTS)
  if (trans && title != nullptr)
    {
      double tmp;
      FILE *fd;
      char filename[BUFSIZ];
      std::sprintf (filename, "%s.results", title);
      fd = std::fopen(filename,"a+");
      if (new_line)
        std::fprintf(fd,"\n  -l %ldk \t", data_buf_len/1024);
      tmp = ((double) nbytes) / realt;
      std::fprintf(fd,"%.2f ", tmp * 8.0 / 1024.0 / 1024.0);
      std::fclose(fd);
    }
#endif

  std::fprintf (stdout,
           "ttcp%s: %lu bytes in %.2f real seconds = %s/sec +++\n",
           trans ? "-t" : "-r",
           nbytes, realt, outfmt (((double) nbytes) / realt));
  if (verbose)
    {
      std::fprintf (stdout,
               "ttcp%s: %lu bytes in %.2f CPU seconds = %s/cpu sec\n",
               trans ? "-t" : "-r",
               nbytes, cput, outfmt (((double) nbytes) / cput));
    }
  std::fprintf (stdout,
           "ttcp%s: %lu I/O calls, msec/call = %.2f, calls/sec = %.2f\n",
           trans ? "-t" : "-r",
           numCalls,
           1024.0 * realt / ((double) numCalls),
           ((double) numCalls) / realt);
  std::fprintf (stdout, "ttcp%s: %s\n", trans ? "-t" : "-r", stats);
  if (verbose)
    {
      std::fprintf (stdout,
               "ttcp%s: buffer address %p\n",
               trans ? "-t" : "-r",
               static_cast<void *> (message_buf.get ()));
    }
  std::exit (0);

usage:
  std::fputs (Usage, stderr);
  return 1;
}

void
err (char const *s)
{
  std::fprintf (stderr, "ttcp%s: ", trans ? "-t" : "-r");
  std::perror (s);
  std::fprintf (stderr, "errno=%d\n", errno);
  std::exit (1);
}

void
mes (char const *s)
{
  std::fprintf (stderr, "ttcp%s: %s\n", trans ? "-t" : "-r", s);
}

void
pattern (char *cp, int cnt)
{
  unsigned char c {};
  while (cnt-- > 0)
    {
      while (!std::isprint (c & 0x7F))
        ++c;
      *cp = static_cast<char> (c & 0x7F);
      ++cp;
      ++c;
    }
}

char *
outfmt (double b)
{
  static char obuf[50];
  switch (fmt)
    {
    case 'G':
      std::sprintf (obuf, "%.2f GB", b / 1024.0 / 1024.0 / 1024.0);
      break;
    default:
    case 'K':
      std::sprintf (obuf, "%.2f KB", b / 1024.0);
      break;
    case 'M':
      std::sprintf (obuf, "%.2f MB", b / 1024.0 / 1024.0);
      break;
    case 'g':
      std::sprintf (obuf, "%.2f Gbit", b * 8.0 / 1024.0 / 1024.0 / 1024.0);
      break;
    case 'k':
      std::sprintf (obuf, "%.2f Kbit", b * 8.0 / 1024.0);
      break;
    case 'm':
      std::sprintf (obuf, "%.2f Mbit", b * 8.0 / 1024.0 / 1024.0);
      break;
    }
  return obuf;
}

static struct itimerval itime0; /* Time at which timing started */
static struct rusage ru0;       /* Resource utilization at the start */

#if defined(SYSV)
/*ARGSUSED */
static void
getrusage (int ignored, struct rusage *ru)
{
  struct tms buf;

  times (&buf);

  /* Assumption: HZ <= 2147 (LONG_MAX/1000000) */
  ru->ru_stime.tv_sec = buf.tms_stime / HZ;
  ru->ru_stime.tv_usec = ((buf.tms_stime % HZ) * 1000000) / HZ;
  ru->ru_utime.tv_sec = buf.tms_utime / HZ;
  ru->ru_utime.tv_usec = ((buf.tms_utime % HZ) * 1000000) / HZ;
}

#endif /* SYSV */
/*
 *                    P R E P _ T I M E R
 */
void
prep_timer ()
{
  itime0.it_interval.tv_sec = 0;
  itime0.it_interval.tv_usec = 0;
  //  itime0.it_value.tv_sec = LONG_MAX / 22; /* greatest possible value , itimer() count backwards */
  itime0.it_value.tv_sec = 3600;
  itime0.it_value.tv_usec = 0;


  getrusage (RUSAGE_SELF, std::addressof (ru0));
  std::fprintf (stdout, "\n");
  std::fprintf (stdout, "beginning user time = %lld sec and %lld usec\n",
           static_cast<long long> (ru0.ru_utime.tv_sec),
           static_cast<long long> (ru0.ru_utime.tv_usec));
  std::fprintf (stdout, "beginning sys time = %lld sec and %lld usec\n",
           static_cast<long long> (ru0.ru_stime.tv_sec),
           static_cast<long long> (ru0.ru_stime.tv_usec));

  /* Init REAL Timer */
  if (setitimer (ITIMER_REAL, std::addressof (itime0), nullptr))
    {
      std::perror ("Setting 'itimer' REAL failed");
      return;
    }
  std::fprintf (stdout, "Beginning transaction time = %lld sec and %lld usec\n",
           static_cast<long long> (itime0.it_value.tv_sec),
           static_cast<long long> (itime0.it_value.tv_usec));
}

/*
 *                    R E A D _ T I M E R
 */
double
read_timer (char *str, int len)
{
  struct itimerval itimedol;
  struct rusage ru1;
  struct timeval td;
  struct timeval tend, tstart;
  char line[132];

  getrusage (RUSAGE_SELF, std::addressof (ru1));
  std::fprintf (stdout, "final user time = %lld sec and %lld usec\n",
           static_cast<long long> (ru1.ru_utime.tv_sec),
           static_cast<long long> (ru1.ru_utime.tv_usec));
  std::fprintf (stdout, "final sys time = %lld sec and %lld usec\n",
           static_cast<long long> (ru1.ru_stime.tv_sec),
           static_cast<long long> (ru1.ru_stime.tv_usec));

  if (getitimer (ITIMER_REAL, std::addressof (itimedol)))
    {
      std::perror ("Getting 'itimer' REAL failed");
      return (0.0);
    }
  std::fprintf (stdout, "End transaction time = %lld sec and %lld usec\n",
           static_cast<long long> (itimedol.it_value.tv_sec),
           static_cast<long long> (itimedol.it_value.tv_usec));
  prusage (ru0, ru1, itime0.it_value, itimedol.it_value, line);
  if (len > 0)
    std::snprintf (str, static_cast<size_t> (len), "%s", line);

  /* Get real time */
  tvsub (td, itime0.it_value, itimedol.it_value);
  realt = td.tv_sec + ((double) td.tv_usec) / 1000000;

  /* Get CPU time (user+sys) */
  tvadd (tend, ru1.ru_utime, ru1.ru_stime);
  tvadd (tstart, ru0.ru_utime, ru0.ru_stime);
  tvsub (td, tend, tstart);
  cput = td.tv_sec + ((double) td.tv_usec) / 1000000;
  if (cput < 0.00001)
    cput = 0.00001;
  return (cput);
}

static void
prusage (struct rusage const &r0, struct rusage const &r1,
         struct timeval const &b, struct timeval const &e, char *outp)
{
  struct timeval tdiff;
  time_t t;
  char const *cp;
  int i;
  int ms;

  t = (r1.ru_utime.tv_sec - r0.ru_utime.tv_sec) * 1000 +
      (r1.ru_utime.tv_usec - r0.ru_utime.tv_usec) / 100000 +
      (r1.ru_stime.tv_sec - r0.ru_stime.tv_sec) * 1000 +
      (r1.ru_stime.tv_usec - r0.ru_stime.tv_usec) / 100000;
  ms = -((e.tv_sec - b.tv_sec) * 1000 + (e.tv_usec - b.tv_usec) / 1000);/* in milliseconds */

#define END(x)  {while(*x) ++x;}
#if defined(SYSV)
  cp = "%Uuser %Ssys %Ereal %P";
#else
  cp = "%Uutime %Sstime %Edtime %P cpu occupancy";
  /*  cp = "%Uuser %Ssys %Ereal %P %Xi+%Dd %Mmaxrss %F+%Rpf %Ccsw";*/
#endif
  for (; *cp; ++cp)
    {
      if (*cp != '%')
        {
          *outp = *cp;
          ++outp;
        }
      else if (cp[1])
        switch (*++cp)
          {
          case 'U':
            tvsub (tdiff, r1.ru_utime, r0.ru_utime);
            /* std::sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec);*/
            std::sprintf (outp, "%f", (tdiff.tv_sec + tdiff.tv_usec/1000000.0));
            END (outp);
            break;

          case 'S':
            tvsub (tdiff, r1.ru_stime, r0.ru_stime);
            /* std::sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec);*/
                      std::sprintf (outp, "%f", (tdiff.tv_sec + tdiff.tv_usec/1000000.0));
            END (outp);
            break;

          case 'E':
            psecs (ms / 1000, outp);
            END (outp);
            break;

          case 'P':
            std::sprintf (outp, "%f%%",  (t * 100.0 / ((ms ? ms : 1))));
            END (outp);
            break;

        #if !defined(SYSV)
          case 'W':
            i = r1.ru_nswap - r0.ru_nswap;
            std::sprintf (outp, "%d", i);
            END (outp);
            break;

          case 'X':
            std::sprintf (outp, "%ld", t == 0 ? 0 : (r1.ru_ixrss - r0.ru_ixrss) / t);
            END (outp);
            break;

          case 'D':
            std::sprintf (outp, "%ld", t == 0 ? 0 :
                     (r1.ru_idrss + r1.ru_isrss - (r0.ru_idrss + r0.ru_isrss)) / t);
            END (outp);
          break;

          case 'K':
            std::sprintf (outp, "%ld", t == 0 ? 0 :
                     ((r1.ru_ixrss + r1.ru_isrss + r1.ru_idrss) -
                     (r0.ru_ixrss + r0.ru_idrss + r0.ru_isrss)) / t);
            END (outp);
          break;

          case 'M':
            std::sprintf (outp, "%ld", r1.ru_maxrss / 2);
            END (outp);
            break;

          case 'F':
            std::sprintf (outp, "%ld", r1.ru_majflt - r0.ru_majflt);
            END (outp);
            break;

          case 'R':
            std::sprintf (outp, "%ld", r1.ru_minflt - r0.ru_minflt);
            END (outp);
            break;

          case 'I':
            std::sprintf (outp, "%ld", r1.ru_inblock - r0.ru_inblock);
            END (outp);
            break;

          case 'O':
            std::sprintf (outp, "%ld", r1.ru_oublock - r0.ru_oublock);
            END (outp);
            break;

          case 'C':
            std::sprintf (outp, "%ld+%ld", r1.ru_nvcsw - r0.ru_nvcsw,
                     r1.ru_nivcsw - r0.ru_nivcsw);
            END (outp);
            break;
        #endif /* !SYSV */
          }
    }
  *outp = '\0';
}

static void
tvadd (struct timeval &tsum, struct timeval const &t0, struct timeval const &t1)
{
  tsum.tv_sec = t0.tv_sec + t1.tv_sec;
  tsum.tv_usec = t0.tv_usec + t1.tv_usec;
  if (tsum.tv_usec > 1000000)
    ++tsum.tv_sec, tsum.tv_usec -= 1000000;
}

static void
tvsub (struct timeval &tdiff, struct timeval const &t1, struct timeval const &t0)
{
  tdiff.tv_sec = t1.tv_sec - t0.tv_sec;
  tdiff.tv_usec = t1.tv_usec - t0.tv_usec;
  if (tdiff.tv_usec < 0)
    --tdiff.tv_sec, tdiff.tv_usec += 1000000;
}

static void
psecs (long l, char *cp)
{
  int i;

  i = l / 3600;
  if (i)
    {
      std::sprintf (cp, "%d hours", i);
      END (cp);
      i = l % 3600;
      std::sprintf (cp, "%d minutes ", (i / 60));
      std::sprintf (cp, "%d seconds ", (i % 60));
      END (cp);
    }
  else
    {
      i = l;
      std::sprintf (cp, "%d minutes ", i / 60);
      END (cp);
    }
  i %= 60;
  *cp = ':';
  ++cp;
  std::sprintf (cp, "%d seconds ", i);
}

int
Nwrite (ACE_SOCK_Stream &s, void *buf, int count)
{
  ++numCalls;
  return s.send (buf, count);
}
