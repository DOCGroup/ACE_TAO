/* -*- C++ -*- */
// $Id$

#include "ComponentContainer_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           ComponentContainer_i, 
           "$Id$")

TAO_ComponentContainer_i::TAO_ComponentContainer_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo)
{
}

TAO_ComponentContainer_i::~TAO_ComponentContainer_i (void)
{
}

CORBA::ComponentIR::ComponentDef_ptr 
TAO_ComponentContainer_i::create_component (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::ComponentDef_ptr base_component,
    const CORBA::InterfaceDefSeq &supports_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  return this->create_component_i (id,
                                   name,
                                   version,
                                   base_component,
                                   supports_interfaces
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ComponentDef_ptr 
TAO_ComponentContainer_i::create_component_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::ComponentDef_ptr base_component,
    const CORBA::InterfaceDefSeq &supports_interfaces
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::HomeDef_ptr 
TAO_ComponentContainer_i::create_home (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::HomeDef_ptr base_home,
    CORBA::ComponentIR::ComponentDef_ptr managed_component,
    const CORBA::InterfaceDefSeq &supports_interfaces,
    CORBA::ValueDef_ptr primary_key
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::HomeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::HomeDef::_nil ());

  return this->create_home_i (id,
                              name,
                              version,
                              base_home,
                              managed_component,
                              supports_interfaces,
                              primary_key
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::HomeDef_ptr 
TAO_ComponentContainer_i::create_home_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::HomeDef_ptr base_home,
    CORBA::ComponentIR::ComponentDef_ptr managed_component,
    const CORBA::InterfaceDefSeq &supports_interfaces,
    CORBA::ValueDef_ptr primary_key
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::EventDef_ptr 
TAO_ComponentContainer_i::create_event (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::ExtInitializerSeq &initializers
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  return this->create_event_i (id,
                               name,
                               version,
                               is_custom,
                               is_abstract,
                               base_value,
                               is_truncatable,
                               abstract_base_values,
                               supported_interfaces,
                               initializers
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::EventDef_ptr 
TAO_ComponentContainer_i::create_event_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::ExtInitializerSeq &initializers
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
