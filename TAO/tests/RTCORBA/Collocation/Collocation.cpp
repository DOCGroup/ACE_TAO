// $Id$

#include "ace/Get_Opt.h"
#include "ace/Array.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"

ACE_RCSID(Collocations, Collocations, "$Id$")

typedef ACE_Array<test_var> Tests;

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          Tests &tests);

  void start (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV);

private:

  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  Tests &tests_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                Tests &tests)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    tests_ (tests)
{
}

void
test_i::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::start\n"));

  Tests::ITERATOR iterator (this->tests_);
  while (!iterator.done ())
    {
      test_var *test = 0;
      iterator.next (test);

      (*test)->method (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::String_var ior =
        this->orb_->object_to_string (test->in (),
                                      ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::Object_var object =
        this->orb_->string_to_object (ior.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK;

      test_var test_from_string =
        test::_narrow (object.in (),
                       ACE_TRY_ENV);
      ACE_CHECK;

      test_from_string->method (ACE_TRY_ENV);
      ACE_CHECK;

      iterator.advance ();
    }
}

void
test_i::method (CORBA::Environment &)
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
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
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

void
create_servant_in_root_poa (Tests &tests,
                            CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr root_poa,
                            CORBA::Environment &ACE_TRY_ENV)
{
  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            root_poa,
                            tests),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  tests.size (tests.size () + 1);
  tests[tests.size () - 1] =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;
}

void
create_poa_and_servant_with_tp_policy (Tests &tests,
                                       CORBA::ORB_ptr orb,
                                       RTCORBA::RTORB_ptr rt_orb,
                                       PortableServer::POA_ptr root_poa,
                                       PortableServer::POAManager_ptr poa_manager,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool (stacksize,
                               static_threads,
                               dynamic_threads,
                               default_thread_priority,
                               allow_request_buffering,
                               max_buffered_requests,
                               max_request_buffer_size,
                               ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("tp_child",
                          poa_manager,
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in (),
                            tests),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  tests.size (tests.size () + 1);
  tests[tests.size () - 1] =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;
}

void
create_poa_and_servant_with_tp_with_lanes_policy (Tests &tests,
                                                  CORBA::ORB_ptr orb,
                                                  RTCORBA::RTORB_ptr rt_orb,
                                                  PortableServer::POA_ptr root_poa,
                                                  PortableServer::POAManager_ptr poa_manager,
                                                  const char *poa_name,
                                                  RTCORBA::PriorityModel priority_model,
                                                  CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ThreadpoolLanes lanes (2);
  lanes.length (2);

  lanes[0].lane_priority = default_thread_priority;
  lanes[0].static_threads = static_threads;
  lanes[0].dynamic_threads = dynamic_threads;

  lanes[1].lane_priority = default_thread_priority + 1;
  lanes[1].static_threads = static_threads;
  lanes[1].dynamic_threads = dynamic_threads;

  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (stacksize,
                                          lanes,
                                          allow_borrowing,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size,
                                          ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var priority_model_policy =
    rt_orb->create_priority_model_policy (priority_model,
                                          0,
                                          ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    priority_model_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA (poa_name,
                          poa_manager,
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK;

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant1 = 0;
  test_i *servant2 = 0;
  ACE_NEW_THROW_EX (servant1,
                    test_i (orb,
                            poa.in (),
                            tests),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (servant2,
                    test_i (orb,
                            poa.in (),
                            tests),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant1 (servant1);
  PortableServer::ServantBase_var safe_servant2 (servant2);

  PortableServer::ObjectId_var id1;
  PortableServer::ObjectId_var id2;

  if (priority_model == RTCORBA::SERVER_DECLARED)
    {
      id1 =
        rt_poa->activate_object_with_priority (servant1,
                                               default_thread_priority,
                                               ACE_TRY_ENV);
      ACE_CHECK;

      id2 =
        rt_poa->activate_object_with_priority (servant2,
                                               default_thread_priority + 1,
                                               ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      id1 =
        rt_poa->activate_object (servant1,
                                 ACE_TRY_ENV);
      ACE_CHECK;

      id2 =
        rt_poa->activate_object (servant2,
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }

  CORBA::Object_var object1 =
    poa->id_to_reference (id1.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var object2 =
    poa->id_to_reference (id2.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  tests.size (tests.size () + 1);
  tests[tests.size () - 1] =
    test::_narrow (object1.in (),
                   ACE_TRY_ENV);
  ACE_CHECK;

  tests.size (tests.size () + 1);
  tests[tests.size () - 1] =
    test::_narrow (object2.in (),
                   ACE_TRY_ENV);
  ACE_CHECK;
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

      default_thread_priority =
        current->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

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

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Tests tests;

      create_servant_in_root_poa (tests,
                                  orb.in (),
                                  root_poa.in  (),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_and_servant_with_tp_policy (tests,
                                             orb.in (),
                                             rt_orb.in (),
                                             root_poa.in  (),
                                             poa_manager.in (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_and_servant_with_tp_with_lanes_policy (tests,
                                                        orb.in (),
                                                        rt_orb.in (),
                                                        root_poa.in  (),
                                                        poa_manager.in (),
                                                        "tp_with_lanes_client_propagated_poa",
                                                        RTCORBA::CLIENT_PROPAGATED,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_and_servant_with_tp_with_lanes_policy (tests,
                                                        orb.in (),
                                                        rt_orb.in (),
                                                        root_poa.in  (),
                                                        poa_manager.in (),
                                                        "tp_with_lanes_server_declared_poa",
                                                        RTCORBA::SERVER_DECLARED,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Tests::ITERATOR iterator (tests);
      while (!iterator.done ())
        {
          test_var *test = 0;
          iterator.next (test);

          (*test)->start (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          iterator.advance ();
        }

      orb->shutdown (1, ACE_TRY_ENV);
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
