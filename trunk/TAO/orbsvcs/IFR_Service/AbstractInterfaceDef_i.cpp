/* -*- C++ -*- */
// $Id$

#include "AbstractInterfaceDef_i.h"
#include "Repository_i.h"

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
TAO_AbstractInterfaceDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_AbstractInterface;
}

CORBA::Boolean
TAO_AbstractInterfaceDef_i::is_a (const char *interface_id
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (interface_id
                       TAO_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_AbstractInterfaceDef_i::is_a_i (const char *interface_id
                                    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/AbstractBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_InterfaceDef_i::is_a_i (interface_id
                                           TAO_ENV_ARG_PARAMETER);
}

