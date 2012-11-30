// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO::PG_Object_Group_Storable::PG_Object_Group_Storable (
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  CORBA::Object_ptr empty_group,
  const PortableGroup::TagGroupTaggedComponent & tagged_component,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO::PG_Property_Set * type_properties)
  : PG_Object_Group(orb,
                    factory_registry,
                    manipulator,
                    empty_group,
                    tagged_component,
                    type_id,
                    the_criteria,
                    type_properties)
{
}

TAO::PG_Object_Group_Storable::~PG_Object_Group_Storable (void)
{
}

const PortableGroup::Location &
TAO::PG_Object_Group_Storable::get_primary_location (void) const
{
  return TAO::PG_Object_Group::get_primary_location ();
}


void
TAO::PG_Object_Group_Storable::add_member (const PortableGroup::Location & the_location,
                                  CORBA::Object_ptr member)
{
  PG_Object_Group::add_member (the_location, member);
}

int
TAO::PG_Object_Group_Storable::set_primary_member (
    TAO_IOP::TAO_IOR_Property * prop,
    const PortableGroup::Location & the_location)
{
  return PG_Object_Group::set_primary_member (prop, the_location);
}

void
TAO::PG_Object_Group_Storable::remove_member (
    const PortableGroup::Location & the_location)
{
  PG_Object_Group::remove_member (the_location);
}


PortableGroup::Locations *
TAO::PG_Object_Group_Storable::locations_of_members (void)
{
  return PG_Object_Group::locations_of_members ();
}

void
TAO::PG_Object_Group_Storable::create_member (
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria)
{
  PG_Object_Group::create_member (the_location,
                                  type_id,
                                  the_criteria);
}

void
TAO::PG_Object_Group_Storable::set_name (const char* group_name)
{
  PG_Object_Group::set_name (group_name);
}

const char*
TAO::PG_Object_Group_Storable::get_name (void)
{
  return PG_Object_Group::get_name ();
}

void
TAO::PG_Object_Group_Storable::initial_populate (void)
{
  PG_Object_Group::initial_populate ();
}

void
TAO::PG_Object_Group_Storable::minimum_populate (void)
{
  PG_Object_Group::minimum_populate ();
}

int
TAO::PG_Object_Group_Storable::has_member_at (const PortableGroup::Location & location)
{
  return PG_Object_Group::has_member_at (location);
}

void
TAO::PG_Object_Group_Storable::distribute (int value)
{
  PG_Object_Group::distribute (value);
}

CORBA::Object_ptr
TAO::PG_Object_Group_Storable::get_member_reference (
  const PortableGroup::Location & the_location)
{
  return PG_Object_Group::get_member_reference (the_location);
}

TAO_END_VERSIONED_NAMESPACE_DECL

