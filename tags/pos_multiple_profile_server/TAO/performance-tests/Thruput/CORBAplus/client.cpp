// $Id$

// ============================================================================
//
// = TAO tests
//    Throughput measurement using the TTCP benchmark adapted to work using TAO
//
// = FILENAME
//   client.cpp
//
//   Main program for the client
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include <iostream.h>
#include <fstream.h>

#include "pbroker/corba/orb.h"
#include "pbroker/corba/request.h"
#include "pbroker/corba/environ.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ttcp.h"
#include "ttcp_decl.h"

ACE_RCSID(CORBAplus, client, "$Id$")

int print_usage (void);

char Usage[] =
     "Usage: client [-options] \n"
     "Common options:\n"
     "-l ##   length of bufs read from or written to network (default 8192)\n"
     "-v      verbose: print more statistics\n"
     "-d ##   debug level\n"
     "-f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n"
     "-h ##   Remote host to send data to\n"
     "-p ##   Port number"
     "-L ##   Output file name to store results\n"
     "-S ##   Total Data Size to be sent\n"
     "-q <type> Send Sequence: Enumeration for various data types:\n"
     "s = short, l = long,  d = double, c = char\n"
     "o = octet, S = BinStruct \n";

u_int _debug_level = 0;

CORBA::Long trans = 1;          // we are a client - used for debugging purposes
CORBA::Long buflen = 8 * 1024;  // length of buffer, default 8,192 bytes
char *buf;              // ptr to dynamic buffer
CORBA::Long nbuf;               // number of buffers to send in sinkmode
CORBA::Short port = ACE_DEFAULT_SERVER_PORT;    // TCP port number
char *host = "localhost";              // ptr to name of host
CORBA::Long verbose = 0;        // 0=print basic info, 1=print cpu
                                // rate, proc resource usage.
CORBA::Char fmt = 'K';          // output
                                // format:k=kilobits,K=kilobytes, m = megabits,
                                // M = megabytes, g = gigabits, G = gigabytes
char *title = 0;        // title of file to gather statistics
char stats[128];                // gathering the statistics
unsigned long srcDataSize;      // Total amount of source data
unsigned long nbytes;           // bytes on net
unsigned long numCalls;         // # of I/O system calls
double cput, realt;             // user, real time (seconds)
unsigned long   dt;             // data type

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
  try {
  int c;
  CORBA::ORB_ptr        orb_ptr;   // underlying ORB
  CORBA::String  objkey = "TTCP_IIOP_test"; // name of the TTCP object on the
                                            // server
  CORBA::Object_ptr     objref = CORBA::Object::_nil(); // object reference
  ttcp_sequence_ptr     ttcp_seq = 0;  // obj reference to TTCP object
  CORBA::Environment    env;       // environment
  CORBA::String str;               // holds the IOR

  fstream iorfile;

  ACE_UNUSED_ARG (objkey);

  // parse the arguments
  ACE_Get_Opt get_opt (argc, argv, "d:vh:f:l:L:S:q:"); // Command line options
  _debug_level = 0;
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
        case 'd':
          _debug_level = ACE_OS::atoi (get_opt.optarg);
          if (_debug_level > 10)
            _debug_level = 10;
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
          //    default:
          //return print_usage ();
        }
    }

  //
  // Transmitter
  //

  // get a handle to the ORB
  orb_ptr = CORBA::ORB_init (argc,
                             argv);
  if (env.exception () != 0)
    {
      //      env.print_exception ("ORB initialization", stdout);
      return -1;
    }

  // Retrieve a factory objref.
  objref = orb_ptr->resolve_initial_references ("TTCP_IIOP_test");
  //  @@ put exception handling here !!

  if (CORBA::is_nil (objref) == CORBA_TRUE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "resolve_initial_references returned null object for ttcp_sequence\n"),
                      -1);


  if (!CORBA::is_nil (objref))
    {
      // if it is a valid obj ref, narrow it to a ttcp_sequence CORBA object
      ttcp_seq = ttcp_sequence::_narrow (objref);

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
          ttcp_seq->start_timer (); // ask the server to start its timer
          if (env.exception () != 0)
            {
              //              env.print_exception ("start_timer operation", stdout);
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
                ttcp_seq->sendShortSeq (*sseq);
                nbytes += sseq->length () * sizeof (CORBA::Short);
                break;
              case SEND_LONG:
                ttcp_seq->sendLongSeq (*lseq);
                nbytes += lseq->length () * sizeof (CORBA::Long);
                break;
              case SEND_OCTET:
                ttcp_seq->sendOctetSeq (*oseq);
                nbytes += oseq->length () * sizeof (CORBA::Octet);
                break;
              case SEND_DOUBLE:
                ttcp_seq->sendDoubleSeq (*dseq);
                nbytes += dseq->length () * sizeof (CORBA::Double);
                break;
              case SEND_CHAR:
                ttcp_seq->sendCharSeq (*cseq);
                nbytes += cseq->length () * sizeof (CORBA::Char);
                break;
              case SEND_STRUCT:
                ttcp_seq->sendStructSeq (*Sseq);
                nbytes += Sseq->length () * sizeof (BinStruct);
                break;
              }

              numCalls++; // nbytes and numCalls are used in the thruput
                          // measurement
            }
#if defined (ACE_HAS_QUANTIFY)
          quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */
          //
          // Stop the timer
          //
          // stop the timer on the server side
          ttcp_seq->stop_timer ();

          if (env.exception () != 0)
            {
              //              env.print_exception ("stop_timer operation", stdout);
              return -1;
            }
          // stop our timer
          (void) read_timer (stats, sizeof (stats));

          // print results
          PrintStats();
        }
    }

  CORBA::release (ttcp_seq);
  CORBA::release (objref);
  CORBA::release (orb_ptr);
  }
  catch(const CORBA::SystemException& exc)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA::SystemException caught: %s (minor: %d): client.cpp\n\n",
                  exc._repository_id (),
                  exc.minor ()));
    }
  catch(const CORBA::UserException& exc)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA::UserException caught: %s: client.cpp\n\n",
                  exc._repository_id ()));
    }
  catch(...)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Exception caught: client.cpp\n\n"));
    }

  return (0);
}

int print_usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage error\n"));
  ACE_ERROR ((LM_ERROR, "%s\n", Usage));
  return -1;
}
