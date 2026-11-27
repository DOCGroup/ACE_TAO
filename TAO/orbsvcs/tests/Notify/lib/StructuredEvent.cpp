#include "StructuredEvent.h"

TAO_Notify_Tests_StructuredEvent::TAO_Notify_Tests_StructuredEvent () = default;

TAO_Notify_Tests_StructuredEvent::~TAO_Notify_Tests_StructuredEvent () = default;

TAO_Notify_Tests_StructuredEvent::TAO_Notify_Tests_StructuredEvent (char const *name)
{
  event_.header.fixed_header.event_name = CORBA::string_dup(name);
}

TAO_Notify_Tests_StructuredEvent::TAO_Notify_Tests_StructuredEvent (char const *domain, char const *type)
{
  event_.header.fixed_header.event_type.domain_name = CORBA::string_dup(domain);
  event_.header.fixed_header.event_type.type_name = CORBA::string_dup(type);
}

TAO_Notify_Tests_StructuredEvent::TAO_Notify_Tests_StructuredEvent (char const *name, char const *domain, char const *type)
{
  event_.header.fixed_header.event_name = CORBA::string_dup(name);
  event_.header.fixed_header.event_type.domain_name = CORBA::string_dup(domain);
  event_.header.fixed_header.event_type.type_name = CORBA::string_dup(type);
}

void
TAO_Notify_Tests_StructuredEvent::name (char const *name)
{
  event_.header.fixed_header.event_name = CORBA::string_dup(name);
}

void
TAO_Notify_Tests_StructuredEvent::type (char const *domain, char const *type)
{
  event_.header.fixed_header.event_type.domain_name = CORBA::string_dup(domain);
  event_.header.fixed_header.event_type.type_name = CORBA::string_dup(type);
}

char const *
TAO_Notify_Tests_StructuredEvent::type ()
{
  return event_.header.fixed_header.event_type.type_name;
}

void
TAO_Notify_Tests_StructuredEvent::opt_header (char const *name,
                                             CORBA::Any const & value)
{
  CosNotification::PropertySeq& opt =  event_.header.variable_header;
  opt.length (opt.length () + 1);

  opt[opt.length () - 1].name = CORBA::string_dup (name);
  opt[opt.length () - 1].value = value;
}

void
TAO_Notify_Tests_StructuredEvent::filter (char const *name,
                                          CORBA::Any const & value)
{
  CosNotification::PropertySeq& filter = event_.filterable_data;
  filter.length (filter.length () + 1);

  filter[filter.length () - 1].name = CORBA::string_dup (name);
  filter[filter.length () - 1].value = value;
}

void
TAO_Notify_Tests_StructuredEvent::qos (char const *name,
                                       CORBA::Any const & value)
{
  this->opt_header (name, value);
}

void
TAO_Notify_Tests_StructuredEvent::payload (CORBA::Any const & value)
{
  event_.remainder_of_body = value;
}

CosNotification::StructuredEvent&
TAO_Notify_Tests_StructuredEvent::event ()
{
  return this->event_;
}
