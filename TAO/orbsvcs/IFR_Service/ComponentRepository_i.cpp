/* -*- C++ -*- */
// $Id$

#include "ComponentRepository_i.h"

ACE_RCSID(IFR_Service, ComponentRepository_i, "$Id$")

TAO_ComponentRepository_i::TAO_ComponentRepository_i (
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    ACE_Configuration *config
  )
  : TAO_IRObject_i (0, ACE_Configuration_Section_Key ()),
    TAO_Container_i (0, ACE_Configuration_Section_Key ()),
    TAO_Repository_i (orb, poa, config)
{
}

TAO_ComponentRepository_i::~TAO_ComponentRepository_i (void)
{
}

IR::ComponentDef_ptr
TAO_ComponentRepository_i::create_component (
    const char *id,
    const char *name,
    const char *version,
    IR::ComponentDef_ptr base_component,
    const CORBA_InterfaceDefSeq & supports_interfaces
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::ComponentDef::_nil ());

  return this->create_component_i (id,
                                   name,
                                   version,
                                   base_component,
                                   supports_interfaces
                                   TAO_ENV_ARG_PARAMETER);
}

IR::ComponentDef_ptr
TAO_ComponentRepository_i::create_component_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::ComponentDef_ptr /* base_component */,
    const CORBA_InterfaceDefSeq & /* supports_interfaces */
    TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::HomeDef_ptr
TAO_ComponentRepository_i::create_home (
    const char *id,
    const char *name,
    const char *version,
    IR::HomeDef_ptr base_component,
    IR::ComponentDef_ptr managed_component,
    CORBA_ValueDef_ptr primary_key
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::HomeDef::_nil ());

  return this->create_home_i (id,
                              name,
                              version,
                              base_component,
                              managed_component,
                              primary_key
                              TAO_ENV_ARG_PARAMETER);
}

IR::HomeDef_ptr
TAO_ComponentRepository_i::create_home_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::HomeDef_ptr /* base_component */,
    IR::ComponentDef_ptr /* managed_component */,
    CORBA_ValueDef_ptr /* primary_key */
    TAO_ENV_ARG_DECL_NOT_USED /* TAO_ENV_SINGLE_ARG_PARAMETER */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

