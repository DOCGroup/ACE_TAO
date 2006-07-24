// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/EventPortDef_i.h"
#include "orbsvcs/IFRService/EventDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"


ACE_RCSID (IFRService,
           EventPortDef_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EventPortDef_i::TAO_EventPortDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_EventPortDef_i::~TAO_EventPortDef_i (void)
{
}

CORBA::ComponentIR::EventDef_ptr
TAO_EventPortDef_i::event (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  return this->event_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ComponentIR::EventDef_ptr
TAO_EventPortDef_i::event_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_type",
                                            holder);
  CORBA::Contained_var obj = this->repo_->lookup_id (holder.fast_rep ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  return CORBA::ComponentIR::EventDef::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_EventPortDef_i::event (
    CORBA::ComponentIR::EventDef_ptr event
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_i (event
                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_EventPortDef_i::event_i (
    CORBA::ComponentIR::EventDef_ptr event
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_value (this->section_key_,
                                        "base_type");

  if (CORBA::is_nil (event))
    {
      return;
    }

  const char *tmp =
    TAO_IFR_Service_Utils::reference_to_path (event);

  ACE_Configuration_Section_Key new_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       tmp,
                                       new_key,
                                       0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (new_key,
                                            "id",
                                            holder);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "base_type",
                                            holder);
}

CORBA::Boolean
TAO_EventPortDef_i::is_a (
    const char *event_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (event_id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_EventPortDef_i::is_a_i (
    const char *event_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_type",
                                            holder);
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            holder.fast_rep (),
                                            holder);
  ACE_Configuration_Section_Key key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       holder,
                                       key,
                                       0);
  TAO_EventDef_i impl (this->repo_);
  impl.section_key (key);
  return impl.is_a_i (event_id
                      ACE_ENV_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_EventPortDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_EventPortDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ComponentIR::EventPortDescription epd;
  TAO_IFR_Desc_Utils<CORBA::ComponentIR::EventPortDescription,
                     TAO_EventPortDef_i>::fill_desc_begin (
                                              epd,
                                              this->repo_,
                                              this->section_key_
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK_RETURN (0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                            "base_type",
                            holder);
  epd.event = holder.fast_rep ();

  CORBA::Contained::Description *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::Contained::Description,
                  0);

  retval->kind = this->def_kind ();
  retval->value <<= epd;
  return retval;
}

CORBA::DefinitionKind
TAO_EventPortDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_none;
}

TAO_END_VERSIONED_NAMESPACE_DECL
