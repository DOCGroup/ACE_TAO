// $Id$

#include "orbsvcs/IFRService/ArrayDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ArrayDef_i::TAO_ArrayDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_ArrayDef_i::~TAO_ArrayDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ArrayDef_i::def_kind (void)
{
  return CORBA::dk_Array;
}

void
TAO_ArrayDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_ArrayDef_i::destroy_i (void)
{
  // Only if it is (w)string, fixed, array or sequence.
  this->destroy_element_type ();

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->arrays_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_ArrayDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ArrayDef_i::type_i (void)
{
  // Store the current array's section key.
  ACE_Configuration_Section_Key
    key_holder = this->section_key_;

  CORBA::TypeCode_var element_typecode =
    this->element_type_i ();

  // If this array contains another nested array (an array of an array
  // or an array of a struct that has a member array etc.) at some
  // level, the above call will have changed the array section key so
  // we have to replace it with the original value we stored above.
  this->section_key (key_holder);

  CORBA::ULong length = this->length_i ();

  return this->repo_->tc_factory ()->create_array_tc (
                                         length,
                                         element_typecode.in ()
                                       );
}

CORBA::ULong
TAO_ArrayDef_i::length (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->length_i ();
}

CORBA::ULong
TAO_ArrayDef_i::length_i (void)
{
  u_int length = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "length",
                                             length);

  return static_cast<CORBA::ULong> (length);
}

void
TAO_ArrayDef_i::length (CORBA::ULong length)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->length_i (length);
}

void
TAO_ArrayDef_i::length_i (CORBA::ULong length)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "length",
                                             length);
}

CORBA::TypeCode_ptr
TAO_ArrayDef_i::element_type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->element_type_i ();
}

CORBA::TypeCode_ptr
TAO_ArrayDef_i::element_type_i (void)
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);
  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (element_path,
                                            this->repo_);

  return impl->type_i ();
}

CORBA::IDLType_ptr
TAO_ArrayDef_i::element_type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->element_type_def_i ();
}

CORBA::IDLType_ptr
TAO_ArrayDef_i::element_type_def_i (void)
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (element_path,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_ArrayDef_i::element_type_def (CORBA::IDLType_ptr element_type_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->element_type_def_i (element_type_def);
}

void
TAO_ArrayDef_i::element_type_def_i (CORBA::IDLType_ptr element_type_def)
{
  this->destroy_element_type ();

  char *new_element_path =
    TAO_IFR_Service_Utils::reference_to_path (element_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "element_path",
                                            new_element_path);
}

void
TAO_ArrayDef_i::destroy_element_type (
  )
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);

  ACE_Configuration_Section_Key element_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       element_path,
                                       element_key,
                                       0);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (element_key,
                                             ACE_TEXT("def_kind"),
                                             kind);

  CORBA::DefinitionKind def_kind =
    TAO_IFR_Service_Utils::path_to_def_kind (element_path,
                                             this->repo_);

  switch (def_kind)
  {
    // These exist only as our elements, so the type should
    // be destroyed when we are destroyed, or when our element type
    // is mutated.
    case CORBA::dk_String:
    case CORBA::dk_Wstring:
    case CORBA::dk_Fixed:
    case CORBA::dk_Array:
    case CORBA::dk_Sequence:
    {
      TAO_IDLType_i *impl = this->repo_->select_idltype (def_kind);
      impl->section_key (element_key);

      impl->destroy_i ();

      break;
    }
    default:
      break;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
