// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ValueMemberDef_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ValueMemberDef_i::TAO_ValueMemberDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ValueMemberDef_i::~TAO_ValueMemberDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueMemberDef_i::def_kind (void)
{
  return CORBA::dk_ValueMember;
}

CORBA::Contained::Description *
TAO_ValueMemberDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ValueMemberDef_i::describe_i (void)
{
  CORBA::ValueMember vm;
  TAO_IFR_Desc_Utils<CORBA::ValueMember,
                     TAO_ValueMemberDef_i>::fill_desc_begin (
                                                vm,
                                                this->repo_,
                                                this->section_key_
                                              );

  vm.type = this->type_i ();

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_);

  vm.type_def = CORBA::IDLType::_narrow (obj.in ());

  CORBA::ULong val = 0;
  this->repo_->config ()->get_integer_value  (this->section_key_,
                                              "access",
                                              val);
  vm.access = static_cast<CORBA::Visibility> (val);

  CORBA::Contained::Description *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::Contained::Description,
                  0);

  retval->kind = CORBA::dk_ValueMember;
  retval->value <<= vm;
  return retval;
}

CORBA::TypeCode_ptr
TAO_ValueMemberDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ValueMemberDef_i::type_i (void)
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                            this->repo_);
  return impl->type_i ();
}

CORBA::IDLType_ptr
TAO_ValueMemberDef_i::type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->type_def_i ();
}

CORBA::IDLType_ptr
TAO_ValueMemberDef_i::type_def_i (void)
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            holder);
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_ValueMemberDef_i::type_def (CORBA::IDLType_ptr type_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->type_def_i (type_def);
}

void
TAO_ValueMemberDef_i::type_def_i (CORBA::IDLType_ptr type_def)
{
  const char *path = TAO_IFR_Service_Utils::reference_to_path (type_def);
  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            path);
}

CORBA::Visibility
TAO_ValueMemberDef_i::access (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->access_i ();
}

CORBA::Visibility
TAO_ValueMemberDef_i::access_i (void)
{
  CORBA::ULong val = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "access",
                                             val);
  return static_cast<CORBA::Visibility> (val);
}

void
TAO_ValueMemberDef_i::access (CORBA::Visibility access)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->access_i (access);
}

void
TAO_ValueMemberDef_i::access_i (CORBA::Visibility access)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "access",
                                             access);
}

TAO_END_VERSIONED_NAMESPACE_DECL
