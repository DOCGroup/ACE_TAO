/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "EventPortDef_i.h"

ACE_RCSID (IFRService, 
           EventPortDef_i, 
           "$Id$")

TAO_EventPortDef_i::TAO_EventPortDef_i (TAO_Repository_i *repo)
  : TAO_Contained_i (repo),
    TAO_IRObject_i (repo)
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
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
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
    CORBA::ComponentIR::EventDef_ptr /* event */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
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
    const char * /* event_id */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
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
TAO_EventPortDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ComponentIR::EventPortDescription *ev_desc = 0;
  ACE_NEW_RETURN (ev_desc,
                  CORBA::ComponentIR::EventPortDescription,
                  0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                            "name",
                            holder);
  ev_desc->name = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                            "id",
                            holder);
  ev_desc->id = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                            "container_id",
                            holder);
  ev_desc->defined_in = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                            "version",
                            holder);
  ev_desc->version = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                            "base_type",
                            holder);
  ev_desc->version = holder.fast_rep ();

  CORBA::Contained::Description *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::Contained::Description,
                  0);

  retval->kind = this->def_kind ();
  retval->value <<= ev_desc;
  return retval;
}
