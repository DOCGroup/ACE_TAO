// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          CORBA::Short server_priority,
          CORBA::Short client_priority);

  CORBA::Short method (void);

  void prioritized_method (void);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA (void);

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
test_i::method (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));

  return this->client_priority_;
}

void
test_i::prioritized_method (void)
{
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTCurrent");

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in ());

  CORBA::Short priority =
    current->the_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "test_i::prioritized_method: client = %d server = %d (should be %d)\n",
              this->client_priority_,
              priority,
              this->server_priority_));

  ACE_ASSERT (this->server_priority_ == priority);
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

static CORBA::Short server_priority;
static CORBA::Short client_priority;
static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 2;
static CORBA::ULong dynamic_threads = 0;
static RTCORBA::Priority default_thread_priority;
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;
static CORBA::Boolean allow_borrowing = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        ::server_priority = ACE_OS::atoi (get_opts.opt_arg ());
      break;

      case 'c':
        ::client_priority = ACE_OS::atoi (get_opts.opt_arg ());
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
                    const char *filename)
{
  FILE *file =
    ACE_OS::fopen (filename, "w");
  ACE_ASSERT (file != 0);

  CORBA::String_var ior =
    orb->object_to_string (object);

  u_int result = 0;
  result =
    ACE_OS::fprintf (file,
                     "%s",
                     ior.in ());

  ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));
  ACE_UNUSED_ARG (result);

  ACE_OS::fclose (file);
}

class server
{
public:
  server (CORBA::ORB_ptr orb,
          RTCORBA::RTORB_ptr rt_orb,
          PortableServer::POA_ptr root_poa,
          PortableServer::POAManager_ptr poa_manager);

  void test_root_poa (void);
  void test_child_poa (void);

  typedef void (server::*test_function) (CORBA::PolicyList &policies,
                                         CORBA::Short server_priority,
                                         CORBA::Short client_priority,
                                         const char *test_name);

  void test_default_pool_poa (CORBA::Short server_priority,
                              CORBA::Short client_priority,
                              server::test_function function,
                              const char *test_name);

  void test_bands_poa (CORBA::PolicyList &policies,
                       CORBA::Short server_priority,
                       CORBA::Short client_priority,
                       server::test_function function,
                       const char *test_name);

  void test_no_lanes_poa (CORBA::Short server_priority,
                          CORBA::Short client_priority,
                          server::test_function function,
                          const char *test_name);

  void test_lanes_poa (CORBA::Short server_priority,
                       CORBA::Short client_priority,
                       server::test_function function,
                       const char *test_name);

  void test_no_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                            CORBA::Short server_priority,
                                            CORBA::Short client_priority,
                                            const char *test_name);

  void test_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                         CORBA::Short server_priority,
                                         CORBA::Short client_priority,
                                         const char *test_name);

  void test_no_bands_server_declared_poa (CORBA::PolicyList &policies,
                                          CORBA::Short server_priority,
                                          CORBA::Short client_priority,
                                          const char *test_name);

  void test_bands_server_declared_poa (CORBA::PolicyList &policies,
                                       CORBA::Short server_priority,
                                       CORBA::Short client_priority,
                                       const char *test_name);

  void test_default_pool_no_bands_client_propagated_poa (void);

  void test_no_lanes_no_bands_client_propagated_poa (void);

  void test_lanes_no_bands_client_propagated_poa (void);

  void test_default_pool_bands_client_propagated_poa (void);

  void test_no_lanes_bands_client_propagated_poa (void);

  void test_lanes_bands_client_propagated_poa (void);

  void test_default_pool_no_bands_server_declared_poa (void);

  void test_no_lanes_no_bands_server_declared_poa (void);

  void test_lanes_no_bands_server_declared_poa (void);

  void test_default_pool_bands_server_declared_poa (void);

  void test_no_lanes_bands_server_declared_poa (void);

  void test_lanes_bands_server_declared_poa (void);

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
server::test_root_poa (void)
{
  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            this->root_poa_.in (),
                            TAO_INVALID_PRIORITY,
                            TAO_INVALID_PRIORITY),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    this->root_poa_->activate_object (servant);

  CORBA::Object_var object_act = this->root_poa_->id_to_reference (id.in ());

  test_var test =
    test::_narrow (object_act.in ());

  write_iors_to_file (test.in (),
                      this->orb_.in (),
                      "root");
}

void
server::test_child_poa (void)
{
  CORBA::PolicyList policies;
  policies.length (1);
  policies[policies.length () - 1] =
    this->root_poa_->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  PortableServer::POA_var poa =
    this->root_poa_->create_POA ("child",
                                 this->poa_manager_.in (),
                                 policies);

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            TAO_INVALID_PRIORITY,
                            TAO_INVALID_PRIORITY),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    poa->activate_object (servant);

  CORBA::Object_var object_act = poa->id_to_reference (id.in ());

  test_var test =
    test::_narrow (object_act.in ());

  write_iors_to_file (test.in (),
                      this->orb_.in (),
                      "child");
}

void
server::test_default_pool_poa (CORBA::Short server_priority,
                               CORBA::Short client_priority,
                               server::test_function function,
                               const char *test_name)
{
  CORBA::PolicyList empty_policies;
  (this->*function) (empty_policies,
                     server_priority,
                     client_priority,
                     test_name);
}

void
server::test_bands_poa (CORBA::PolicyList &policies,
                        CORBA::Short server_priority,
                        CORBA::Short client_priority,
                        server::test_function function,
                        const char *test_name)
{
  RTCORBA::PriorityBands bands;
  bands.length (3);

  bands[0].low = default_thread_priority;
  bands[0].high = default_thread_priority;
  bands[1].low = ::server_priority - 1;
  bands[1].high = ::server_priority + 1;
  bands[2].low = ::client_priority - 1;
  bands[2].high = ::client_priority + 1;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_banded_connection_policy (bands);

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name);
}

void
server::test_no_lanes_poa (CORBA::Short server_priority,
                           CORBA::Short client_priority,
                           server::test_function function,
                           const char *test_name)
{
  RTCORBA::ThreadpoolId threadpool_id =
    this->rt_orb_->create_threadpool (stacksize,
                                      static_threads,
                                      dynamic_threads,
                                      default_thread_priority,
                                      allow_request_buffering,
                                      max_buffered_requests,
                                      max_request_buffer_size);

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id);

  CORBA::PolicyList policies;
  policies.length (1);
  policies[0] =
    threadpool_policy;

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name);
}

void
server::test_lanes_poa (CORBA::Short server_priority,
                        CORBA::Short client_priority,
                        server::test_function function,
                        const char *test_name)
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
                                                 max_request_buffer_size);

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id);

  CORBA::PolicyList policies;
  policies.length (1);
  policies[0] =
    threadpool_policy;

  (this->*function) (policies,
                     server_priority,
                     client_priority,
                     test_name);
}

void
server::test_no_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                             CORBA::Short server_priority,
                                             CORBA::Short client_priority,
                                             const char *test_name)
{
  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                                 default_thread_priority);

  PortableServer::POA_var poa =
    this->root_poa_->create_POA (test_name,
                                 this->poa_manager_.in (),
                                 policies);

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in ());

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            server_priority,
                            client_priority),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    rt_poa->activate_object (servant);

  CORBA::Object_var object =
    poa->id_to_reference (id.in ());

  write_iors_to_file (object.in (),
                      this->orb_.in (),
                      test_name);
}

void
server::test_bands_client_propagated_poa (CORBA::PolicyList &policies,
                                          CORBA::Short server_priority,
                                          CORBA::Short client_priority,
                                          const char *test_name)
{
  this->test_bands_poa (policies,
                        server_priority,
                        client_priority,
                        &server::test_no_bands_client_propagated_poa,
                        test_name);
}

void
server::test_default_pool_no_bands_client_propagated_poa (void)
{
  this->test_default_pool_poa (::client_priority + 1,
                               ::client_priority + 1,
                               &server::test_no_bands_client_propagated_poa,
                               "default_pool_no_bands_client_propagated");
}

void
server::test_no_lanes_no_bands_client_propagated_poa (void)
{
  this->test_no_lanes_poa (::client_priority - 1,
                           ::client_priority - 1,
                           &server::test_no_bands_client_propagated_poa,
                           "no_lanes_no_bands_client_propagated");
}

void
server::test_lanes_no_bands_client_propagated_poa (void)
{
  this->test_lanes_poa (::client_priority,
                        ::client_priority,
                        &server::test_no_bands_client_propagated_poa,
                        "lanes_no_bands_client_propagated");
}

void
server::test_default_pool_bands_client_propagated_poa (void)
{
  this->test_default_pool_poa (::client_priority + 1,
                               ::client_priority + 1,
                               &server::test_bands_client_propagated_poa,
                               "default_pool_bands_client_propagated");
}

void
server::test_no_lanes_bands_client_propagated_poa (void)
{
  this->test_no_lanes_poa (::client_priority - 1,
                           ::client_priority - 1,
                           &server::test_bands_client_propagated_poa,
                           "no_lanes_bands_client_propagated");
}

void
server::test_lanes_bands_client_propagated_poa (void)
{
  this->test_lanes_poa (::client_priority,
                        ::client_priority + 1,
                        &server::test_bands_client_propagated_poa,
                        "lanes_bands_client_propagated");

}

void
server::test_no_bands_server_declared_poa (CORBA::PolicyList &policies,
                                           CORBA::Short server_priority,
                                           CORBA::Short client_priority,
                                           const char *test_name)
{
  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    this->rt_orb_->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                 default_thread_priority);

  PortableServer::POA_var poa =
    this->root_poa_->create_POA (test_name,
                                 this->poa_manager_.in (),
                                 policies);

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in ());

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            server_priority,
                            client_priority),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var id =
    rt_poa->activate_object_with_priority (servant,
                                           ::server_priority);

  CORBA::Object_var object =
    poa->id_to_reference (id.in ());

  write_iors_to_file (object.in (),
                      this->orb_.in (),
                      test_name);
}

void
server::test_bands_server_declared_poa (CORBA::PolicyList &policies,
                                        CORBA::Short server_priority,
                                        CORBA::Short client_priority,
                                        const char *test_name)
{
  this->test_bands_poa (policies,
                        server_priority,
                        client_priority,
                        &server::test_no_bands_server_declared_poa,
                        test_name);
}

void
server::test_default_pool_no_bands_server_declared_poa (void)
{
  this->test_default_pool_poa (::server_priority,
                               ::client_priority + 1,
                               &server::test_no_bands_server_declared_poa,
                               "default_pool_no_bands_server_declared");
}

void
server::test_no_lanes_no_bands_server_declared_poa (void)
{
  this->test_no_lanes_poa (::server_priority,
                           ::client_priority - 1,
                           &server::test_no_bands_server_declared_poa,
                           "no_lanes_no_bands_server_declared");
}

void
server::test_lanes_no_bands_server_declared_poa (void)
{
  this->test_lanes_poa (::server_priority,
                        ::client_priority + 1,
                        &server::test_no_bands_server_declared_poa,
                        "lanes_no_bands_server_declared");
}

void
server::test_default_pool_bands_server_declared_poa (void)
{
  this->test_default_pool_poa (::server_priority,
                               ::client_priority - 1,
                               &server::test_bands_server_declared_poa,
                               "default_pool_bands_server_declared");
}

void
server::test_no_lanes_bands_server_declared_poa (void)
{
  this->test_no_lanes_poa (::server_priority,
                           ::client_priority + 1,
                           &server::test_bands_server_declared_poa,
                           "no_lanes_bands_server_declared");
}

void
server::test_lanes_bands_server_declared_poa (void)
{
  this->test_lanes_poa (::server_priority,
                        ::client_priority - 1,
                        &server::test_bands_server_declared_poa,
                        "lanes_bands_server_declared");
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // The following sets the current thread to the lowest priority
      // for this scheduling policy.  This will give us the biggest
      // range on NT since the default priority is 0 where as the
      // lowest priority is -15.
      ACE_hthread_t current_thread;
      ACE_Thread::self (current_thread);

      int sched_policy =
        this->orb_->orb_core ()->orb_params ()->ace_sched_policy ();

      int minimum_priority =
        ACE_Sched_Params::priority_min (sched_policy);

      int result =
        ACE_Thread::setprio (current_thread,
                             minimum_priority);
      if (result != 0)
        return result;

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      client_priority =
        default_thread_priority + 2;

      server_priority =
        default_thread_priority + 5;

      object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      server server (this->orb_.in (),
                     rt_orb.in (),
                     root_poa.in (),
                     poa_manager.in ());

      server.test_root_poa ();

      server.test_child_poa ();

      server.test_default_pool_no_bands_client_propagated_poa ();

      server.test_default_pool_no_bands_server_declared_poa ();

      server.test_no_lanes_no_bands_client_propagated_poa ();

      server.test_no_lanes_no_bands_server_declared_poa ();

      server.test_lanes_no_bands_client_propagated_poa ();

      server.test_lanes_no_bands_server_declared_poa ();

      server.test_default_pool_bands_client_propagated_poa ();

      server.test_default_pool_bands_server_declared_poa ();

      server.test_no_lanes_bands_client_propagated_poa ();

      server.test_no_lanes_bands_server_declared_poa ();

      server.test_lanes_bands_client_propagated_poa ();

      server.test_lanes_bands_server_declared_poa ();

      poa_manager->activate ();

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %C\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
