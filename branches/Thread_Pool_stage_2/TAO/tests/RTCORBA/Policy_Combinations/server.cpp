// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          CORBA::Short server_priority,
          CORBA::Short client_priority);

  CORBA::Short method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void prioritized_method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV);

private:

  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  CORBA::Short server_priority_;
  CORBA::Short client_priority_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                CORBA::Short server_priority,
                CORBA::Short client_priority)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    server_priority_ (server_priority),
    client_priority_ (client_priority)
{
}

CORBA::Short
test_i::method (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));

  return this->client_priority_;
}

void
test_i::prioritized_method (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTCurrent",
                                            ACE_TRY_ENV);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in (),
                               ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Short priority =
    current->the_priority (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "test_i::prioritized_method suppose to run at priority %d "
              "and is running at priority %d\n",
              this->server_priority_,
              priority));

  ACE_ASSERT (this->server_priority_ == priority);
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

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "s:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        ::server_priority = ::atoi (get_opts.optarg);
      break;

      case 'c':
        ::client_priority = ::atoi (get_opts.optarg);
      break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-s server priority "
                           "-c client priority "
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

class server
{
public:
  server (CORBA::ORB_ptr orb,
          RTCORBA::RTORB_ptr rt_orb,
          PortableServer::POA_ptr root_poa,
          PortableServer::POAManager_ptr poa_manager);

  void test_root_poa (CORBA::Environment &ACE_TRY_ENV);
  void test_child_poa (CORBA::Environment &ACE_TRY_ENV);

  typedef void (server::*test_function) (CORBA::PolicyList &policies,
                                         CORBA::Short server_priority,
                                         CORBA::Short client_priority,
                                         const char *test_name,
                                         CORBA::Environment &ACE_TRY_ENV);

  void test_default_lane_poa (CORBA::Short server_priority,
                              CORBA::Short client_priority,
                              server::test_function function,
                              const char *test_name,
                              CORBA::Environment &ACE_TRY_ENV);

  void test_bands_poa (CORBA::PolicyList &policies,
                       CORBA::Short server_priority,
                       CORBA::Short client_priority,
                       server::test_function function,
                       const char *test_name,
                       CORBA::Environment &ACE_TRY_ENV);

  void test_no_lanes_poa (CORBA::Short server_priority,
                          CORBA::Short client_priority,
                          server::test_function function,
                          const char *test_name,
                          CORBA::Environment &ACE_TRY_ENV);

  void test_lanes_poa (CORBA::Short server_priority,
                       CORBA::Short client_priority,
                       server::test_function function,
                       const char *test_name,
                       CORBA::Environment &ACE_TRY_ENV);

  void test_no_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                            CORBA::Short server_priority,
                                            CORBA::Short client_priority,
                                            const char *test_name,
                                            CORBA::Environment &ACE_TRY_ENV);

  void test_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                         CORBA::Short server_priority,
                                         CORBA::Short client_priority,
                                         const char *test_name,
                                         CORBA::Environment &ACE_TRY_ENV);

  void test_no_bands_server_declared_poa (CORBA::PolicyList &policies,
                                          CORBA::Short server_priority,
                                          CORBA::Short client_priority,
                                          const char *test_name,
                                          CORBA::Environment &ACE_TRY_ENV);

  void test_bands_server_declared_poa (CORBA::PolicyList &policies,
                                       CORBA::Short server_priority,
                                       CORBA::Short client_priority,
                                       const char *test_name,
                                       CORBA::Environment &ACE_TRY_ENV);

  void test_default_lane_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_no_lanes_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_lanes_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_default_lane_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_no_lanes_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_lanes_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_default_lane_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_no_lanes_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_lanes_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_default_lane_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_no_lanes_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

  void test_lanes_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV);

private:

  CORBA::ORB_var orb_;
  RTCORBA::RTORB_var rt_orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POAManager_var poa_manager_;
};

server::server (CORBA::ORB_ptr orb,
                RTCORBA::RTORB_ptr rt_orb,
                PortableServer::POA_ptr root_poa,
                PortableServer::POAManager_ptr poa_manager)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    rt_orb_ (RTCORBA::RTORB::_duplicate (rt_orb)),
    root_poa_ (PortableServer::POA::_duplicate (root_poa)),
    poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager))
{
}

void
server::test_root_poa (CORBA::Environment &ACE_TRY_ENV)
{
  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            this->root_poa_.in (),
                            TAO_INVALID_PRIORITY,
                            TAO_INVALID_PRIORITY),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      this->orb_.in (),
                      "root",
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_child_poa (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList policies;
  policies.length (1);
  policies[policies.length () - 1] =
    this->root_poa_->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var poa =
    this->root_poa_->create_POA ("child",
                                 this->poa_manager_.in (),
                                 policies,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            TAO_INVALID_PRIORITY,
                            TAO_INVALID_PRIORITY),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      this->orb_.in (),
                      "child",
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_default_lane_poa (CORBA::Short server_priority,
                               CORBA::Short client_priority,
                               server::test_function function,
                               const char *test_name,
                               CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList empty_policies;
  (this->*function) (empty_policies,
                     server_priority,
                     client_priority,
                     test_name,
                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_bands_poa (CORBA::PolicyList &policies,
                        CORBA::Short server_priority,
                        CORBA::Short client_priority,
                        server::test_function function,
                        const char *test_name,
                        CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::PriorityBands bands;
  bands.length (3);

  bands[0].low = default_thread_priority;
  bands[0].high = default_thread_priority;
  bands[1].low = ::server_priority;
  bands[1].high = ::server_priority + 1;
  bands[2].low = ::client_priority - 1;
  bands[2].high = ::client_priority;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_banded_connection_policy (bands,
                                                             ACE_TRY_ENV);
  ACE_CHECK;

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name,
                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_lanes_poa (CORBA::Short server_priority,
                           CORBA::Short client_priority,
                           server::test_function function,
                           const char *test_name,
                           CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ThreadpoolId threadpool_id =
    this->rt_orb_->create_threadpool (stacksize,
                                      static_threads,
                                      dynamic_threads,
                                      default_thread_priority,
                                      allow_request_buffering,
                                      max_buffered_requests,
                                      max_request_buffer_size,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policies;
  policies.length (1);
  policies[0] =
    threadpool_policy;

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name,
                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_lanes_poa (CORBA::Short server_priority,
                        CORBA::Short client_priority,
                        server::test_function function,
                        const char *test_name,
                        CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ThreadpoolLanes lanes;
  lanes.length (3);

  lanes[0].lane_priority = ::client_priority;
  lanes[0].static_threads = static_threads;
  lanes[0].dynamic_threads = dynamic_threads;

  lanes[1].lane_priority = ::server_priority;
  lanes[1].static_threads = static_threads;
  lanes[1].dynamic_threads = dynamic_threads;

  lanes[2].lane_priority = default_thread_priority;
  lanes[2].static_threads = static_threads;
  lanes[2].dynamic_threads = dynamic_threads;

  RTCORBA::ThreadpoolId threadpool_id =
    this->rt_orb_->create_threadpool_with_lanes (stacksize,
                                                 lanes,
                                                 allow_borrowing,
                                                 allow_request_buffering,
                                                 max_buffered_requests,
                                                 max_request_buffer_size,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policies;
  policies.length (1);
  policies[0] =
    threadpool_policy;

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name,
                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                             CORBA::Short server_priority,
                                             CORBA::Short client_priority,
                                             const char *test_name,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                                 0,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var poa =
    this->root_poa_->create_POA (test_name,
                                 this->poa_manager_.in (),
                                 policies,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            server_priority,
                            client_priority),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    rt_poa->activate_object (servant,
                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var object =
    poa->id_to_reference (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (object.in (),
                      this->orb_.in (),
                      test_name,
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                          CORBA::Short server_priority,
                                          CORBA::Short client_priority,
                                          const char *test_name,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  this->test_bands_poa (policies,
                        server_priority,
                        client_priority,
                        &server::test_no_bands_client_propagated_poa,
                        test_name,
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_default_lane_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_default_lane_poa (::client_priority,
                               ::client_priority,
                               &server::test_no_bands_client_propagated_poa,
                               "default_lane_no_bands_client_propagated",
                               ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_lanes_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_no_lanes_poa (::client_priority - 1,
                           ::client_priority - 1,
                           &server::test_no_bands_client_propagated_poa,
                           "no_lanes_no_bands_client_propagated",
                           ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_lanes_no_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_lanes_poa (::client_priority,
                        ::client_priority,
                        &server::test_no_bands_client_propagated_poa,
                        "lanes_no_bands_client_propagated",
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_default_lane_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_default_lane_poa (::client_priority - 1,
                               ::client_priority - 1,
                               &server::test_bands_client_propagated_poa,
                               "default_lane_bands_client_propagated",
                               ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_lanes_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_no_lanes_poa (::client_priority - 1,
                           ::client_priority - 1,
                           &server::test_bands_client_propagated_poa,
                           "no_lanes_bands_client_propagated",
                           ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_lanes_bands_client_propagated_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_lanes_poa (::client_priority,
                        ::client_priority - 1,
                        &server::test_bands_client_propagated_poa,
                        "lanes_bands_client_propagated",
                        ACE_TRY_ENV);
  ACE_CHECK;

}

void
server::test_no_bands_server_declared_poa (CORBA::PolicyList &policies,
                                           CORBA::Short server_priority,
                                           CORBA::Short client_priority,
                                           const char *test_name,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                 0,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var poa =
    this->root_poa_->create_POA (test_name,
                                 this->poa_manager_.in (),
                                 policies,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            server_priority,
                            client_priority),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    rt_poa->activate_object_with_priority (servant,
                                           ::server_priority,
                                           ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var object =
    poa->id_to_reference (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (object.in (),
                      this->orb_.in (),
                      test_name,
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_bands_server_declared_poa (CORBA::PolicyList &policies,
                                        CORBA::Short server_priority,
                                        CORBA::Short client_priority,
                                        const char *test_name,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  this->test_bands_poa (policies,
                        server_priority,
                        client_priority,
                        &server::test_no_bands_server_declared_poa,
                        test_name,
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_default_lane_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_default_lane_poa (::server_priority,
                               ::client_priority,
                               &server::test_no_bands_server_declared_poa,
                               "default_lane_no_bands_server_declared",
                               ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_lanes_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_no_lanes_poa (::server_priority,
                           ::client_priority,
                           &server::test_no_bands_server_declared_poa,
                           "no_lanes_no_bands_server_declared",
                           ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_lanes_no_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_lanes_poa (::server_priority,
                        ::client_priority,
                        &server::test_no_bands_server_declared_poa,
                        "lanes_no_bands_server_declared",
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_default_lane_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_default_lane_poa (::server_priority,
                               ::client_priority,
                               &server::test_bands_server_declared_poa,
                               "default_lane_bands_server_declared",
                               ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_no_lanes_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_no_lanes_poa (::server_priority,
                           ::client_priority,
                           &server::test_bands_server_declared_poa,
                           "no_lanes_bands_server_declared",
                           ACE_TRY_ENV);
  ACE_CHECK;
}

void
server::test_lanes_bands_server_declared_poa (CORBA::Environment &ACE_TRY_ENV)
{
  this->test_lanes_poa (::server_priority,
                        ::client_priority,
                        &server::test_bands_server_declared_poa,
                        "lanes_bands_server_declared",
                        ACE_TRY_ENV);
  ACE_CHECK;
}

int
main (int argc, char **argv)
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
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

      server server (orb.in (),
                     rt_orb.in (),
                     root_poa.in (),
                     poa_manager.in ());

      server.test_root_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_child_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_default_lane_no_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_default_lane_no_bands_server_declared_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_no_lanes_no_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_no_lanes_no_bands_server_declared_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_lanes_no_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_lanes_no_bands_server_declared_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_default_lane_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_default_lane_bands_server_declared_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_no_lanes_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_no_lanes_bands_server_declared_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_lanes_bands_client_propagated_poa (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server.test_lanes_bands_server_declared_poa (ACE_TRY_ENV);
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
