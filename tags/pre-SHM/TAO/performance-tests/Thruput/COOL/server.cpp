// $Id$

// ============================================================================
//
// = Chorus COOL tests
//    Throughput measurement using the TTCP benchmark adapted to work using
//    Chorus COOL
//
// = FILENAME
//   server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//    Sergio Flores      Ported from TAO
//
// ============================================================================

#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/stream.h"

#include "ttcp_i.h"

#include <api/binding.H>
#include <api/api.H>
#include <api/coolThreadPool.H>
#include <corba/eoa.H>
#include <corba/eorb.H>

#include "ttcp.H"
#include "sk_ttcp.H"
#include "sk_ttcp.C"

ACE_RCSID(COOL, server, "$Id$")

char Usage[] = "\
Usage: server [COOL options] [Common options] \n\
  [COOL options] ** these are required ** \n\
    cool-tcp://<THIS_HOSTNAME>:<THIS_HOST_PORT> -cool-tcp -cool-iiop \n\n\
      THIS_HOSTNAME : the name of the host that the server will be running on. \n\
         This is useful if you want to specify a different interface (e.g. ATM, \n\
         100Mbit Ethernet)\n\n\
      THIS_HOST_PORT: is the port number at which the server will start the high\n\
                  priority servant. The low priority servant will be created at\n\
                  (THIS_HOST_PORT+1).\n\n\
  [Common options]:\n\
    -l ##   length of bufs read from or written to network (default 8192)\n\
    -v      verbose: print more statistics\n\
    -d ##   set debug level \n\
    -f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
    -L ##   Output file name for the data type used\n\n\
";

CORBA::Long trans = 0;            // we are the receiver
CORBA::Long buflen = 8 * 1024;    // length of buffer
CORBA::Char *buf;                         // ptr to dynamic buffer
CORBA::Long nbuf;                         // number of buffers to send in sinkmode
CORBA::Long verbose = 0;          // 0=print basic info, 1=print cpu rate, proc
                                  // resource usage.
CORBA::Char fmt = 'K';            // output format:k=kilobits,K=kilobytes, m =
                                  // megabits, M = megabytes, g = gigabits, G =
                                  // gigabytes
u_int debug_level = 0;

CORBA::String title = 0;           // results filename

CORBA::Char stats[128];       // gathering the statistics
CORBA::ULong srcDataSize;         // Total amount of source data
CORBA::ULong nbytes;              // bytes on net
CORBA::ULong numCalls;            // # of I/O system calls
CORBA::Double cput, realt;        // user, real time (seconds)
CORBA::ULong dt;              // data type

/* declare struct variables for various message types */
ttcp_sequence::ShortSeq         *sseq;
ttcp_sequence::LongSeq          *lseq;
ttcp_sequence::OctetSeq         *oseq;
ttcp_sequence::DoubleSeq        *dseq;
ttcp_sequence::CharSeq          *cseq;
ttcp_sequence::StructSeq        *Sseq;

// main program - Driver
int
main (int argc, char **argv)
{

  int c;                      // option
  CORBA::Environment env;     // environment
  CORBA_ORB_ptr     orb_ptr; // handle to the ORB
  CORBA_BOA_ptr      oa_ptr;  // Object adapter
  CORBA::String      str; // for stringified representation of the object reference
  ttcp_sequence_i my_ttcp_i ("TTCP_IIOP_test");  // instance of the target object
  ttcp_sequence_ptr my_ttcp;

  COOL::EOA::bind(argv[1], env);
  if (env.exception() != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Impossible to bind the ORB to the description:%s\n",
                         argv[1]), -1);
    }

  // initialize the underlying ORB and get a handle to it
  orb_ptr = CORBA_ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      ACE_DEBUG ((LM_ERROR, "ORB_init failed..\n"));
      CORBA::SystemException* ex;

      ex = CORBA::SystemException::_narrow(env.exception());
      if (ex)
        {
          CORBA::String_var msg = ex->message();
          fprintf(stderr, "%s.\n", (const char*) msg);
        }
      else
        {
          fprintf(stderr, "Unknown user exception.\n");
        }
      return -1;
    }

  // now get a handle to the object adapter
  oa_ptr = orb_ptr->OA_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      CORBA::SystemException* ex;

      ex = CORBA::SystemException::_narrow(env.exception());
      if (ex)
        {
          CORBA::String_var msg = ex->message();
          fprintf(stderr, "%s.\n", (const char*) msg);
        }
      else
        {
          fprintf(stderr, "Unknown user exception.\n");
        }
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the POA.\n"),
                        -1);
    }

  // for parsing the arguments
  ACE_Get_Opt get_opt (argc, argv, "l:vd:f:L:", 2, 1);
  debug_level = 0;
  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'v':
          verbose = 1;
          break;
        case 'L':
          // title of output file that stores result
          title = ACE_OS::strdup (get_opt.optarg);
          break;
        case 'd':
          // debugging level
          debug_level = ACE_OS::atoi (get_opt.optarg);
          if (debug_level > 10)
            debug_level = 10;
          break;
        case 'f':
          // output format i.e., Mbps, Kbps, etc
          fmt = *get_opt.optarg;
          break;
        default:
          ACE_ERROR ((LM_ERROR, "Usage error\n"));
          ACE_ERROR ((LM_ERROR, "%s\n", Usage));
          return -1;
        }
    }

  //
  // Receiver
  //

  COOL::EOABindingData bindingData;

  COOL_bind (my_ttcp_i, my_ttcp, bindingData, env);

  if (env.exception ())
    {
      ACE_DEBUG ((LM_ERROR, "Bind failed.\n"));
      return -1;
    }

  if (debug_level > 0)
    {
      // get a stringified representation of the object reference created above
      str = orb_ptr->object_to_string (my_ttcp, env);
      if (env.exception() != 0)
        {
          CORBA::SystemException* ex;

          ex = CORBA::SystemException::_narrow(env.exception());
          if (ex)
            {
              CORBA::String_var msg = ex->message();
              fprintf(stderr, "%s.\n", (const char*) msg);
            }
          else
            {
              fprintf(stderr, "Unknown user exception.\n");
            }
          return -1;
        }
      ACE_DEBUG ((LM_DEBUG, "stringified obj reference = %s\n", str));
    }

#if defined (ACE_HAS_QUANTIFY)
  // gather profile data
  quantify_clear_data();
  quantify_start_recording_data();
#endif /* ACE_HAS_QUANTIFY */

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  ACE_DEBUG ((LM_DEBUG, "Entering boa->run ().\n"));
  oa_ptr->run ();

  return 0;
}
