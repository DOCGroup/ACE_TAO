/* -*- C++ -*- */
// $Id$

#include "NativeDef_i.h"
#include "Repository_i.h"

ACE_RCSID(IFR_Service, NativeDef_i, "$Id$")

TAO_NativeDef_i::TAO_NativeDef_i (TAO_Repository_i *repo,
                                  ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_TypedefDef_i (repo, section_key)
{
}

TAO_NativeDef_i::~TAO_NativeDef_i (void)
{
}

CORBA::DefinitionKind
TAO_NativeDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Native;
}

CORBA::TypeCode_ptr
TAO_NativeDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_NativeDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  return this->repo_->tc_factory ()->create_native_tc (id.c_str (),
                                                       name.c_str ()
                                                       ACE_ENV_ARG_PARAMETER);
}

