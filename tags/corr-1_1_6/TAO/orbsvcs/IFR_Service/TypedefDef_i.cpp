/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "TypedefDef_i.h"

ACE_RCSID(IFR_Service, TypedefDef_i, "$Id$")

TAO_TypedefDef_i::TAO_TypedefDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_TypedefDef_i::~TAO_TypedefDef_i (void)
{
}

IR::Contained::Description *
TAO_TypedefDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

IR::Contained::Description *
TAO_TypedefDef_i::describe_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    IR::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  IR::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  IR::TypeDescription td;

  ACE_CHECK_RETURN (0);
  td.name = this->name_i (ACE_TRY_ENV);

  td.id = this->id_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  td.defined_in = container_id.c_str ();

  td.version = this->version_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  td.type = this->type_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  retval->value <<= td;

  return retval._retn ();
}

