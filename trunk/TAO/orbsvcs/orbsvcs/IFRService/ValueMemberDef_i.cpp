/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueMemberDef_i.h"
#include "IDLType_i.h"
#include "IFR_Service_Utils_T.h"

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
TAO_ValueMemberDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ValueMember vm;
  TAO_IFR_Desc_Utils<CORBA::ValueMember,
                     TAO_ValueMemberDef_i>::fill_desc_begin_ex (
                                                vm,
                                                this->repo_,
                                                this->section_key_
                                                ACE_ENV_ARG_PARAMETER
                                              );
  ACE_CHECK_RETURN (0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  vm.type_def = CORBA::IDLType::_narrow (obj.in ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong val = 0;
  this->repo_->config ()->get_integer_value  (this->section_key_,
                                              "access",
                                              val);
  vm.access = ACE_static_cast (CORBA::Visibility,
                               val);

  CORBA::Contained::Description *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::Contained::Description,
                  0);

  retval->kind = CORBA::dk_ValueMember;
  retval->value <<= vm;
  return retval;
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
TAO_ValueMemberDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                            this->repo_);
  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_ValueMemberDef_i::type_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
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
TAO_ValueMemberDef_i::type_def_i (CORBA::IDLType_ptr type_def
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const char *path = TAO_IFR_Service_Utils::reference_to_path (type_def);
  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            path);
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
TAO_ValueMemberDef_i::access_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong val = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "access",
                                             val);
  return ACE_static_cast (CORBA::Visibility,
                          val);
}

void
TAO_ValueMemberDef_i::access (CORBA::Visibility access
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->access_i (access
                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueMemberDef_i::access_i (CORBA::Visibility access
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "access",
                                             access);
}
