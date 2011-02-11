// $Id$

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
#define SYSV /* required on SGI IRIX releases before 3.3 */

//FUZZ: disable check_for_lack_ACE_OS
//FUZZ: disable check_for_improper_main_declaration

#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h> /* struct itimerval */
#include <limits.h>
#include <sys/un.h>
#include <unistd.h>

ACE_SOCK_Connector connector_factory;
ACE_SOCK_Acceptor acceptor_factory;
ACE_INET_Addr address;

#if defined(SYSV)
#define bcopy(b1,b2,n)  memcpy(b2,b1,n)
#define bzero(b1,n)             memset(b1,0,n)
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

struct sockaddr_in sinme;
struct sockaddr_un sunme;
struct sockaddr_in sinhim;
struct sockaddr_un sunhim;
struct sockaddr_in frominet;
struct sockaddr_un fromunix;

int send_n (const void *buf, int len);
int recv_n (void *buf, int len);
int connection_descriptor;

struct Session_Control_Message
{
  long nbuf_;
  // number of buffers that will be sent this round.
  long size_;
  // size of the buffers that will be sent
} session_control_buf;

struct Data_Control_Message
{
  long size_;
  char data_;
} *message_buf;

int fromlen;
int domain = PF_INET;           /* Default is to use Internet domain sockets. */
char *domainname;               /* Rendezvous address for UNIX domain sockets. */
int fd;                         /* fd of network socket */

int data_buf_len = 1024 * 1024 * 2; // length of data portion
long total_msg_len;                 // length of entire message
char *data_buf;                     // pointer to data portion
int nbuf = 2 * 1024;                /* number of buffers to send in sinkmode */

int bufoffset = 0;        /* align buffer to this */
int bufalign = 16 * 1024; /* modulo this */

int udp = 0;          /* 0 = tcp, !0 = udp */
int options = 0;      /* socket options */
int one = 1;          /* for 4.3 BSD style setsockopt() */
short port = 5001;    /* TCP port number */
char *host;           /* ptr to name of host */
int trans;            /* 0=receive, !0=transmit mode */
int sinkmode = 0;     /* 0=normal I/O, !0=sink/source mode */
int verbose = 0;      /* 0=print basic info, 1=print cpu rate, proc
                       * resource usage. */
int nodelay = 0;      /* set TCP_NODELAY socket option */
int b_flag = 0;       /* use mread() */
int sockbufsize = 0;  /* socket buffer size to use */
char fmt = 'K';       /* output format: k = kilobits, K = kilobytes,
                       * m = megabits, M = megabytes,
                       * g = gigabits, G = gigabytes */
int touchdata = 0;    /* access data after reading */

struct hostent *addr;
extern int errno;
extern int optind;
extern char *optarg;

char Usage[] = "\
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
unsigned long nbytes;       /* bytes on net */
unsigned long numCalls = 0; /* # of I/O system calls */
double cput, realt;         /* user, real time (seconds) */

void err (char *s);
void mes (char *s);
void pattern (register char *cp, register int cnt);
char *outfmt (double b);
static void getrusage (int ignored, register struct rusage *ru);
void prep_timer (void);
double read_timer (char *str, int len);
static void prusage (register struct rusage *r0, struct rusage *r1, struct timeval *e, struct timeval *b, char *outp);
static void tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1);
static void tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
static void psecs (long l, register char *cp);
void delay (int us);
int mread (int fd, register char *bufp, unsigned n);
int Nread (ACE_SOCK_Stream &s, void *buf, int count);
int Nwrite (ACE_SOCK_Stream &s, void *buf, int count);

#if !defined (__cplusplus)
typedef void (*SIG_TYP)();
#endif

#ifdef SVR4
void
sigpipe (int foo)
#else
void
sigpipe ()
#endif
{
}

char *title = 0;
int new_line = 0;

int
main (int argc, char *argv[])
{
  ACE_SOCK_Stream connection_stream;
  unsigned long addr_tmp;
  int c;

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
          fprintf (stderr,
                   "ttcp: -D option ignored: TCP_NODELAY socket option not supported\n");
    #endif
          break;
        case 'n':
          nbuf = atoi (optarg);
          break;
        case 'l':
          data_buf_len = atoi (optarg);
          break;
        case 's':
          sinkmode = !sinkmode;
          break;
        case 'p':
          port = atoi (optarg);
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
          bufalign = atoi (optarg);
          break;
        case 'O':
          bufoffset = atoi (optarg);
          break;
        case 'b':
    #if defined(SO_SNDBUF) || defined(SO_RCVBUF)
          sockbufsize = atoi (optarg);
    #else
          fprintf (stderr,
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

  /* if transmitter, create remote address to transmit to.  */

  if (trans)
    {
      if (address.set (port, host) == -1)
        perror ("address.set"), exit (1);
    }

  /* else, receiver create address to listen on */
  else
    {
      address.set (port);
    }

  total_msg_len = sizeof (long) + data_buf_len;

  // allocate the buffer
  message_buf = (Data_Control_Message *) malloc (total_msg_len);
  if (message_buf == 0)
    err ("malloc");

  //  if (bufalign != 0)
  //    message_buf += (bufalign - ((int) message_buf % bufalign) + bufoffset) % bufalign;

  // let's go ahead and set the control message for every send right now
  message_buf->size_ = data_buf_len;

  session_control_buf.nbuf_ = nbuf;
  session_control_buf.size_ = data_buf_len;

  //
  // print out option values for trans and receiver
  //

  if (trans)
    {
      fprintf (stdout,
               "ttcp-t: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%d",
               data_buf_len, nbuf, bufalign, bufoffset, port);
      if (sockbufsize)
        fprintf (stdout, ", sockbufsize=%d", sockbufsize);
      fprintf (stdout, "  %s  -> %s\n",
               domain == PF_INET ? (udp ? "udp" : "tcp") : "unix",
        host == 0 ? domainname : host);
    }
  else // receiver
    {
      fprintf (stdout,
              "ttcp-r: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%d",
              data_buf_len, nbuf, bufalign, bufoffset, port);
      if (sockbufsize)
        fprintf (stdout, ", sockbufsize=%d", sockbufsize);
      fprintf (stdout, "  %s\n", domain == PF_INET ? (udp ? "udp" : "tcp") : "unix");
    }

  mes ("socket");

  //
  // connect and accept
  //

  if (!udp)
    {
      signal (SIGPIPE, (SIG_TYP) sigpipe);

      /* the transmitter will set options and connect to receiver */
      if (trans)
        {
          if (connector_factory.connect (connection_stream, address) == -1)
            perror ("connection failed"), exit (1);
          fprintf (stdout,
                  "ttcp-t: data_buf_len=%d, nbuf=%d, align=%d/%d, port=%d",
                  data_buf_len, nbuf, bufalign, bufoffset, port);

          // turn off weird ack things
          if (nodelay)
            {
              struct protoent *p = getprotobyname ("tcp");

              if (p && connection_stream.set_option (p->p_proto,
                                                    TCP_NODELAY,
                                                    (char *)& one,
                                                    sizeof (one)))
                err ("setsockopt: nodelay");
              mes ("nodelay");
            }
          if (sockbufsize)
            {
              if (connection_stream.set_option (SOL_SOCKET,
                                                SO_SNDBUF,
                                                (char *) &sockbufsize,
                                                sizeof sockbufsize) == -1)
                err ("acceptor_factory.set_option");
              mes ("sndbuf");
            }
        }

      /* receiver will listen for connections from the transmitter */
      else
        {
          if (acceptor_factory.open (address, 1) == -1)
            perror ("acceptor open"), exit (1);

          ACE_INET_Addr remote_address;

          if (acceptor_factory.accept (connection_stream,
                                      (ACE_Addr *) &remote_address) == -1)
            perror ("acceptor accept"), exit (1);

          // set the window size
          if (sockbufsize)
            {
              if (connection_stream.set_option (SOL_SOCKET,
                                                SO_RCVBUF,
                                                (char *) &sockbufsize,
                                                sizeof sockbufsize) == -1)
                err ("acceptor_factory.set_option");
              mes ("rcvbuf");
            }

          fprintf (stderr,
                  "ttcp-r: accept from %s\n",
                  remote_address.get_host_name());
        }
    }

  //
  // start timer
  //

  errno = 0;

  // used in send_n and recv_n
  connection_descriptor = connection_stream.get_handle ();

  if (trans)
    {
      pattern (& (message_buf->data_), data_buf_len);
      prep_timer ();

      ACE_DEBUG ((LM_DEBUG, "Sending session control message"
                  " nbuf %d, size %d\n", session_control_buf.nbuf_,
                  session_control_buf.size_));
      if (send_n ((char *) &session_control_buf,
                  sizeof (Session_Control_Message))
          != sizeof (Session_Control_Message))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p send session control failed\n",
                           "ttcp"),
                          -1);

      long ack;
      int send_result;
      while (nbuf--)
        {
          send_result = send_n ((char *) message_buf, total_msg_len);
          if (send_result != total_msg_len)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p only sent %d of %d bytes on call %d\n",
                               "ttcp", send_result, total_msg_len, numCalls + 1),
                              -1);
          numCalls++;
          nbytes += data_buf_len;

          if (recv_n ((char *) &ack, sizeof ack) != sizeof ack)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p recv of ack failed\n",
                               "ttcp"),
                              -1);

          if (ack != data_buf_len)
            ACE_DEBUG ((LM_DEBUG, "received ack for only %d bytes\n", ack));
        }
    }
  else
    {
      prep_timer ();

      if (recv_n ((char *) &session_control_buf,
                  sizeof (Session_Control_Message))
          != sizeof (Session_Control_Message))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p recv session control failed\n",
                           "ttcp"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "received session control message"
                  " nbuf %d, size %d\n", session_control_buf.nbuf_,
                  session_control_buf.size_));

      nbuf = session_control_buf.nbuf_;
      // ignore session_control_buf.size_ for now

      long cnt;

      while (nbuf--)
        {
          if (recv_n ((char *) message_buf, sizeof (long)) != sizeof (long))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p recv data control failed\n",
                               "ttcp"),
                              -1);

          cnt = recv_n (& (message_buf->data_), message_buf->size_);
          if (cnt != message_buf->size_)
            ACE_ERROR_RETURN ((LM_ERROR, "recv data failed\n"), -1);

          numCalls++;
          nbytes += cnt;

          if (send_n ((char *) &cnt, sizeof cnt) != sizeof cnt)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p send ack failed\n",
                               "ttcp"),
                              -1);
        }
    }

  if (errno)
    err ("IO");

  //
  // stop the timer
  //

  (void) read_timer (stats, sizeof (stats));
  if (udp && trans)
    {
      (void) Nwrite (connection_stream, message_buf, 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf, 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf, 4); /* rcvr end */
      (void) Nwrite (connection_stream, message_buf, 4); /* rcvr end */
    }
  if (cput <= 0.0)
    cput = 0.001;
  if (realt <= 0.0)
    realt = 0.001;

#if defined (LM_RESULTS)
  if (trans && (title != 0))
    {
      double tmp;
      FILE *fd;
      char filename[BUFSIZ];
      ACE_OS::sprintf (filename, "%s.results", title);
      fd = fopen(filename, "a+");
      if (new_line)
        fprintf(fd, "\n  -l %ldk \t", data_buf_len/1024);
      tmp = ((double) nbytes) / realt;
      fprintf(fd, "%.2f ", tmp * 8.0 / 1024.0 / 1024.0);
      fclose(fd);
    }
#endif

  fprintf (stdout,
           "ttcp%s: %ld bytes in %.2f real seconds = %s/sec +++\n",
           trans ? "-t" : "-r",
           nbytes, realt, outfmt (((double) nbytes) / realt));
  if (verbose)
    {
      fprintf (stdout,
               "ttcp%s: %ld bytes in %.2f CPU seconds = %s/cpu sec\n",
               trans ? "-t" : "-r",
               nbytes, cput, outfmt (((double) nbytes) / cput));
    }
  fprintf (stdout,
           "ttcp%s: %d I/O calls, msec/call = %.2f, calls/sec = %.2f\n",
           trans ? "-t" : "-r",
           numCalls,
           1024.0 * realt / ((double) numCalls),
           ((double) numCalls) / realt);
  fprintf (stdout, "ttcp%s: %s\n", trans ? "-t" : "-r", stats);
  if (verbose)
    {
      fprintf (stdout,
        "ttcp%s: buffer address %#x\n",
        trans ? "-t" : "-r",
        message_buf);
    }
  exit (0);

usage:
  fprintf (stderr, Usage);
  return 1;
}

int
send_n (const void *buf, int len)
{
  size_t bytes_written;
  int n;

  for (bytes_written = 0; bytes_written < len; bytes_written += n)
    if ((n = write (connection_descriptor, (const char *) buf + bytes_written,
         len - bytes_written)) == -1)
      return -1;

  return bytes_written;
}

int
recv_n (void *buf, int len)
{
  size_t bytes_read;
  int n;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    if ((n = read (connection_descriptor, (char *) buf + bytes_read,
         len - bytes_read)) == -1)
      return -1;
    else if (n == 0)
      break;

  return bytes_read;
}

void
err (char *s)
{
  fprintf (stderr, "ttcp%s: ", trans ? "-t" : "-r");
  perror (s);
  fprintf (stderr, "errno=%d\n", errno);
  exit (1);
}

void
mes (char *s)
{
  fprintf (stderr, "ttcp%s: %s\n", trans ? "-t" : "-r", s);
}

void
pattern (register char *cp, register int cnt)
{
  register char c;
  c = 0;
  while (cnt-- > 0)
  {
    while (!isprint ((c & 0x7F)))
      c++;
    *cp++ = (c++ & 0x7F);
  }
}

char *
outfmt (double b)
{
  static char obuf[50];
  switch (fmt)
    {
    case 'G':
      sprintf (obuf, "%.2f GB", b / 1024.0 / 1024.0 / 1024.0);
      break;
    default:
    case 'K':
      sprintf (obuf, "%.2f KB", b / 1024.0);
      break;
    case 'M':
      sprintf (obuf, "%.2f MB", b / 1024.0 / 1024.0);
      break;
    case 'g':
      sprintf (obuf, "%.2f Gbit", b * 8.0 / 1024.0 / 1024.0 / 1024.0);
      break;
    case 'k':
      sprintf (obuf, "%.2f Kbit", b * 8.0 / 1024.0);
      break;
    case 'm':
      sprintf (obuf, "%.2f Mbit", b * 8.0 / 1024.0 / 1024.0);
      break;
    }
  return obuf;
}

static struct itimerval itime0; /* Time at which timing started */
static struct rusage ru0;       /* Resource utilization at the start */

#if defined(SYSV)
/*ARGSUSED */
static void
getrusage (int ignored, register struct rusage *ru)
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
  itime0.it_value.tv_sec = LONG_MAX / 22; /* greatest possible value , itimer() count backwards */
  itime0.it_value.tv_usec = 0;


  getrusage (RUSAGE_SELF, &ru0);

  /* Init REAL Timer */
  if (setitimer (ITIMER_REAL, &itime0, 0))
    {
      perror ("Setting 'itimer' REAL failed");
      return;
    }

}

/*
 *                    R E A D _ T I M E R
 *
 */
double
read_timer (char *str, int len)
{
  struct itimerval itimedol;
  struct rusage ru1;
  struct timeval td;
  struct timeval tend, tstart;
  char line[132];

  getrusage (RUSAGE_SELF, &ru1);

  if (getitimer (ITIMER_REAL, &itimedol))
    {
      perror ("Getting 'itimer' REAL failed");
      return (0.0);
    }

  prusage (&ru0, &ru1, &itime0.it_value, &itimedol.it_value, line);
  (void) strncpy (str, line, len);

  /* Get real time */
  tvsub (&td, &itime0.it_value, &itimedol.it_value);
  realt = td.tv_sec + ((double) td.tv_usec) / 1000000;

  /* Get CPU time (user+sys) */
  tvadd (&tend, &ru1.ru_utime, &ru1.ru_stime);
  tvadd (&tstart, &ru0.ru_utime, &ru0.ru_stime);
  tvsub (&td, &tend, &tstart);
  cput = td.tv_sec + ((double) td.tv_usec) / 1000000;
  if (cput < 0.00001)
    cput = 0.00001;
  return (cput);
}

static void
prusage (register struct rusage *r0, struct rusage *r1,
         struct timeval *e, struct timeval *b, char *outp)
{
  struct timeval tdiff;
  register time_t t;
  register char *cp;
  register int i;
  int ms;

  t = (r1->ru_utime.tv_sec - r0->ru_utime.tv_sec) * 100 +
      (r1->ru_utime.tv_usec - r0->ru_utime.tv_usec) / 10000 +
      (r1->ru_stime.tv_sec - r0->ru_stime.tv_sec) * 100 +
      (r1->ru_stime.tv_usec - r0->ru_stime.tv_usec) / 10000;
  ms = (e->tv_sec - b->tv_sec) * 100 + (e->tv_usec - b->tv_usec) / 10000;

#define END(x)  {while(*x) x++;}
#if defined(SYSV)
  cp = "%Uuser %Ssys %Ereal %P";
#else
#if defined(sgi) /* IRIX 3.3 will show 0 for %M,%F,%R,%C */
  cp = "%Uuser %Ssys %Ereal %P %Mmaxrss %F+%Rpf %Ccsw";
#else
  cp = "%Uuser %Ssys %Ereal %P %Xi+%Dd %Mmaxrss %F+%Rpf %Ccsw";
#endif
#endif
  for (; *cp; cp++)
    {
      if (*cp != '%')
        *outp++ = *cp;
      else if (cp[1])
        switch (*++cp)
          {
          case 'U':
            tvsub (&tdiff, &r1->ru_utime, &r0->ru_utime);
            sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
            END (outp);
            break;

          case 'S':
            tvsub (&tdiff, &r1->ru_stime, &r0->ru_stime);
            sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
            END (outp);
            break;

          case 'E':
            psecs (ms / 100, outp);
            END (outp);
            break;

          case 'P':
            sprintf (outp, "%d%%", (int) (t * 100 / ((ms ? ms : 1))));
            END (outp);
            break;

  #if !defined(SYSV)
          case 'W':
            i = r1->ru_nswap - r0->ru_nswap;
            sprintf (outp, "%d", i);
            END (outp);
            break;

          case 'X':
            sprintf (outp, "%d", t == 0 ? 0 : (r1->ru_ixrss - r0->ru_ixrss) / t);
            END (outp);
            break;

          case 'D':
            sprintf (outp, "%d", t == 0 ? 0 :
                     (r1->ru_idrss + r1->ru_isrss - (r0->ru_idrss + r0->ru_isrss)) / t);
            END (outp);
            break;

          case 'K':
            sprintf (outp, "%d", t == 0 ? 0 :
                     ((r1->ru_ixrss + r1->ru_isrss + r1->ru_idrss) -
                     (r0->ru_ixrss + r0->ru_idrss + r0->ru_isrss)) / t);
            END (outp);
            break;

          case 'M':
            sprintf (outp, "%d", r1->ru_maxrss / 2);
            END (outp);
            break;

          case 'F':
            sprintf (outp, "%d", r1->ru_majflt - r0->ru_majflt);
            END (outp);
            break;

          case 'R':
            sprintf (outp, "%d", r1->ru_minflt - r0->ru_minflt);
            END (outp);
            break;

          case 'I':
            sprintf (outp, "%d", r1->ru_inblock - r0->ru_inblock);
            END (outp);
            break;

          case 'O':
            sprintf (outp, "%d", r1->ru_oublock - r0->ru_oublock);
            END (outp);
            break;
          case 'C':
            sprintf (outp, "%d+%d", r1->ru_nvcsw - r0->ru_nvcsw,
                     r1->ru_nivcsw - r0->ru_nivcsw);
            END (outp);
            break;
  #endif /* !SYSV */
          }
    }
  *outp = '\0';
}

static void
tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1)
{

  tsum->tv_sec = t0->tv_sec + t1->tv_sec;
  tsum->tv_usec = t0->tv_usec + t1->tv_usec;
  if (tsum->tv_usec > 1000000)
    tsum->tv_sec++, tsum->tv_usec -= 1000000;
}

static void
tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0)
{

  tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
  tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
  if (tdiff->tv_usec < 0)
    tdiff->tv_sec--, tdiff->tv_usec += 1000000;
}

static void
psecs (long l, register char *cp)
{
  register int i;

  i = l / 3600;
  if (i)
    {
      sprintf (cp, "%d:", i);
      END (cp);
      i = l % 3600;
      sprintf (cp, "%d%d", (i / 60) / 10, (i / 60) % 10);
      END (cp);
    }
  else
    {
      i = l;
      sprintf (cp, "%d", i / 60);
      END (cp);
    }
  i %= 60;
  *cp++ = ':';
  sprintf (cp, "%d%d", i / 10, i % 10);
}

/*
 *                    N R E A D
 */
int
Nread (ACE_SOCK_Stream &s, void *buf, int count)
{
  numCalls++;
  return (s.recv (buf, count));
}

/*
 *                    N W R I T E
 */
int
Nwrite (ACE_SOCK_Stream &s, void *buf, int count)
{
  numCalls++;
  return s.send (buf, count);
}

void
delay (int us)
{
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = us;
  (void) select (1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

/*
 *                    M R E A D
 *
 * This function performs the function of a read(II) but will
 * call read(II) multiple times in order to get the requested
 * number of characters.  This can be necessary because
 * network connections don't deliver data with the same
 * grouping as it is written with.  Written by Robert S. Miles, BRL.
 */
int
mread (int fd, register char *bufp, unsigned n)
{
  register unsigned count = 0;
  register int nread;

  do
    {
      nread = read (fd, bufp, n - count);
      numCalls++;
      if (nread < 0)
      {
        perror ("ttcp_mread");
        return (-1);
      }
      if (nread == 0)
        return ((int) count);
      count += (unsigned) nread;
      bufp += nread;
    }
  while (count < n);

  return ((int) count);
}
