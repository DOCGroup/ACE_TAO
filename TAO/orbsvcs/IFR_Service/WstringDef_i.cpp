/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "WstringDef_i.h"

ACE_RCSID(IFR_Service, StringDef_i, "$Id$")

TAO_WstringDef_i::TAO_WstringDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_WstringDef_i::~TAO_WstringDef_i (void)
{
}

CORBA::DefinitionKind
TAO_WstringDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Wstring;
}

void
TAO_WstringDef_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_WstringDef_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
TAO_WstringDef_i::type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_WstringDef_i::type_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong bound = this->bound_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_wstring_tc (bound
                                                        TAO_ENV_ARG_PARAMETER);
}
CORBA::ULong
TAO_WstringDef_i::bound (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->bound_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}


CORBA::ULong
TAO_WstringDef_i::bound_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->bound_i (bound
                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_WstringDef_i::bound_i (CORBA::ULong bound
                           TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

