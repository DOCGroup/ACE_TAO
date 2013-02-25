// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Properties_Support.cpp
 *
 *  $Id$
 *
 *  This file implements classes to help manage PortableGroup::Properties
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/PortableGroup/PG_Properties_Support.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PG_Properties_Support::PG_Properties_Support ()
{
  TAO::PG_Property_Set *props;
  ACE_NEW_THROW_EX (props,
                    TAO::PG_Property_Set (),
                    CORBA::NO_MEMORY());
  this->default_properties_.reset (props);
}

TAO::PG_Properties_Support::~PG_Properties_Support ()
{
  this->properties_map_.unbind_all ();
}

void TAO::PG_Properties_Support::set_default_property (const char * name,
      const PortableGroup::Value & value)
{
  this->default_properties_->set_property(name, value);
}

void TAO::PG_Properties_Support::set_default_properties (const PortableGroup::Properties & props)
{
  this->default_properties_->decode (props);
}

PortableGroup::Properties *
TAO::PG_Properties_Support::get_default_properties ()
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX ( result, PortableGroup::Properties(), CORBA::NO_MEMORY());
  this->default_properties_->export_properties (*result);
  return result._retn ();
}

void TAO::PG_Properties_Support::remove_default_properties (
    const PortableGroup::Properties & props)
{
  this->default_properties_->remove (props);
}

void
TAO::PG_Properties_Support::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  TAO::PG_Property_Set_var typeid_properties;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    TAO::PG_Property_Set *props;
    ACE_NEW_THROW_EX (props,
                      TAO::PG_Property_Set (overrides,
                                            this->default_properties_),
                      CORBA::NO_MEMORY());
    typeid_properties.reset (props);
    this->properties_map_.bind (type_id, typeid_properties);
  }
  typeid_properties->clear ();
  typeid_properties->decode (overrides);
}

PortableGroup::Properties *
TAO::PG_Properties_Support::get_type_properties (
    const char *type_id)
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX (result, PortableGroup::Properties(), CORBA::NO_MEMORY ());

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->internals_, 0);

  TAO::PG_Property_Set_var typeid_properties;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    typeid_properties->export_properties (*result);
  }
  return result._retn ();
}

void
TAO::PG_Properties_Support::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props)
{
  // NOTE: do not actually delete the properties for this type.
  // There may be object groups depending on these.
  // Reference counted pointers could be used to allow property sets
  // for unused typeids to be deleted.

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  TAO::PG_Property_Set_var typeid_properties;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    typeid_properties->remove (props);
  }
}


TAO::PG_Property_Set_var
TAO::PG_Properties_Support::find_typeid_properties (const char *type_id)
{
  TAO::PG_Property_Set_var result;
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->internals_, result);

  if ( 0 != this->properties_map_.find (type_id, result))
  {
    TAO::PG_Property_Set * props;
    ACE_NEW_THROW_EX (props,
                      TAO::PG_Property_Set (this->default_properties_),
                      CORBA::NO_MEMORY());
    result.reset (props);
    this->properties_map_.bind (type_id, result);
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
