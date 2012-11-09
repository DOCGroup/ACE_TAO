// -*- C++ -*-

//=============================================================================
/**
 * @file   TAO_FT_Naming_Manager.cpp
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"

#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"
#include "orbsvcs/PortableGroup/PG_Object_Group.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/SString.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
  if (TAO_debug_level > 6)    \
  {                           \
    ACE_DEBUG (( LM_DEBUG,    \
    "Enter %s\n", #name       \
      ));                     \
  }

// Use this macro to return from CORBA methods
// to aid in debugging.  Note that you can specify
// the return value after the macro, for example:
// METHOD_RETURN(Plugh::plover) xyzzy; is equivalent
// to return xyzzy;
// METHOD_RETURN(Plugh::troll); is equivalent to
// return;
// WARNING: THIS GENERATES TWO STATEMENTS!!! THE FOLLOWING
// will not do what you want it to:
//  if (cave_is_closing) METHOD_RETURN(Plugh::pirate) aarrggh;
// Moral:  Always use braces.
#define METHOD_RETURN(name)   \
  if (TAO_debug_level > 6)    \
  {                           \
    ACE_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
  }                           \
  return /* value goes here */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Naming_Manager::TAO_FT_Naming_Manager (void)
  : group_factory_ (),
    built_in_balancing_strategy_info_name_ (1),
    built_in_balancing_strategy_name_ (1),
    custom_balancing_strategy_name_ (1),
    factory_registry_ ("NamingManager::FactoryRegistry")

{

}

TAO_FT_Naming_Manager::~TAO_FT_Naming_Manager (void)
{
}


void
TAO_FT_Naming_Manager::set_default_properties (
    const PortableGroup::Properties & props)
{

  this->properties_support_.set_default_properties (props);
  //@@ validate properties?
}

PortableGroup::Properties *
TAO_FT_Naming_Manager::get_default_properties ()
{
  return this->properties_support_.get_default_properties ();
}

void
TAO_FT_Naming_Manager::remove_default_properties (
    const PortableGroup::Properties & props)
{
  this->properties_support_.remove_default_properties (props);
}

void
TAO_FT_Naming_Manager::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides)
{
  this->properties_support_.set_type_properties (
    type_id,
    overrides);
}

PortableGroup::Properties *
TAO_FT_Naming_Manager::get_type_properties (
    const char *type_id)
{
  return this->properties_support_.get_type_properties (type_id);
}

void
TAO_FT_Naming_Manager::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props)
{
  this->properties_support_.remove_type_properties (
    type_id,
    props);
}

void
TAO_FT_Naming_Manager::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Properties & overrides)
{

  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->set_properties_dynamically (overrides);
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
}

PortableGroup::Properties *
TAO_FT_Naming_Manager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX (result, PortableGroup::Properties(), CORBA::NO_MEMORY ());

  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->get_properties (result);
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

PortableGroup::ObjectGroup_ptr
TAO_FT_Naming_Manager::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil();
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->create_member (the_location, type_id, the_criteria);
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::create_member: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

PortableGroup::ObjectGroup_ptr
TAO_FT_Naming_Manager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member)
{
  METHOD_ENTRY (TAO::FT_Naming_Manager::add_member);
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->add_member (
      the_location,
      member);

    result = group->reference ();

  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::add_member to unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  METHOD_RETURN (TAO::FT_Naming_Manager::add_member) result._retn ();
}


PortableGroup::ObjectGroup_ptr
TAO_FT_Naming_Manager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->remove_member (the_location);

    group->minimum_populate ();
      //@@ how about the case where the member was removed successfully,
      // but for one reason or another we were unable to bring the group
      // back up to minimum_number_of_replicas?

    result = group->reference ();
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn ();
}


PortableGroup::Locations *
TAO_FT_Naming_Manager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::Locations * result = 0;

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->locations_of_members ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::locations_of_members: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result;
}

PortableGroup::ObjectGroups *
TAO_FT_Naming_Manager::groups_at_location (
    const PortableGroup::Location & the_location)
{
  return this->group_factory_.groups_at_location (the_location);
}

PortableGroup::ObjectGroupId
TAO_FT_Naming_Manager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::ObjectGroupId result = 0;
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->get_object_group_id ();
    result = group->get_object_group_id ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::get_object_group_id: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result;
}

PortableGroup::ObjectGroup_ptr
TAO_FT_Naming_Manager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::get_object_group_ref: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

PortableGroup::ObjectGroup_ptr TAO_FT_Naming_Manager::get_object_group_ref_from_id (
    PortableGroup::ObjectGroupId group_id)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (group_id, group))
  {
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::get_object_group_ref_from_id: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

CORBA::Object_ptr
TAO_FT_Naming_Manager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location)
{
  CORBA::Object_var result = CORBA::Object::_nil();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->get_member_reference (the_location);
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::get_member_ref: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

CORBA::Object_ptr
TAO_FT_Naming_Manager::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out
      factory_creation_id)
{
  METHOD_ENTRY (TAO::FT_Naming_Manager::create_object)

  ////////////////////////////////
  // find the properties for this
  // type of object group
  TAO::PG_Property_Set * typeid_properties
    = this->properties_support_.find_typeid_properties (
      type_id);

  TAO::PG_Object_Group * group
    = this->group_factory_.create_group (
      type_id,
      the_criteria,
      typeid_properties);

  group->initial_populate ();
    //@@ on error we should remove the group from the Group_Factory
    // doing this "right" will require a var-type pointer to the object group
    // that knows about the factory, too.

 // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId_ptr factory_id_ptr = 0;
  ACE_NEW_THROW_EX (factory_id_ptr,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  PortableGroup::GenericFactory::FactoryCreationId_var factory_id = factory_id_ptr;
  PortableGroup::ObjectGroupId group_id = group->get_object_group_id ();
  factory_id <<= group_id;
  factory_creation_id = factory_id._retn();

  METHOD_RETURN (TAO::FT_Naming_Manager::create_object) group->reference ();
}

void
TAO_FT_Naming_Manager::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id)
{

  PortableGroup::ObjectGroupId group_id = 0;
  if (factory_creation_id >>= group_id)
  {
    this->group_factory_.delete_group (
      group_id);
  }
  else
  {
    throw PortableGroup::ObjectNotFound ();
  }
}

void
TAO_FT_Naming_Manager::initialize (CORBA::ORB_ptr orb,
                                   PortableServer::POA_ptr naming_mgr_poa)
{
  // Initialize the components used to implement the PortableGroup interfaces
  this->factory_registry_.init (orb);
  this->group_factory_.init (orb, naming_mgr_poa, factory_registry_.reference());
}


void
TAO_FT_Naming_Manager::preprocess_properties (PortableGroup::Properties & props)
{

}


TAO_END_VERSIONED_NAMESPACE_DECL
