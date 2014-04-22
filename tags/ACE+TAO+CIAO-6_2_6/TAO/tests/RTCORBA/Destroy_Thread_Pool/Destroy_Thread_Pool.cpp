// $Id$

#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "../check_supported_priorities.cpp"

static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static RTCORBA::Priority default_thread_priority;
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::Boolean allow_borrowing = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;
static int iterations = 5;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
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
create_threadpool (RTCORBA::RTORB_ptr rt_orb)
{
  RTCORBA::ThreadpoolId id =
    rt_orb->create_threadpool (stacksize,
                               static_threads,
                               dynamic_threads,
                               default_thread_priority,
                               allow_request_buffering,
                               max_buffered_requests,
                               max_request_buffer_size);

  return id;
}

RTCORBA::ThreadpoolId
create_threadpool_with_lanes (RTCORBA::RTORB_ptr rt_orb)
{
  RTCORBA::ThreadpoolLanes lanes (2);
  lanes.length (2);

  lanes[0].lane_priority = default_thread_priority;
  lanes[0].static_threads = static_threads;
  lanes[0].dynamic_threads = dynamic_threads;

  lanes[1].lane_priority = default_thread_priority;
  lanes[1].static_threads = static_threads;
  lanes[1].dynamic_threads = dynamic_threads;

  RTCORBA::ThreadpoolId id =
    rt_orb->create_threadpool_with_lanes (stacksize,
                                          lanes,
                                          allow_borrowing,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size);

  return id;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        orb->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =
        get_implicit_thread_CORBA_priority (orb.in ());

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      int i = 0;
      for (i = 0;
           i < iterations;
           ++i)
        {
          RTCORBA::ThreadpoolId id =
            create_threadpool (rt_orb.in ());

          rt_orb->destroy_threadpool (id);

          id =
            create_threadpool_with_lanes (rt_orb.in ());

          rt_orb->destroy_threadpool (id);
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
