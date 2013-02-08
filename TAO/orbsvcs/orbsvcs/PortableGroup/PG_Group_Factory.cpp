// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Factory.cpp
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/PortableGroup/PG_Group_Factory.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_Group_List_Store.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "orbsvcs/PortableGroupC.h"
#include "orbsvcs/PortableGroup/PG_Object_Group.h"
#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"
#include <orbsvcs/PortableGroup/PG_Utils.h>

#include "tao/Storable_Factory.h"

#include <ace/SString.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  // Find the elements in set1 that
  // are missing in set2.
  template <class T>
  void
  find_missing(const std::set<T> & set1,
               const std::set<T> & set2,
               std::set<T> & missing_in_2)
  {
    missing_in_2.clear();
    for (typename std::set<T>::const_iterator it = set1.begin();
         it != set1.end(); ++it)
      {
        if (set2.find(*it) == set2.end())
          {
            missing_in_2.insert(*it);
          }
      }
  }
}

TAO::PG_Group_Factory::PG_Group_Factory ()
  : use_persistence_ (false)
  , list_store_ (0)
  , orb_ (CORBA::ORB::_nil())
  , poa_ (PortableServer::POA::_nil())
  , manipulator_ ()
  , domain_id_ ("default-domain")
  , groups_read_ (false)
  , storable_factory_ (0)
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
  delete this->list_store_;
  delete this->storable_factory_;
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

  this->factory_registry_ =
    PortableGroup::FactoryRegistry::_duplicate (factory_registry);


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
  PortableGroup::ObjectGroup_var empty_group;

  if (this->use_persistence_)
    {
      group_id = this->list_store_->get_next_group_id ();
      empty_group =
        this->manipulator_.create_object_group_using_id (
          type_id,
          this->domain_id_,
          group_id);
    }
  else
    {
      empty_group =
        this->manipulator_.create_object_group (
          type_id,
          this->domain_id_,
          group_id);
    }

  // pick up the object group information as assigned by
  // ObjectGroupManager

  PortableGroup::TagGroupTaggedComponent tagged_component;
  if (! TAO::PG_Utils::get_tagged_component (empty_group, tagged_component))
  {
    throw PortableGroup::ObjectNotCreated();
  }

  TAO::PG_Object_Group * objectGroup = 0;

  if (this->use_persistence_)
    {
      objectGroup = this->create_persistent_group (
          this->orb_.in (),
          this->factory_registry_.in (),
          this->manipulator_,
          empty_group.in (),
          tagged_component,
          type_id,
          the_criteria,
          typeid_properties,
          *storable_factory_);

      this->list_store_->add(group_id);
    }
  else
    {
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
    }

  if (this->group_map_.bind (group_id, objectGroup) != 0)
  {
    delete objectGroup;
    throw PortableGroup::ObjectNotCreated();
  }
  return objectGroup;
}

void TAO::PG_Group_Factory::delete_group (
  PortableGroup::ObjectGroup_ptr object_group)
{
  if (! destroy_group (object_group))
  {
    throw PortableGroup::ObjectNotFound ();
  }
}


void TAO::PG_Group_Factory::delete_group (
  PortableGroup::ObjectGroupId group_id)
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

int TAO::PG_Group_Factory::insert_group (PortableGroup::ObjectGroupId group_id,
                                         ::TAO::PG_Object_Group * group)
{
  return (this->get_group_map ().bind (group_id, group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroupId group_id,
                                       ::TAO::PG_Object_Group *& group)
{
  return (this->get_group_map ().find (group_id , group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroup_ptr object_group,
                                       ::TAO::PG_Object_Group *& group)
{
  int result = 0;
  PortableGroup::TagGroupTaggedComponent tc;
  if (TAO::PG_Utils::get_tagged_component (object_group, tc))
  {
    result = find_group (tc.object_group_id, group);
  }
  return result;
}

int TAO::PG_Group_Factory::find_group_with_name (const char* target_group_name,
                                                 TAO::PG_Object_Group *& group_target)
{
  int result = 0;

  // Search through the group map for the group with that property
  Group_Map & group_map = this->get_group_map ();
  for (Group_Map_Iterator it = group_map.begin ();
    it != group_map.end ();
    ++it)
  {
    TAO::PG_Object_Group * a_group = (*it).int_id_;
    // If the group has the group name in the property
    //
    const char* a_group_name = a_group->get_name ();
    if (a_group_name != 0 &&
        ACE_OS::strcmp (target_group_name,
                        a_group_name) == 0)
      { // This is the group we were looking for
        group_target = a_group;
        result = 1;
        break;
      }
  }
  return result;
}

int TAO::PG_Group_Factory::destroy_group (PortableGroup::ObjectGroupId group_id)
{
  ::TAO::PG_Object_Group * group = 0;
  int result = (this->get_group_map ().unbind (group_id, group) == 0);
  if (result)
  {
    if (this->use_persistence_)
      {
        PG_Object_Group_Storable *og =
          dynamic_cast<PG_Object_Group_Storable *> (group);
        if (!og)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%T %n (%P|%t) - PG_Group_Factory ")
                        ACE_TEXT ("In destroying group could not cast ")
                        ACE_TEXT ("to PG_Object_Group_Storable\n")));
            result = 0;
          }
        else
          {
            og->set_destroyed (true);
            result = (this->list_store_->remove (group->get_object_group_id ())
                      == 0);
          }

      }
    if (result)
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
  Group_Map & group_map = this->get_group_map ();
  size_t upper_limit = group_map.current_size ();
  PortableGroup::ObjectGroups * result = 0;
  ACE_NEW_THROW_EX (
    result,
    PortableGroup::ObjectGroups (upper_limit),
    CORBA::NO_MEMORY());

  result->length(upper_limit);

  size_t group_count = 0;
  for (Group_Map_Iterator it = group_map.begin ();
    it != group_map.end ();
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

PortableGroup::ObjectGroups *
TAO::PG_Group_Factory::all_groups (void)
{
  Group_Map & group_map = this->get_group_map ();
  size_t upper_limit = group_map.current_size ();
  PortableGroup::ObjectGroups * result = 0;
  ACE_NEW_THROW_EX (
    result,
    PortableGroup::ObjectGroups (upper_limit),
    CORBA::NO_MEMORY());

  result->length(upper_limit);

  size_t group_count = 0;
  for (Group_Map_Iterator it = group_map.begin ();
    it != group_map.end ();
    ++it)
  {
    TAO::PG_Object_Group * group = (*it).int_id_;
    (*result)[group_count] = CORBA::Object::_duplicate(group->reference ());
    ++group_count;
  }
  result->length (group_count);
  return result;
}

void
TAO::PG_Group_Factory::set_object_group_storable_factory (
  TAO::Storable_Factory * factory)
{
  this->use_persistence_ = true;
  this->storable_factory_  = factory;
  ACE_NEW_THROW_EX (this->list_store_,
                    TAO::PG_Group_List_Store (*this->storable_factory_),
                    CORBA::NO_MEMORY ());

}

TAO::PG_Group_Factory::Group_Map &
TAO::PG_Group_Factory::get_group_map ()
{
  if (this->use_persistence_)
    {
      // List of groups in persistent store may
      // have changed since group_map_ was last
      // updated.

      if (!this->groups_read_ || this->list_store_->list_obsolete ())
        {
          // Extract IDs from group_map_ to set for comparison with IDs in persistent store
          // This is to avoid having to repopulate the map from scratch.
          PG_Group_List_Store::Group_Ids map_ids;
          for (Group_Map_Iterator it = group_map_.begin ();
                                  it != group_map_.end (); ++it)
            {
              map_ids.insert (it->key ());
            }

          // Get the latest groups from persistent store
          const PG_Group_List_Store::Group_Ids & persistent_ids =
            list_store_->get_group_ids ();

          // Find groups added since map was last updated
          PG_Group_List_Store::Group_Ids groups_added;
          find_missing (persistent_ids, map_ids, groups_added);

          // Find groups removed since map was last updated
          PG_Group_List_Store::Group_Ids groups_removed;
          find_missing (map_ids, persistent_ids, groups_removed);

          // Bind added groups
          for (PG_Group_List_Store::Group_Id_Const_Iterator it = groups_added.begin ();
              it != groups_added.end (); ++it)
            {
              PortableGroup::ObjectGroupId group_id = *it;
              TAO::PG_Object_Group * objectGroup = 0;
              objectGroup = this->restore_persistent_group (
                group_id,
                this->orb_.in (),
                this->factory_registry_.in (),
                this->manipulator_,
                *storable_factory_);

              if (this->group_map_.bind (group_id, objectGroup) != 0)
                {
                  delete objectGroup;
                  throw PortableGroup::ObjectNotCreated ();
                }
            }

          // Unbind removed groups
          for (PG_Group_List_Store::Group_Id_Const_Iterator it = groups_removed.begin ();
               it != groups_removed.end (); ++it)
            {
              PortableGroup::ObjectGroupId group_id = *it;
              PG_Object_Group * group = 0;
              int result = (this->get_group_map ().unbind (group_id, group) == 0);
              if (result)
                {
                  delete group;
                }
              else
                throw PortableGroup::ObjectGroupNotFound ();
            }

          this->groups_read_ = true;

        }

    }

  return group_map_;
}

TAO::PG_Object_Group_Storable *
TAO::PG_Group_Factory::create_persistent_group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      TAO::PG_Property_Set * type_properties,
      TAO::Storable_Factory & storable_factory)
{
  TAO::PG_Object_Group_Storable * objectGroup = 0;
  ACE_NEW_THROW_EX (
  objectGroup,
  TAO::PG_Object_Group_Storable (
    orb,
    factory_registry,
    manipulator,
    empty_group,
    tagged_component,
    type_id,
    the_criteria,
    type_properties,
    storable_factory
    ),
  CORBA::NO_MEMORY());
  return objectGroup;
}

TAO::PG_Object_Group_Storable *
TAO::PG_Group_Factory::restore_persistent_group (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory)
{
  TAO::PG_Object_Group_Storable * objectGroup = 0;
  ACE_NEW_THROW_EX (
    objectGroup,
    TAO::PG_Object_Group_Storable (
      group_id,
      orb,
      factory_registry,
      manipulator,
      storable_factory
      ),
    CORBA::NO_MEMORY());
  return objectGroup;
}


TAO_END_VERSIONED_NAMESPACE_DECL
