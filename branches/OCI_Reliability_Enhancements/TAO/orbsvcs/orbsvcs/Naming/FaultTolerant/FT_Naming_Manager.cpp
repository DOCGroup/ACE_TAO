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
#include "orbsvcs/FT_NamingManagerC.h"
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
  : factory_registry_ ("NamingManager::FactoryRegistry"),
    group_factory_ (),
    built_in_balancing_strategy_name_ (1),
    object_group_property_name_ (1)

{
  // The name for the property which contains the load balancing strategy value
  this->built_in_balancing_strategy_name_.length (1);
  this->built_in_balancing_strategy_name_[0].id =
    CORBA::string_dup (FT::TAO_FT_LOAD_BALANCING_STRATEGY);

  // The name for the property which contains the object group name 
  this->object_group_property_name_.length (1);
  this->object_group_property_name_[0].id = CORBA::string_dup (FT::TAO_FT_OBJECT_GROUP_NAME);

}

TAO_FT_Naming_Manager::~TAO_FT_Naming_Manager (void)
{
}


CORBA::Object_ptr 
TAO_FT_Naming_Manager::create_object_group (
    const char * group_name,
    const char * type_id,
    const ::PortableGroup::Criteria & the_criteria)
{
  // Add the group name to the criteria and create the object
  TAO::PG_Property_Set property_set (the_criteria);
  PortableGroup::Value value;
  value <<= group_name;
  property_set.set_property (FT::TAO_FT_OBJECT_GROUP_NAME, value);

  // If no load balancing strategy was provided, set the default to FT::ROUND_ROBIN
  const PortableGroup::Value* lb_strat_value;
  int found = property_set.find (FT::TAO_FT_LOAD_BALANCING_STRATEGY, lb_strat_value);
  if (!found)
    {
      // Add the default load balancing strategy
      value <<= FT::ROUND_ROBIN;
      property_set.set_property (FT::TAO_FT_LOAD_BALANCING_STRATEGY, value);
    }

  PortableGroup::Criteria new_criteria;
  property_set.export_properties (new_criteria);
  PortableGroup::GenericFactory::FactoryCreationId_var fcid;
  return this->create_object (type_id, new_criteria, fcid.out());
}

void 
TAO_FT_Naming_Manager::delete_object_group (const char * group_name)
{
  // Find the object group with the specified name and delete the object
  PortableGroup::ObjectGroup_var group =
    this->get_object_group_ref_from_name (group_name);

  if (!CORBA::is_nil (group.in()))
  {
    PortableGroup::ObjectGroupId group_id =
      this->get_object_group_id (group);

    // Delete the object group from the factory
    this->group_factory_.delete_group (group_id);
  }
  else 
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }

}

PortableGroup::ObjectGroup_ptr 
TAO_FT_Naming_Manager::get_object_group_ref_from_name (const char * group_name)
{

  CORBA::String_var group_name_str (group_name);

  // Search for an object group that has a FT::TAO_FT_OBJECT_GROUP_NAME equal 
  // to the provided group_name
  PortableGroup::Property group_name_property;
  group_name_property.nam.length (1);
  group_name_property.nam[0].id = CORBA::string_dup (FT::TAO_FT_OBJECT_GROUP_NAME);
  group_name_property.val <<= group_name;

  TAO::PG_Object_Group* group;
  if (this->group_factory_.find_group (group_name_property, group))
  {
    return group->reference ();
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
}

::FT::GroupNames * 
TAO_FT_Naming_Manager::groups (void)
{
  PortableGroup::ObjectGroups *all_groups = this->group_factory_.all_groups ();
  int num_groups = all_groups->length ();

  FT::GroupNames* group_names;
  ACE_NEW_THROW_EX (
    group_names,
    FT::GroupNames,
    CORBA::NO_MEMORY());

  group_names->length (num_groups);
  std::string name;
  for (int i = 0; i < num_groups; ++i)
  {
    PortableGroup::ObjectGroup_var obj_group = (*all_groups)[i];
    if (this->group_name (obj_group.in (), &name))
    {
      (*group_names)[i] = CORBA::string_dup (name.c_str());
    }
    else 
    {
      (*group_names)[i] = CORBA::string_dup ("<unnamed group>");
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::groups: no name property set on group.\n")
        ));
    }
  }
  return group_names;
}

bool 
TAO_FT_Naming_Manager::group_name (PortableGroup::ObjectGroup_ptr group, std::string *name)
{
  if (CORBA::is_nil (group))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("%T %n (%P|%t) - FT_Naming_Manager::group_name: cannot get name for a null object.\n")
      ));
    return false;
  }
  
  PortableGroup::Properties* props = this->get_properties (group);
  PortableGroup::Value value;
  CORBA::Boolean found = TAO_PG::get_property_value (object_group_property_name_, 
                                                     *props,
                                                     value);
  if (found)
    { // Found the name property 
      value >>= *name;
      return true;
    }
  else
    return false;
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
    try {
      group->add_member (the_location,
                         member);
    }
    catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                 "TAO_FT_Naming_Manager::add_member - Issue with IOR of group or member"));
      throw PortableGroup::ObjectNotAdded ();
    }

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

  // Dont distributed the object group for its usage in the FT_Naming_Manager
  group->distribute (0);

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

bool  
TAO_FT_Naming_Manager::next_location (PortableGroup::ObjectGroup_ptr object_group,
                                      PortableGroup::Location& loc)
{
  bool result = false;

  PortableGroup::Properties* props = this->get_properties (object_group);
  PortableGroup::Value value;
  CORBA::Boolean found = TAO_PG::get_property_value (built_in_balancing_strategy_name_, 
                                                     *props,
                                                     value);

  // If there is no TAO_FT_LOAD_BALANCING_STRATEGY property in the object group
  // return failure
  if (!found) 
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T %n (%P|%t) - TAO_FT_Naming_Manager::next_location: object group has no TAO_FT_LOAD_BALANCING_STRATEGY property.\n")
               ));
    return false;
  }

  // Extract the load balancing strategy value
  FT::LoadBalancingStrategyValue load_bal_strategy;
  value >>= load_bal_strategy;

  switch (load_bal_strategy)
  {
  case FT::ROUND_ROBIN:
    result = this->round_robin_.next_location (object_group, this, loc);
    break;
  default:
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T %n (%P|%t) - TAO_FT_Naming_Manager::next_location: unsupported load balancing strategy requested.\n")
               ));

    result = false;
    break;
  }

  return result;
}

void
TAO_FT_Naming_Manager::preprocess_properties (PortableGroup::Properties & props)
{

}


TAO_END_VERSIONED_NAMESPACE_DECL
