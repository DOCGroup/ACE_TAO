/* -*- C++ -*- */
// $Id$

#include "AliasDef_i.h"
#include "Repository_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, AliasDef_i, "$Id$")

TAO_AliasDef_i::TAO_AliasDef_i (TAO_Repository_i *repo,
                                ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_TypedefDef_i (repo, section_key)
{
}

TAO_AliasDef_i::~TAO_AliasDef_i (void)
{
}

IR::DefinitionKind
TAO_AliasDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Alias;
}

CORBA::TypeCode_ptr 
TAO_AliasDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_AliasDef_i::type_i (CORBA::Environment &ACE_TRY_ENV)
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

  ACE_TString original_type;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "original_type",
                                            original_type);

  ACE_Configuration_Section_Key ot_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       original_type,
                                       ot_key,
                                       0);

  TAO_IDLType_i *impl =
    this->repo_->servant_factory ()->create_idltype (ot_key,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  CORBA::TypeCode_var tc = impl->type_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_alias_tc (id.c_str (),
                                                      name.c_str (),
                                                      tc.in (),
                                                      ACE_TRY_ENV);
}

IR::IDLType_ptr 
TAO_AliasDef_i::original_type_def (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::IDLType::_nil ());

  return this->original_type_def_i (ACE_TRY_ENV);
}

IR::IDLType_ptr 
TAO_AliasDef_i::original_type_def_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString original_type;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "original_type",
                                            original_type);

  ACE_Configuration_Section_Key ot_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       original_type,
                                       ot_key,
                                       0);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (ot_key,
                                             "def_kind",
                                             kind);

  IR::DefinitionKind def_kind = 
    ACE_static_cast (IR::DefinitionKind, kind);

  CORBA::Object_var obj = 
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    original_type.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::IDLType::_nil ());

  return IR::IDLType::_narrow (obj.in (),
                               ACE_TRY_ENV);
}

void 
TAO_AliasDef_i::original_type_def (IR::IDLType_ptr original_type_def,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->original_type_def_i (original_type_def,
                             ACE_TRY_ENV);
}

void 
TAO_AliasDef_i::original_type_def_i (IR::IDLType_ptr original_type_def,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (original_type_def,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var original_type =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "original_type",
                                            original_type.in ());
}

