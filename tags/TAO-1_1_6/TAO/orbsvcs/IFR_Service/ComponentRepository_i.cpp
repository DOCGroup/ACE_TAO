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
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::ComponentDef_ptr /* base_component */,
    const IR::InterfaceDefSeq & /* supports_interfaces */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::HomeDef_ptr 
TAO_ComponentRepository_i::create_home (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::HomeDef_ptr /* base_component */,
    IR::ComponentDef_ptr /* managed_component */,
    IR::ValueDef_ptr /* primary_key */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

