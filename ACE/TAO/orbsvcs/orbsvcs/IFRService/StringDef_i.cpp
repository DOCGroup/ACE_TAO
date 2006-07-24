// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/StringDef_i.h"

#include "ace/SString.h"


ACE_RCSID (IFRService,
           StringDef_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_StringDef_i::TAO_StringDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_StringDef_i::~TAO_StringDef_i (void)
{
}

CORBA::DefinitionKind
TAO_StringDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_String;
}

void
TAO_StringDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_StringDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->strings_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_StringDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_StringDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong bound = this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_string_tc (
                                         bound
                                         ACE_ENV_ARG_PARAMETER
                                       );
}

CORBA::ULong
TAO_StringDef_i::bound (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ULong
TAO_StringDef_i::bound_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int retval = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "bound",
                                             retval);

  return static_cast<CORBA::ULong> (retval);
}

void
TAO_StringDef_i::bound (CORBA::ULong bound
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
TAO_StringDef_i::bound_i (CORBA::ULong bound
                          ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

TAO_END_VERSIONED_NAMESPACE_DECL
