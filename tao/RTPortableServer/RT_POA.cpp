// $Id$

#include "tao/RTPortableServer/RT_POA.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTPortableServer/RT_Acceptor_Filters.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Exception.h"
#include "tao/Stub.h"
#include "tao/Policy_Manager.h"
#include "tao/debug.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Thread_Lane_Resources_Manager.h"

#include "tao/RTCORBA/RT_Policy_i.h"

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/RTPortableServer/RT_Policy_Validator.h"

#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/RTPortableServer/RT_POA.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_POA::TAO_RT_POA (const TAO_Root_POA::String &name,
                        PortableServer::POAManager_ptr poa_manager,
                        const TAO_POA_Policy_Set &policies,
                        TAO_Root_POA *parent,
                        ACE_Lock &lock,
                        TAO_SYNCH_MUTEX &thread_lock,
                        TAO_ORB_Core &orb_core,
                        TAO_Object_Adapter *object_adapter
                        )
  : TAO_Regular_POA (name,
                     poa_manager,
                     policies,
                     parent,
                     lock,
                     thread_lock,
                     orb_core,
                     object_adapter),
  thread_pool_ (0)
{
  // Parse the RT policies and update our policy cache.
  this->parse_rt_policies (this->policies ());
}

TAO_RT_POA::~TAO_RT_POA (void)
{
}

TAO_Root_POA *
TAO_RT_POA::new_POA (const String &name,
                     PortableServer::POAManager_ptr poa_manager,
                     const TAO_POA_Policy_Set &policies,
                     TAO_Root_POA *parent,
                     ACE_Lock &lock,
                     TAO_SYNCH_MUTEX &thread_lock,
                     TAO_ORB_Core &orb_core,
                     TAO_Object_Adapter *object_adapter)
{
  TAO_RT_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_RT_POA (name,
                                poa_manager,
                                policies,
                                parent,
                                lock,
                                thread_lock,
                                orb_core,
                                object_adapter
                               ),
                    CORBA::NO_MEMORY ());

  return poa;
}

void
TAO_RT_POA::parse_rt_policies (TAO_POA_Policy_Set &policies)
{
  {
    CORBA::Policy_var policy =
      policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

    RTCORBA::PriorityModelPolicy_var priority_model =
      RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

    if (!CORBA::is_nil (priority_model.in ()))
      {
        RTCORBA::PriorityModel rt_priority_model =
          priority_model->priority_model ();

        this->cached_policies_.priority_model (
          TAO::Portable_Server::Cached_Policies::PriorityModel (rt_priority_model));

        RTCORBA::Priority priority =
          priority_model->server_priority ();

        this->cached_policies_.server_priority (priority);
      }
  }

  this->thread_pool_ =
    TAO_POA_RT_Policy_Validator::extract_thread_pool (this->orb_core_,
                                                      policies.policies ());
}

void
TAO_RT_POA::validate_priority (RTCORBA::Priority priority)
{
  if (priority < RTCORBA::minPriority
           // The line below will always be false unless the value of
           // RTCORBA::maxPriority, which is now assigned the value of
           // 32767, is changed in RTCORBA.pidl.
//      || priority > RTCORBA::maxPriority
     )
    {
      throw ::CORBA::BAD_PARAM ();
    }

  // If this POA is using a thread pool with lanes, make sure the
  // priority matches one of the thread lanes.  Note that in this
  // case, bands do not matter since matching the lanes priority is a
  // stricter condition than meeting the band ranges.  In addition,
  // when the POA was created, the bands had to match the lanes.
  if (this->thread_pool_ != 0 && this->thread_pool_->with_lanes ())
    {
      TAO_Thread_Lane **lanes = this->thread_pool_->lanes ();

      for (CORBA::ULong i = 0;
           i != this->thread_pool_->number_of_lanes ();
           ++i)
        {
          if (lanes[i]->lane_priority () == priority)
            return;
        }

      throw ::CORBA::BAD_PARAM ();
    }
  else
    // Else we are dealing with a thread pool without lanes.
    {
      // Check if we have bands.
      CORBA::Policy_var bands =
        this->policies ().get_cached_policy (
          TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

      RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
        = RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands.in ());

      TAO_PriorityBandedConnectionPolicy *priority_bands_i =
        dynamic_cast <TAO_PriorityBandedConnectionPolicy *>
                          (priority_bands.in ());

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

          throw ::CORBA::BAD_PARAM ();
        }
    }
}

void
TAO_RT_POA::validate_policies (void)
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
      throw PortableServer::POA::WrongPolicy ();
    }

  // For each of the above operations, if the POA does not support the
  // SERVER_DECLARED option for the PriorityModelPolicy then the ORB
  // shall raise a WrongPolicy user exception.
  if (this->cached_policies_.priority_model () !=
      TAO::Portable_Server::Cached_Policies::SERVER_DECLARED)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

  // In all other respects the semantics of the corresponding
  // (i.e. without the name extensions "_with_priority" and
  // "_and_priority") PortableServer::POA operations shall be
  // observed.
}

TAO_Stub *
TAO_RT_POA::key_to_stub_i (const TAO::ObjectKey &object_key,
                           const char *type_id,
                           CORBA::Short priority)
{
  // Client exposed policies.
  CORBA::PolicyList_var client_exposed_policies =
    this->client_exposed_policies (priority);

  // Server protocol policy.
  CORBA::Policy_var protocol =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (protocol.in ());

  TAO_ServerProtocolPolicy *server_protocol =
    dynamic_cast <TAO_ServerProtocolPolicy *> (server_protocol_policy.in ());

  // Filter for server protocol.
  TAO_Server_Protocol_Acceptor_Filter filter (server_protocol->protocols_rep ());

  // If this POA is using the default thread pool or a thread pool
  // without lanes, create the IOR with the acceptors in the thread
  // pool.
  if (this->thread_pool_ == 0 ||
      !this->thread_pool_->with_lanes ())
    {
      TAO_Acceptor_Registry *acceptor_registry = 0;

      if (this->thread_pool_ == 0)
        {
          TAO_Thread_Lane_Resources_Manager &thread_lane_resources_manager =
            this->orb_core_.thread_lane_resources_manager ();

          TAO_Thread_Lane_Resources &resources =
            thread_lane_resources_manager.default_lane_resources ();

          acceptor_registry = &resources.acceptor_registry ();
        }
      else
        {
          TAO_Thread_Lane **lanes = this->thread_pool_->lanes ();

          TAO_Thread_Lane_Resources &resources = lanes[0]->resources ();

          acceptor_registry = &resources.acceptor_registry ();
        }

      return
        this->TAO_Regular_POA::create_stub_object (object_key,
                                           type_id,
                                           client_exposed_policies._retn (),
                                           &filter,
                                           *acceptor_registry);
    }

  // If this POA has the SERVER_DECLARED policy, create the IOR with
  // the acceptors in the only thread lane that matches the priority
  // of the object.
  if (this->cached_policies_.priority_model () ==
      TAO::Portable_Server::Cached_Policies::SERVER_DECLARED)
    {
      TAO_Thread_Lane **lanes =
        this->thread_pool_->lanes ();

      for (CORBA::ULong i = 0;
           i != this->thread_pool_->number_of_lanes ();
           ++i)
        {
          if (lanes[i]->lane_priority () == priority)
            return this->TAO_Regular_POA::create_stub_object (object_key,
                                                      type_id,
                                                      client_exposed_policies._retn (),
                                                      &filter,
                                                      lanes[i]->resources ().acceptor_registry ()
                                                     );
        }

      ACE_ASSERT (0);
    }

  // If this POA has the CLIENT_PROPAGATED policy, create the IOR with
  // the acceptors in the thread lanes that matches the bands in this
  // POA.  If there are no bands, all the thread lanes are used.
  CORBA::Policy_var bands =
    this->policies ().get_cached_policy (
      TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION
     );

  RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
    = RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands.in ());

  TAO_PriorityBandedConnectionPolicy *priority_bands_i =
    dynamic_cast <TAO_PriorityBandedConnectionPolicy *> (priority_bands.in ());

  return this->create_stub_object (object_key,
                                   type_id,
                                   client_exposed_policies._retn (),
                                   &filter,
                                   priority_bands_i);
}

TAO_Stub *
TAO_RT_POA::create_stub_object (const TAO::ObjectKey &object_key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                TAO_PriorityBandedConnectionPolicy *priority_bands)
{
  bool error = false;

  // Count the number of endpoints.
  size_t const profile_count = this->endpoint_count ();

  // Create a profile container and have acceptor registries populate
  // it with profiles as appropriate.
  TAO_MProfile mprofile (0);

  // Allocate space for storing the profiles.  There can never be more
  // profiles than there are endpoints.  In some cases, there can be
  // less profiles than endpoints.
  int result =
    mprofile.set (static_cast <CORBA::ULong> (profile_count));
  if (result == -1)
    error = true;

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
            error = true;
        }
    }

  if (!error)
    result = filter->encode_endpoints (mprofile);
  if (result == -1)
    error = 1;

  if (error)
    throw ::CORBA::INTERNAL (
      CORBA::SystemException::_tao_minor_code (
        TAO_MPROFILE_CREATION_ERROR,
        0),
      CORBA::COMPLETED_NO);

  // Make sure we have at least one profile.  <mp> may end up being
  // empty if none of the acceptor endpoints have the right priority
  // for this object, for example.
  if (mprofile.profile_count () == 0)
    throw ::CORBA::BAD_PARAM (
      CORBA::SystemException::_tao_minor_code (
        TAO_MPROFILE_CREATION_ERROR,
        0),
      CORBA::COMPLETED_NO);

  return
    this->orb_core_.create_stub_object (mprofile, type_id, policy_list);
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
TAO_RT_POA::client_exposed_policies (CORBA::Short object_priority)
{
  CORBA::PolicyList *client_exposed_policies = 0;
  ACE_NEW_THROW_EX (client_exposed_policies,
                    CORBA::PolicyList (),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  CORBA::PolicyList_var safe_client_exposed_policies = client_exposed_policies;

  // Add in all of the client exposed policies.
  this->policies_.add_client_exposed_fixed_policies (client_exposed_policies);

  // Check if the priority model policy has been set, and if so, let
  // the client know about it.
  CORBA::Short poa_priority =
    this->cached_policies_.server_priority ();

  if (poa_priority != TAO_INVALID_PRIORITY)
    {
      TAO::Portable_Server::Cached_Policies::PriorityModel priority_model =
        this->cached_policies_.priority_model ();

      // If the priority model is client propagated, let the client
      // about the default server priority (the POA priority).  If
      // the priority model is server declared, tell the client the
      // servant's priority.
      CORBA::Short priority;
      if (priority_model == TAO::Portable_Server::Cached_Policies::CLIENT_PROPAGATED)
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
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));

      (*client_exposed_policies)[current_length] = priority_model_policy;
    }

  return safe_client_exposed_policies._retn ();
}


// Standard POA interfaces
PortableServer::POA_ptr
TAO_RT_POA::create_POA (const char *adapter_name,
                        PortableServer::POAManager_ptr poa_manager,
                        const CORBA::PolicyList &policies)
{
  return this->TAO_Regular_POA::create_POA (adapter_name, poa_manager, policies);
}

PortableServer::POA_ptr
TAO_RT_POA::find_POA (const char *adapter_name, CORBA::Boolean activate_it)
{
  return this->TAO_Regular_POA::find_POA (adapter_name, activate_it);
}

void
TAO_RT_POA::destroy (CORBA::Boolean etherealize_objects,
                     CORBA::Boolean wait_for_completion)
{
  this->TAO_Regular_POA::destroy (etherealize_objects, wait_for_completion);
}


#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::ThreadPolicy_ptr
TAO_RT_POA::create_thread_policy (PortableServer::ThreadPolicyValue value)
{
  return this->TAO_Regular_POA::create_thread_policy (value);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if !defined (CORBA_E_MICRO)
PortableServer::LifespanPolicy_ptr
TAO_RT_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value)
{
  return this->TAO_Regular_POA::create_lifespan_policy (value);
}
#endif

#if !defined (CORBA_E_MICRO)
PortableServer::IdUniquenessPolicy_ptr
TAO_RT_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value)
{
  return this->TAO_Regular_POA::create_id_uniqueness_policy (value);
}
#endif

#if !defined (CORBA_E_MICRO)
PortableServer::IdAssignmentPolicy_ptr
TAO_RT_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value)
{
  return this->TAO_Regular_POA::create_id_assignment_policy (value);
}
#endif

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::ImplicitActivationPolicy_ptr
TAO_RT_POA::create_implicit_activation_policy (
  PortableServer::ImplicitActivationPolicyValue value)
{
  return this->TAO_Regular_POA::create_implicit_activation_policy (value);
}

PortableServer::ServantRetentionPolicy_ptr
TAO_RT_POA::create_servant_retention_policy (
  PortableServer::ServantRetentionPolicyValue value)
{
  return this->TAO_Regular_POA::create_servant_retention_policy (value);
}


PortableServer::RequestProcessingPolicy_ptr
TAO_RT_POA::create_request_processing_policy (
  PortableServer::RequestProcessingPolicyValue value)
{
  return this->TAO_Regular_POA::create_request_processing_policy (value);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

char *
TAO_RT_POA::the_name (void)
{
  return this->TAO_Regular_POA::the_name ();
}

PortableServer::POA_ptr
TAO_RT_POA::the_parent (void)
{
  return this->TAO_Regular_POA::the_parent ();
}

PortableServer::POAList *
TAO_RT_POA::the_children (void)
{
  return this->TAO_Regular_POA::the_children ();
}

PortableServer::POAManager_ptr
TAO_RT_POA::the_POAManager (void)
{
  return this->TAO_Regular_POA::the_POAManager ();
}


#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::AdapterActivator_ptr
TAO_RT_POA::the_activator (void)
{
  return this->TAO_Regular_POA::the_activator ();;
}

void
TAO_RT_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator)
{
  this->TAO_Regular_POA::the_activator (adapter_activator);
}

PortableServer::ServantManager_ptr
TAO_RT_POA::get_servant_manager (void)
{
  return this->TAO_Regular_POA::get_servant_manager ();
}

void
TAO_RT_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr)
{
  this->TAO_Regular_POA::set_servant_manager (imgr);
}

PortableServer::Servant
TAO_RT_POA::get_servant (void)
{
  return this->TAO_Regular_POA::get_servant ();
}

void
TAO_RT_POA::set_servant (PortableServer::Servant servant)
{
  this->TAO_Regular_POA::set_servant (servant);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

PortableServer::ObjectId *
TAO_RT_POA::activate_object (PortableServer::Servant p_servant)
{
  return this->TAO_Regular_POA::activate_object (p_servant);
}

#if !defined (CORBA_E_MICRO)
void
TAO_RT_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                     PortableServer::Servant p_servant)
{
  this->TAO_Regular_POA::activate_object_with_id (id, p_servant);
}
#endif

void
TAO_RT_POA::deactivate_object (const PortableServer::ObjectId &oid)
{
  this->TAO_Regular_POA::deactivate_object (oid);
}

CORBA::Object_ptr
TAO_RT_POA::create_reference (const char *intf)
{
  return this->TAO_Regular_POA::create_reference (intf);
}

#if !defined (CORBA_E_MICRO)
CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id (const PortableServer::ObjectId &oid,
                                      const char *intf)
{
  return this->TAO_Regular_POA::create_reference_with_id (oid, intf);
}
#endif

PortableServer::ObjectId *
TAO_RT_POA::servant_to_id (PortableServer::Servant p_servant)
{
  return this->TAO_Regular_POA::servant_to_id (p_servant);
}

CORBA::Object_ptr
TAO_RT_POA::servant_to_reference (PortableServer::Servant p_servant)
{
  return this->TAO_Regular_POA::servant_to_reference (p_servant);
}

PortableServer::Servant
TAO_RT_POA::reference_to_servant (CORBA::Object_ptr reference)
{
  return this->TAO_Regular_POA::reference_to_servant (reference);
}

PortableServer::ObjectId *
TAO_RT_POA::reference_to_id (CORBA::Object_ptr reference)
{
  return this->TAO_Regular_POA::reference_to_id (reference);
}

PortableServer::Servant
TAO_RT_POA::id_to_servant (const PortableServer::ObjectId &oid)
{
  return this->TAO_Regular_POA::id_to_servant (oid);
}

CORBA::Object_ptr
TAO_RT_POA::id_to_reference (const PortableServer::ObjectId &oid)
{
  return this->TAO_Regular_POA::id_to_reference (oid);
}

CORBA::OctetSeq *
TAO_RT_POA::id (void)
{
  return this->TAO_Regular_POA::id ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
