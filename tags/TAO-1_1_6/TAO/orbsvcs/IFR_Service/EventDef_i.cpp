/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "EventDef_i.h"

ACE_RCSID(IFR_Service, EventDef_i, "$Id$")

TAO_EventDef_i::TAO_EventDef_i (TAO_Repository_i *repo,
                                ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_EventDef_i::~TAO_EventDef_i (void)
{
}

void 
TAO_EventDef_i::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_EventDef_i::describe (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean 
TAO_EventDef_i::is_a (const char * /* event_id */,
                      CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ValueDef_ptr 
TAO_EventDef_i::event (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

