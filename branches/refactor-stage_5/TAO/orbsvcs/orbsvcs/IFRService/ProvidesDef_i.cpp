/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ProvidesDef_i.h"
#include "IFR_ComponentsS.h"
#include "IFR_Service_Utils_T.h"

ACE_RCSID (IFRService, 
           ProvidesDef_i, 
           "$Id$")

TAO_ProvidesDef_i::TAO_ProvidesDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ProvidesDef_i::~TAO_ProvidesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ProvidesDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Provides;
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ComponentIR::ProvidesDescription pd;
  TAO_IFR_Desc_Utils<CORBA::ComponentIR::ProvidesDescription,
                     TAO_ProvidesDef_i>::fill_desc_begin (
                                             pd,
                                             this->repo_,
                                             this->section_key_
                                             ACE_ENV_ARG_PARAMETER
                                           );
  ACE_CHECK_RETURN (0);

  ACE_TString base_type_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_type",
                                            base_type_id);
  pd.interface_type = base_type_id.fast_rep ();

  CORBA::Contained::Description *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::Contained::Description,
                  0);

  retval->kind = CORBA::dk_Provides;
  retval->value <<= pd;
  return retval;
}

CORBA::InterfaceDef_ptr
TAO_ProvidesDef_i::interface_type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::InterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return this->interface_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDef_ptr
TAO_ProvidesDef_i::interface_type_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_type",
                                            holder);
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            holder.fast_rep (),
                                            holder);
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return CORBA::InterfaceDef::_narrow (obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ProvidesDef_i::interface_type (
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->interface_type_i (interface_type
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ProvidesDef_i::interface_type_i (
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  this->repo_->config ()->remove_value (this->section_key_,
                                        "base_type");

  if (CORBA::is_nil (interface_type))
    {
      return;
    }

  this->repo_->config ()->set_string_value (
                              this->section_key_,
                              "base_type",
                              interface_type->_interface_repository_id ()
                            );
}
