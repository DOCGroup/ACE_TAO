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

#include <pbroker/corba/xpsorb.h>
#include <pbroker/corba/xpsboa.h>
#include <pbroker/corba/boad/boafull.h>   // will force usage of "Full" BOA API
#include <pbroker/unixsvc/unixsvc.h>

#include "ace/ACE.h"
#include "ttcp_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID(CORBAplus, server, "$Id$")

char Usage[] = "\
Usage: server [TAO options] [options] \n\
Common options:\n\
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

char *title = 0;           // results filename

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

ttcp_sequence_i *my_ttcp;  // instance of the target object

u_int _debug_level = 0;

// main program - Driver
int
main (int argc, char **argv)
{

  int c;                      // option
  CORBA::Environment env;     // environment
  XpsORB_ptr         orb_ptr = XpsORB::_nil (); // handle to the ORB
  XpsBOA *           oa_ptr  = XpsBOA::_nil ();  // Object adapter
  CORBA::String      key = (CORBA::String) "key0"; // key assigned to our
  // target object
  CORBA::String      str; // for stringified representation of the object reference

  ACE_UNUSED_ARG (key);

  try
    {
      // initialize the underlying ORB and get a handle to it
      orb_ptr = XpsORB::init (argc, argv);

      // now get a handle to the object adapter
      oa_ptr = XpsBOA::init (argc, argv);

      if (CORBA::is_nil(orb_ptr) || CORBA::is_nil(oa_ptr))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the ORB and/or the BOA\n"),
                            1);
        }

      // for parsing the arguments
      ACE_Get_Opt get_opt (argc, argv, "l:vd:f:L:");

      _debug_level = 0;
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
              _debug_level = ACE_OS::atoi (get_opt.optarg);
              if (_debug_level > 10)
                _debug_level = 10;
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

      if (1)//_debug_level > 0)
        {
          // get a stringified representation of the object reference created above
          str = orb_ptr->object_to_url (my_ttcp);
          ACE_DEBUG ((LM_DEBUG, "stringified obj reference = %s\n", str));
        }

#if defined (ACE_HAS_QUANTIFY)
      // gather profile data
      quantify_clear_data();
      quantify_start_recording_data();
#endif /* ACE_HAS_QUANTIFY */

      oa_ptr->obj_is_ready (my_ttcp, nil);
      oa_ptr->registerAlias (my_ttcp, "TTCP_IIOP_test");

      // Handle requests for this object until we're killed, or one of the
      // methods asks us to exit.
      XpsEventService  eventService;
      eventService.mainloop();   // control will not return to this point

    } /* end of try() */
  catch(const CORBA::SystemException& exc)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA::SystemException caught: %s (minor: %d): server.cpp\n\n",
                  exc._repository_id (),
                  exc.minor ()));
    }
  catch(const CORBA::UserException& exc)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA::UserException caught: %s: server.cpp\n\n",
                  exc._repository_id ()));
    }
  catch(...)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Exception caught: server.cpp\n\n"));
    }

  return 0;

  // usage:
  // fprintf (stderr, Usage);
  // return(1);
}
