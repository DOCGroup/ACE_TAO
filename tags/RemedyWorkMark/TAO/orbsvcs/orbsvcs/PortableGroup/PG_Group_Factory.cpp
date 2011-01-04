// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Factory.cpp
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/PortableGroup/PG_Group_Factory.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "orbsvcs/PortableGroupC.h"
#include "orbsvcs/PortableGroup/PG_Object_Group.h"
#include <orbsvcs/PortableGroup/PG_Utils.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PG_Group_Factory::PG_Group_Factory ()
  : orb_ (CORBA::ORB::_nil())
  , poa_ (PortableServer::POA::_nil())
  , manipulator_ ()
  , domain_id_ ("default-domain")

{
}

TAO::PG_Group_Factory::~PG_Group_Factory (void)
{
  for (Group_Map_Iterator it = this->group_map_.begin ();
    it != this->group_map_.end ();
    ++it)
  {
    TAO::PG_Object_Group * group = (*it).int_id_;
    delete group;
  }
  this->group_map_.unbind_all ();
}


void TAO::PG_Group_Factory::init (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  PortableGroup::FactoryRegistry_ptr factory_registry)
{
  ACE_ASSERT (CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT (CORBA::is_nil (this->poa_.in ()));
  ACE_ASSERT (CORBA::is_nil (this->factory_registry_.in ()));

  this->orb_ = CORBA::ORB::_duplicate(orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);
  this->factory_registry_ = PortableGroup::FactoryRegistry::_duplicate (factory_registry);


  ACE_ASSERT (!CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));
  ACE_ASSERT (!CORBA::is_nil (this->factory_registry_.in ()));

  this->manipulator_.init (orb, poa);
}


TAO::PG_Object_Group * TAO::PG_Group_Factory::create_group (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    TAO::PG_Property_Set * typeid_properties)
{
  ///////////////////////////////////
  // Create an empty group reference

  PortableGroup::ObjectGroupId group_id = 0;
  PortableGroup::ObjectGroup_var empty_group =
    this->manipulator_.create_object_group (
      type_id,
      this->domain_id_,
      group_id);

  // pick up the object group information as assigned by
  // ObjectGroupManager

  PortableGroup::TagGroupTaggedComponent tagged_component;
  if (! TAO::PG_Utils::get_tagged_component (empty_group, tagged_component))
  {
    throw PortableGroup::ObjectNotCreated();
  }

  TAO::PG_Object_Group * objectGroup = 0;

  ACE_NEW_THROW_EX (
    objectGroup,
    TAO::PG_Object_Group (
      this->orb_.in (),
      this->factory_registry_.in (),
      this->manipulator_,
      empty_group.in (),
      tagged_component,
      type_id,
      the_criteria,
      typeid_properties
      ),
    CORBA::NO_MEMORY());

  if (this->group_map_.bind (group_id, objectGroup) != 0)
  {
    delete objectGroup;
    throw PortableGroup::ObjectNotCreated();
  }
  return objectGroup;
}

void TAO::PG_Group_Factory::delete_group (PortableGroup::ObjectGroup_ptr object_group)
{
  if (! destroy_group (object_group))
  {
    throw PortableGroup::ObjectNotFound ();
  }
}


void TAO::PG_Group_Factory::delete_group (PortableGroup::ObjectGroupId group_id)
{
  if (! destroy_group (group_id))
  {
    throw PortableGroup::ObjectNotFound ();
  }
}

    // insert group.  Take ownership
int TAO::PG_Group_Factory::insert_group ( ::TAO::PG_Object_Group * group)
{
  return insert_group (group->get_object_group_id(), group);
}

int TAO::PG_Group_Factory::insert_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group * group)
{
  return (this->group_map_.bind (group_id, group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group *& group) const
{
  return (this->group_map_.find (group_id , group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroup_ptr object_group, ::TAO::PG_Object_Group *& group) const
{
  int result = 0;
  PortableGroup::TagGroupTaggedComponent tc;
  if (TAO::PG_Utils::get_tagged_component (object_group, tc))
  {
    result = find_group (tc.object_group_id, group);
  }
  return result;
}

int TAO::PG_Group_Factory::destroy_group (PortableGroup::ObjectGroupId group_id)
{
  ::TAO::PG_Object_Group * group = 0;
  int result = (this->group_map_.unbind (group_id, group) == 0);
  if (result)
  {
    delete group;
  }
  return result;
}

int TAO::PG_Group_Factory::destroy_group (PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::TagGroupTaggedComponent tc;
  TAO::PG_Utils::get_tagged_component (object_group, tc);
  return destroy_group (tc.object_group_id);
}



PortableGroup::ObjectGroups *
TAO::PG_Group_Factory::groups_at_location (
    const PortableGroup::Location & the_location)
{
  size_t upper_limit = this->group_map_.current_size ();
  PortableGroup::ObjectGroups * result = 0;
  ACE_NEW_THROW_EX (
    result,
    PortableGroup::ObjectGroups (upper_limit),
    CORBA::NO_MEMORY());

  result->length(upper_limit);

  size_t group_count = 0;
  for (Group_Map_Iterator it = this->group_map_.begin ();
    it != this->group_map_.end ();
    ++it)
  {
    TAO::PG_Object_Group * group = (*it).int_id_;
    if (group->has_member_at (the_location))
    {
      (*result)[group_count] = group->reference ();
      ++group_count;
    }
  }
  result->length (group_count);
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
