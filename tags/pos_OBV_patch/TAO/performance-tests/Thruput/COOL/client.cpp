// $Id$

// ============================================================================
//
// = Chorus COOL tests
//    Throughput measurement using the TTCP benchmark adapted to work using COOL
//
// = FILENAME
//   client.cpp
//
//   Main program for the client
//
// = AUTHOR
//    Aniruddha Gokhale
//    Sergio Flores        Ported code to use Chorus COOL ORB
//
// ============================================================================

#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/stream.h"
#include "ttcp.H"
#include "ttcp_decl.h"

#include "api/api.H"

ACE_RCSID(COOL, client, "$Id$")

int print_usage (void);

char Usage[] =
     "Usage: client -cool-tcp -cool-iiop [Common options] \n"
     "[Common options]:\n"
     "  -l ##   length of bufs read from or written to network (default 8192)\n"
     "  -v      verbose: print more statistics\n"
     "  -d ##   debug level\n"
     "  -f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n"
     "  -h ##   Remote host to send data to\n"
     "  -p ##   Port number"
     "  -L ##   Output file name to store results\n"
     "  -S ##   Total Data Size to be sent\n"
     "  -q <type> Send Sequence: Enumeration for various data types:\n"
     "         s = short, l = long,  d = double, c = char\n"
     "         o = octet, S = BinStruct \n";

CORBA::Long trans = 1;          // we are a client - used for debugging purposes
CORBA::Long buflen = 8 * 1024;  // length of buffer, default 8,192 bytes
char *buf;              // ptr to dynamic buffer
CORBA::Long nbuf;               // number of buffers to send in sinkmode
CORBA::Short port = 3000;       // default TCP port number
char *host = "localhost";              // ptr to name of host
CORBA::Long verbose = 0;        // 0=print basic info, 1=print cpu
                                // rate, proc resource usage.
CORBA::Char fmt = 'K';          // output
                                // format:k=kilobits,K=kilobytes, m = megabits,
                                // M = megabytes, g = gigabits, G = gigabytes
CORBA::String title = 0;        // title of file to gather statistics
char stats[128];                // gathering the statistics
unsigned long srcDataSize;      // Total amount of source data
unsigned long nbytes;           // bytes on net
unsigned long numCalls;         // # of I/O system calls
double cput, realt;             // user, real time (seconds)
unsigned long   dt;             // data type

u_int debug_level = 0;

// declare variables for various message types
ttcp_sequence::ShortSeq         *sseq;
ttcp_sequence::LongSeq          *lseq;
ttcp_sequence::OctetSeq         *oseq;
ttcp_sequence::DoubleSeq        *dseq;
ttcp_sequence::CharSeq          *cseq;
ttcp_sequence::StructSeq        *Sseq;

int
main (int argc, char *argv[])
{

  int c;
  CORBA_ORB_ptr        orb_ptr;   // underlying ORB
  CORBA::Object_ptr     objref = CORBA::Object::_nil(); // object reference
  ttcp_sequence_ptr     ttcp_seq = 0;  // obj reference to TTCP object
  CORBA::Environment    env;       // environment
  CORBA::String str;               // holds the IOR

  fstream iorfile;

  // parse the arguments
  ACE_Get_Opt get_opt (argc, argv, "d:vh:p:f:l:L:S:q:", 3); // Command line options
  debug_level = 0;
  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'h':
          host = ACE_OS::strdup (get_opt.optarg);
          break;
        case 'L':
          title = ACE_OS::strdup (get_opt.optarg);
          break;
        case 'p':
          port = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'd':
          debug_level = ACE_OS::atoi (get_opt.optarg);
          if (debug_level > 10)
            debug_level = 10;
          break;
        case 'l':
          buflen = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'v':
          verbose = 1;
          break;
        case 'f':
          fmt = *get_opt.optarg;
          break;
        case 'S':       /* total source data to send. */
          srcDataSize = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'q':       /* Send sequence of desired data type */
          switch(*get_opt.optarg){
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
        default:
          return print_usage ();
        }
    }

  //
  // Transmitter
  //

  // get a handle to the ORB
  orb_ptr = CORBA_ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB initialization\n"));
      //env.print_exception ("ORB initialization", stdout);
      return -1;
    }

  // allocate storage to read the IOR string
  str = CORBA::string_alloc (500);

  sprintf (str, "cool-tcp://%s:%d", host, port);

  ACE_DEBUG ((LM_DEBUG, "Composed IOR string as: %s\n" , str));

  COOL::EOABindingData bindingData (str);
  ttcp_seq = ttcp_sequence::_bind(bindingData, env);

  CORBA::string_free (str);

  if (!CORBA::is_nil (ttcp_seq))
    {
      // the number of iterations is based on the total data size and the
      // individual buffer size sent
      nbuf = srcDataSize/buflen;
      ACE_DEBUG ((LM_DEBUG, "data size = %d, buflen = %d, nbuf = %d\n",
                  srcDataSize, buflen, nbuf));

      //
      // Prepare the Message to be sent
      //


      // first allocate a buffer of the desired size and alignment
      errno = 0;
      if ((buf = (char *) ACE_OS::malloc (buflen)) == (char *) NULL)
        err ("malloc");

      // fill the buffer with the data type to be sent
      FillPattern (buf, buflen, dt);

      //
      // Start the timers on the client and server sides
      //

      prep_timer ();  // start our time
      ttcp_seq->start_timer (env); // ask the server to start its timer
      if (env.exception () != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "start_timer operation\n"));
          //env.print_exception ("start_timer operation", stdout);
          return -1;
        }

#if defined (ACE_HAS_QUANTIFY)
      /* start recording quantify data from here */
      quantify_clear_data ();
      quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */
      // send the same buffer nbuf times
      while (nbuf--)
        {
          switch (dt){
          case SEND_SHORT:
            ttcp_seq->sendShortSeq (*sseq, env);
            nbytes += sseq->length () * sizeof (CORBA::Short);
            break;
          case SEND_LONG:
            ttcp_seq->sendLongSeq (*lseq, env);
            nbytes += lseq->length () * sizeof (CORBA::Long);
            break;
          case SEND_OCTET:
            ttcp_seq->sendOctetSeq (*oseq, env);
            nbytes += oseq->length () * sizeof (CORBA::Octet);
            break;
          case SEND_DOUBLE:
            ttcp_seq->sendDoubleSeq (*dseq, env);
            nbytes += dseq->length () * sizeof (CORBA::Double);
            break;
          case SEND_CHAR:
            ttcp_seq->sendCharSeq (*cseq, env);
            nbytes += cseq->length () * sizeof (CORBA::Char);
            break;
          case SEND_STRUCT:
            ttcp_seq->sendStructSeq (*Sseq, env);
            nbytes += Sseq->length () * sizeof (BinStruct);
            break;
          }
          numCalls++; // nbytes and numCalls are used in the thruput
          // measurement
          if (env.exception () != 0)
            {
              ACE_DEBUG ((LM_DEBUG, "send operation\n"));
              //env.print_exception ("send operation", stdout);
              return -1;
            }
        }
#if defined (ACE_HAS_QUANTIFY)
      quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */
      //
      // Stop the timer
      //
      // stop the timer on the server side
      ttcp_seq->stop_timer (env);
      if (env.exception () != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "stop_timer operation\n"));
          //env.print_exception ("stop_timer operation", stdout);
          return -1;
        }
      // stop our timer
      (void) read_timer (stats, sizeof (stats));

      // print results
      PrintStats();
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "error: objref is nil\n"));
    }


  CORBA::release (ttcp_seq);
  CORBA::release (objref);

  return (0);
}

int print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage error\n"));
  ACE_ERROR ((LM_ERROR, "%s\n", Usage));
  return -1;
}
