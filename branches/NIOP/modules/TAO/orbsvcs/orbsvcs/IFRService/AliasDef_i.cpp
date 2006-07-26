// $Id$

#include "orbsvcs/IFRService/AliasDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"


ACE_RCSID (IFRService,
           AliasDef_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_AliasDef_i::TAO_AliasDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_TypedefDef_i (repo)
{
}

TAO_AliasDef_i::~TAO_AliasDef_i (void)
{
}

CORBA::DefinitionKind
TAO_AliasDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Alias;
}

CORBA::TypeCode_ptr
TAO_AliasDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_AliasDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (original_type,
                                            this->repo_);
  if (0 == impl)
  {
     ACE_THROW_RETURN ( CORBA::OBJECT_NOT_EXIST(), CORBA::TypeCode::_nil () );
  }

  CORBA::TypeCode_var tc = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_alias_tc (id.c_str (),
                                                      name.c_str (),
                                                      tc.in ()
                                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_AliasDef_i::original_type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->original_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_AliasDef_i::original_type_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString original_type;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "original_type",
                                            original_type);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (original_type,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_AliasDef_i::original_type_def (CORBA::IDLType_ptr original_type_def
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->original_type_def_i (original_type_def
                             ACE_ENV_ARG_PARAMETER);
}

void
TAO_AliasDef_i::original_type_def_i (CORBA::IDLType_ptr original_type_def
                                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *original_type =
    TAO_IFR_Service_Utils::reference_to_path (original_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "original_type",
                                            original_type);
}

TAO_END_VERSIONED_NAMESPACE_DECL
