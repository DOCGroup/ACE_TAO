// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV)
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
test_i::method (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));
}

void
test_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::POA_ptr
test_i::_default_POA (CORBA_Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

static CORBA::Short server_priority = 2;
static CORBA::Short client_priority = 6;
static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 2;
static CORBA::ULong dynamic_threads = 2;
static RTCORBA::Priority default_thread_priority =
RTCORBA::Priority (ACE_DEFAULT_THREAD_PRIORITY);
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;
static CORBA::Boolean allow_borrowing = 0;

static CORBA::ULong number_of_lanes = 1;
static CORBA::Short lane_priority[1];
static CORBA::ULong number_of_bands = 1;
static RTCORBA::PriorityBands band_priority;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    const char *filename,
                    CORBA::Environment &ACE_TRY_ENV)
{
  FILE *file =
    ACE_OS::fopen (filename, "w");
  ACE_ASSERT (file != 0);

  CORBA::String_var ior =
    orb->object_to_string (object,
                           ACE_TRY_ENV);
  ACE_CHECK;

  u_int result = 0;
  result =
    ACE_OS::fprintf (file,
                     "%s",
                     ior.in ());

  ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));

  ACE_OS::fclose (file);
}

int
main (int argc, char **argv)
{
  lane_priority[0] =
    default_thread_priority;

  band_priority.length (number_of_bands);

  band_priority[0].low =
    default_thread_priority;

  band_priority[0].high =
    default_thread_priority;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in (),
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      object =
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

      CORBA::ULong i = 0;

      RTCORBA::ThreadpoolLanes lanes;
      lanes.length (number_of_lanes);

      for (i = 0;
           i < number_of_lanes;
           ++i)
        {
          lanes[i].lane_priority = lane_priority[i];
          lanes[i].static_threads = static_threads;
          lanes[i].dynamic_threads = dynamic_threads;
        }

      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Policy_var threadpool_policy =
        rt_orb->create_threadpool_policy (threadpool_id,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::PriorityBands bands;
      bands.length (number_of_bands);

      for (i = 0;
           i < number_of_bands;
           ++i)
        {
          bands[i].low = band_priority[i].low;
          bands[i].high = band_priority[i].high;
        }

      CORBA::Policy_var banded_connection_policy =
        rt_orb->create_priority_banded_connection_policy (bands,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Policy_var priority_model_policy =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              default_thread_priority,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Implicit_activation policy.
      CORBA::Policy_var implicit_activation_policy =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies;
      policies.length (4);

      policies[0] =
        threadpool_policy;

      policies[1] =
        banded_connection_policy;

      policies[2] =
        priority_model_policy;

      policies[3] =
        implicit_activation_policy;

      PortableServer::POA_var poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_i *servant = 0;
      ACE_NEW_THROW_EX (servant,
                        test_i (orb.in (),
                                poa.in ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_servant (servant);

      test_var test =
        servant->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      write_iors_to_file (test.in (),
                          orb.in (),
                          "ior",
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
