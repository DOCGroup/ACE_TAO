#include "GOA.h"

ACE_RCSID (PortableServer,
           GOA,
           "$Id$")

#include "tao/ORB_Core.h"
#include "tao/ORB.h"

#include "Default_Acceptor_Filter.h"
#include "PortableGroup_Hooks.h"

#include "ace/Auto_Ptr.h"

PortableServer::ObjectId *
TAO_GOA::create_id_for_reference (CORBA::Object_ptr the_ref
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ))
{
  TAO_POA_PortableGroup_Hooks *hooks = this->orb_core_.portable_group_poa_hooks ();
  if (hooks == 0)
    {
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
    }

  PortableServer::ObjectId *obj_id =
    hooks->create_id_for_reference (*this, the_ref ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return obj_id;
}

PortableServer::IDs *
TAO_GOA::reference_to_ids (CORBA::Object_ptr the_ref
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ))
{
  TAO_POA_PortableGroup_Hooks *hooks = this->orb_core_.portable_group_poa_hooks ();
  if (hooks == 0)
    {
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                        0);
    }

  PortableServer::IDs *id_list =
    hooks->reference_to_ids (*this, the_ref ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return id_list;
}

void
TAO_GOA::associate_reference_with_id (CORBA::Object_ptr ref,
                                      const PortableServer::ObjectId & oid
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ))
{
  TAO_POA_PortableGroup_Hooks *hooks = this->orb_core_.portable_group_poa_hooks ();
  if (hooks == 0)
    {
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }

  hooks->associate_reference_with_id (*this, ref, oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_GOA::disassociate_reference_with_id (CORBA::Object_ptr ref,
                                         const PortableServer::ObjectId & oid
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ))
{
  TAO_POA_PortableGroup_Hooks *hooks = this->orb_core_.portable_group_poa_hooks ();
  if (hooks == 0)
    {
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }

  hooks->disassociate_reference_with_id (*this, ref, oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


TAO_GOA::TAO_GOA (const TAO_POA::String &name,
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
             ACE_ENV_ARG_PARAMETER)
{
}

TAO_GOA::~TAO_GOA (void)
{
}

TAO_POA *
TAO_GOA::new_POA (const String &name,
                     TAO_POA_Manager &poa_manager,
                     const TAO_POA_Policy_Set &policies,
                     TAO_POA *parent,
                     ACE_Lock &lock,
                     TAO_SYNCH_MUTEX &thread_lock,
                     TAO_ORB_Core &orb_core,
                     TAO_Object_Adapter *object_adapter
                     ACE_ENV_ARG_DECL)
{
  TAO_GOA *poa;

  ACE_NEW_THROW_EX (poa,
                    TAO_GOA (name,
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

// Standard POA interfaces
PortableServer::POA_ptr
TAO_GOA::create_POA (const char *adapter_name,
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
TAO_GOA::find_POA (const char *adapter_name,
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
TAO_GOA::destroy (CORBA::Boolean etherealize_objects,
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
TAO_GOA::create_thread_policy (PortableServer::ThreadPolicyValue value
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
TAO_GOA::create_lifespan_policy (PortableServer::LifespanPolicyValue value
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
TAO_GOA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value
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
TAO_GOA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value
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
TAO_GOA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value
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
TAO_GOA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value
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
TAO_GOA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value
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
TAO_GOA::the_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char * name =
    this->TAO_POA::the_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (name);
  return name;
}

PortableServer::POA_ptr
TAO_GOA::the_parent (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr parent =
    this->TAO_POA::the_parent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (parent);
  return parent;
}

PortableServer::POAList *
TAO_GOA::the_children (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAList *children =
    this->TAO_POA::the_children (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (children);
  return children;
}

PortableServer::POAManager_ptr
TAO_GOA::the_POAManager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAManager_ptr poa_manager =
    this->TAO_POA::the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (poa_manager);
  return poa_manager;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::AdapterActivator_ptr
TAO_GOA::the_activator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::AdapterActivator_ptr activator =
    this->TAO_POA::the_activator (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (activator);
  return activator;
}

void
TAO_GOA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_POA::the_activator (adapter_activator
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::ServantManager_ptr
TAO_GOA::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ServantManager_ptr servant_manager =
    this->TAO_POA::get_servant_manager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant_manager);
  return servant_manager;
}

void
TAO_GOA::set_servant_manager (PortableServer::ServantManager_ptr imgr
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_POA::set_servant_manager (imgr
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::Servant
TAO_GOA::get_servant (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_GOA::set_servant (PortableServer::Servant servant
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
TAO_GOA::activate_object (PortableServer::Servant p_servant
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
TAO_GOA::activate_object_with_id (const PortableServer::ObjectId &id,
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
TAO_GOA::deactivate_object (const PortableServer::ObjectId &oid
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
TAO_GOA::create_reference (const char *intf
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
TAO_GOA::create_reference_with_id (const PortableServer::ObjectId &oid,
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
TAO_GOA::servant_to_id (PortableServer::Servant p_servant
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
TAO_GOA::servant_to_reference (PortableServer::Servant p_servant
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
TAO_GOA::reference_to_servant (CORBA::Object_ptr reference
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
TAO_GOA::reference_to_id (CORBA::Object_ptr reference
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
TAO_GOA::id_to_servant (const PortableServer::ObjectId &oid
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
TAO_GOA::id_to_reference (const PortableServer::ObjectId &oid
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
TAO_GOA::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->TAO_POA::id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


