/* -*- C++ -*- */
// $Id$

#include "SequenceDef_i.h"
#include "Repository_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, SequenceDef_i, "$Id$")

TAO_SequenceDef_i::TAO_SequenceDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_SequenceDef_i::~TAO_SequenceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_SequenceDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Sequence;
}

void
TAO_SequenceDef_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Only if it is (w)string, fixed, array or sequence.
  this->destroy_element_type (TAO_ENV_SINGLE_ARG_PARAMETER);
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
TAO_SequenceDef_i::type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::type_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var element_typecode =
    this->element_type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  CORBA::ULong bound = this->bound_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_sequence_tc (
                                         bound,
                                         element_typecode.in ()
                                         TAO_ENV_ARG_PARAMETER
                                       );
}

CORBA::ULong
TAO_SequenceDef_i::bound (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->bound_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ULong
TAO_SequenceDef_i::bound_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->bound_i (bound
                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::bound_i (CORBA::ULong bound
                            TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->element_type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_SequenceDef_i::element_type_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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

  TAO_IDLType_i *impl =
    this->repo_->servant_factory ()->create_idltype (element_key
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  return impl->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_SequenceDef_i::element_type_def (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_IDLType::_nil ());

  return this->element_type_def_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_SequenceDef_i::element_type_def_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
                                             "def_kind",
                                             kind);

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    element_path.c_str ()
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_IDLType::_nil ());

  return CORBA_IDLType::_narrow (obj.in ()
                                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::element_type_def (CORBA_IDLType_ptr element_type_def
                                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->element_type_def_i (element_type_def
                            TAO_ENV_ARG_PARAMETER);
}

void
TAO_SequenceDef_i::element_type_def_i (CORBA_IDLType_ptr element_type_def
                                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->destroy_element_type (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (element_type_def
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var element_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "element_path",
                                            element_path.in ());
}

void
TAO_SequenceDef_i::destroy_element_type (
      TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
                                             "def_kind",
                                             kind);

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

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
        this->repo_->servant_factory ()->create_idltype (element_key
                                                         TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      auto_ptr<TAO_IDLType_i> safety (impl);

      impl->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      break;
    }
    default:
      break;
  }
}

