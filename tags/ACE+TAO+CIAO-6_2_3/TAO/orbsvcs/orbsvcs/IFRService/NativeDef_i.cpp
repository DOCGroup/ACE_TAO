// $Id$

#include "orbsvcs/IFRService/NativeDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NativeDef_i::TAO_NativeDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_TypedefDef_i (repo)
{
}

TAO_NativeDef_i::~TAO_NativeDef_i (void)
{
}

CORBA::DefinitionKind
TAO_NativeDef_i::def_kind (void)
{
  return CORBA::dk_Native;
}

CORBA::TypeCode_ptr
TAO_NativeDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_NativeDef_i::type_i (void)
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  return this->repo_->tc_factory ()->create_native_tc (id.c_str (),
                                                       name.c_str ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
