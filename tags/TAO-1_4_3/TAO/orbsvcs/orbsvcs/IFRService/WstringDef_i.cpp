/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "WstringDef_i.h"

ACE_RCSID (IFRService, 
           WstringDef_i, 
           "$Id$")

TAO_WstringDef_i::TAO_WstringDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_WstringDef_i::~TAO_WstringDef_i (void)
{
}

CORBA::DefinitionKind
TAO_WstringDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Wstring;
}

void
TAO_WstringDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_WstringDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->wstrings_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_WstringDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_WstringDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong bound = this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_wstring_tc (bound
                                                        ACE_ENV_ARG_PARAMETER);
}
CORBA::ULong
TAO_WstringDef_i::bound (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}


CORBA::ULong
TAO_WstringDef_i::bound_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int retval = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "bound",
                                             retval);

  return ACE_static_cast (CORBA::ULong, retval);
}

void
TAO_WstringDef_i::bound (CORBA::ULong bound
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->bound_i (bound
                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_WstringDef_i::bound_i (CORBA::ULong bound
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

