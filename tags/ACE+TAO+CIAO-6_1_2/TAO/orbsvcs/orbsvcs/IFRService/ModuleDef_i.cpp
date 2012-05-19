// $Id$

#include "orbsvcs/IFRService/ModuleDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ModuleDef_i::TAO_ModuleDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_Container_i (repo),
    TAO_ComponentContainer_i (repo),
    TAO_ComponentModuleDef_i (repo)
{
}

TAO_ModuleDef_i::~TAO_ModuleDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ModuleDef_i::def_kind (void)
{
  return CORBA::dk_Module;
}

void
TAO_ModuleDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_ModuleDef_i::destroy_i (void)
{
  // Destroy our members.
  TAO_Container_i::destroy_i ();

  // Destroy ourself.
  TAO_Contained_i::destroy_i ();
}

CORBA::Contained::Description *
TAO_ModuleDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ModuleDef_i::describe_i (void)
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::ModuleDescription md;

  md.name = this->name_i ();

  md.id = this->id_i ();

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  md.defined_in = container_id.c_str ();

  md.version = this->version_i ();

  retval->value <<= md;

  return retval._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
