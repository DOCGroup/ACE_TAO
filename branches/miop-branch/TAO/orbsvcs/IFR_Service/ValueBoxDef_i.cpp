/* -*- C++ -*- */
// $Id$

#include "ValueBoxDef_i.h"
#include "Repository_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, ValueBoxDef_i, "$Id$")

TAO_ValueBoxDef_i::TAO_ValueBoxDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_TypedefDef_i (repo, section_key)
{
}

TAO_ValueBoxDef_i::~TAO_ValueBoxDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueBoxDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_ValueBox;
}

CORBA::TypeCode_ptr
TAO_ValueBoxDef_i::type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ValueBoxDef_i::type_i (TAO_ENV_SINGLE_ARG_DECL)
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

  ACE_TString boxed_type;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type);

  ACE_Configuration_Section_Key boxed_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       boxed_type,
                                       boxed_key,
                                       0);

  TAO_IDLType_i *impl =
    this->repo_->servant_factory ()->create_idltype (boxed_key
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  CORBA::TypeCode_var tc = impl->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_value_box_tc (id.c_str (),
                                                          name.c_str (),
                                                          tc.in ()
                                                          TAO_ENV_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_ValueBoxDef_i::original_type_def (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_IDLType::_nil ());

  return this->original_type_def_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_ValueBoxDef_i::original_type_def_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString boxed_type;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type);

  ACE_Configuration_Section_Key boxed_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       boxed_type,
                                       boxed_key,
                                       0);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (boxed_key,
                                             "def_kind",
                                             kind);

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    boxed_type.c_str ()
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_IDLType::_nil ());

  return CORBA_IDLType::_narrow (obj.in ()
                                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_ValueBoxDef_i::original_type_def (CORBA_IDLType_ptr original_type_def
                                      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->original_type_def_i (original_type_def
                             TAO_ENV_ARG_PARAMETER);
}

void
TAO_ValueBoxDef_i::original_type_def_i (CORBA_IDLType_ptr original_type_def
                                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (original_type_def
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var boxed_type =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "boxed_type",
                                            boxed_type.in ());
}

