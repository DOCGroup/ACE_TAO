// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Globals.cpp
//
// = AUTHOR
//    Nagarajan Surendran
//
// ============================================================================

#include "Globals.h"

Globals::Globals (void)
  : ior_file (0),
    base_port (0),
    num_of_objs (2),
    use_name_service (1),
    thread_per_rate (0),
    use_multiple_priority (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    barrier_ (0)
{
  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    ACE_OS::perror ("gethostname");
}

int
Globals::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "sh:p:t:f:rm");
  int c;

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              hostname));

  while ((c = opts ()) != -1)
    {
      //      ACE_DEBUG ((LM_DEBUG,"parse_args:%c ,",c));
      switch (c)
      {
      case 'm':
        use_multiple_priority = 1;
        break;
      case 'r':
        thread_per_rate = 1;
        break;
      case 's':
        //        ACE_DEBUG ((LM_DEBUG,"Not using naming service\n"));
        use_name_service = 0;
        break;
      case 'f':
        //        ior_file = opts.optarg;
        ACE_NEW_RETURN (ior_file,
                        char[BUFSIZ],-1);
        ACE_OS::strcpy (ior_file,
                        opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG,"Using file %s",ior_file));
        break;
      case 'h':
        ACE_OS::strcpy (hostname, opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG, "h\n"));
        break;
      case 'p':
        base_port = ACE_OS::atoi (opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "base_port:%d\n",base_port));
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG,"num_of_objs:%d\n",num_of_objs));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " \t[-s Means NOT to use the name service] \n"
                           " \t[-p <port>]\n"
                           " \t[-h <my_hostname>]\n"
                           " \t[-t <num_objects>]\n"
                           " \t[-f <ior_file>]\n"
                           " \t[-r Use thread per rate]\n"
                           "\n", argv [0]),
                          -1);
      }
    }
  if (thread_per_rate == 1)
    num_of_objs = 4;

  // Indicates successful parsing of command line
  return 0;
}




















