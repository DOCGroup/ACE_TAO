// -*- C++ -*-
// $Id$

// =========================================================================
//
// = LIBRARY
//    PortableGroup
//
// = FILENAME
//    POA_Hooks.h
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =========================================================================

#include "POA_Hooks.h"
#include "PortableGroup_Loader.h"
#include "tao/Stub.h"

ACE_RCSID(tao, PortableGroup, "$Id$")

TAO_POA_Hooks::TAO_POA_Hooks (TAO_PortableGroup_Loader *portable_group_adapter) :
  portable_group_adapter_ (portable_group_adapter)
{
}

TAO_POA_Hooks::~TAO_POA_Hooks (void)
{
}

PortableServer::ObjectId *
TAO_POA_Hooks::create_id_for_reference (
    TAO_POA &the_poa,
    CORBA::Object_ptr the_ref,
    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::POA::WrongAdapter
    ))
{

  // Get the RepositoryId from the Group reference so
  // we know what kind of reference to make.
  const char* repository_id = the_ref->_stubobj ()->type_id;

  // Create a temporary object reference and then get the
  // ObjectId out of it.
  CORBA::Object_var obj_ref = the_poa.create_reference (repository_id,
                                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  PortableServer::ObjectId_var obj_id = the_poa.reference_to_id (obj_ref.in (),
                                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Create the acceptors necessary to receive requests for the
  // specified group reference.
  this->create_group_acceptors (the_ref,
                                this->portable_group_adapter_->acceptor_registry_,
                                the_poa.orb_core (),
                                ACE_TRY_ENV);

  ACE_CHECK_RETURN (0);

  // Find the Group Component.
  PortableGroup::TagGroupTaggedComponent *tmp_group_id;
  ACE_NEW_THROW_EX (tmp_group_id,
                    PortableGroup::TagGroupTaggedComponent,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  PortableGroup::TagGroupTaggedComponent_var group_id = tmp_group_id;

  if (this->find_group_component (the_ref, group_id.inout ()) != 0)
    {
      // Group component wasn't found.  The group reference
      // that was passed in must be bogus.
      ACE_THROW_RETURN (CORBA::INV_OBJREF (),
                        0);
    }

  const TAO_ObjectKey &key = obj_ref->_object_key ();

  // Add a mapping from GroupId to Object key in the PortableGroup

  this->portable_group_adapter_->group_map_.add_groupid_objectkey_pair (
                                   group_id._retn (),
                                   key,
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return obj_id._retn ();
}

int
TAO_POA_Hooks::find_group_component (const CORBA::Object_ptr the_ref,
                                     PortableGroup::TagGroupTaggedComponent &group)
{
  const TAO_MProfile& profiles = the_ref->_stubobj ()->base_profiles ();
  const TAO_Profile* profile;
  CORBA::ULong slot;

  // Iterate through the tagged profiles, and
  // create acceptors for the multicast ones.
  slot = 0;
  while (profile = profiles.get_profile (slot))
    {
      if (this->find_group_component_in_profile (profile, group) == 0)
        return 0;

      ++slot;
    }

  // Not found.
  return -1;
}

int
TAO_POA_Hooks::find_group_component_in_profile (const TAO_Profile* profile,
                                                PortableGroup::TagGroupTaggedComponent &group)
{
  // Iterate through the tagged components looking for
  // group tag.
  const TAO_Tagged_Components& components = profile->tagged_components ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_GROUP;

  // Try to find it.
  if (components.get_component (tagged_component) == 0)
    return -1;

  // Found it.
  const CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                       tagged_component.component_data.length ());

  // Extract the Byte Order.
  CORBA::Boolean byte_order;
  if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

  if ((in_cdr >> group) == 0)
    return -1;

  return 0;
}

int
TAO_POA_Hooks::create_group_acceptors (CORBA::Object_ptr the_ref,
                                       TAO_PortableGroup_Acceptor_Registry &acceptor_registry,
                                       TAO_ORB_Core &orb_core,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  const TAO_MProfile& profiles = the_ref->_stubobj ()->base_profiles ();
  const TAO_Profile* profile;
  CORBA::ULong slot;
  int num = 0;

  // Iterate through the tagged profiles, and
  // create acceptors for the multicast ones.
  slot = 0;
  while (profile = profiles.get_profile (slot))
    {
      if (profile->supports_multicast ())
        {
          acceptor_registry.open (profile, orb_core, ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
          ++num;
        }

      ++slot;
    }

  // Return the number of acceptors registered.
  return num;
}


PortableServer::IDs *
TAO_POA_Hooks::reference_to_ids (
    TAO_POA &the_poa,
    CORBA::Object_ptr the_ref,
    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::POA::WrongAdapter
    ))
{

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
