// $Id$

#include "ace/Get_Opt.h"
#include "ace/Array_Base.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"

ACE_RCSID(Collocations, Collocations, "$Id$")

RTCORBA::Priority default_thread_priority;

class test_i;

struct Test_Object_And_Servant
{
  test_var object_;
  test_i *servant_;
};

typedef ACE_Array_Base<Test_Object_And_Servant> Tests;

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          Tests &tests);

  void start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

  int client_propagated (void);

  void client_propagated (int);

  void thread_info (const char *method_name);

  void invocation_pool_and_lane (CORBA::ULong pool,
                                 CORBA::ULong lane);

  void invocation_pool (CORBA::ULong pool);

  void invocation_lane (CORBA::ULong lane);

private:

  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  Tests &tests_;

  CORBA::ULong pool_;
  CORBA::ULong lane_;

  int client_propagated_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                Tests &tests)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    tests_ (tests),
    client_propagated_ (0)
{
}

int
test_i::client_propagated (void)
{
  return this->client_propagated_;
}

void
test_i::client_propagated (int client_propagated)
{
  this->client_propagated_ = client_propagated;
}

void
test_i::invocation_pool_and_lane (CORBA::ULong pool,
                                  CORBA::ULong lane)
{
  this->pool_ = pool;
  this->lane_ = lane;
}

void
test_i::invocation_pool (CORBA::ULong pool)
{
  this->pool_ = pool;
}

void
test_i::invocation_lane (CORBA::ULong lane)
{
  this->lane_ = lane;
}

void
test_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  this->thread_info ("test_i::start");

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  Tests::ITERATOR iterator (this->tests_);
  while (!iterator.done ())
    {
      Test_Object_And_Servant *test = 0;
      iterator.next (test);

      if (test->servant_->client_propagated ())
        {
          CORBA::Object_var object =
            this->orb_->resolve_initial_references ("RTCurrent"
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          RTCORBA::Current_var current =
            RTCORBA::Current::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          CORBA::Short current_thread_priority =
            current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (current_thread_priority == default_thread_priority)
            test->servant_->invocation_lane (0);
          else
            test->servant_->invocation_lane (1);
        }

      test->object_->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var ior =
        this->orb_->object_to_string (test->object_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var object =
        this->orb_->string_to_object (ior.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      test_var test_from_string =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      test_from_string->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      iterator.advance ();
    }
}

void
test_i::thread_info (const char *method_name)
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_->orb_core ()->get_tss_resources ();

  /// Get the lane attribute in TSS.
  TAO_Thread_Lane *lane =
    (TAO_Thread_Lane *) tss->lane_;

  if (lane)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s invoked by thread %t (pool id = %d; lane id = %d)\n",
                  method_name,
                  lane->pool ().id (),
                  lane->id ()));

      ACE_ASSERT (this->pool_ == lane->pool ().id ());
      ACE_ASSERT (this->lane_ == lane->id ());
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s invoked by thread %t (default thread pool)\n",
                  method_name));

      ACE_ASSERT (this->pool_ == 0);
      ACE_ASSERT (this->lane_ == 0);
    }
}

void
test_i::method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->thread_info ("test_i::method");
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

class Server
{
public:
  Server (int argc,
          char *argv[]
          ACE_ENV_ARG_DECL);

  void create_servant_in_root_poa (ACE_ENV_SINGLE_ARG_DECL);
  void create_poa_and_servant_with_tp_policy (ACE_ENV_SINGLE_ARG_DECL);
  void create_poa_and_servant_with_tp_with_lanes_policy (const char *poa_name,
                                                         RTCORBA::PriorityModel priority_model
                                                         ACE_ENV_ARG_DECL);
  void test (ACE_ENV_SINGLE_ARG_DECL);
  void start_testing (ACE_ENV_SINGLE_ARG_DECL);
  void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  CORBA::ORB_var orb_;
  RTCORBA::RTORB_var rt_orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POAManager_var poa_manager_;
  RTCORBA::Current_var current_;

  Tests tests_;

  CORBA::ULong stacksize_;
  CORBA::ULong static_threads_;
  CORBA::ULong dynamic_threads_;
  CORBA::Boolean allow_request_buffering_;
  CORBA::Boolean allow_borrowing_;
  CORBA::ULong max_buffered_requests_;
  CORBA::ULong max_request_buffer_size_;
};

Server::Server (int argc,
                char *argv[]
                ACE_ENV_ARG_DECL)
  : stacksize_ (0),
    static_threads_ (1),
    dynamic_threads_ (0),
    allow_request_buffering_ (0),
    allow_borrowing_ (0),
    max_buffered_requests_ (0),
    max_request_buffer_size_ (0)
{
  this->orb_ =
    CORBA::ORB_init (argc,
                     argv,
                     ""
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->rt_orb_ =
    RTCORBA::RTORB::_narrow (object.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object =
    this->orb_->resolve_initial_references ("RTCurrent"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->current_ =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  default_thread_priority =
    this->current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  object =
    this->orb_->resolve_initial_references ("RootPOA"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->root_poa_ =
    PortableServer::POA::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_manager_ =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_manager_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Server::create_servant_in_root_poa (ACE_ENV_SINGLE_ARG_DECL)
{
  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            this->root_poa_.in (),
                            this->tests_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  servant->invocation_pool_and_lane (0, 0);

  PortableServer::ServantBase_var safe_servant (servant);

  this->tests_.size (this->tests_.size () + 1);
  this->tests_[this->tests_.size () - 1].servant_ =
    servant;
  this->tests_[this->tests_.size () - 1].object_ =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Server::create_poa_and_servant_with_tp_policy (ACE_ENV_SINGLE_ARG_DECL)
{
  RTCORBA::ThreadpoolId threadpool_id =
    this->rt_orb_->create_threadpool (this->stacksize_,
                                      this->static_threads_,
                                      this->dynamic_threads_,
                                      default_thread_priority,
                                      this->allow_request_buffering_,
                                      this->max_buffered_requests_,
                                      this->max_request_buffer_size_
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var implicit_activation_policy =
    this->root_poa_->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  PortableServer::POA_var poa =
    this->root_poa_->create_POA ("tp_child",
                                 this->poa_manager_.in (),
                                 policies
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (this->orb_.in (),
                            poa.in (),
                            this->tests_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  servant->invocation_pool_and_lane (1, 0);

  PortableServer::ServantBase_var safe_servant (servant);

  this->tests_.size (this->tests_.size () + 1);
  this->tests_[this->tests_.size () - 1].servant_ =
    servant;
  this->tests_[this->tests_.size () - 1].object_ =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Server::create_poa_and_servant_with_tp_with_lanes_policy (const char *poa_name,
                                                          RTCORBA::PriorityModel priority_model
                                                          ACE_ENV_ARG_DECL)
{
  RTCORBA::ThreadpoolLanes lanes (2);
  lanes.length (2);

  lanes[0].lane_priority = default_thread_priority;
  lanes[0].static_threads = this->static_threads_;
  lanes[0].dynamic_threads = this->dynamic_threads_;

  lanes[1].lane_priority = default_thread_priority + 1;
  lanes[1].static_threads = this->static_threads_;
  lanes[1].dynamic_threads = this->dynamic_threads_;

  RTCORBA::ThreadpoolId threadpool_id =
    this->rt_orb_->create_threadpool_with_lanes (this->stacksize_,
                                                 lanes,
                                                 this->allow_borrowing_,
                                                 this->allow_request_buffering_,
                                                 this->max_buffered_requests_,
                                                 this->max_request_buffer_size_
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    this->rt_orb_->create_threadpool_policy (threadpool_id
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var priority_model_policy =
    this->rt_orb_->create_priority_model_policy (priority_model,
                                                 default_thread_priority
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    priority_model_policy;

  PortableServer::POA_var poa =
    this->root_poa_->create_POA (poa_name,
                                 this->poa_manager_.in (),
                                 policies
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (poa.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  test_i *servant1 = 0;
  test_i *servant2 = 0;
  ACE_NEW_THROW_EX (servant1,
                    test_i (this->orb_.in (),
                            poa.in (),
                            this->tests_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_NEW_THROW_EX (servant2,
                    test_i (this->orb_.in (),
                            poa.in (),
                            this->tests_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant1 (servant1);
  PortableServer::ServantBase_var safe_servant2 (servant2);

  PortableServer::ObjectId_var id1;
  PortableServer::ObjectId_var id2;

  if (priority_model == RTCORBA::SERVER_DECLARED)
    {
      servant1->invocation_pool_and_lane (3, 0);
      servant2->invocation_pool_and_lane (3, 1);

      id1 =
        rt_poa->activate_object_with_priority (servant1,
                                               default_thread_priority
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      id2 =
        rt_poa->activate_object_with_priority (servant2,
                                               default_thread_priority + 1
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      servant1->invocation_pool (2);
      servant2->invocation_pool (2);
      servant1->client_propagated (1);
      servant2->client_propagated (1);

      id1 =
        rt_poa->activate_object (servant1
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      id2 =
        rt_poa->activate_object (servant2
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  CORBA::Object_var object1 =
    poa->id_to_reference (id1.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var object2 =
    poa->id_to_reference (id2.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->tests_.size (this->tests_.size () + 1);
  this->tests_[this->tests_.size () - 1].object_ =
    test::_narrow (object1.in ()
                   ACE_ENV_ARG_PARAMETER);
  this->tests_[this->tests_.size () - 1].servant_ =
    servant1;
  ACE_CHECK;

  this->tests_.size (this->tests_.size () + 1);
  this->tests_[this->tests_.size () - 1].object_ =
    test::_narrow (object2.in ()
                   ACE_ENV_ARG_PARAMETER);
  this->tests_[this->tests_.size () - 1].servant_ =
    servant2;
  ACE_CHECK;
}

void
Server::start_testing (ACE_ENV_SINGLE_ARG_DECL)
{
  Tests::ITERATOR iterator (this->tests_);
  while (!iterator.done ())
    {
      Test_Object_And_Servant *test = 0;
      iterator.next (test);

      if (test->servant_->client_propagated ())
        {
          CORBA::Short current_thread_priority =
            this->current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (current_thread_priority == default_thread_priority)
            test->servant_->invocation_lane (0);
          else
            test->servant_->invocation_lane (1);
        }

      test->object_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      iterator.advance ();
    }
}

void
Server::test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->start_testing (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "\n\n*** Changing priority to be higher ***\n\n"));

  this->current_->the_priority (default_thread_priority + 1
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->start_testing (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Server::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->orb_->shutdown (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      Server server (argc,
                     argv
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.create_servant_in_root_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.create_poa_and_servant_with_tp_policy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.create_poa_and_servant_with_tp_with_lanes_policy ("tp_with_lanes_client_propagated_poa",
                                                               RTCORBA::CLIENT_PROPAGATED
                                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.create_poa_and_servant_with_tp_with_lanes_policy ("tp_with_lanes_server_declared_poa",
                                                               RTCORBA::SERVER_DECLARED
                                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<Test_Object_And_Servant>;
template class ACE_Array_Iterator<Test_Object_And_Servant>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<Test_Object_And_Servant>
#pragma instantiate ACE_Array_Iterator<Test_Object_And_Servant>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
