/* -*- C++ -*- */
// $Id$

#include "AbstractInterfaceDef_i.h"

ACE_RCSID(IFR_Service, AbstractInterfaceDef_i, "$Id$")

TAO_AbstractInterfaceDef_i::TAO_AbstractInterfaceDef_i (
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

TAO_AbstractInterfaceDef_i::~TAO_AbstractInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_AbstractInterfaceDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_AbstractInterface;
}

