#include "orbsvcs/PortableGroup/GOA.h"


ACE_RCSID (PortableGroup,
           GOA,
           "$Id$")


#include "orbsvcs/PortableGroup/PortableGroup_Acceptor_Registry.h"
#include "orbsvcs/PortableGroup/PortableGroup_Request_Dispatcher.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Stub.h"
#include "tao/Tagged_Components.h"
#include "tao/Profile.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

PortableServer::ObjectId *
TAO_GOA::create_id_for_reference (CORBA::Object_ptr the_ref
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableGroup::NotAGroupObject
    ))
{
  // Get the RepositoryId from the Group reference so
  // we know what kind of reference to make.
  const char* repository_id = the_ref->_stubobj ()->type_id.in ();

  // Create a temporary object reference and then get the
  // ObjectId out of it.
  CORBA::Object_var obj_ref = this->create_reference (repository_id
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PortableServer::ObjectId_var obj_id =
    this->reference_to_id (obj_ref.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Associate the object reference with the group reference.
  this->associate_group_with_ref (the_ref,
                                  obj_ref.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return obj_id._retn ();
}

PortableGroup::IDs *
TAO_GOA::reference_to_ids (CORBA::Object_ptr the_ref
                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableGroup::NotAGroupObject
    ))
{
  ACE_UNUSED_ARG (the_ref);

  return 0;
}

void
TAO_GOA::associate_reference_with_id (CORBA::Object_ptr ref,
                                      const PortableServer::ObjectId & oid
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableGroup::NotAGroupObject
    ))
{
  // Create a reference for the specified ObjectId, since
  // it is much easier to extract the object key from the
  // reference.
  CORBA::Object_var obj_ref = this->id_to_reference (oid
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Associate the object reference with the group reference.
  this->associate_group_with_ref (ref,
                                  obj_ref.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_GOA::disassociate_reference_with_id (CORBA::Object_ptr ref,
                                         const PortableServer::ObjectId & oid
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableGroup::NotAGroupObject
    ))
{
  ACE_UNUSED_ARG (ref);
  ACE_UNUSED_ARG (oid);
}


TAO_GOA::TAO_GOA (const TAO_Root_POA::String &name,
                  PortableServer::POAManager_ptr poa_manager,
                  const TAO_POA_Policy_Set &policies,
                  TAO_Root_POA *parent,
                  ACE_Lock &lock,
                  TAO_SYNCH_MUTEX &thread_lock,
                  TAO_ORB_Core &orb_core,
                  TAO_Object_Adapter *object_adapter
                  ACE_ENV_ARG_DECL)
  : TAO_Regular_POA (name,
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

TAO_Root_POA *
TAO_GOA::new_POA (const String &name,
                  PortableServer::POAManager_ptr poa_manager,
                  const TAO_POA_Policy_Set &policies,
                  TAO_Root_POA *parent,
                  ACE_Lock &lock,
                  TAO_SYNCH_MUTEX &thread_lock,
                  TAO_ORB_Core &orb_core,
                  TAO_Object_Adapter *object_adapter
                  ACE_ENV_ARG_DECL)
{
  TAO_GOA *poa = 0;

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
  PortableServer::POA_ptr poa = this->TAO_Regular_POA::create_POA (adapter_name,
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
  PortableServer::POA_ptr poa = this->TAO_Regular_POA::find_POA (adapter_name,
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
  this->TAO_Regular_POA::destroy (etherealize_objects,
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
    this->TAO_Regular_POA::create_thread_policy (value
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
    this->TAO_Regular_POA::create_lifespan_policy (value
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
    this->TAO_Regular_POA::create_id_uniqueness_policy (value
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
    this->TAO_Regular_POA::create_id_assignment_policy (value
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
    this->TAO_Regular_POA::create_implicit_activation_policy (value
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
    this->TAO_Regular_POA::create_servant_retention_policy (value
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
    this->TAO_Regular_POA::create_request_processing_policy (value
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
    this->TAO_Regular_POA::the_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (name);
  return name;
}

PortableServer::POA_ptr
TAO_GOA::the_parent (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr parent =
    this->TAO_Regular_POA::the_parent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (parent);
  return parent;
}

PortableServer::POAList *
TAO_GOA::the_children (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAList *children =
    this->TAO_Regular_POA::the_children (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (children);
  return children;
}

PortableServer::POAManager_ptr
TAO_GOA::the_POAManager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POAManager_ptr poa_manager =
    this->TAO_Regular_POA::the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (poa_manager);
  return poa_manager;
}


#if (TAO_HAS_MINIMUM_POA == 0)

PortableServer::AdapterActivator_ptr
TAO_GOA::the_activator (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::AdapterActivator_ptr activator =
    this->TAO_Regular_POA::the_activator (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (activator);
  return activator;
}

void
TAO_GOA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_Regular_POA::the_activator (adapter_activator
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::ServantManager_ptr
TAO_GOA::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  PortableServer::ServantManager_ptr servant_manager =
    this->TAO_Regular_POA::get_servant_manager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant_manager);
  return servant_manager;
}

void
TAO_GOA::set_servant_manager (PortableServer::ServantManager_ptr imgr
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_Regular_POA::set_servant_manager (imgr
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
    this->TAO_Regular_POA::get_servant (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (servant);
  return servant;
}

void
TAO_GOA::set_servant (PortableServer::Servant servant
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  this->TAO_Regular_POA::set_servant (servant
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
    this->TAO_Regular_POA::activate_object (p_servant
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
  this->TAO_Regular_POA::activate_object_with_id (id,
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
  this->TAO_Regular_POA::deactivate_object (oid
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
    this->TAO_Regular_POA::create_reference (intf
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::Object_ptr
TAO_GOA::create_reference_with_id (const PortableServer::ObjectId &oid,
                                   const char *intf
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr obj =
    this->TAO_Regular_POA::create_reference_with_id (oid,
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
    this->TAO_Regular_POA::servant_to_id (p_servant
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
    this->TAO_Regular_POA::servant_to_reference (p_servant
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
    this->TAO_Regular_POA::reference_to_servant (reference
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
    this->TAO_Regular_POA::reference_to_id (reference
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
    this->TAO_Regular_POA::id_to_servant (oid
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
    this->TAO_Regular_POA::id_to_reference (oid
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (obj);
  return obj;
}

CORBA::OctetSeq *
TAO_GOA::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->TAO_Regular_POA::id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
TAO_GOA::find_group_component (const CORBA::Object_ptr the_ref,
                               PortableGroup::TagGroupTaggedComponent &group)
{
  const TAO_MProfile& profiles = the_ref->_stubobj ()->base_profiles ();
  const TAO_Profile* profile;
  CORBA::ULong slot;

  // Iterate through the tagged profiles, and
  // create acceptors for the multicast ones.
  slot = 0;
  while (0 != (profile = profiles.get_profile (slot)))
    {
      if (this->find_group_component_in_profile (profile, group) == 0)
        return 0;

      ++slot;
    }

  // Not found.
  return -1;
}

int
TAO_GOA::find_group_component_in_profile (const TAO_Profile* profile,
                                          PortableGroup::TagGroupTaggedComponent &group)
{
  // Iterate through the tagged components looking for
  // group tag.
  const TAO_Tagged_Components& components = profile->tagged_components ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_GROUP;

  // Try to find it.
  if (components.get_component (tagged_component) == 0)
    return -1;

  // Found it.
  const CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  TAO_InputCDR in_cdr (reinterpret_cast <const char*> (buf),
                       tagged_component.component_data.length ());

  // Extract the Byte Order.
  CORBA::Boolean byte_order;
  if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  in_cdr.reset_byte_order (static_cast <int> (byte_order));

  if ((in_cdr >> group) == 0)
    return -1;

  return 0;
}

int
TAO_GOA::create_group_acceptors (CORBA::Object_ptr the_ref,
                                 TAO_PortableGroup_Acceptor_Registry &acceptor_registry,
                                 TAO_ORB_Core &orb_core
                                 ACE_ENV_ARG_DECL)
{
  const TAO_MProfile& profiles = the_ref->_stubobj ()->base_profiles ();
  const TAO_Profile* profile;
  CORBA::ULong slot;
  int num = 0;

  // Iterate through the tagged profiles, and
  // create acceptors for the multicast ones.
  slot = 0;
  while (0 != (profile = profiles.get_profile (slot)))
    {
      if (profile->supports_multicast ())
        {
          acceptor_registry.open (profile,
                                  orb_core
                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          ++num;
        }

      ++slot;
    }

  // Return the number of acceptors registered.
  return num;
}

void
TAO_GOA::associate_group_with_ref (
      CORBA::Object_ptr group_ref,
      CORBA::Object_ptr obj_ref
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::NotAGroupObject))
{
  // Find the Group Component so that we can extract the Group ID.
  PortableGroup::TagGroupTaggedComponent *tmp_group_id;
  ACE_NEW_THROW_EX (tmp_group_id,
                    PortableGroup::TagGroupTaggedComponent,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableGroup::TagGroupTaggedComponent_var group_id = tmp_group_id;

  if (this->find_group_component (group_ref, group_id.inout ()) != 0)
    {
      // Group component wasn't found.  The group reference
      // that was passed in must be bogus.
      ACE_THROW (PortableGroup::NotAGroupObject ());
    }

  PortableGroup_Request_Dispatcher *rd =
    dynamic_cast <PortableGroup_Request_Dispatcher*>(
      this->orb_core_.request_dispatcher());

  // Create the acceptors necessary to receive requests for the
  // specified group reference.
  this->create_group_acceptors (group_ref,
                                rd->acceptor_registry_,
                                this->orb_core_
                                ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;


  // Add a mapping from GroupId to Object key in the PortableGroup
  const TAO::ObjectKey &key =
    obj_ref->_stubobj ()->profile_in_use ()->object_key ();
  rd->group_map_.add_groupid_objectkey_pair (group_id._retn (),
                                             key
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

TAO_END_VERSIONED_NAMESPACE_DECL
