/* -*- C++ -*- */
// $Id$

#include "SequenceDef_i.h"
#include "Repository_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           SequenceDef_i, 
           "$Id$")

TAO_SequenceDef_i::TAO_SequenceDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_SequenceDef_i::~TAO_SequenceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_SequenceDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Sequence;
}

void
TAO_SequenceDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Only if it is (w)string, fixed, array or sequence.
  this->destroy_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->sequences_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var element_typecode =
    this->element_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  CORBA::ULong bound = this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_sequence_tc (
                                         bound,
                                         element_typecode.in ()
                                         ACE_ENV_ARG_PARAMETER
                                       );
}

CORBA::ULong
TAO_SequenceDef_i::bound (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->bound_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ULong
TAO_SequenceDef_i::bound_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int bound = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "bound",
                                             bound);

  return ACE_static_cast (CORBA::ULong, bound);
}

void
TAO_SequenceDef_i::bound (CORBA::ULong bound
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->bound_i (bound
                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::bound_i (CORBA::ULong bound
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->element_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);

  TAO_IDLType_i *impl = 
    TAO_IFR_Service_Utils::path_to_idltype (element_path,
                                            this->repo_);

  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_SequenceDef_i::element_type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->element_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_SequenceDef_i::element_type_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString element_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "element_path",
                                            element_path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (element_path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::element_type_def (CORBA::IDLType_ptr element_type_def
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->element_type_def_i (element_type_def
                            ACE_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::element_type_def_i (CORBA::IDLType_ptr element_type_def
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->destroy_element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  char *element_path = 
    TAO_IFR_Service_Utils::reference_to_path (element_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "element_path",
                                            element_path);
}

void
TAO_SequenceDef_i::destroy_element_type (
      ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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

      impl->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      break;
    }
    default:
      break;
  }
}
