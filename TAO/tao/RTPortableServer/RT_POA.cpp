// @(#) $Id$

#include "RT_POA.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Exception.h"
#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Registry.h"

#include "tao/RTCORBA/RT_Policy_i.h"

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/RTPortableServer/RT_Policy_Validator.h"
#include "RT_Acceptor_Filters.h"

// auto_ptr class
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "RT_POA.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, POA, "$Id$")

TAO_RT_POA::TAO_RT_POA (const TAO_POA::String &name,
                        TAO_POA_Manager &poa_manager,
                        const TAO_POA_Policy_Set &policies,
                        TAO_POA *parent,
                        ACE_Lock &lock,
                        TAO_SYNCH_MUTEX &thread_lock,
                        TAO_ORB_Core &orb_core,
                        TAO_Object_Adapter *object_adapter
                        ACE_ENV_ARG_DECL)
  : TAO_POA (name,
             poa_manager,
             policies,
             parent,
             lock,
             thread_lock,
             orb_core,
             object_adapter
             ACE_ENV_ARG_PARAMETER),
  thread_pool_ (0)
{
  ACE_CHECK;

  // Parse the RT policies and update our policy cache.
  this->parse_rt_policies (this->policies ()
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_RT_POA::~TAO_RT_POA (void)
{
}

TAO_POA *
TAO_RT_POA::new_POA (const String &name,
                     TAO_POA_Manager &poa_manager,
                     const TAO_POA_Policy_Set &policies,
                     TAO_POA *parent,
                     ACE_Lock &lock,
                     TAO_SYNCH_MUTEX &thread_lock,
                     TAO_ORB_Core &orb_core,
                     TAO_Object_Adapter *object_adapter
                     ACE_ENV_ARG_DECL)
{
  TAO_RT_POA *poa;

  ACE_NEW_THROW_EX (poa,
                    TAO_RT_POA (name,
                                poa_manager,
                                policies,
                                parent,
                                lock,
                                thread_lock,
                                orb_core,
                                object_adapter
                                ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}

void
TAO_RT_POA::parse_rt_policies (TAO_POA_Policy_Set &policies
                               ACE_ENV_ARG_DECL)
{
  {
    CORBA::Policy_var policy =
      policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

    RTCORBA::PriorityModelPolicy_var priority_model =
      RTCORBA::PriorityModelPolicy::_narrow (policy.in ()
                                             ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (!CORBA::is_nil (priority_model.in ()))
      {
        RTCORBA::PriorityModel rt_priority_model =
          priority_model->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        this->cached_policies ().priority_model (
          TAO_POA_Cached_Policies::PriorityModel (rt_priority_model));

        RTCORBA::Priority priority =
          priority_model->server_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        this->cached_policies ().server_priority (priority);
      }
  }

  this->thread_pool_ =
    TAO_POA_RT_Policy_Validator::extract_thread_pool (this->orb_core_,
                                                      policies.policies ()
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_POA::validate_priority (RTCORBA::Priority priority
                               ACE_ENV_ARG_DECL)
{
  if (priority < RTCORBA::minPriority
           // The line below will always be false unless the value of
           // RTCORBA::maxPriority, which is now assigned the value of
           // 32767, is changed in RTCORBA.pidl.
//      || priority > RTCORBA::maxPriority
     )
    {
      ACE_THROW (CORBA::BAD_PARAM ());
    }

  // If this POA is using a thread pool with lanes, make sure the
  // priority matches one of the thread lanes.  Note that in this
  // case, bands do not matter since matching the lanes priority is a
  // stricter condition than meeting the band ranges.  In addition,
  // when the POA was created, the bands had to match the lanes.
  if (this->thread_pool_ != 0 &&
      this->thread_pool_->with_lanes ())
    {
      TAO_Thread_Lane **lanes =
        this->thread_pool_->lanes ();

      for (CORBA::ULong i = 0;
           i != this->thread_pool_->number_of_lanes ();
           ++i)
        {
          if (lanes[i]->lane_priority () == priority)
            return;
        }

      ACE_THROW (CORBA::BAD_PARAM ());
    }
  else
    // Else we are dealing with a thread pool without lanes.
    {
      // Check if we have bands.
      CORBA::Policy_var bands =
        this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

      RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
        = RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands.in ()
                                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_PriorityBandedConnectionPolicy *priority_bands_i =
        ACE_dynamic_cast (TAO_PriorityBandedConnectionPolicy *,
                          priority_bands.in ());

      if (priority_bands_i)
        {
          // If we do have bands, make sure that the priority is
          // matching one of the bands.
          RTCORBA::PriorityBands &bands =
            priority_bands_i->priority_bands_rep ();

          for (CORBA::ULong i = 0;
               i < bands.length ();
               ++i)
            {
              if (bands[i].low <= priority &&
                  bands[i].high >= priority)
                return;
            }

          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

void
TAO_RT_POA::validate_policies (ACE_ENV_SINGLE_ARG_DECL)
{
  // For each of the above operations, if the POA supports the
  // IMPLICIT_ACTIVATION option for the ImplicitActivationPolicy then
  // the ORB shall raise a WrongPolicy user exception. This relieves
  // an ORB implementation of the need to retrieve the target object's
  // priority from "somewhere" when a request arrives for an inactive
  // object.
  if (this->cached_policies_.implicit_activation () ==
      PortableServer::IMPLICIT_ACTIVATION)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  // For each of the above operations, if the POA does not support the
  // SERVER_DECLARED option for the PriorityModelPolicy then the ORB
  // shall raise a WrongPolicy user exception.
  if (this->cached_policies_.priority_model () !=
      TAO_POA_Cached_Policies::SERVER_DECLARED)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  // In all other respects the semantics of the corresponding
  // (i.e. without the name extensions "_with_priority" and
  // "_and_priority") PortableServer::POA operations shall be
  // observed.
}

TAO_Stub *
TAO_RT_POA::key_to_stub_i (const TAO::ObjectKey &object_key,
                           const char *type_id,
                           CORBA::Short priority
                           ACE_ENV_ARG_DECL)
{
  // Client exposed policies.
  CORBA::PolicyList_var client_exposed_policies =
    this->client_exposed_policies (priority
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Server protocol policy.
  CORBA::Policy_var protocol =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (protocol.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_ServerProtocolPolicy *server_protocol =
    ACE_dynamic_cast (TAO_ServerProtocolPolicy *,
                      server_protocol_policy.in ());

  // Filter for server protocol.
  TAO_Server_Protocol_Acceptor_Filter filter (server_protocol->protocols_rep ());

  // If this POA is using the default thread pool or a thread pool
  // without lanes, create the IOR with the acceptors in the thread
  // pool.
  if (this->thread_pool_ == 0 ||
      !this->thread_pool_->with_lanes ())
    {
      TAO_Acceptor_Registry *acceptor_registry =
        TAO_POA_RT_Policy_Validator::extract_acceptor_registry (this->orb_core_,
                                                                this->thread_pool_);

      return
        this->TAO_POA::create_stub_object (object_key,
                                           type_id,
                                           client_exposed_policies._retn (),
                                           &filter,
                                           *acceptor_registry
                                           ACE_ENV_ARG_PARAMETER);
    }

  // If this POA has the SERVER_DECLARED policy, create the IOR with
  // the acceptors in the only thread lane that matches the priority
  // of the object.
  if (this->cached_policies_.priority_model () ==
      TAO_POA_Cached_Policies::SERVER_DECLARED)
    {
      TAO_Thread_Lane **lanes =
        this->thread_pool_->lanes ();

      for (CORBA::ULong i = 0;
           i != this->thread_pool_->number_of_lanes ();
           ++i)
        {
          if (lanes[i]->lane_priority () == priority)
            return this->TAO_POA::create_stub_object (object_key,
                                                      type_id,
                                                      client_exposed_policies._retn (),
                                                      &filter,
                                                      lanes[i]->resources ().acceptor_registry ()
                                                      ACE_ENV_ARG_PARAMETER);
        }

      ACE_ASSERT (0);
    }

  // If this POA has the CLIENT_PROPAGATED policy, create the IOR with
  // the acceptors in the thread lanes that matches the bands in this
  // POA.  If there are no bands, all the thread lanes are used.
  CORBA::Policy_var bands =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

  RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
    = RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands.in ()
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_PriorityBandedConnectionPolicy *priority_bands_i =
    ACE_dynamic_cast (TAO_PriorityBandedConnectionPolicy *,
                      priority_bands.in ());

  return this->create_stub_object (object_key,
                                   type_id,
                                   client_exposed_policies._retn (),
                                   &filter,
                                   priority_bands_i
                                   ACE_ENV_ARG_PARAMETER);
}

TAO_Stub *
TAO_RT_POA::create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_PriorityBandedConnectionPolicy *priority_bands
                                ACE_ENV_ARG_DECL)
{
  int error = 0;

  // Count the number of endpoints.
  size_t profile_count =
    this->endpoint_count ();

  // Create a profile container and have acceptor registries populate
  // it with profiles as appropriate.
  TAO_MProfile mprofile (0);

  // Allocate space for storing the profiles.  There can never be more
  // profiles than there are endpoints.  In some cases, there can be
  // less profiles than endpoints.
  int result =
    mprofile.set (ACE_static_cast (CORBA::ULong, profile_count));
  if (result == -1)
    error = 1;

  TAO_Thread_Lane **lanes =
    this->thread_pool_->lanes ();

  // Leave it to the filter to decide which acceptors/in which order
  // go into the mprofile.
  for (CORBA::ULong i = 0;
       i != this->thread_pool_->number_of_lanes () &&
         !error;
       ++i)
    {
      if (this->lane_required (lanes[i],
                               priority_bands))
        {
          TAO_Acceptor_Registry &acceptor_registry =
            lanes[i]->resources ().acceptor_registry ();

          result =
            filter->fill_profile (object_key,
                                  mprofile,
                                  acceptor_registry.begin (),
                                  acceptor_registry.end (),
                                  lanes[i]->lane_priority ());
          if (result == -1)
            error = 1;
        }
    }

  if (!error)
    result = filter->encode_endpoints (mprofile);
  if (result == -1)
    error = 1;

  if (error)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_MPROFILE_CREATION_ERROR,
                          0),
                        CORBA::COMPLETED_NO),
                      0);

  // Make sure we have at least one profile.  <mp> may end up being
  // empty if none of the acceptor endpoints have the right priority
  // for this object, for example.
  if (mprofile.profile_count () == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_MPROFILE_CREATION_ERROR,
                          0),
                        CORBA::COMPLETED_NO),
                      0);

  return
    this->orb_core_.create_stub_object (mprofile,
                                        type_id,
                                        policy_list
                                        ACE_ENV_ARG_PARAMETER);
}

size_t
TAO_RT_POA::endpoint_count (void)
{
  size_t profile_count = 0;

  TAO_Thread_Lane **lanes =
    this->thread_pool_->lanes ();

  for (CORBA::ULong i = 0;
       i != this->thread_pool_->number_of_lanes ();
       ++i)
    profile_count +=
      lanes[i]->resources ().acceptor_registry ().endpoint_count ();

  return profile_count;
}

int
TAO_RT_POA::lane_required (TAO_Thread_Lane *lane,
                           TAO_PriorityBandedConnectionPolicy *priority_bands)
{
  if (priority_bands == 0)
    return 1;

  RTCORBA::PriorityBands &bands =
    priority_bands->priority_bands_rep ();

  for (CORBA::ULong i = 0;
       i < bands.length ();
       ++i)
    {
      if (bands[i].low <= lane->lane_priority () &&
          bands[i].high >= lane->lane_priority ())
        return 1;
    }

  return 0;
}

CORBA::PolicyList *
TAO_RT_POA::client_exposed_policies (CORBA::Short object_priority
                                     ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList *client_exposed_policies = 0;
  ACE_NEW_THROW_EX (client_exposed_policies,
                    CORBA::PolicyList (),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList_var safe_client_exposed_policies = client_exposed_policies;

  // Add in all of the client exposed policies.
  this->policies_.add_client_exposed_fixed_policies (client_exposed_policies
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Check if the priority model policy has been set, and if so, let
  // the client know about it.
  CORBA::Short poa_priority =
    this->cached_policies_.server_priority ();

  if (poa_priority != TAO_INVALID_PRIORITY)
    {
      TAO_POA_Cached_Policies::PriorityModel priority_model =
        this->cached_policies_.priority_model ();

      // If the priority model is client propagated, let the client
      // about the default server priority (the POA priority).  If
      // the priority model is server declared, tell the client the
      // servant's priority.
      CORBA::Short priority;
      if (priority_model == TAO_POA_Cached_Policies::CLIENT_PROPAGATED)
        priority = poa_priority;
      else
        priority = object_priority;

      const CORBA::ULong current_length =
        client_exposed_policies->length ();
      client_exposed_policies->length (current_length + 1);

      TAO_PriorityModelPolicy *priority_model_policy;
      ACE_NEW_THROW_EX (priority_model_policy,
                        TAO_PriorityModelPolicy (RTCORBA::PriorityModel (priority_model),
                                                 priority),
                        CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      (*client_exposed_policies)[current_length] = priority_model_policy;
    }

  return safe_client_exposed_policies._retn ();
}


// Standard POA interfaces
PortableServer::POA_ptr
TAO_RT_POA::create_POA (const char *adapter_name,
                        PortableServer::POAManager_ptr poa_manager,
                        const CORBA::PolicyList &policies
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterAlreadyExists,
                   PortableServer::POA::InvalidPolicy))
{
  PortableServer::POA_ptr poa = this->TAO_POA::create_POA (adapter_name,
                                                           poa_manager,
                                                           policies
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (poa);
  return poa;
}

PortableServer::POA_ptr
TAO_RT_POA::find_POA (const char *adapter_name,
                      CORBA::Boolean activate_it
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterNonExistent))
{
  PortableServer::POA_ptr poa = this->TAO_POA::find_POA (adapter_name,
                                                         activate_it
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (poa);
  return poa;
}

void
TAO_RT_POA::destroy (CORBA::Boolean etherealize_objects,
                     CORBA::Boolean wait_for_completion
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_POA::destroy (etherealize_objects,
                          wait_for_completion
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::ThreadPolicy_ptr
TAO_RT_POA::create_thread_policy (PortableServer::ThreadPolicyValue value
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ThreadPolicy_ptr policy =
    this->TAO_POA::create_thread_policy (value
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

PortableServer::LifespanPolicy_ptr
TAO_RT_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::LifespanPolicy_ptr policy =
    this->TAO_POA::create_lifespan_policy (value
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}

PortableServer::IdUniquenessPolicy_ptr
TAO_RT_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::IdUniquenessPolicy_ptr policy =
    this->TAO_POA::create_id_uniqueness_policy (value
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}


PortableServer::IdAssignmentPolicy_ptr
TAO_RT_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::IdAssignmentPolicy_ptr policy =
    this->TAO_POA::create_id_assignment_policy (value
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::ImplicitActivationPolicy_ptr
TAO_RT_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ImplicitActivationPolicy_ptr policy =
    this->TAO_POA::create_implicit_activation_policy (value
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}

PortableServer::ServantRetentionPolicy_ptr
TAO_RT_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ServantRetentionPolicy_ptr policy =
    this->TAO_POA::create_servant_retention_policy (value
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}


PortableServer::RequestProcessingPolicy_ptr
TAO_RT_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::RequestProcessingPolicy_ptr policy =
    this->TAO_POA::create_request_processing_policy (value
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (policy);
  return policy;
}


#endif /* TAO_HAS_MINIMUM_POA == 0 */

char *
TAO_RT_POA::the_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char * name =
    this->TAO_POA::the_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (name);
  return name;
}

PortableServer::POA_ptr
TAO_RT_POA::the_parent (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr parent =
    this->TAO_POA::the_parent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (parent);
  return parent;
}

PortableServer::POAList *
TAO_RT_POA::the_children (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAList *children =
    this->TAO_POA::the_children (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (children);
  return children;
}

PortableServer::POAManager_ptr
TAO_RT_POA::the_POAManager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAManager_ptr poa_manager =
    this->TAO_POA::the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (poa_manager);
  return poa_manager;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::AdapterActivator_ptr
TAO_RT_POA::the_activator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::AdapterActivator_ptr activator =
    this->TAO_POA::the_activator (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (activator);
  return activator;
}

void
TAO_RT_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_POA::the_activator (adapter_activator
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::ServantManager_ptr
TAO_RT_POA::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ServantManager_ptr servant_manager =
    this->TAO_POA::get_servant_manager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant_manager);
  return servant_manager;
}

void
TAO_RT_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::set_servant_manager (imgr
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::Servant
TAO_RT_POA::get_servant (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::NoServant,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::get_servant (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant);
  return servant;
}

void
TAO_RT_POA::set_servant (PortableServer::Servant servant
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::set_servant (servant
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

PortableServer::ObjectId *
TAO_RT_POA::activate_object (PortableServer::Servant p_servant
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::activate_object (p_servant
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

void
TAO_RT_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                     PortableServer::Servant p_servant
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::activate_object_with_id (id,
                                          p_servant
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_POA::deactivate_object (const PortableServer::ObjectId &oid
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::deactivate_object (oid
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Object_ptr
TAO_RT_POA::create_reference (const char *intf
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::create_reference (intf
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id (const PortableServer::ObjectId &oid,
                                      const char *intf
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::create_reference_with_id (oid,
                                             intf
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}

PortableServer::ObjectId *
TAO_RT_POA::servant_to_id (PortableServer::Servant p_servant
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::servant_to_id (p_servant
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

CORBA::Object_ptr
TAO_RT_POA::servant_to_reference (PortableServer::Servant p_servant
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::servant_to_reference (p_servant
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}


PortableServer::Servant
TAO_RT_POA::reference_to_servant (CORBA::Object_ptr reference
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongAdapter,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::reference_to_servant (reference
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant);
  return servant;
}

PortableServer::ObjectId *
TAO_RT_POA::reference_to_id (CORBA::Object_ptr reference
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongAdapter,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::reference_to_id (reference
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

PortableServer::Servant
TAO_RT_POA::id_to_servant (const PortableServer::ObjectId &oid
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::id_to_servant (oid
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant);
  return servant;
}

CORBA::Object_ptr
TAO_RT_POA::id_to_reference (const PortableServer::ObjectId &oid
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::id_to_reference (oid
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::OctetSeq *
TAO_RT_POA::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->TAO_POA::id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Policy *
TAO_RT_POA::server_protocol (void)
{
  CORBA::Policy *result =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_.policy_manager ();

      if (policy_manager != 0)
        result =
          policy_manager->get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);
    }

  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
