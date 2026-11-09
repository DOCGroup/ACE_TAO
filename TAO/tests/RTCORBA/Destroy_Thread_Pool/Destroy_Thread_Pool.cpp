#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "../check_supported_priorities.cpp"

struct Test_Options
{
  int iterations {5};
};

struct Threadpool_Options
{
  CORBA::ULong stacksize {};
  CORBA::ULong static_threads {1};
  CORBA::ULong dynamic_threads {};
  RTCORBA::Priority default_thread_priority {};
  CORBA::Boolean allow_request_buffering {};
  CORBA::Boolean allow_borrowing {};
  CORBA::ULong max_buffered_requests {};
  CORBA::ULong max_request_buffer_size {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <iterations> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

RTCORBA::ThreadpoolId
create_threadpool (RTCORBA::RTORB_ptr rt_orb,
                   Threadpool_Options const &options)
{
  return rt_orb->create_threadpool (options.stacksize,
                                    options.static_threads,
                                    options.dynamic_threads,
                                    options.default_thread_priority,
                                    options.allow_request_buffering,
                                    options.max_buffered_requests,
                                    options.max_request_buffer_size);
}

RTCORBA::ThreadpoolId
create_threadpool_with_lanes (RTCORBA::RTORB_ptr rt_orb,
                              Threadpool_Options const &options)
{
  RTCORBA::ThreadpoolLanes lanes (2);
  lanes.length (2);

  lanes[0].lane_priority = options.default_thread_priority;
  lanes[0].static_threads = options.static_threads;
  lanes[0].dynamic_threads = options.dynamic_threads;

  lanes[1].lane_priority = options.default_thread_priority;
  lanes[1].static_threads = options.static_threads;
  lanes[1].dynamic_threads = options.dynamic_threads;

  return rt_orb->create_threadpool_with_lanes (options.stacksize,
                                               lanes,
                                               options.allow_borrowing,
                                               options.allow_request_buffering,
                                               options.max_buffered_requests,
                                               options.max_request_buffer_size);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      Test_Options options;
      int result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      Threadpool_Options threadpool_options;
      threadpool_options.default_thread_priority =
        get_implicit_thread_CORBA_priority (orb.in ());

      for (int i {}; i < options.iterations; ++i)
        {
          RTCORBA::ThreadpoolId id =
            create_threadpool (rt_orb.in (), threadpool_options);

          rt_orb->destroy_threadpool (id);

          id =
            create_threadpool_with_lanes (rt_orb.in (), threadpool_options);

          rt_orb->destroy_threadpool (id);
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
