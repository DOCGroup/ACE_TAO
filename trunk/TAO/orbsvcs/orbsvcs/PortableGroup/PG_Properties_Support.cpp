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
}

TAO::PG_Properties_Support::~PG_Properties_Support ()
{

}

void TAO::PG_Properties_Support::set_default_property (const char * name,
      const PortableGroup::Value & value
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->default_properties_.set_property(name, value ACE_ENV_ARG_PARAMETER);
}

void TAO::PG_Properties_Support::set_default_properties (const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->default_properties_.decode (props ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Properties *
TAO::PG_Properties_Support::get_default_properties (
      ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::InvalidProperty,
    PortableGroup::UnsupportedProperty))
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX ( result, PortableGroup::Properties(), CORBA::NO_MEMORY());
  ACE_CHECK_RETURN (0);
  this->default_properties_.export_properties (*result);
  return result._retn ();
}

void TAO::PG_Properties_Support::remove_default_properties (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->default_properties_.remove (props);
}

void
TAO::PG_Properties_Support::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ( (CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  TAO::PG_Property_Set * typeid_properties;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    ACE_NEW_THROW_EX (
      typeid_properties,
      TAO::PG_Property_Set (overrides, & this->default_properties_),
      CORBA::NO_MEMORY());
    this->properties_map_.bind (type_id, typeid_properties);
  }
  typeid_properties->clear ();
  typeid_properties->decode (overrides ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Properties *
TAO::PG_Properties_Support::get_type_properties (
    const char *type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ( (CORBA::SystemException))
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX (result, PortableGroup::Properties(), CORBA::NO_MEMORY ());

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->internals_, 0);

  TAO::PG_Property_Set * typeid_properties = 0;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    typeid_properties->export_properties (*result);
  }
  return result._retn ();
}

void
TAO::PG_Properties_Support::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ( (CORBA::SystemException))
{
  // NOTE: do not actually delete the properties for this type.
  // There may be object groups depending on these.
  // Reference counted pointers could be used to allow property sets
  // for unused typeids to be deleted.

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  TAO::PG_Property_Set * typeid_properties = 0;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    typeid_properties->remove (props);
  }
}


TAO::PG_Property_Set *
TAO::PG_Properties_Support::find_typeid_properties (
    const char *type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->internals_, 0);

  TAO::PG_Property_Set * typeid_properties = 0;
  if ( 0 != this->properties_map_.find (type_id, typeid_properties))
  {
    ACE_NEW_THROW_EX (
      typeid_properties,
      TAO::PG_Property_Set (& this->default_properties_),
      CORBA::NO_MEMORY());
    this->properties_map_.bind (type_id, typeid_properties);
  }
  return typeid_properties;
}

TAO_END_VERSIONED_NAMESPACE_DECL
