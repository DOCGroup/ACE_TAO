// $Id$

#include "ttcp_i.h"

ACE_RCSID(Orbix, Srv_Main, "$Id$")

char Usage[] = "\
Usage: server [options] \n\
Common options:\n\
        -l ##   length of bufs read from or written to network (default 8192)\n\
        -A      align the start of buffers to this modulus (default 16384)\n\
        -O      start buffers at this offset from the modulus (default 0)\n\
        -v      verbose: print more statistics\n\
        -d      set SO_DEBUG socket option\n\
        -b ##   set socket buffer size (if supported)\n\
        -f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
	-L ##	Output file name for the data type used\n\
Options specific to -r:\n\
        -B      for -s, only output full blocks as specified by -l (for TAR)\n\
        -T      \"touch\": access each byte as it's read\n\
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

int fromlen;
int domain = PF_INET;           /* Default is to use Internet domain sockets. */
char *domainname;		/* Rendezvous address for UNIX domain sockets. */
int fd;				/* fd of network socket */
int buflen = 8 * 1024;		/* length of buffer */
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
char fmt = 'K';			/* output format:k=kilobits,K=kilobytes,
				 *  m = megabits, M = megabytes,
				 *  g = gigabits, G = gigabytes */
int touchdata = 0;		/* access data after reading */

struct hostent *addr;
char *title = 0;

char stats[128];
unsigned long srcDataSize;	/* Total amount of source data */
unsigned long nbytes;		/* bytes on net */
unsigned long numCalls;		/* # of I/O system calls */
double cput, realt;		/* user, real time (seconds) */
unsigned long	dt;

/* declare struct variables for various message types */
ttcp_sequence::ShortSeq		*sseq;
ttcp_sequence::LongSeq		*lseq;
ttcp_sequence::OctetSeq		*oseq;
ttcp_sequence::DoubleSeq	*dseq;
ttcp_sequence::CharSeq		*cseq;
ttcp_sequence::StructSeq	*Sseq;
ttcp_sequence::CompositeSeq	*Cseq;

unsigned short isDone = 0;

int
main (int argc, char *argv[])
{

    unsigned long addr_tmp;
    int c;
    unsigned short done = 0;


    if (argc < 2)
      goto usage;

    // Now process the options other than Orbix specific options
    while (!done && ((c = getopt (argc, argv, "dvb:f:l:L:O")) != -1))
      {
	  switch (c)
	    {
	    case 'L':
		title = optarg;
		break;
	    case 'd':
		options |= SO_DEBUG;
		break;
	    case 'l':
		buflen = atoi (optarg);
		break;
	    case 'v':
		verbose = 1;
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
	    case 'O':
	    case '?':
		done = 1;
		break;
	    default:
		goto usage;
	    }
      }

    ttcp_sequence_i *my_ttcp;
    //
    // Receiver
    //
    try {
	    char *service_name = new char[15];
	    strcpy(service_name,"SII_ttcp");

#if defined (USE_TIE)
	    // TIE approach
	    my_ttcp = new ttcp_sequence_i;
	    _tie_ttcp_sequence<ttcp_sequence_i> *my_ttcp_tmpl = new
	      _tie_ttcp_sequence<ttcp_sequence_i> (*my_ttcp);

#else
	    my_ttcp = new ttcp_sequence_i;
#endif

#if defined (ACE_HAS_QUANTIFY)
    quantify_clear_data();
    quantify_start_recording_data();
#endif
	    CORBA::Orbix.impl_is_ready(service_name);
    }
    catch(CORBA::SystemException &se) {
	// an error occured calling impl_is_ready() - output the error.
	cout << "System Exception getting the implementation ready " << &se << endl;
	return -1;
    }
    catch(...) {
	// an error occured calling obj_is_ready() - output the error.
	cout << "Undefined Exception getting the implementation ready " << endl;
	return -1;
    }
    CORBA::release(my_ttcp);
    return (0);
usage:
    fprintf (stderr, Usage);
    return(1);
}
