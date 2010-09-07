// -*- C++ -*-
// $Id$

#include "tao/RTPortableServer/RT_Policy_Validator.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/PortableServer/POA_Cached_Policies.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/RTCORBA/RT_ORB.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/Transport_Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_POA_RT_Policy_Validator::TAO_POA_RT_Policy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core),
    thread_pool_ (0)
{
  // No-Op.
}

TAO_POA_RT_Policy_Validator::~TAO_POA_RT_Policy_Validator (void)
{
  // No-Op.
}

void
TAO_POA_RT_Policy_Validator::validate_impl (TAO_Policy_Set &policies)
{
  this->validate_thread_pool (policies);

  this->validate_server_protocol (policies);

  this->validate_priorities (policies);
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
TAO_POA_RT_Policy_Validator::validate_server_protocol (TAO_Policy_Set &policies)
{
  // Make sure we have an endpoint for at least one of the protocols
  // specified in the RTCORBA::ServerProtocolPolicy.  This ensure we
  // will be able to create non-nil object references.
  CORBA::Policy_var protocol =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  if (CORBA::is_nil (protocol.in ()))
    {
      // If the server protocol policy has not been specified, then
      // add a server policy that reflects the protocols supported by
      // the acceptor registries of the POA's thread pool.
      protocol =
        TAO_POA_RT_Policy_Validator::server_protocol_policy_from_thread_pool (this->thread_pool_,
                                                                              this->orb_core_);

      if (!CORBA::is_nil (protocol.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (protocol.in ());
        }
    }

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (protocol.in ());

  TAO_ServerProtocolPolicy *server_protocol =
    dynamic_cast <TAO_ServerProtocolPolicy *>
                      (server_protocol_policy.in ());

  RTCORBA::ProtocolList &protocols =
    server_protocol->protocols_rep ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      bool found = false;
      CORBA::ULong const protocol_type = protocols[j].protocol_type;

      if (this->thread_pool_)
        {
          TAO_Thread_Lane **lanes =
            this->thread_pool_->lanes ();

          for (CORBA::ULong i = 0;
               i != this->thread_pool_->number_of_lanes ();
               ++i)
            {
              TAO_Thread_Lane_Resources &resources =
                lanes[i]->resources ();

              TAO_Acceptor_Registry &acceptor_registry =
                resources.acceptor_registry ();

              for (TAO_AcceptorSetIterator a = acceptor_registry.begin ();
                   a != acceptor_registry.end ();
                   ++a)
                {
                  if ((*a)->tag () == protocol_type)
                    {
                      found = true;
                      break;
                    }
                }
            }
        }
      else
        {
          TAO_Thread_Lane_Resources_Manager &thread_lane_resources_manager =
            this->orb_core_.thread_lane_resources_manager ();

          TAO_Thread_Lane_Resources &resources =
            thread_lane_resources_manager.default_lane_resources ();

          TAO_Acceptor_Registry &acceptor_registry =
            resources.acceptor_registry ();

          for (TAO_AcceptorSetIterator a = acceptor_registry.begin ();
               a != acceptor_registry.end ();
               ++a)
            {
              if ((*a)->tag () == protocol_type)
                {
                  found = true;
                  break;
                }
            }
        }

      if (!found)
        throw PortableServer::POA::InvalidPolicy ();
    }
}

void
TAO_POA_RT_Policy_Validator::validate_priorities (TAO_Policy_Set &policies)
{
  // Initialize to the default priority/priority model.
  CORBA::Short priority =
    TAO_INVALID_PRIORITY;
  TAO::Portable_Server::Cached_Policies::PriorityModel rt_priority_model =
    TAO::Portable_Server::Cached_Policies::NOT_SPECIFIED;

  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

  RTCORBA::PriorityModelPolicy_var priority_model =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

  if (!CORBA::is_nil (priority_model.in ()))
    {
      priority = priority_model->server_priority ();

      rt_priority_model =
        TAO::Portable_Server::Cached_Policies::PriorityModel (
          priority_model->priority_model ());

      // Check that the priority is in bounds.
      if (priority < RTCORBA::minPriority
               // The line below will always be false unless the value of
               // RTCORBA::maxPriority, which is now assigned the value of
               // 32767, is changed in RTCORBA.pidl.
//          || priority > RTCORBA::maxPriority
         )
        {
          throw PortableServer::POA::InvalidPolicy ();
        }
    }
  else
    // If priority model was not specified, then we better not have a
    // thread pool with lanes.
    {
      if (this->thread_pool_ != 0 &&
          this->thread_pool_->with_lanes ())
        throw PortableServer::POA::InvalidPolicy ();
    }

  policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

  RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
    = RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy.in ());

  TAO_PriorityBandedConnectionPolicy *bands_policy =
    dynamic_cast<TAO_PriorityBandedConnectionPolicy *> (priority_bands.in ());

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
      if (rt_priority_model == TAO::Portable_Server::Cached_Policies::NOT_SPECIFIED)
        throw PortableServer::POA::InvalidPolicy ();

      RTCORBA::PriorityBands &bands =
        bands_policy->priority_bands_rep ();

      // Checks 1.
      if (bands.length () == 0)
        throw PortableServer::POA::InvalidPolicy ();

      // Checks 2.
      for (CORBA::ULong i = 0; i < bands.length (); ++i)
        {
          //  2a. low is not < RTCORBA::minPriority
          //  2b. low is not > high
          //  2c. high is not > RTCORBA::maxPriority
          if (bands[i].low < RTCORBA::minPriority
              || bands[i].low > bands[i].high
                   // The line below will always be false unless the value of
                   // RTCORBA::maxPriority, which is now assigned the value of
                   // 32767, is changed in RTCORBA.pidl.
//              || bands[i].high > RTCORBA::maxPriority
             )
            {
              throw PortableServer::POA::InvalidPolicy ();
            }
        }

      // Check 3.
      if (rt_priority_model == TAO::Portable_Server::Cached_Policies::SERVER_DECLARED)
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
            throw PortableServer::POA::InvalidPolicy ();
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
            throw PortableServer::POA::InvalidPolicy ();
        }

      // Done with checks.
      return;
    }

  // If priority banded connections are not set, and the priority
  // model is SERVER_DECLARED, make sure we have at least one thread
  // lane that can provide service for the specified SERVER_DECLARED
  // priority.
  if (rt_priority_model == TAO::Portable_Server::Cached_Policies::SERVER_DECLARED)
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
        throw PortableServer::POA::InvalidPolicy ();

      // Done with checks.
      return;
    }

}

void
TAO_POA_RT_Policy_Validator::validate_thread_pool (TAO_Policy_Set &policies)
{
  this->thread_pool_ =
    TAO_POA_RT_Policy_Validator::extract_thread_pool (this->orb_core_, policies);
}

void
TAO_POA_RT_Policy_Validator::merge_policies_impl (TAO_Policy_Set &policies)
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
          policies.set_policy (priority_model.in ());
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
          policies.set_policy (server_protocol.in ());
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
          policies.set_policy (thread_pool.in ());
        }
    }
}

/* static */
RTCORBA::ServerProtocolPolicy_ptr
TAO_POA_RT_Policy_Validator::server_protocol_policy_from_thread_pool (TAO_Thread_Pool *thread_pool,
                                                                      TAO_ORB_Core &orb_core)
{
  RTCORBA::ProtocolList protocols;

  if (thread_pool)
    {
      TAO_Thread_Lane **lanes =
        thread_pool->lanes ();

      for (CORBA::ULong i = 0;
           i != thread_pool->number_of_lanes ();
           ++i)
        {
          TAO_Thread_Lane_Resources &resources =
            lanes[i]->resources ();

          TAO_Acceptor_Registry &acceptor_registry =
            resources.acceptor_registry ();

          TAO_POA_RT_Policy_Validator::server_protocol_policy_from_acceptor_registry (protocols,
                                                                                      acceptor_registry,
                                                                                      orb_core);
        }
    }
  else
    {
      TAO_Thread_Lane_Resources_Manager &thread_lane_resources_manager =
        orb_core.thread_lane_resources_manager ();

      TAO_Thread_Lane_Resources &resources =
        thread_lane_resources_manager.default_lane_resources ();

      TAO_Acceptor_Registry &acceptor_registry =
        resources.acceptor_registry ();

      TAO_POA_RT_Policy_Validator::server_protocol_policy_from_acceptor_registry (protocols,
                                                                                  acceptor_registry,
                                                                                  orb_core);
    }

  // Set ServerProtocolPolicy.
  TAO_ServerProtocolPolicy *server_protocol_policy = 0;
  ACE_NEW_RETURN (server_protocol_policy,
                  TAO_ServerProtocolPolicy (protocols),
                  0);

  return server_protocol_policy;
}

/* static */
void
TAO_POA_RT_Policy_Validator::server_protocol_policy_from_acceptor_registry (RTCORBA::ProtocolList &protocols,
                                                                            TAO_Acceptor_Registry &acceptor_registry,
                                                                            TAO_ORB_Core &orb_core)
{
  TAO_AcceptorSetIterator end = acceptor_registry.end ();

  for (TAO_AcceptorSetIterator acceptor = acceptor_registry.begin ();
       acceptor != end;
       ++acceptor)
    {
      if (*acceptor == 0)
        continue;

      CORBA::ULong current_length = protocols.length ();

      // Make sure that this protocol is not already in the protocol
      // list.
      bool protocol_already_present = false;
      for (CORBA::ULong i = 0;
           i < current_length && !protocol_already_present;
           ++i)
        {
          if (protocols[i].protocol_type == (*acceptor)->tag ())
            protocol_already_present = true;
        }

      if (protocol_already_present)
        continue;

      protocols.length (current_length + 1);

      protocols[current_length].protocol_type =
        (*acceptor)->tag ();

      protocols[current_length].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      protocols[current_length].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property ((*acceptor)->tag (),
                                                                             &orb_core);
    }
}

/* static */
TAO_Thread_Pool *
TAO_POA_RT_Policy_Validator::extract_thread_pool (TAO_ORB_Core &orb_core,
                                                  TAO_Policy_Set &policies)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_THREADPOOL);

  RTCORBA::ThreadpoolPolicy_var thread_pool_policy =
    RTCORBA::ThreadpoolPolicy::_narrow (policy.in ());

  if (CORBA::is_nil (thread_pool_policy.in ()))
    return 0;

  RTCORBA::ThreadpoolId thread_pool_id = thread_pool_policy->threadpool ();

  // Get the RTORB.
  CORBA::Object_var object = orb_core.resolve_rt_orb ();

  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());

  TAO_RT_ORB * const tao_rt_orb =
    dynamic_cast <TAO_RT_ORB *> (rt_orb.in ());

  if (!tao_rt_orb)
    throw CORBA::INTERNAL ();

  TAO_Thread_Pool_Manager & tp_manager = tao_rt_orb->tp_manager ();

  TAO_Thread_Pool * const thread_pool =
    tp_manager.get_threadpool (thread_pool_id);

  if (thread_pool == 0)
    throw PortableServer::POA::InvalidPolicy ();

  return thread_pool;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
