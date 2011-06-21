// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/TypedefDef_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_TypedefDef_i::TAO_TypedefDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_TypedefDef_i::~TAO_TypedefDef_i (void)
{
}

CORBA::Contained::Description *
TAO_TypedefDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_TypedefDef_i::describe_i (void)
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::TypeDescription td;

  td.name = this->name_i ();

  td.id = this->id_i ();

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  td.defined_in = container_id.c_str ();

  td.version = this->version_i ();

  td.type = this->type_i ();

  retval->value <<= td;

  return retval._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
