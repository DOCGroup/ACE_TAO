// -*- C++ -*-

#include "LB_PropertyManager.h"

ACE_RCSID (LoadBalancing,
           LB_PropertyManager,
           "$Id$")

// TAO_LB_PropertyManager::TAO_LB_PropertyManager (void)
// {
// }

void
TAO_LB_PropertyManager::set_default_properties (
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_LB_PropertyManager::remove_default_properties (
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
}

void
TAO_LB_PropertyManager::set_type_properties (
    const char * type_id,
    const LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_type_properties (
    const char * type_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO_LB_PropertyManager::remove_type_properties (
    const char * type_id,
    const LoadBalancing::Properties & props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
}

void
TAO_LB_PropertyManager::set_properties_dynamically (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
}

LoadBalancing::Properties *
TAO_LB_PropertyManager::get_properties (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
