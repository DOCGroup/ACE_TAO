/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ProvidesDef_i.h"

ACE_RCSID (IFRService, 
           ProvidesDef_i, 
           "$Id$")

TAO_ProvidesDef_i::TAO_ProvidesDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ProvidesDef_i::~TAO_ProvidesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ProvidesDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Provides;
}

void
TAO_ProvidesDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ProvidesDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::InterfaceDef_ptr
TAO_ProvidesDef_i::interface_type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::InterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return this->interface_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDef_ptr
TAO_ProvidesDef_i::interface_type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ProvidesDef_i::interface_type (
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->interface_type_i (interface_type
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ProvidesDef_i::interface_type_i (
    CORBA::InterfaceDef_ptr /* interface_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  // TODO
}
