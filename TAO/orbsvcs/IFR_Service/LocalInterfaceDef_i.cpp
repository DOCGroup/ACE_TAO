/* -*- C++ -*- */
// $Id$

#include "LocalInterfaceDef_i.h"

ACE_RCSID(IFR_Service, LocalInterfaceDef_i, "$Id$")

TAO_LocalInterfaceDef_i::TAO_LocalInterfaceDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_InterfaceDef_i (repo, section_key)
{
}

TAO_LocalInterfaceDef_i::~TAO_LocalInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_LocalInterfaceDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_LocalInterface;
}

