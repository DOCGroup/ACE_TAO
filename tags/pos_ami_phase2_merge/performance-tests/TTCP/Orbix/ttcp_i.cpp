// $Id$

/*
 *    T T C P . C
 */

/* #define BSD43 */
/* #define BSD42 */
/* #define BSD41a */
#define SYSV /* required on SGI IRIX releases before 3.3 */

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
#include <sys/time.h>		/* struct itimerval */
#include <limits.h>
#include <sys/un.h>
#include <unistd.h>

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

ACE_RCSID(Orbix, ttcp_i, "$Id$")

void err (char *s);
void mes (char *s);
void pattern (register char *cp, register int cnt);
char *outfmt (double b);
static void getrusage (int ignored, register struct rusage *ru);
static void gettimeofday (struct timeval *tp, struct timezone *zp);
void prep_timer (void);
double read_timer (char *str, int len);
static void prusage (register struct rusage *r0, struct rusage *r1, struct timeval *e, struct timeval *b, char *outp);
static void tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1);
static void tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
static void psecs (long l, register char *cp);
void delay (int us);
int mread (int fd, register char *bufp, unsigned n);
int Nread (int fd, void *buf, int count);
int Nwrite (int fd, void *buf, int count);
static void SD_Ready (int io_descriptor);
void set_sock_desc(int starter);
void c_stop_timer (unsigned long numbytes);

int global_done = 0;

int fromlen;
int domain = PF_INET;           /* Default is to use Internet domain sockets. */
char *domainname;		/* Rendezvous address for UNIX domain sockets. */
int fd;				/* fd of network socket */

int buflen = 1024 * 1024 * 2;		/* length of buffer */
char *buf;			/* ptr to dynamic buffer */
int nbuf = 2 * 1024;		/* number of buffers to send in sinkmode */

int bufoffset = 0;		/* align buffer to this */
int bufalign = 16 * 1024;	/* modulo this */

int udp = 0;			/* 0 = tcp, !0 = udp */
int options = 0;		/* socket options */
int one = 1;			/* for 4.3 BSD style setsockopt() */
short port = 5001;		/* TCP port number */
char *host;                     /* ptr to name of host */
int trans;			/* 0=receive, !0=transmit mode */
int sinkmode = 0;		/* 0=normal I/O, !0=sink/source mode */
int verbose = 0;		/* 0=print basic info, 1=print cpu rate, proc
				 * resource usage. */
int nodelay = 0;		/* set TCP_NODELAY socket option */
int b_flag = 0;			/* use mread() */
int sockbufsize = 0;		/* socket buffer size to use */
int new_line=0;                /* This is a special flag */
int write_to_file=1;            /* indecates writing to file (default)*/
int SEQUENCE=0;                 /* String parameter is the default */
char fmt = 'K';			/* output format:k=kilobits,K=kilobytes,
				 *  m = megabits, M = megabytes, 
				 *  g = gigabits, G = gigabytes */
int touchdata = 0;		/* access data after reading */

struct hostent *addr;
extern int errno;
extern int optind;
extern char *optarg;
char *title = 0;

char stats[128];
unsigned long nbytes;		/* bytes on net */
unsigned long numCalls;		/* # of I/O system calls */
double cput, realt;		/* user, real time (seconds) */

// AAAAHHHHHHH what's this!!! C++ in the middle of C code???????!!!!!!


#include <iostream.h>
#include "ttcp_i.h"

char *orb_host;

// ttcp_i is the server side



// ttcp_i is the server side

static void  
SD_Ready (int io_descriptor)
{
  cout << "Orbix is using " << io_descriptor << endl;
  if (sockbufsize)
    {
      if (setsockopt (io_descriptor, SOL_SOCKET, SO_RCVBUF, (char *) &sockbufsize,
		      sizeof sockbufsize) < 0)
	perror ("setsockopt: rcvbuf"), exit (1);
    }
}


ttcp_sequence_i::ttcp_sequence_i()
{
  this->nbytes_ = 0;
  // register a callback so we can futs with the descriptor
  // being used by orbix.

  if (CORBA::Orbix.registerIOCallback (OrbixIOCallback(SD_Ready), FD_OPEN_CALLBACK) != 0)
    perror ("can't register callback"), exit (1);
}

void
ttcp_sequence_i::start_timer (CORBA::Environment &IT_env)
{
    this->nbytes_ = 0;
    prep_timer ();
}

void 
ttcp_sequence_i::stop_timer (CORBA::Environment &IT_env)
{
  c_stop_timer (this->nbytes_);
  global_done = 1;
}

long
ttcp_sequence_i::send(const ttcp_sequence::my_sequence& ttcp_seq,CORBA::Environment &IT_env)
{
    this->nbytes_ += ttcp_seq._length;
    return this->nbytes_;
}

ttcp_string_i::ttcp_string_i()
{
  this->nbytes_ = 0;
  // register a callback so we can futs with the descriptor
  // being used by orbix.
 if (CORBA::Orbix.registerIOCallback (OrbixIOCallback(SD_Ready), FD_OPEN_CALLBACK) != 0)
    perror ("can't register callback"), exit (1);
}

void
ttcp_string_i::start_timer (CORBA::Environment &IT_env)
{
  prep_timer ();
}


void 
ttcp_string_i::stop_timer (CORBA::Environment &IT_env)
{
    c_stop_timer (this->nbytes_);
    global_done = 1;
}

long
ttcp_string_i::send(const char * ttcp_string,CORBA::Environment &IT_env)
{
    this->nbytes_ += strlen (ttcp_string);
    return this->nbytes_;
}

// common to sequence and string
void
c_stop_timer (unsigned long numbytes)
{
  (void) read_timer (stats, sizeof (stats));

  if (cput <= 0.0)
    cput = 0.001;
  if (realt <= 0.0)
    realt = 0.001;
  fprintf (stdout,
	   "ttcp%s%s: %ld bytes in %.2f real seconds = %s/sec +++\n",
	   trans ? "-t" : "-r",	SEQUENCE ? "(Seq)" : "(Str)",
	   numbytes, realt, outfmt (((double) numbytes) / realt));


  if (verbose)
    {
      fprintf (stdout,
	       "ttcp%s%s: %ld bytes in %.2f CPU seconds = %s/cpu sec\n",
	       trans ? "-t" : "-r",SEQUENCE ? "(Seq)" : "(Str)",
	       numbytes, cput, outfmt (((double) numbytes) / cput));
    }
  fprintf (stdout,
	   "ttcp%s%s: %d I/O calls, msec/call = %.2f, calls/sec = %.2f\n",
	   trans ? "-t" : "-r",SEQUENCE  ? "(Seq)" : "(Str)",
	   numCalls,
	   1024.0 * realt / ((double) numCalls),
	   ((double) numCalls) / realt);
  fprintf (stdout, "ttcp%s%s: %s\n", trans ? "-t" : "-r",SEQUENCE  ? "(Seq)" : "(Str)", stats);
  if (verbose)
    {
      fprintf (stdout,
	       "ttcp%s: buffer address %#x\n",
	       trans ? "-t" : "-r",
	       buf);
    }
numbytes = 0;

  // you shoudl exit here when you use -p (profiler) so the server will 
  // produce mon.out
#ifdef PROFILE
  exit(1);
#endif
} // end of stop_timer


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
        -L<test_title>  the title of the current test\n\
        -F don't write to a file (writting in a file dat.out is default)\n\
        -q run the test using sequence parameter (string is defualt) \n\
";

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

int
main (int argc, char *argv[])
{
        
    unsigned long addr_tmp;
    int c;
    
    if (argc < 2)
	goto usage;
    
    while ((c = getopt (argc, argv, "qFdrstU:uvBDTb:L:f:l:n:p:A:O:h:x")) != -1)
    {
	switch (c)
	{
	  case 'L':
	    title = optarg;
	    fprintf(stdout,"---->title=%s\n",title);
	    break;
	  case 'x':
	    new_line = 1;
	    break;
	  case 'q':
	    SEQUENCE = 1;
	    break;
	  case 'F':
	    write_to_file = 0;
	    break;
	  case 'h':
	    orb_host = optarg;
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
	    buflen = atoi (optarg);
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
	    fprintf (stderr, "ttcp: -b option ignored: SO_SNDBUF/SO_RCVBUF socket options not supported\n");
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
    
    //
    // Receiver
    //
    
    if (!trans) 
    {
	
	// Sequence and String Interfaces
	// instantiate ONLY one object at a time, so no dispatching overhead is counted
	char *interface_name = new char[15];
	if (SEQUENCE)
	{
	    ttcp_sequence *my_ttcp = new ttcp_sequence_i;
	    strcpy(interface_name,"ttcp_sequence");
	}
	else
	{
	    ttcp_string *my_ttcp_s = new ttcp_string_i;
	    strcpy(interface_name,"ttcp_string");
	}

	// tell Orbix/ORBeline that we have completed the server's initialization:
	TRY {
	    CORBA::Orbix.impl_is_ready(interface_name, 0, IT_X);

	    while ((!global_done) && (!IT_X))
	      CORBA::Orbix.processNextEvent (CORBA::ORB::INFINITE_TIMEOUT, IT_X);
	}
	CATCHANY {
	    // an error occured calling impl_is_ready() - output the error.
	    cout << IT_X;
	}
	ENDTRY;
	
	cout << "server exiting" << endl;
	return 0;
    }
    
    //
    // Transmitter
    //
    
//    CORBA::Environment env;
    ttcp_sequence *ttcp_seq;
    ttcp_string *ttcp_str;
    

    CORBA::Object *ttcp_;

    if (SEQUENCE)
    {
	TRY 
	{
	    ttcp_ = ttcp_seq = ttcp_sequence::_bind ("", orb_host, IT_X);
	} 
	CATCHANY 
	{
	    // an error occurred while trying to bind to the logger object.
	    cerr << "Bind to object failed" << endl;
	    cerr << "Unexpected exception " << IT_X << endl;
	    return -1;
	} ENDTRY;
	
    }
    else  // string parameter used
    {
	TRY 
	{
	    ttcp_ = ttcp_str = ttcp_string::_bind ("", orb_host, IT_X);
	} 
	CATCHANY 
	{
	    // an error occurred while trying to bind to the logger object.
	    cerr << "Bind to object failed" << endl;
	    cerr << "Unexpected exception " << IT_X << endl;
	    return -1;
	} ENDTRY; 
    }
    
    if (sockbufsize)
    {
	if (setsockopt (ttcp_->_fd (), SOL_SOCKET, SO_SNDBUF, (char *) &sockbufsize,
			sizeof sockbufsize) < 0)
	    err ("setsockopt: sndbuf");
	mes ("sndbuf");
    }
    
    if (nodelay)
    {
	struct protoent *p;
	p = getprotobyname ("tcp");
	if (setsockopt (ttcp_->_fd (), 
			p->p_proto,
			TCP_NODELAY,
			(char *)& one, 
			sizeof (one)))
	    err ("setsockopt: nodelay");
	mes ("nodelay");
    }
    
    // 
    // Prepare the Message to be sent
    //
    
    errno = 0;
    if (sinkmode)
    {
	if ((buf = (char *) malloc (buflen + bufalign)) == (char *) NULL)
	    err ("malloc");
	if (bufalign != 0)
	    buf += (bufalign - ((int) buf % bufalign) + bufoffset) % bufalign;
	
	//      ttcp_sequence::my_sequence sequence_message; ORBIX
	ttcp_sequence::my_sequence message (buflen + 1);
	::buf[::buflen-1] = '\0';
	if (SEQUENCE)
	{
	    message._maximum = buflen;
	    message._length = buflen;
	    message._buffer = buf;
	}
	
	
	pattern (buf, buflen);
	//
	// Start the timers on the client and server sides
	//
	
	prep_timer ();
	
	if (SEQUENCE)
	    ttcp_seq->start_timer ();
	else
	    ttcp_str->start_timer ();
	while (nbuf--)
	{
	    if (SEQUENCE)
		ttcp_seq->send (message);
	    else
		ttcp_str->send (buf);
	    
	    numCalls++;
	    nbytes += buflen;
	}
	
    }
    else
    {
	register int cnt;
	while ((cnt = read (0, buf, buflen)) > 0 &&
	       Nwrite (fd, buf, cnt) == cnt)
	    nbytes += cnt;
    }
    
    if (errno)
	err ("IO");
    
    
    //
    // Stop the timers on both sides
    //
    
    
    if (SEQUENCE)
	ttcp_seq->stop_timer();
    else 
	ttcp_str->stop_timer();
    
    
    (void) read_timer (stats, sizeof (stats));
    
    
    // 
    // Print the results.
    //
    
    if (cput <= 0.0)
	cput = 0.001;
    if (realt <= 0.0)
	realt = 0.001;

    if (write_to_file)
      {
	if (title != 0)
	  {
	    double tmp;
	    FILE *fd;
	    char filename[BUFSIZ];
	    sprintf (filename, "%s.results", title);
	    fd = fopen(filename,"a+");
	    if (new_line)
	      fprintf(fd,"\n  -l %ldk \t", buflen/1024);
	    tmp = ((double) nbytes) / realt;
	    fprintf(fd,"%.2f ", tmp * 8.0 / 1024.0 / 1024.0);
	    fclose(fd);
	  }
      }

    fprintf (stdout,
	     "\nttcp%s%s: %ld bytes in %.2f real seconds = %s/sec +++\n",
	     trans ? "-t" : "-r",	SEQUENCE ? "(Seq)" : "(Str)",
	     nbytes, realt, outfmt (((double) nbytes) / realt));
    
    if (verbose)
    {
	fprintf (stdout,
		 "ttcp%s%s: %ld bytes in %.2f CPU seconds = %s/cpu sec\n",
		 trans ? "-t" : "-r",	SEQUENCE ? "(Seq)" : "(Str)",
		 nbytes, cput, outfmt (((double) nbytes) / cput));
    }
    fprintf (stdout,
	     "ttcp%s%s: %d I/O calls, msec/call = %.2f, calls/sec = %.2f\n",
	     trans ? "-t" : "-r",	SEQUENCE ? "(Seq)" : "(Str)",
	     numCalls,
	     1024.0 * realt / ((double) numCalls),
	     ((double) numCalls) / realt);
    fprintf (stdout, "ttcp%s%s: %s\n",trans ? "-t" : "-r",	SEQUENCE ? "(Seq)" : "(Str)", stats);
    if (verbose)
    {
	fprintf (stdout,
		 "ttcp%s: buffer address %#x\n",
		 trans ? "-t" : "-r",
		 buf);
    }
    exit (0);
    
  usage:
    fprintf (stderr, Usage);
    return 1;
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
  fprintf (stderr, "ttcp%s%s: %s\n", trans ? "-t" : "-r",SEQUENCE ? "(Seq)" : "(Str)", s);
//  fprintf (stderr, "ttcp%s: %s\n", trans ? "-t" : "-r", s);
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

static struct itimerval itime0;	/* Time at which timing started */
static struct rusage ru0;	/* Resource utilization at the start */

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

/*ARGSUSED */
static void
gettimeofday (struct timeval *tp, struct timezone *zp)
{
  tp->tv_sec = time (0);
  tp->tv_usec = 0;
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
  itime0.it_value.tv_sec = LONG_MAX / 22;	/* greatest possible value , itimer() count backwards */
  itime0.it_value.tv_usec = 0;


  getrusage (RUSAGE_SELF, &ru0);

  /* Init REAL Timer */
  if (setitimer (ITIMER_REAL, &itime0, NULL))
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
#if defined(sgi)		/* IRIX 3.3 will show 0 for %M,%F,%R,%C */
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
Nread (int fd, void *buf, int count)
{
  struct sockaddr_in from;
  int len = sizeof (from);
  register int cnt;
  if (udp)
    {
      cnt = recvfrom (fd, (char *) buf, count, 0, (struct sockaddr *) &from, &len);
      numCalls++;
    }
  else
    {
      if (b_flag)
	cnt = mread (fd, (char *) buf, count);	/* fill buf */
      else
	{
	  cnt = read (fd, buf, count);
	  numCalls++;
	}
      if (touchdata && cnt > 0)
	{
	  register int c = cnt, sum;
	  register char *b = (char *) buf;
	  while (c--)
	    sum += *b++;
	}
    }
  return (cnt);
}


/*
 *                    N W R I T E
 */
int
Nwrite (int fd, void *buf, int count)
{
  return 0;
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
