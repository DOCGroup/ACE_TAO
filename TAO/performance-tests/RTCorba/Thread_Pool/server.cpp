// $Id$

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "tao/debug.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "testS.h"

ACE_RCSID(Thread_Pools, server, "$Id$")

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::ULong work,
               CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method (CORBA::ULong work,
                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::method: %hd units of work\n",
                work));

  const u_long prime_number = 9619;

  for (; work != 0; work--)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);
}

PortableServer::POA_ptr
test_i::_default_POA (CORBA_Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
  ACE_CHECK;
}

static const char *ior_output_file = "ior";
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static CORBA::ULong number_of_lanes = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:l:");
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

      case 'l':
        number_of_lanes = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-s <static threads> "
                           "-l <lanes> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
write_ior_to_file (const char *ior_file,
                   CORBA::ORB_ptr orb,
                   CORBA::Object_ptr object,
                   CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior =
    orb->object_to_string (object,
                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  FILE *output_file =
    ACE_OS::fopen (ior_file,
                   "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);

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

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTORB",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in (),
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (),
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::Priority default_thread_priority =
        current->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      RTCORBA::ThreadpoolId threadpool_id;
      CORBA::PolicyList policies;

      if (number_of_lanes != 0)
        {
          CORBA::Boolean allow_borrowing = 0;

          RTCORBA::ThreadpoolLanes lanes;
          lanes.length (number_of_lanes);

          RTCORBA::PriorityBands bands;
          bands.length (number_of_lanes);

          CORBA::Short priority_range =
            RTCORBA::maxPriority - RTCORBA::minPriority;

          ACE_DEBUG ((LM_DEBUG,
                      "\nUsing %d lanes\n",
                      number_of_lanes));

          for (CORBA::ULong i = 0;
               i < number_of_lanes;
               ++i)
            {
              CORBA::Short high_priority =
                CORBA::Short (
                  ACE_OS::floor ((priority_range /
                                  double (number_of_lanes)) *
                                 (i + 1)));

              CORBA::Short low_priority =
                CORBA::Short (
                  ACE_OS::ceil ((priority_range /
                                 double (number_of_lanes)) *
                                i));

              lanes[i].lane_priority = high_priority;
              lanes[i].static_threads = static_threads;
              lanes[i].dynamic_threads = dynamic_threads;

              bands[i].high = high_priority;
              bands[i].low = low_priority;

              ACE_DEBUG ((LM_DEBUG,
                          "%d: [%d %d] ",
                          i + 1,
                          low_priority,
                          high_priority));
            }

          ACE_DEBUG ((LM_DEBUG,
                      "\n\n"));

          threadpool_id =
            rt_orb->create_threadpool_with_lanes (stacksize,
                                                  lanes,
                                                  allow_borrowing,
                                                  allow_request_buffering,
                                                  max_buffered_requests,
                                                  max_request_buffer_size,
                                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_priority_banded_connection_policy (bands,
                                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                                  default_thread_priority,
                                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          threadpool_id =
            rt_orb->create_threadpool (stacksize,
                                       static_threads,
                                       dynamic_threads,
                                       default_thread_priority,
                                       allow_request_buffering,
                                       max_buffered_requests,
                                       max_request_buffer_size,
                                       ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_implicit_activation_policy
        (PortableServer::IMPLICIT_ACTIVATION,
         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        rt_orb->create_threadpool_policy (threadpool_id,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        root_poa->create_POA ("RT POA",
                              poa_manager.in (),
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_i *servant =
        new test_i (orb.in (),
                    poa.in ());

      PortableServer::ServantBase_var safe_servant (servant);
      ACE_UNUSED_ARG (safe_servant);

      test_var test =
        servant->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      result =
        write_ior_to_file (ior_output_file,
                           orb.in (),
                           test.in (),
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

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
