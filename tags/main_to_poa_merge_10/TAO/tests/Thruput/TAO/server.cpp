// $Id$
// ============================================================================
//
// = TAO tests
//    Throughput measurement using the TTCP benchmark adapted to work using TAO
//
// = FILENAME
//   server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include <iostream.h>
#include <fstream.h>

#include "ace/ACE.h"
#include "ttcp_i.h"
#include "ace/Get_Opt.h"

char Usage[] = "\
Usage: server [TAO options] [options] \n\
Common options:\n\
-l ##   length of bufs read from or written to network (default 8192)\n\
-v      verbose: print more statistics\n\
-d ##   set debug level \n\
-f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
-L ##	Output file name for the data type used\n\n\
";

CORBA::Long trans = 0;            // we are the receiver
CORBA::Long buflen = 8 * 1024;	  // length of buffer
CORBA::Char *buf;		          // ptr to dynamic buffer
CORBA::Long nbuf;		          // number of buffers to send in sinkmode
CORBA::Long verbose = 0;	  // 0=print basic info, 1=print cpu rate, proc
                                  // resource usage.
CORBA::Char fmt = 'K';		  // output format:k=kilobits,K=kilobytes, m =
                                  // megabits, M = megabytes, g = gigabits, G =
                                  // gigabytes

CORBA::Char *title = 0;           // results filename

CORBA::Char stats[128];       // gathering the statistics
CORBA::ULong srcDataSize;	  // Total amount of source data
CORBA::ULong nbytes;		  // bytes on net
CORBA::ULong numCalls;		  // # of I/O system calls
CORBA::Double cput, realt;	  // user, real time (seconds)
CORBA::ULong dt;              // data type

/* declare struct variables for various message types */
ttcp_sequence::ShortSeq		*sseq;
ttcp_sequence::LongSeq		*lseq;
ttcp_sequence::OctetSeq		*oseq;
ttcp_sequence::DoubleSeq	*dseq;
ttcp_sequence::CharSeq		*cseq;
ttcp_sequence::StructSeq	*Sseq;

ttcp_sequence_i *my_ttcp;  // instance of the target object

// main program - Driver
int
main (int argc, char **argv)
{

  int c;                      // option
  CORBA::Environment env;     // environment
  CORBA::ORB_ptr     orb_ptr; // handle to the ORB
  CORBA::POA_ptr     oa_ptr;  // Object adapter
  CORBA::String      key = (CORBA::String) "key0"; // key assigned to our
                                                   // target object
  char              *oa_name = "POA"; // name of our OA
  char              *orb_name = "internet"; // name of our ORB
  CORBA::String      str; // for stringified representation of the object reference

  ACE_UNUSED_ARG (key);

  // initialize the underlying ORB and get a handle to it
  orb_ptr = CORBA::ORB_init (argc, argv, orb_name, env);
  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // now get a handle to the object adapter
  oa_ptr = orb_ptr->POA_init (argc, argv, oa_name);
  if (env.exception () != 0)
    {
      env.print_exception ("OA init");
      return 1;
    }

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      1);

  // for parsing the arguments
  ACE_Get_Opt get_opt (argc, argv, "l:vd:f:L:");

  TAO_debug_level = 0;
  for (; (c = get_opt ()) != EOF;)
    {
      switch (c)
        {
        case 'L':
          // title of output file that stores result
          title = ACE_OS::strdup (get_opt.optarg);
          break;
        case 'd':
          // debugging level
          TAO_debug_level = ACE_OS::atoi (get_opt.optarg);
          if (TAO_debug_level > 10)
            TAO_debug_level = 10;
          break;
        case 'f':
          // output format i.e., Mbps, Kbps, etc
          fmt = *get_opt.optarg;
          break;
        }
    }

  //
  // Receiver
  //

  // create an instance of an object implementing the "ttcp" interface
  my_ttcp = new ttcp_sequence_i ("TTCP_IIOP_test"); // this is its name

  if (TAO_debug_level > 0)
    {
      // get a stringified representation of the object reference created above
      str = orb_ptr->object_to_string (my_ttcp, env);
      if (env.exception() != 0)
        {
          env.print_exception ("object_to_string", stdout);
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "stringified obj reference = %s\n", str));
    }

#if defined (USE_QUANTIFY)
  // gather profile data
  quantify_clear_data();
  quantify_start_recording_data();
#endif

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;

  // usage:
  // fprintf (stderr, Usage);
  // return(1);
}
