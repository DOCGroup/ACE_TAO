// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"

ACE_RCSID(Thread_Pools, server, "$Id$")

const char *ior_output_file = "ior";
CORBA::ULong static_threads = 2;
CORBA::ULong dynamic_threads = 2;
long nap_time = 1000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:d:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;

      case 's':
        static_threads = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'd':
        dynamic_threads = ACE_OS::atoi (get_opts.optarg);
        break;

      case 't':
        nap_time = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-s <static_threads> "
                           "-d <dynamic_threads> "
                           "-t <nap_time> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "",
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Panic <RootPOA> is nil\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      test_i servant (orb.in (),
                      nap_time);
      test_var test =
        servant._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (test.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      FILE *output_file =
        ACE_OS::fopen (ior_output_file,
                       "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          -1);

      ACE_OS::fprintf (output_file,
                       "%s",
                       ior.in ());

      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong stacksize = 0;
      RTCORBA::Priority default_priority =
        RTCORBA::Priority (ACE_DEFAULT_THREAD_PRIORITY);
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      object =
        orb->resolve_initial_references ("RTORB",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in (),
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::ThreadpoolId id1 =
        rt_orb->create_threadpool (stacksize,
                                   static_threads,
                                   dynamic_threads,
                                   default_priority,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_UNUSED_ARG (id1);

      CORBA::Boolean allow_borrowing = 0;
      RTCORBA::ThreadpoolLanes lanes;
      lanes.length (1);

      lanes[0].lane_priority = default_priority;
      lanes[0].static_threads = static_threads;
      lanes[0].dynamic_threads = dynamic_threads;

      RTCORBA::ThreadpoolId id2 =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_UNUSED_ARG (id2);

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
