// -*- C++ -*-

#include "LB_PropertyManager.h"

ACE_RCSID (LoadBalancing,
           LB_PropertyManager,
           "$Id$")

TAO_LB_PropertyManager::TAO_LB_PropertyManager (void)
{
}

void
TAO_LB_PropertyManager::set_default_properties (
    const TAO_LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
}

TAO_LoadBalancing::Properties *
TAO_LB_PropertyManager::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_PropertyManager::remove_default_properties (
    const TAO_LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
}

void
TAO_LB_PropertyManager::set_type_properties (
    const char * type_id,
    const TAO_LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
}

TAO_LoadBalancing::Properties *
TAO_LB_PropertyManager::get_type_properties (
    const char * type_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_PropertyManager::remove_type_properties (
    const char * type_id,
    const TAO_LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
}

void
TAO_LB_PropertyManager::set_properties_dynamically (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
}

TAO_LoadBalancing::Properties *
TAO_LB_PropertyManager::get_properties (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
}
