#include "RT_Policy_Validator.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/RTCORBA/RT_ORB.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Pluggable.h"
#include "tao/PortableServer/POA_Cached_Policies.h"

ACE_RCSID (tao,
           POA,
           "$Id$")

TAO_POA_RT_Policy_Validator::TAO_POA_RT_Policy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core),
    acceptor_registry_ (0),
    thread_pool_ (0)
{
  // No-Op.
}

TAO_POA_RT_Policy_Validator::~TAO_POA_RT_Policy_Validator (void)
{
  // No-Op.
}

TAO_Acceptor_Registry *
TAO_POA_RT_Policy_Validator::acceptor_registry (void)
{
  if (this->acceptor_registry_ == 0)
    this->acceptor_registry_ =
      TAO_POA_RT_Policy_Validator::extract_acceptor_registry (this->orb_core_,
                                                              this->thread_pool_);
  return this->acceptor_registry_;
}

void
TAO_POA_RT_Policy_Validator::validate_impl (TAO_Policy_Set &policies
                                            ACE_ENV_ARG_DECL)
{
  this->validate_thread_pool (policies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->validate_lifespan (policies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->validate_server_protocol (policies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->validate_priorities (policies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Boolean
TAO_POA_RT_Policy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE ||
          type == RTCORBA::THREADPOOL_POLICY_TYPE ||
          type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE ||
          type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE ||
          type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE);
}

void
TAO_POA_RT_Policy_Validator::validate_server_protocol (TAO_Policy_Set &policies
                                                       ACE_ENV_ARG_DECL)
{
  // Make sure we have an endpoint for at least one of the protocols
  // specified in the RTCORBA::ServerProtocolPolicy.  This ensure we
  // will be able to create non-nil object references.
  CORBA::Policy_var protocol =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (protocol.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ServerProtocolPolicy *server_protocol =
    ACE_dynamic_cast (TAO_ServerProtocolPolicy *,
                      server_protocol_policy.in ());

  RTCORBA::ProtocolList &protocols =
    server_protocol->protocols_rep ();

  TAO_Acceptor_Registry *acceptor_registry =
    this->acceptor_registry ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      int found = 0;
      CORBA::ULong protocol_type = protocols[j].protocol_type;
      for (TAO_AcceptorSetIterator a = acceptor_registry->begin ();
           a != acceptor_registry->end ();
           ++a)
        {
          if ((*a)->tag () == protocol_type)
            {
              found = 1;
              break;
            }
        }

      if (!found)
        ACE_THROW (PortableServer::POA::InvalidPolicy ());
    }

}

void
TAO_POA_RT_Policy_Validator::validate_priorities (TAO_Policy_Set &policies
                                                  ACE_ENV_ARG_DECL)
{
  // Initialize to the default priority/priority model.
  CORBA::Short priority =
    TAO_INVALID_PRIORITY;
  TAO_POA_Cached_Policies::PriorityModel rt_priority_model =
    TAO_POA_Cached_Policies::NOT_SPECIFIED;

  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

  RTCORBA::PriorityModelPolicy_var priority_model =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (priority_model.in ()))
    {
      priority = priority_model->server_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      rt_priority_model =
        TAO_POA_Cached_Policies::PriorityModel (
          priority_model->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_CHECK;

      // Check that the priority is in bounds.
      if (priority < RTCORBA::minPriority ||
          priority > RTCORBA::maxPriority)
        ACE_THROW (PortableServer::POA::InvalidPolicy ());
    }
  else
    // If priority model was not specified, then we better not have a
    // thread pool with lanes.
    {
      if (this->thread_pool_ != 0 &&
          this->thread_pool_->with_lanes ())
        ACE_THROW (PortableServer::POA::InvalidPolicy ());
    }

  policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

  RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
    = RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy.in ()
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_PriorityBandedConnectionPolicy *bands_policy =
    ACE_dynamic_cast (TAO_PriorityBandedConnectionPolicy *,
                      priority_bands.in ());

  // If priority banded connections are set, make sure that:
  //  0. A priority model was specified.
  //  1. There is at least one band.
  //  2a. low is not < RTCORBA::minPriority
  //  2b. low <= high
  //  2c. high is not > RTCORBA::maxPriority
  //  3. If priority model is SERVER_DECLARED, server_priority must
  //  match one of the bands.
  //  4. If this POA has a thread pool with lanes, then for each band,
  //  there must be at least one thread lane that can service it,
  //  i.e., whose priority falls into the band's range.
  if (bands_policy != 0)
    {
      // Checks 0.
      if (rt_priority_model == TAO_POA_Cached_Policies::NOT_SPECIFIED)
        ACE_THROW (PortableServer::POA::InvalidPolicy ());

      RTCORBA::PriorityBands &bands =
        bands_policy->priority_bands_rep ();

      // Checks 1.
      if (bands.length () == 0)
        ACE_THROW (PortableServer::POA::InvalidPolicy ());

      // Checks 2.
      for (CORBA::ULong i = 0; i < bands.length (); ++i)
        {
          //  2a. low is not < RTCORBA::minPriority
          //  2b. low is not > high
          //  2c. high is not > RTCORBA::maxPriority
          if (bands[i].low < RTCORBA::minPriority ||
              bands[i].low > bands[i].high ||
              bands[i].high > RTCORBA::maxPriority)
            ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }

      // Check 3.
      if (rt_priority_model == TAO_POA_Cached_Policies::SERVER_DECLARED)
        {
          int match = 0;
          for (CORBA::ULong i = 0; i < bands.length (); ++i)
            {
              if (priority <= bands[i].high &&
                  priority >= bands[i].low)
                {
                  match = 1;
                  break;
                }
            }

          if (!match)
            ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }

      //
      // Check 4.
      //

      // If this POA is using the default thread pool (which doesn't
      // have lanes) or a thread pool without lanes, we are done with
      // the checks.
      if (this->thread_pool_ == 0 ||
          !this->thread_pool_->with_lanes ())
        return;

      // If this POA is using a thread pool with lanes, make sure we
      // have at least one thread lane that corresponds to these
      // each band.
      TAO_Thread_Lane **lanes =
        this->thread_pool_->lanes ();

      for (CORBA::ULong band = 0;
           band < bands.length ();
           ++band)
        {
          int match = 0;
          for (CORBA::ULong lane = 0;
               lane != this->thread_pool_->number_of_lanes () && !match;
               ++lane)
            {
              CORBA::Short lane_priority =
                lanes[lane]->lane_priority ();

              if (lane_priority <= bands[band].high &&
                  lane_priority >= bands[band].low)
                match = 1;
            }
          if (!match)
            ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }

      // Done with checks.
      return;
    }

  // If priority banded connections are not set, and the priority
  // model is SERVER_DECLARED, make sure we have at least one thread
  // lane that can provide service for the specified SERVER_DECLARED
  // priority.
  if (rt_priority_model == TAO_POA_Cached_Policies::SERVER_DECLARED)
    {
      // If this POA is using the default thread pool (which doesn't
      // have lanes) or a thread pool without lanes, we are done with
      // the checks.
      if (this->thread_pool_ == 0 ||
          !this->thread_pool_->with_lanes ())
        return;

      // If this POA is using a thread pool with lanes, make sure we
      // have at least one thread lane that can provide service for
      // the specified SERVER_DECLARED priority.
      TAO_Thread_Lane **lanes =
        this->thread_pool_->lanes ();

      int match = 0;
      for (CORBA::ULong lane = 0;
           lane != this->thread_pool_->number_of_lanes () && !match;
           ++lane)
        {
          CORBA::Short lane_priority =
            lanes[lane]->lane_priority ();

          if (lane_priority <= priority &&
              lane_priority >= priority)
            match = 1;
        }
      if (!match)
        ACE_THROW (PortableServer::POA::InvalidPolicy ());

      // Done with checks.
      return;
    }

}

void
TAO_POA_RT_Policy_Validator::validate_thread_pool (TAO_Policy_Set &policies
                                                   ACE_ENV_ARG_DECL)
{
  this->thread_pool_ =
    TAO_POA_RT_Policy_Validator::extract_thread_pool (this->orb_core_,
                                                      policies
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_POA_RT_Policy_Validator::validate_lifespan (TAO_Policy_Set &policies
                                                ACE_ENV_ARG_DECL)
{
  // If this POA is using a RTCORBA thread pool, make sure the
  // lifespan policy is not persistent since we cannot support it
  // right now.
  if (this->thread_pool_ != 0)
    {
      CORBA::Policy_var policy =
        policies.get_cached_policy (TAO_CACHED_POLICY_LIFESPAN);
      PortableServer::LifespanPolicy_var lifespan_policy =
        PortableServer::LifespanPolicy::_narrow (policy.in ()
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      PortableServer::LifespanPolicyValue lifespan =
        lifespan_policy->value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (lifespan == PortableServer::PERSISTENT)
        {
          ACE_THROW (PortableServer::POA::InvalidPolicy ());
        }
    }
}

void
TAO_POA_RT_Policy_Validator::merge_policies_impl (TAO_Policy_Set &policies
                                                  ACE_ENV_ARG_DECL)
{
  // Check if the user has specified the priority model policy.
  CORBA::Policy_var priority_model =
    policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

  if (CORBA::is_nil (priority_model.in ()))
    {
      // If not, check if the priority model policy has been specified
      // at the ORB level.
      priority_model =
        this->orb_core_.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);
      if (!CORBA::is_nil (priority_model.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (priority_model.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  // Check if the user has specified the server protocol policy.
  CORBA::Policy_var server_protocol =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  if (CORBA::is_nil (server_protocol.in ()))
    {
      // If not, check if the server protocol policy has been
      // specified at the ORB level.
      server_protocol =
        this->orb_core_.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);
      if (!CORBA::is_nil (server_protocol.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (server_protocol.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  // Check if the user has specified the thread pool policy.
  CORBA::Policy_var thread_pool =
    policies.get_cached_policy (TAO_CACHED_POLICY_THREADPOOL);

  if (CORBA::is_nil (thread_pool.in ()))
    {
      // If not, check if the thread pool policy has been specified at
      // the ORB level.
      thread_pool =
        this->orb_core_.get_cached_policy (TAO_CACHED_POLICY_THREADPOOL);
      if (!CORBA::is_nil (thread_pool.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (thread_pool.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

/* static */
TAO_Thread_Pool *
TAO_POA_RT_Policy_Validator::extract_thread_pool (TAO_ORB_Core &orb_core,
                                                  TAO_Policy_Set &policies
                                                  ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_THREADPOOL);

  RTCORBA::ThreadpoolPolicy_var thread_pool_policy =
    RTCORBA::ThreadpoolPolicy::_narrow (policy.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (thread_pool_policy.in ()))
    return 0;

  RTCORBA::ThreadpoolId thread_pool_id =
    thread_pool_policy->threadpool (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the RTORB.
  CORBA::Object_var object =
    orb_core.resolve_rt_orb (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  RTCORBA::RTORB_var rt_orb =
    RTCORBA::RTORB::_narrow (object.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_RT_ORB *tao_rt_orb =
    ACE_dynamic_cast (TAO_RT_ORB *,
                      rt_orb.in ());

  TAO_Thread_Pool_Manager &tp_manager =
    tao_rt_orb->tp_manager ();

  TAO_Thread_Pool_Manager::THREAD_POOLS &thread_pools =
    tp_manager.thread_pools ();

  // Check that the thread pool id is valid.
  TAO_Thread_Pool *thread_pool = 0;
  int result =
    thread_pools.find (thread_pool_id,
                       thread_pool);

  if (result != 0)
    ACE_THROW_RETURN (PortableServer::POA::InvalidPolicy (),
                      0);

  return thread_pool;
}

/* static */
TAO_Acceptor_Registry *
TAO_POA_RT_Policy_Validator::extract_acceptor_registry (TAO_ORB_Core &orb_core,
                                                        TAO_Thread_Pool *thread_pool)
{
  TAO_Acceptor_Registry *acceptor_registry = 0;

  // If <thread_pool_> != 0, it means that we have a RT thread pool.
  if (thread_pool)
    {
      TAO_Thread_Lane **lanes =
        thread_pool->lanes ();

      // All the lanes have similar acceptor registries.  Therefore,
      // looking at the first lane should suffice.
      TAO_Thread_Lane_Resources &resources =
        lanes[0]->resources ();

      acceptor_registry =
        &resources.acceptor_registry ();
    }
  else
    // We are dealing with the default thread pool.
    {
      TAO_Thread_Lane_Resources_Manager &thread_lane_resources_manager =
        orb_core.thread_lane_resources_manager ();

      TAO_Thread_Lane_Resources &resources =
        thread_lane_resources_manager.default_lane_resources ();

      acceptor_registry =
        &resources.acceptor_registry ();
    }

  return acceptor_registry;
}
