// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ProvidesDef_i.h"
#include "orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"

TAO_ProvidesDef_i::TAO_ProvidesDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ProvidesDef_i::~TAO_ProvidesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ProvidesDef_i::def_kind (void)
{
  return CORBA::dk_Provides;
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ProvidesDef_i::describe_i (void)
{
  CORBA::ComponentIR::ProvidesDescription pd;
  TAO_IFR_Desc_Utils<CORBA::ComponentIR::ProvidesDescription,
                     TAO_ProvidesDef_i>::fill_desc_begin (
                                             pd,
                                             this->repo_,
                                             this->section_key_
                                           );

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
TAO_ProvidesDef_i::interface_type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::InterfaceDef::_nil ());

  this->update_key ();

  return this->interface_type_i ();
}

CORBA::InterfaceDef_ptr
TAO_ProvidesDef_i::interface_type_i (void)
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
                                              this->repo_);

  return CORBA::InterfaceDef::_narrow (obj.in ());
}

void
TAO_ProvidesDef_i::interface_type (
    CORBA::InterfaceDef_ptr interface_type
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->interface_type_i (interface_type);
}

void
TAO_ProvidesDef_i::interface_type_i (
    CORBA::InterfaceDef_ptr interface_type
  )
{
  this->repo_->config ()->remove_value (this->section_key_,
                                        "base_type");

  if (CORBA::is_nil (interface_type))
    {
      return;
    }

  const char *tmp =
    TAO_IFR_Service_Utils::reference_to_path (interface_type);

  ACE_Configuration_Section_Key new_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       tmp,
                                       new_key,
                                       0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (new_key,
                                            "id",
                                            holder);

  this->repo_->config ()->set_string_value (
                              this->section_key_,
                              "base_type",
                              holder
                            );
}
