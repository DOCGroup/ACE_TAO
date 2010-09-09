// $Id$

#include "orbsvcs/IFRService/ValueBoxDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ValueBoxDef_i::TAO_ValueBoxDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_TypedefDef_i (repo)
{
}

TAO_ValueBoxDef_i::~TAO_ValueBoxDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueBoxDef_i::def_kind (void)
{
  return CORBA::dk_ValueBox;
}

CORBA::TypeCode_ptr
TAO_ValueBoxDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ValueBoxDef_i::type_i (void)
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  ACE_TString boxed_type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type_path);

  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (boxed_type_path,
                                            this->repo_);

  CORBA::TypeCode_var tc = impl->type_i ();

  return this->repo_->tc_factory ()->create_value_box_tc (id.c_str (),
                                                          name.c_str (),
                                                          tc.in ());
}

CORBA::IDLType_ptr
TAO_ValueBoxDef_i::original_type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->original_type_def_i ();
}

CORBA::IDLType_ptr
TAO_ValueBoxDef_i::original_type_def_i (void)
{
  ACE_TString boxed_type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type_path);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (boxed_type_path,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_ValueBoxDef_i::original_type_def (CORBA::IDLType_ptr original_type_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->original_type_def_i (original_type_def);
}

void
TAO_ValueBoxDef_i::original_type_def_i (CORBA::IDLType_ptr original_type_def)
{
  char *boxed_type =
    TAO_IFR_Service_Utils::reference_to_path (original_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type);
}

TAO_END_VERSIONED_NAMESPACE_DECL
