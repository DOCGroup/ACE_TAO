// $Id$

// ============================================================================
//
// = TAO tests
//    Throughput measurement using the TTCP benchmark adapted to work
//    using TAO
//
// = FILENAME
//   server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "ace/ACE.h"
#include "ace/Get_Opt.h"

#include "ttcp_i.h"

ACE_RCSID(TAO, server, "$Id$")

char Usage[] = "\
Usage: server [TAO options] [options] \n\
Common options:\n\
-l ##   length of bufs read from or written to network (default 8192)\n\
-v      verbose: print more statistics\n\
-d ##   set debug level \n\
-f X    format for rate: k,K = kilo{bit,byte}; m,M = mega; g,G = giga\n\
-L ##   Output file name for the data type used\n\n\
-o ior_filename\
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

CORBA::Char *title = 0;           // results filename

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

// main program - Driver
int
main (int argc, char **argv)
{

  int c;                      // option
  CORBA::ORB_var     orb_var; // handle to the ORB
  PortableServer::POA_var root_poa; // Object adapter
  CORBA::Object_var obj_var;
  const char *oa_name = "RootPOA"; // name of our OA
  const char *orb_name = "internet"; // name of our ORB
  CORBA::String      str; // for stringified representation of the object reference
  FILE *ior_file = 0;

  // initialize the underlying ORB and get a handle to it
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb_var = CORBA::ORB_init (argc, argv, orb_name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      obj_var = orb_var->resolve_initial_references(oa_name,
                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil(obj_var.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      root_poa = PortableServer::POA::_narrow (obj_var.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      PortableServer::POA_var good_poa =
        root_poa->create_POA ("child_poa",
                              poa_manager.in (),
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // for parsing the arguments
      ACE_Get_Opt get_opt (argc, argv, "l:vd:f:L:o:");

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
            case 'o':
              ior_file = ACE_OS::fopen (get_opt.optarg,"w");
              ACE_DEBUG ((LM_DEBUG,"ior_file is %s\n",get_opt.optarg));
              break;
            }
        }
      
      //
      // Receiver
      //

      // create an instance of an object implementing the "ttcp" interface
      my_ttcp = new ttcp_sequence_i; // this is its name

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("TTCP_IIOP_test");
      good_poa->activate_object_with_id (id.in (),
                                         my_ttcp,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      obj_var = good_poa->id_to_reference (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // get a stringified representation of the object reference
      // created above
      str = orb_var->object_to_string (obj_var.in (),
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "stringified obj reference = %s\n", str));
        }
      
      if (ior_file != 0)
        {
          // write ior to a file
          ACE_OS::fprintf (ior_file,"%s",str);
          ACE_OS::fclose (ior_file);
        }

#if defined (ACE_HAS_QUANTIFY)
      // gather profile data
      quantify_clear_data();
      quantify_start_recording_data();
#endif /* ACE_HAS_QUANTIFY */
      
      // Handle requests for this object until we're killed, or one of the
      // methods asks us to exit.
      if (orb_var->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "server");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
