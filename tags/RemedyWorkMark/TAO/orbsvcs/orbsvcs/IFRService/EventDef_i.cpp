// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/EventDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EventDef_i::TAO_EventDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_ValueDef_i (repo),
    TAO_ExtValueDef_i (repo)
{
}

TAO_EventDef_i::~TAO_EventDef_i (void)
{
}

CORBA::DefinitionKind
TAO_EventDef_i::def_kind (void)
{
  return CORBA::dk_Event;
}

void
TAO_EventDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_EventDef_i::destroy_i (void)
{
  // An event type is a restriction of a value type, not an extension.
  this->TAO_ExtValueDef_i::destroy_i ();
}

CORBA::Contained::Description *
TAO_EventDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_EventDef_i::describe_i (void)
{
  return this->TAO_ValueDef_i::describe_i ();
}

CORBA::Boolean
TAO_EventDef_i::is_a (const char *event_id)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->is_a_i (event_id);
}

CORBA::Boolean
TAO_EventDef_i::is_a_i (const char *event_id)
{
  if (ACE_OS::strcmp (event_id, "IDL:omg.org/CORBA/EventBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_ValueDef_i::is_a_i (event_id);
}

TAO_END_VERSIONED_NAMESPACE_DECL
