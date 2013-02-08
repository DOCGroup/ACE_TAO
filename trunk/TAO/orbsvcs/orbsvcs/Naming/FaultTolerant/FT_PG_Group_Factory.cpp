// -*- C++ -*-

//=============================================================================
/**
 * @file  FT_PG_Group_Factory.cpp
 *
 * $Id$
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Naming/FaultTolerant/FT_PG_Group_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_PG_Object_Group_Storable.h"

#include "orbsvcs/PortableGroup/PG_Group_List_Store.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::FT_PG_Group_Factory::FT_PG_Group_Factory()
{
}

TAO::FT_PG_Group_Factory::~FT_PG_Group_Factory()
{
}

void
TAO::FT_PG_Group_Factory::set_object_group_stale (
  const FT_Naming::ObjectGroupUpdate & group_info)
{
  if (this->use_persistence_)
    {
      PortableGroup::ObjectGroupId group_id = group_info.id;

      // If a group was added or destroyed then the group list could be stale.
      if (group_info.change_type == FT_Naming::NEW ||
          group_info.change_type == FT_Naming::DELETED)
        {
          if (TAO_debug_level > 3)
            {
              ACE_CString change_type_str ("created");
              if (group_info.change_type == FT_Naming::DELETED)
                change_type_str = "deleted";
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("%T %n (%P|%t) - FT_PG_Group_Factory: ")
                            ACE_TEXT ("Setting list store as stale "),
                            ACE_TEXT ("because of group with ID %lld "),
                            ACE_TEXT ("was %s"),
                            group_id, change_type_str.c_str ()
                            ));
            }
          this->list_store_->stale(true);
        }

      PG_Object_Group * group = 0;
      if (!find_group (group_id, group))
        {
          throw PortableGroup::ObjectNotFound ();
        }
      FT_PG_Object_Group_Storable * og =
        dynamic_cast<FT_PG_Object_Group_Storable *> (group);

      if (!og)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%T %n (%P|%t) - FT_PG_Group_Factory ")
                      ACE_TEXT ("In setting object group stale could not cast ")
                      ACE_TEXT ("to FT_PG_Object_Group_Storable\n")));
          throw CORBA::INTERNAL ();
        }

      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%T %n (%P|%t) - ")
                      ACE_TEXT ("Setting object group with ID %lld as stale"),
                      group_id
                      ));
        }
      og->stale (true);
    }
  else
    {
      throw CORBA::INTERNAL ();
    }
}

TAO::PG_Object_Group_Storable *
TAO::FT_PG_Group_Factory::create_persistent_group (
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
  TAO::FT_PG_Object_Group_Storable (
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
TAO::FT_PG_Group_Factory::restore_persistent_group (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory)
{
  TAO::PG_Object_Group_Storable * objectGroup = 0;
  ACE_NEW_THROW_EX (
    objectGroup,
    TAO::FT_PG_Object_Group_Storable (
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
