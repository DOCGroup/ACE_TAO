/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "UsesDef_i.h"

ACE_RCSID (IFRService, 
           UsesDef_i, 
           "$Id$")

TAO_UsesDef_i::TAO_UsesDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_UsesDef_i::~TAO_UsesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_UsesDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Uses;
}

void
TAO_UsesDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_UsesDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_UsesDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_UsesDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::InterfaceDef_ptr
TAO_UsesDef_i::interface_type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::InterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return this->interface_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDef_ptr
TAO_UsesDef_i::interface_type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_UsesDef_i::interface_type (
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
TAO_UsesDef_i::interface_type_i (
    CORBA::InterfaceDef_ptr /* interface_type */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  // TODO
}

CORBA::Boolean
TAO_UsesDef_i::is_multiple (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_multiple_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_UsesDef_i::is_multiple_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_UsesDef_i::is_multiple (
    CORBA::Boolean is_multiple
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_multiple_i (is_multiple
                       ACE_ENV_ARG_PARAMETER);
}

void 
TAO_UsesDef_i::is_multiple_i (
    CORBA::Boolean /* is_multiple */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  // TODO
}

