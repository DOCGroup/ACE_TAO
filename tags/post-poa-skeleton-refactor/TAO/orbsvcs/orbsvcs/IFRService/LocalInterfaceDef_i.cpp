/* -*- C++ -*- */
// $Id$

#include "LocalInterfaceDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           LocalInterfaceDef_i, 
           "$Id$")

TAO_LocalInterfaceDef_i::TAO_LocalInterfaceDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo)
{
}

TAO_LocalInterfaceDef_i::~TAO_LocalInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_LocalInterfaceDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_LocalInterface;
}

CORBA::Boolean
TAO_LocalInterfaceDef_i::is_a (const char *interface_id
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (interface_id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_LocalInterfaceDef_i::is_a_i (const char *interface_id
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/LocalBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_InterfaceDef_i::is_a_i (interface_id
                                           ACE_ENV_ARG_PARAMETER);
}

