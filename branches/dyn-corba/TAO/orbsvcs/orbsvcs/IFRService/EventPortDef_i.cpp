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
    CORBA::ComponentIR::EventDef_ptr event
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
    const char * event_id
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
