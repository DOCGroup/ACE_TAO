/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "EventDef_i.h"

ACE_RCSID (IFRService, 
           EventDef_i, 
           "$Id$")

TAO_EventDef_i::TAO_EventDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_Container_i (repo),
    TAO_IDLType_i (repo),
    TAO_ValueDef_i (repo),
    TAO_ExtValueDef_i (repo)
{
}

TAO_EventDef_i::~TAO_EventDef_i (void)
{
}

CORBA::DefinitionKind
TAO_EventDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Event;
}

void
TAO_EventDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EventDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // An event type is a restriction of a value type, not an extension.
  this->TAO_ExtValueDef_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_EventDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_EventDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->TAO_ValueDef_i::describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_EventDef_i::is_a (const char *event_id
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (event_id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_EventDef_i::is_a_i (const char *event_id
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (event_id, "IDL:omg.org/CORBA/EventBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_ValueDef_i::is_a_i (event_id
                                       ACE_ENV_ARG_PARAMETER);
}

