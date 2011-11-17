// $Id$

#include "orbsvcs/IFRService/SequenceDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SequenceDef_i::TAO_SequenceDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_SequenceDef_i::~TAO_SequenceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_SequenceDef_i::def_kind (void)
{
  return CORBA::dk_Sequence;
}

void
TAO_SequenceDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_SequenceDef_i::destroy_i (void)
{
  // Only if it is (w)string, fixed, array or sequence.
  this->destroy_element_type ();

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->sequences_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::type_i (void)
{
  CORBA::TypeCode_var element_typecode =
    this->element_type_i ();

  CORBA::ULong bound = this->bound_i ();

  return this->repo_->tc_factory ()->create_sequence_tc (
                                         bound,
                                         element_typecode.in ()
                                       );
}

CORBA::ULong
TAO_SequenceDef_i::bound (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->bound_i ();
}

CORBA::ULong
TAO_SequenceDef_i::bound_i (void)
{
  u_int bound = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "bound",
                                             bound);

  return static_cast<CORBA::ULong> (bound);
}

void
TAO_SequenceDef_i::bound (CORBA::ULong bound)
{
  TAO_IFR_WRITE_GUARD;

  this->bound_i (bound);
}

void
TAO_SequenceDef_i::bound_i (CORBA::ULong bound)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->element_type_i ();
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type_i (void)
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
TAO_SequenceDef_i::element_type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->element_type_def_i ();
}

CORBA::IDLType_ptr
TAO_SequenceDef_i::element_type_def_i (void)
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
TAO_SequenceDef_i::element_type_def (CORBA::IDLType_ptr element_type_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->element_type_def_i (element_type_def);
}

void
TAO_SequenceDef_i::element_type_def_i (CORBA::IDLType_ptr element_type_def)
{
  this->destroy_element_type ();

  char *element_path =
    TAO_IFR_Service_Utils::reference_to_path (element_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "element_path",
                                            element_path);
}

void
TAO_SequenceDef_i::destroy_element_type ()
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);

  CORBA::DefinitionKind def_kind =
    TAO_IFR_Service_Utils::path_to_def_kind (element_path,
                                             this->repo_);

  switch (def_kind)
  {
    // These exist only as our elements, so the type should
    // be destroyed when we are destroyed or our element type
    // is mutated.
    case CORBA::dk_String:
    case CORBA::dk_Wstring:
    case CORBA::dk_Fixed:
    case CORBA::dk_Array:
    case CORBA::dk_Sequence:
    {
      TAO_IDLType_i *impl =
        TAO_IFR_Service_Utils::path_to_idltype (element_path,
                                                this->repo_);

      impl->destroy_i ();

      break;
    }
    default:
      break;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
