// @(#) $Id$

#include "RT_POA.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Exception.h"
#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/Acceptor_Registry.h"

#include "tao/RTCORBA/RT_Policy_i.h"

#include "tao/PortableServer/Default_Acceptor_Filter.h"
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
                        TAO_Object_Adapter *object_adapter,
                        CORBA::Environment &ACE_TRY_ENV)
  : TAO_POA (name,
             poa_manager,
             policies,
             parent,
             lock,
             thread_lock,
             orb_core,
             object_adapter,
             ACE_TRY_ENV)
{
  ACE_CHECK;

  // Parse the RT policies and update our policy cache.
  this->parse_rt_policies (this->policies (),
                           ACE_TRY_ENV);
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
                     TAO_Object_Adapter *object_adapter,
                     CORBA_Environment &ACE_TRY_ENV)
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
                                object_adapter,
                                ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}

void
TAO_RT_POA::parse_rt_policies (TAO_POA_Policy_Set &policies,
                               CORBA::Environment &ACE_TRY_ENV)
{

  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL);

  RTCORBA::PriorityModelPolicy_var priority_model =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in (),
                                           ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (priority_model.in ()))
    {
      RTCORBA::PriorityModel rt_priority_model =
        priority_model->priority_model (ACE_TRY_ENV);
      ACE_CHECK;

      this->cached_policies ().priority_model (
                    TAO_POA_Cached_Policies::PriorityModel (rt_priority_model));

      RTCORBA::Priority priority = priority_model->server_priority (ACE_TRY_ENV);
      ACE_CHECK;

      this->cached_policies ().server_priority (priority);
    }
}

void
TAO_RT_POA::valid_priority (RTCORBA::Priority priority,
                            CORBA_Environment &ACE_TRY_ENV)
{
  // Make sure <priority> matches our resource configuration:
  // 1. If Priority Banded Connections are set, <priority> must match
  //    one of the bands.
  // 2. If no Priority Banded Connections are set, at least one server
  //    endpoint must provide service at the specified <priority>.

  // @@ RT CORBA Subsetting: optimize out locks.
  CORBA::Policy_var policy =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

  RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
    = RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy.in (),
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  TAO_PriorityBandedConnectionPolicy *bands_policy =
    ACE_dynamic_cast (TAO_PriorityBandedConnectionPolicy *,
                      priority_bands.in ());

  if (bands_policy != 0)
    // Case 1.
    {
      RTCORBA::PriorityBands &bands =
        bands_policy->priority_bands_rep ();

      for (CORBA::ULong i = 0; i < bands.length (); ++i)
        {
          if (priority <= bands[i].high
              && priority >= bands[i].low)
            return;
        }
    }
  else
    // Case 2.
    {
      TAO_Acceptor_Registry *ar =
        this->orb_core_.acceptor_registry ();

      for (TAO_Acceptor **a = ar->begin (); a != ar->end (); ++a)
        {
          if ((*a)->priority () == priority)
            return;
        }
    }

  ACE_THROW (CORBA::BAD_PARAM ());
}

void
TAO_RT_POA::validate_policies (CORBA::Environment &ACE_TRY_ENV)
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
TAO_RT_POA::key_to_stub_i (const TAO_ObjectKey &key,
                           const char *type_id,
                           CORBA::Short priority,
                           CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList_var client_exposed_policies =
    this->client_exposed_policies (priority,
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_Stub *data = 0;

  CORBA::Policy_var protocol =
    this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (protocol.in (),
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_ServerProtocolPolicy *server_protocol =
    ACE_dynamic_cast (TAO_ServerProtocolPolicy *,
                      server_protocol_policy.in ());


  // If the POA has RTCORBA::SERVER_DECLARED priority model
  // then regardless of the fact that there are or that there
  // are not bands then we need to pass only one endpoint that
  // is either the one associated to the bands to which the
  // server belongs, or the one associated to the server priority.
  //
  // If the POA has  RTCORBA::CLIENT_EXPOSED, than all endpoints
  // should be passed.

  if (this->cached_policies_.priority_model ()
      == TAO_POA_Cached_Policies::SERVER_DECLARED)
    {
      CORBA::Policy_var bands =
        this->policies ().get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

      RTCORBA::PriorityBandedConnectionPolicy_var priority_bands
        = RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands.in (),
                                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      TAO_PriorityBandedConnectionPolicy *priority_bands_i =
        ACE_dynamic_cast (TAO_PriorityBandedConnectionPolicy *,
                          priority_bands.in ());

      if (priority_bands_i != 0)
        {
          TAO_Bands_Acceptor_Filter
            filter (server_protocol->protocols_rep (),
                    priority_bands_i->priority_bands_rep());

          data = this->orb_core_.create_stub_object (key,
                                                     type_id,
                                                     client_exposed_policies._retn (),
                                                     &filter,
                                                     ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          RTCORBA::Priority object_priority =
            this->cached_policies_.server_priority () > priority ?
                this->cached_policies_.server_priority () : priority;
          TAO_Priority_Acceptor_Filter filter (server_protocol->protocols_rep (),
                                               object_priority);

          data = this->orb_core_.create_stub_object (key,
                                                     type_id,
                                                     client_exposed_policies._retn (),
                                                     &filter,
                                                     ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
        }
    }
  else
    {
      // Client propagated.
      TAO_Server_Protocol_Acceptor_Filter filter (server_protocol->protocols_rep ());
      data = this->orb_core_.create_stub_object (key,
                                                 type_id,
                                                 client_exposed_policies._retn (),
                                                 &filter,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return data;
}

CORBA::PolicyList *
TAO_RT_POA::client_exposed_policies (CORBA::Short object_priority,
                                     CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList *client_exposed_policies = 0;
  ACE_NEW_THROW_EX (client_exposed_policies,
                    CORBA::PolicyList (),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  // Add in all of the client exposed policies.
  this->policies_.add_client_exposed_fixed_policies (client_exposed_policies,
                                                     ACE_TRY_ENV);
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

      TAO_PriorityModelPolicy *priority_model_policy;
      ACE_NEW_THROW_EX (priority_model_policy,
                        TAO_PriorityModelPolicy (RTCORBA::PriorityModel (priority_model),
                                                 priority),
                        CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      CORBA::ULong current_length = client_exposed_policies->length ();
      client_exposed_policies->length (current_length + 1);
      (*client_exposed_policies)[current_length] = priority_model_policy;
    }

  return client_exposed_policies;
}


// Standard POA interfaces
PortableServer::POA_ptr
TAO_RT_POA::create_POA (const char *adapter_name,
                        PortableServer::POAManager_ptr poa_manager,
                        const CORBA::PolicyList &policies,
                        CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterAlreadyExists,
                   PortableServer::POA::InvalidPolicy))
{
  PortableServer::POA_ptr poa = this->TAO_POA::create_POA (adapter_name,
                                                           poa_manager,
                                                           policies,
                                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (poa);
  return poa;
}

PortableServer::POA_ptr
TAO_RT_POA::find_POA (const char *adapter_name,
                      CORBA::Boolean activate_it,
                      CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::AdapterNonExistent))
{
  PortableServer::POA_ptr poa = this->TAO_POA::find_POA (adapter_name,
                                                         activate_it,
                                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (poa);
  return poa;
}

void
TAO_RT_POA::destroy (CORBA::Boolean etherealize_objects,
              CORBA::Boolean wait_for_completion,
              CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_POA::destroy (etherealize_objects,
                          wait_for_completion,
                          ACE_TRY_ENV);
  ACE_CHECK;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::ThreadPolicy_ptr
TAO_RT_POA::create_thread_policy (PortableServer::ThreadPolicyValue value,
                                                       CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ThreadPolicy_ptr policy =
    this->TAO_POA::create_thread_policy (value,
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

PortableServer::LifespanPolicy_ptr
TAO_RT_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                                           CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::LifespanPolicy_ptr policy =
    this->TAO_POA::create_lifespan_policy (value,
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}

PortableServer::IdUniquenessPolicy_ptr
TAO_RT_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                                                    CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::IdUniquenessPolicy_ptr policy =
    this->TAO_POA::create_id_uniqueness_policy (value,
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}


PortableServer::IdAssignmentPolicy_ptr
TAO_RT_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                                                    CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::IdAssignmentPolicy_ptr policy =
    this->TAO_POA::create_id_assignment_policy (value,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::ImplicitActivationPolicy_ptr
TAO_RT_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                                CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ImplicitActivationPolicy_ptr policy =
    this->TAO_POA::create_implicit_activation_policy (value,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}

PortableServer::ServantRetentionPolicy_ptr
TAO_RT_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                                                            CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ServantRetentionPolicy_ptr policy =
    this->TAO_POA::create_servant_retention_policy (value,
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}


PortableServer::RequestProcessingPolicy_ptr
TAO_RT_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                                                              CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::RequestProcessingPolicy_ptr policy =
    this->TAO_POA::create_request_processing_policy (value,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (policy);
  return policy;
}


#endif /* TAO_HAS_MINIMUM_POA == 0 */

char *
TAO_RT_POA::the_name (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char * name =
    this->TAO_POA::the_name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (name);
  return name;
}

PortableServer::POA_ptr
TAO_RT_POA::the_parent (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr parent =
    this->TAO_POA::the_parent (ACE_TRY_ENV);
  ACE_CHECK_RETURN (parent);
  return parent;
}

PortableServer::POAList *
TAO_RT_POA::the_children (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAList *children =
    this->TAO_POA::the_children (ACE_TRY_ENV);
  ACE_CHECK_RETURN (children);
  return children;
}

PortableServer::POAManager_ptr
TAO_RT_POA::the_POAManager (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAManager_ptr poa_manager =
    this->TAO_POA::the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (poa_manager);
  return poa_manager;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::AdapterActivator_ptr
TAO_RT_POA::the_activator (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::AdapterActivator_ptr activator =
    this->TAO_POA::the_activator (ACE_TRY_ENV);
  ACE_CHECK_RETURN (activator);
  return activator;
}

void
TAO_RT_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                    CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_POA::the_activator (adapter_activator,
                                ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::ServantManager_ptr
TAO_RT_POA::get_servant_manager (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ServantManager_ptr servant_manager =
    this->TAO_POA::get_servant_manager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (servant_manager);
  return servant_manager;
}

void
TAO_RT_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr,
                          CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::set_servant_manager (imgr,
                                      ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::Servant
TAO_RT_POA::get_servant (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::NoServant,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::get_servant (ACE_TRY_ENV);
  ACE_CHECK_RETURN (servant);
  return servant;
}

void
TAO_RT_POA::set_servant (PortableServer::Servant servant,
                  CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::set_servant (servant,
                              ACE_TRY_ENV);
  ACE_CHECK;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

PortableServer::ObjectId *
TAO_RT_POA::activate_object (PortableServer::Servant p_servant,
                                           CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::activate_object (p_servant,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

void
TAO_RT_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                              PortableServer::Servant p_servant,
                              CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::activate_object_with_id (id,
                                          p_servant,
                                          ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_RT_POA::deactivate_object (const PortableServer::ObjectId &oid,
                        CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::deactivate_object (oid,
                                    ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::Object_ptr
TAO_RT_POA::create_reference (const char *intf,
                                    CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::create_reference (intf,
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id (const PortableServer::ObjectId &oid,
                                            const char *intf,
                                            CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::create_reference_with_id (oid,
                                             intf,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (obj);
  return obj;
}

PortableServer::ObjectId *
TAO_RT_POA::servant_to_id (PortableServer::Servant p_servant,
                                         CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::servant_to_id (p_servant,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

CORBA::Object_ptr
TAO_RT_POA::servant_to_reference (PortableServer::Servant p_servant,
                                        CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::servant_to_reference (p_servant,
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (obj);
  return obj;
}


PortableServer::Servant
TAO_RT_POA::reference_to_servant (CORBA::Object_ptr reference,
                                              CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongAdapter,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::reference_to_servant (reference,
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (servant);
  return servant;
}

PortableServer::ObjectId *
TAO_RT_POA::reference_to_id (CORBA::Object_ptr reference,
                                           CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongAdapter,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ObjectId *object_id =
    this->TAO_POA::reference_to_id (reference,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (object_id);
  return object_id;
}

PortableServer::Servant
TAO_RT_POA::id_to_servant (const PortableServer::ObjectId &oid,
                                       CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::Servant servant =
    this->TAO_POA::id_to_servant (oid,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (servant);
  return servant;
}

CORBA::Object_ptr
TAO_RT_POA::id_to_reference (const PortableServer::ObjectId &oid,
                                   CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ObjectNotActive,
                   PortableServer::POA::WrongPolicy))
{
  CORBA::Object_ptr obj =
    this->TAO_POA::id_to_reference (oid,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::OctetSeq *
TAO_RT_POA::id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->TAO_POA::id (ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

