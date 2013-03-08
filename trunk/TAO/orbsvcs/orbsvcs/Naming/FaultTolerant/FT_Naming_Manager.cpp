// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Manager.cpp
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
#include "orbsvcs/PortableGroup/PG_Utils.h"

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
    object_group_property_name_ (1),
    shutdown_ (0)

{
  // The name for the property which contains the load balancing strategy value
  this->built_in_balancing_strategy_name_.length (1);
  this->built_in_balancing_strategy_name_[0].id =
    ::FT_Naming::TAO_FT_LOAD_BALANCING_STRATEGY;

  // The name for the property which contains the object group name
  this->object_group_property_name_.length (1);
  this->object_group_property_name_[0].id =
    ::FT_Naming::TAO_FT_OBJECT_GROUP_NAME;
}

TAO_FT_Naming_Manager::~TAO_FT_Naming_Manager (void)
{
  this->object_group_property_name_.length (0);
  this->built_in_balancing_strategy_name_.length (0);
}


CORBA::Object_ptr
TAO_FT_Naming_Manager::create_object_group (
    const char * group_name,
    ::FT_Naming::LoadBalancingStrategyValue lb_strategy,
    const ::PortableGroup::Criteria & the_criteria)
{
  // Make sure the object group does not already exist.
  TAO::PG_Object_Group* group;
  if (this->group_factory_.find_group_with_name (group_name,
                                                 group))
    {
      throw PortableGroup::ObjectNotCreated ();
    }

  /// Currently only FT_Naming::ROUND_ROBIN is supported
  if (lb_strategy != FT_Naming::ROUND_ROBIN)
    {
      throw PortableGroup::ObjectNotCreated ();
    }

  // When creating the object group, it starts as a generic
  // CORBA Object. It will become the type of the first added
  // member.
  const char * type_id = "IDL:omg.org:CORBA/Object:1.0";

  // Add the group name to the criteria and create the object
  TAO::PG_Property_Set property_set (the_criteria);
  PortableGroup::Value value;
  value <<= group_name;
  property_set.set_property (::FT_Naming::TAO_FT_OBJECT_GROUP_NAME, value);

  // Add the load balancing strategy to the properties
  value <<= lb_strategy;
  property_set.set_property (::FT_Naming::TAO_FT_LOAD_BALANCING_STRATEGY, value);

  PortableGroup::Criteria new_criteria;
  property_set.export_properties (new_criteria);
  PortableGroup::GenericFactory::FactoryCreationId_var fcid;

  return this->create_object (group_name, type_id, new_criteria, fcid.out());
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
      this->get_object_group_id (group.in ());

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
  TAO::PG_Object_Group* group;
  if (this->group_factory_.find_group_with_name (group_name, group))
  {
    return group->reference ();
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
}

::FT_Naming::GroupNames *
TAO_FT_Naming_Manager::groups (::FT_Naming::LoadBalancingStrategyValue target_strategy)
{
  PortableGroup::ObjectGroups_var all_groups = this->group_factory_.all_groups ();
  int num_groups = all_groups->length ();

  ::FT_Naming::GroupNames* group_names;
  ACE_NEW_THROW_EX (
    group_names,
    ::FT_Naming::GroupNames (num_groups),
    CORBA::NO_MEMORY());

  int matching_groups = 0;
  for (int i = 0; i < num_groups; ++i)
  {
    PortableGroup::ObjectGroup_var obj_group = (all_groups.in ())[i].in ();

    // Extract the group's Load Balancing property
    PortableGroup::Name lb_strat_property_name (1);
    lb_strat_property_name.length (1);
    lb_strat_property_name[0].id =
      CORBA::string_dup (::FT_Naming::TAO_FT_LOAD_BALANCING_STRATEGY);
    PortableGroup::Properties_var props =
      this->get_properties (obj_group.in ());
    PortableGroup::Value value;
    if (!TAO_PG::get_property_value (lb_strat_property_name, props.in (), value))
      {
        (*group_names)[i] =
          CORBA::string_dup ("<group without LB property>");
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::groups: no LB ")
                    ACE_TEXT ("property set on group.\n")
                    ));
        continue;
      }

    ::FT_Naming::LoadBalancingStrategyValue lb_strategy_val;
    value >>= lb_strategy_val;

    if (lb_strategy_val == target_strategy)
    { // Groups load balancing strategy matches the target
      // Increment the count of matching groups
      ++matching_groups;
      char* name;

      // Get the group name and add it to the list to return.
      if (this->group_name (obj_group.in (), name))
      { // Group does have a name
        group_names->length (matching_groups);
        (*group_names)[matching_groups-1] = name;
      }
      else
      {
        { // Group has no name
          (*group_names)[i] = CORBA::string_dup ("<unnamed group>");
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::groups: no name ")
                      ACE_TEXT ("property set on group.\n")
            ));
        }
      }
    }
  }
  // Set the length to the actual num added
  group_names->length (matching_groups);
  return group_names;
}

void
TAO_FT_Naming_Manager::set_load_balancing_strategy (
  const char * group_name,
  ::FT_Naming::LoadBalancingStrategyValue lb_strategy)
{
  CORBA::Object_var group = this->get_object_group_ref_from_name (group_name);

  TAO::PG_Property_Set property_set;
  PortableGroup::Value value;
  // Add the load balancing strategy to the properties
  value <<= lb_strategy;
  property_set.set_property (::FT_Naming::TAO_FT_LOAD_BALANCING_STRATEGY,
                             value);
  PortableGroup::Properties properties;
  property_set.export_properties (properties);
  this->set_properties_dynamically (group.in (), properties);
}


bool
TAO_FT_Naming_Manager::group_name (PortableGroup::ObjectGroup_ptr group,
                                   char*& name)
{
  if (CORBA::is_nil (group))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::group_name: ")
                ACE_TEXT ("cannot get name for a null object.\n")
      ));
    return false;
  }

  TAO::PG_Object_Group* pg_group;
  if (this->group_factory_.find_group (group, pg_group))
  { // Found the object group in the factory
    const char* grp_name = pg_group->get_name ();
    if (grp_name != 0)
    { // Valid group name defined
      name = CORBA::string_dup (grp_name);
      return true;
    }
    else
    { // The group has no name
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_FT_Naming_Manager::group_name - ")
                         ACE_TEXT ("object group does not have a name")),
                        false);
    }
  }
  else
  { // The factory does not know about the group
    return false;
  }
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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::create_member: ")
                   ACE_TEXT ("unknown group\n")
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

  if (CORBA::is_nil (object_group))
    {
      if (TAO_debug_level > 3)
        {
          ACE_ERROR (
            (LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - TAO_FT_Naming_Manager::add_member")
             ACE_TEXT ("Null object group provided.\n")
             ));
        }
      throw PortableGroup::ObjectGroupNotFound ();
    }

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
                  ACE_TEXT ("TAO_FT_Naming_Manager::add_member - ")
                  ACE_TEXT ("Issue with IOR of group or member.\n")));
      throw PortableGroup::ObjectNotAdded ();
    }

    result = group->reference ();

  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::add_member ")
                   ACE_TEXT ("to unknown group\n")
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
  if (CORBA::is_nil (object_group))
    {
      if (TAO_debug_level > 3)
        {
          ACE_ERROR (
            (LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - TAO_FT_Naming_Manager::add_member")
             ACE_TEXT ("Null object group provided.\n")
             ));
        }
      throw PortableGroup::ObjectGroupNotFound ();
    }

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
  if (CORBA::is_nil (object_group))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT_Naming_Manager::locations_of_members ")
                  ACE_TEXT ("- null object group passed.\n")));
      throw PortableGroup::ObjectGroupNotFound ();
    }

  PortableGroup::Locations_var result = 0;

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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::")
                   ACE_TEXT ("locations_of_members: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn ();
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

  if (CORBA::is_nil (object_group))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT_Naming_Manager::get_object_group_id ")
                  ACE_TEXT ("- null object group passed.\n")));
      throw PortableGroup::ObjectGroupNotFound ();
    }

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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::")
                   ACE_TEXT ("get_object_group_id: unknown group\n")
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
  if (CORBA::is_nil (object_group))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT_Naming_Manager::get_object_group_ref ")
                  ACE_TEXT ("- null object group passed.\n")));
      throw PortableGroup::ObjectGroupNotFound ();
    }

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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::")
                   ACE_TEXT ("get_object_group_ref: unknown group\n")
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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::")
                   ACE_TEXT ("get_object_group_ref_from_id: unknown group\n")
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

  if (CORBA::is_nil (object_group))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT_Naming_Manager::get_member_ref ")
                  ACE_TEXT ("- null object group passed.\n")));
      throw PortableGroup::ObjectGroupNotFound ();
    }

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
                   ACE_TEXT ("TAO (%P|%t) - FT_Naming_Manager::")
                   ACE_TEXT ("get_member_ref: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

CORBA::Object_ptr
TAO_FT_Naming_Manager::create_object (
    const char * object_name,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out
      factory_creation_id)
{
  METHOD_ENTRY (TAO::FT_Naming_Manager::create_object);

  ////////////////////////////////
  // find the properties for this
  // type of object group
  TAO::PG_Property_Set_var typeid_properties =
    (this->properties_support_.find_typeid_properties (type_id));

  TAO::PG_Object_Group * group = this->group_factory_.create_group
    ( type_id, the_criteria, typeid_properties);

  group->set_name (object_name);

  // Dont distribute the object group for its usage in the FT_Naming_Manager
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
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  // Initialize the components used to implement the PortableGroup interfaces
  this->factory_registry_.init (orb);
  PortableGroup::FactoryRegistry_var factory_ref =
    factory_registry_.reference ();
  this->group_factory_.init (orb,
                             naming_mgr_poa,
                             factory_ref.in ());
}

CORBA::Object_ptr
TAO_FT_Naming_Manager::next_member (PortableGroup::ObjectGroup_ptr object_group)
{
  if (CORBA::is_nil (object_group))
    {
      if (TAO_debug_level > 3)
        {
          ACE_ERROR (
            (LM_ERROR,
             ACE_TEXT ("TAO (%P|%t) - TAO_FT_Naming_Manager::add_member")
             ACE_TEXT ("Null object group provided.\n")
             ));
        }
      throw PortableGroup::ObjectGroupNotFound ();
    }

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    CORBA::Object::_nil ()
                    );

  ACE_Auto_Ptr<PortableGroup::Properties> props (
    this->get_properties (object_group));
  PortableGroup::Value value;
  CORBA::Boolean found =
    TAO_PG::get_property_value (built_in_balancing_strategy_name_,
                                *(props.get ()),
                                value);

  // If there is no TAO_FT_LOAD_BALANCING_STRATEGY property in the object group
  // return failure
  if (!found)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) - TAO_FT_Naming_Manager::next_member: ")
                ACE_TEXT ("object group has no TAO_FT_LOAD_BALANCING_STRATEGY ")
                ACE_TEXT ("property.\n")
               ));

    return CORBA::Object::_nil();
  }

  // Extract the load balancing strategy value
  ::FT_Naming::LoadBalancingStrategyValue load_bal_strategy;
  value >>= load_bal_strategy;

  PortableGroup::Location next_location;

  bool result = false;

  switch (load_bal_strategy)
  {
  case ::FT_Naming::ROUND_ROBIN:
    result = this->round_robin_.next_location (object_group, this, next_location);
    break;
  default:
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) - TAO_FT_Naming_Manager::next_location: ")
                ACE_TEXT ("unsupported load balancing strategy requested.\n")
               ));

    return CORBA::Object::_nil();
    break;
  }

  if (result == true)
    return this->get_member_ref (object_group, next_location);
  else
    return CORBA::Object::_nil();
}


void
TAO_FT_Naming_Manager::preprocess_properties (PortableGroup::Properties &)
{
  // Nothing to do here for now.
}

void
TAO_FT_Naming_Manager::set_object_group_storable_factory (TAO::Storable_Factory * factory)
{
  this->group_factory_.set_object_group_storable_factory (factory);
}

void
TAO_FT_Naming_Manager::set_object_group_stale (const FT_Naming::ObjectGroupUpdate & group_info)
{
  this->group_factory_.set_object_group_stale (group_info);
}

TAO_END_VERSIONED_NAMESPACE_DECL
