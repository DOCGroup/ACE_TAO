/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueMemberDef_i.h"

ACE_RCSID (IFRService, 
           ValueMemberDef_i, 
           "$Id$")

TAO_ValueMemberDef_i::TAO_ValueMemberDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ValueMemberDef_i::~TAO_ValueMemberDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueMemberDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_ValueMember;
}

void
TAO_ValueMemberDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ValueMemberDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_ValueMemberDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ValueMemberDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_ValueMemberDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ValueMemberDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::IDLType_ptr
TAO_ValueMemberDef_i::type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_ValueMemberDef_i::type_def_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueMemberDef_i::type_def (CORBA::IDLType_ptr type_def
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->type_def_i (type_def
                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueMemberDef_i::type_def_i (CORBA::IDLType_ptr /* type_def */
                                  ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Visibility
TAO_ValueMemberDef_i::access (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->access_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Visibility
TAO_ValueMemberDef_i::access_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */ )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueMemberDef_i::access (CORBA::Visibility access
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->access (access
                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueMemberDef_i::access_i (CORBA::Visibility /* access */
                                ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */ )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}
