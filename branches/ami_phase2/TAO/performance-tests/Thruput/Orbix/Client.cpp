// $Id$

#include "ttcp_decl.h"

ACE_RCSID(Orbix, Client, "$Id$")

char Usage[] = "\
Usage: client [-options] [ORBeline options] host \n\
Common options:\n\
-l ##   length of bufs read from or written to network (default 8192)\n\
-A      align the start of buffers to this modulus (default 16384)\n\
-o      start buffers at this offset from the modulus (default 0)\n\
-v      verbose: print more statistics\n\
-d      set SO_DEBUG socket option\n\
-b ##   set socket buffer size (if supported)\n\
-f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
-n ##    number of source bufs written to network (default 2048)\n\
-x      Use new lines after each entry in output file\n\
-h ##   Remote host to send data to\n\
-D      don't buffer TCP writes (sets TCP_NODELAY socket option)\n\
-L ##   Output file name for the data type used\n\
-S      <Total Data Size>\n\
-q      <type> Send Sequence: Enumeration for various data types:\n\
s = short, l = long,  d = double, c = char\n\
o = octet, S = struct, C = composite\n\
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
char *domainname;               /* Rendezvous address for UNIX domain sockets. */
int fd;                         /* fd of network socket */
int buflen = 8 * 1024;          /* length of buffer */
char *buf;                      /* ptr to dynamic buffer */
int nbuf = 2 * 1024;            /* number of buffers to send in sinkmode */

int bufoffset = 0;              /* align buffer to this */
int bufalign = 16 * 1024;       /* modulo this */

int udp = 0;                    /* 0 = tcp, !0 = udp */
int options = 0;                /* socket options */
int one = 1;                    /* for 4.3 BSD style setsockopt() */
short port = 5001;              /* TCP port number */
char *host;                     /* ptr to name of host */
int trans =1;                   /* 0=receive, !0=transmit mode */
int sinkmode = 1;               /* 0=normal I/O, !0=sink/source mode */
int verbose = 0;                /* 0=print basic info, 1=print cpu rate, proc
* resource usage. */
int nodelay = 0;                /* set TCP_NODELAY socket option */
int b_flag = 0;                 /* use mread() */
int sockbufsize = 0;            /* socket buffer size to use */
int new_line=0;                /* This is a special flag */
int write_to_file=1;            /* indecates writing to file (default)*/
char fmt = 'K';                 /* output format:k=kilobits,K=kilobytes,
*  m = megabits, M = megabytes,
*  g = gigabits, G = gigabytes */
int touchdata = 0;              /* access data after reading */

struct hostent *addr;
char *title = 0;

char stats[128];
unsigned long srcDataSize;      /* Total amount of source data */
unsigned long nbytes;           /* bytes on net */
unsigned long numCalls;         /* # of I/O system calls */
double cput, realt;             /* user, real time (seconds) */
unsigned long   dt;

/* declare struct variables for various message types */
ttcp_sequence::ShortSeq         *sseq;
ttcp_sequence::LongSeq          *lseq;
ttcp_sequence::OctetSeq         *oseq;
ttcp_sequence::DoubleSeq        *dseq;
ttcp_sequence::CharSeq          *cseq;
ttcp_sequence::StructSeq        *Sseq;
ttcp_sequence::CompositeSeq     *Cseq;


int
main (int argc, char *argv[])
{

    unsigned long addr_tmp;
    unsigned short done = 0;
    int c;

    CORBA::Environment env;
    CORBA::Object_ptr target;  // will hold handle to proxy
    ttcp_sequence_ptr ttcp_seq;

    if (argc < 2)
        goto usage;


    while (!done && ((c = getopt (argc, argv, "dvDb:h:f:l:n:A:o:L:xS:q:O")) != -1))
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
                case 'v':
                    verbose = 1;
                    break;
                case 'A':
                    bufalign = atoi (optarg);
                    break;
                case 'o':
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
                case 'S':       /* total source data to send. */
                    /* modify nbuf accordingly */
                    srcDataSize = atoi(optarg);
                    break;
                case 'q':       /* Send sequence of desired data type */
                    switch(*optarg){
                    case 's':
                        dt = SEND_SHORT;
                        break;
                    case 'l':
                        dt = SEND_LONG;
                        break;
                    case 'd':
                        dt = SEND_DOUBLE;
                        break;
                    case 'c':
                        dt = SEND_CHAR;
                        break;
                    case 'o':
                        dt = SEND_OCTET;
                        break;
                    case 'S':
                        dt = SEND_STRUCT;
                        break;
                    case 'C':
                        dt = SEND_COMPOSITE;
                        break;
                    }
                    break;
                case 'O':
                case '?':
                    done = 1;
                    break;
                default:
                    goto usage;
                }
        }

    //
    // Transmitter
    //

    // now bind to server
    try {
        char *service_name = new char[15];
        strcpy(service_name,":SII_ttcp"); // server object's name

        target = ttcp_sequence::_bind (service_name, host);
        if (!CORBA::is_nil(target)){
            ttcp_seq = ttcp_sequence::_narrow(target);

            if (!CORBA::is_nil(ttcp_seq)){
                /* start recording quantify data from here */
                if (sockbufsize)
                    {
                        /* file descriptor of a proxy is obtained using the _fd() method */
                        if (setsockopt (ttcp_seq->_fd (), SOL_SOCKET, SO_SNDBUF, (char *) &sockbufsize,
                                        sizeof sockbufsize) < 0)
                            err ("setsockopt: sndbuf");
                        mes ("sndbuf");
                    }

                if (nodelay)
                    {
                        struct protoent *p;
                        p = getprotobyname ("tcp");
                        if (setsockopt (ttcp_seq->_fd (),
                                        p->p_proto,
                                        TCP_NODELAY,
                                        (char *)& one,
                                        sizeof (one)))
                            err ("setsockopt: nodelay");
                        mes ("nodelay");
                    }

                /* modify nbuf according to Total data size */
                nbuf = srcDataSize/buflen;
                fprintf(stderr, "data size = %d, buflen = %d, nbuf = %d\n",srcDataSize, buflen, nbuf);

                //
                // Prepare the Message to be sent
                //

                errno = 0;
                if ((buf = (char *) malloc (buflen + bufalign)) == (char *) NULL)
                    err ("malloc");
                if (bufalign != 0)
                    buf += (bufalign - ((int) buf % bufalign) + bufoffset) % bufalign;


                // fill the appropriate buffers
                FillPattern (buf, buflen, dt);
                //
                // Start the timers on the client and server sides
                //

                prep_timer ();
                ttcp_seq->start_timer ();
#if defined (ACE_HAS_QUANTIFY)
                quantify_clear_data();
                quantify_start_recording_data();
#endif
                while (nbuf--)
                    {
                        switch(dt){
                        case SEND_SHORT:
                            ttcp_seq->sendShortSeq(*sseq);
                            nbytes += sseq->length()*sizeof(CORBA::Short);
                            break;
                        case SEND_LONG:
                            ttcp_seq->sendLongSeq(*lseq);
                            nbytes += lseq->length()*sizeof(CORBA::Long);
                            break;
                        case SEND_OCTET:
                            ttcp_seq->sendOctetSeq(*oseq);
                            nbytes += oseq->length()*sizeof(CORBA::Octet);
                            break;
                        case SEND_DOUBLE:
                            ttcp_seq->sendDoubleSeq(*dseq);
                            nbytes += dseq->length()*sizeof(CORBA::Double);
                            break;
                        case SEND_CHAR:
                            ttcp_seq->sendCharSeq(*cseq);
                            nbytes += cseq->length()*sizeof(CORBA::Char);
                            break;
                        case SEND_STRUCT:
                            ttcp_seq->sendStructSeq(*Sseq);
                            nbytes += Sseq->length()*sizeof(PerfStruct);
                            break;
                        case SEND_COMPOSITE:
                            ttcp_seq->sendCompositeSeq(*Cseq);
                            nbytes += Cseq->length()*sizeof(CompositeStruct);
                            break;
                        }
                        numCalls++;
                    }
#if defined(ACE_HAS_QUANTIFY)
                quantify_stop_recording_data();
#endif
                //
                // Stop the timer
                //
                ttcp_seq->stop_timer();
                (void) read_timer (stats, sizeof (stats));

                // print results
                PrintStats();

            }
        }
    }
    catch(CORBA::SystemException &se){
        // an error occurred while trying to bind to the object.
        cerr << "Bind to object failed" << endl;
        cerr << "System exception " << &se << endl;
        return -1;
    }
    catch(...)
        {
            // an error occurred while trying to bind to the object.
            cerr << "Bind to object failed" << endl;
            cerr << "Unexpected exception " << endl;
            return -1;
        };

    CORBA::release(ttcp_seq);
    return(0);
  usage:
    fprintf (stderr, Usage);
    return 1;
}
