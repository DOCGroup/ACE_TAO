/* -*- C++ -*- */
// $Id$

#include "ModuleDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           ModuleDef_i, 
           "$Id$")

TAO_ModuleDef_i::TAO_ModuleDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ModuleDef_i::~TAO_ModuleDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ModuleDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Module;
}

void
TAO_ModuleDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ModuleDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Destroy ourself.
  TAO_Contained_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Contained::Description *
TAO_ModuleDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ModuleDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ModuleDescription md;

  md.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  md.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  md.defined_in = container_id.c_str ();

  md.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retval->value <<= md;

  return retval._retn ();
}
