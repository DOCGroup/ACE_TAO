// $Id$
#include "Notify_Types.h"

TAO_Notify_EventType
TAO_Notify_EventType::special_event_type_ ("*", "%ALL");

TAO_Notify_EventType&
TAO_Notify_EventType::special_event_type (void)
{
  return special_event_type_;
}

TAO_Notify_EventType::TAO_Notify_EventType (void)
{
  // No-Op.
}

TAO_Notify_EventType::TAO_Notify_EventType (const char* domain_name, const char* type_name)
{
  this->event_type_.type_name = type_name;
  this->event_type_.domain_name = domain_name;
  this->recompute_hash ();
}

TAO_Notify_EventType::TAO_Notify_EventType (const CosNotification::EventType& event_type)
{
  // @@ Check it these dups are indeed required.
  this->event_type_.type_name = CORBA::string_dup (event_type.type_name.in ());
  this->event_type_.domain_name = CORBA::string_dup (event_type.domain_name.in ());
  this->recompute_hash ();
}

TAO_Notify_EventType::~TAO_Notify_EventType ()
{
  // No-Op.
}

u_long
TAO_Notify_EventType::hash (void) const
{
  return this->hash_value_;
}

void
TAO_Notify_EventType::recompute_hash (void)
{
  char buffer[BUFSIZ];
  ACE_OS::strcpy (buffer, this->event_type_.domain_name.in ());
  ACE_OS::strcat (buffer, this->event_type_.type_name.in ());

  this->hash_value_ =  ACE::hash_pjw (buffer);
}

void
TAO_Notify_EventType::operator=(const CosNotification::EventType& rhs)
{
  this->event_type_.type_name = rhs.type_name;
  this->event_type_.domain_name = rhs.domain_name;
  this->recompute_hash ();
}

int
TAO_Notify_EventType::operator==(const TAO_Notify_EventType& rhs) const
{
  if (this->hash () == rhs.hash ())
    return 1;
  else
    return 0;
}

CORBA::Boolean
TAO_Notify_EventType::is_special (void) const
{
  if ((event_type_.domain_name == 0 ||
       ACE_OS::strcmp (event_type_.domain_name, "*") == 0) &&
      (event_type_.type_name == 0 ||
       ACE_OS::strcmp (event_type_.type_name, "*") == 0 ||
       ACE_OS::strcmp (event_type_.type_name, "%ALL") == 0))
    return 1;
  else
    return 0;
}

TAO_Notify_Any::TAO_Notify_Any (void)
{
  is_owner_ = 0;
}

TAO_Notify_Any::TAO_Notify_Any (const CORBA::Any & data)
  :data_ ((CORBA::Any*)&data)
{
  is_owner_ = 0;
}

TAO_Notify_Any::~TAO_Notify_Any ()
{
  if (this->is_owner_)
    delete data_;
}

TAO_Notify_Event*
TAO_Notify_Any::clone (void)
{
  TAO_Notify_Any* clone;
  ACE_NEW_RETURN (clone, TAO_Notify_Any (), 0);

  if (this->is_owner_)
    {
      this->is_owner_ = 0;
      clone->data_ = this->data_;
      clone->is_owner_ = 1;
    }
  else
    {
      ACE_NEW_RETURN (clone->data_,
                      CORBA::Any (*this->data_), 0);
      // Later: cleanup *clone if this new fails.
      clone->is_owner_ = 1;
    }

  return clone;
}

void
TAO_Notify_Any::operator=(const TAO_Notify_Any& notify_any)
{
  ACE_DEBUG ((LM_DEBUG, "In TAO_Notify_Any::operator=\n"));
  if (this->is_owner_)
    delete data_;

  ACE_NEW (this->data_,
           CORBA::Any (*notify_any.data_));

  this->is_owner_ = 1;
}

CORBA::Boolean
TAO_Notify_Any::is_special_event_type (void) const
{
  return 1;
}

const TAO_Notify_EventType&
TAO_Notify_Any::event_type (void) const
{
  return TAO_Notify_EventType::special_event_type ();
}

CORBA::Boolean
TAO_Notify_Any::do_match (CosNotifyFilter::Filter_ptr filter, CORBA::Environment &ACE_TRY_ENV) const
{
  return filter->match (*this->data_, ACE_TRY_ENV);
}

void
TAO_Notify_Any::do_push (CosEventComm::PushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const
{
  consumer->push (*this->data_, ACE_TRY_ENV);
}

void
TAO_Notify_Any::do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const
{
  // translation pg. 28
  CosNotification::StructuredEvent event;
  event.remainder_of_body <<= *this->data_;
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("%ANY");
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("");

  consumer->push_structured_event (event, ACE_TRY_ENV);
}

// = TAO_Notify_StructuredEvent

TAO_Notify_StructuredEvent::TAO_Notify_StructuredEvent (void)
{
  is_owner_ = 0;
}

TAO_Notify_StructuredEvent::TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent & notification)
  :data_ ((CosNotification::StructuredEvent*)&notification),
   event_type_ (notification.header.fixed_header.event_type)
{
  is_owner_ = 0;
}

TAO_Notify_StructuredEvent::~TAO_Notify_StructuredEvent ()
{
  if (this->is_owner_)
    delete data_;
}

TAO_Notify_Event*
TAO_Notify_StructuredEvent::clone (void)
{
  TAO_Notify_StructuredEvent* clone;
  ACE_NEW_RETURN (clone, TAO_Notify_StructuredEvent (), 0);

  if (this->is_owner_)
    {
      this->is_owner_ = 0;
      clone->data_ = this->data_;
      clone->is_owner_ = 1;
    }
  else
    {
      ACE_NEW_RETURN (clone->data_,
                      CosNotification::StructuredEvent (*this->data_), 0);
      // Later: cleanup *clone if this new fails.
      clone->is_owner_ = 1;
    }

  return clone;
}

void
TAO_Notify_StructuredEvent::operator=(const TAO_Notify_StructuredEvent& structured_event)
{
  ACE_DEBUG ((LM_DEBUG, "In TAO_Notify_StructuredEvent::operator=\n"));
  if (this->is_owner_)
    delete data_;

  ACE_NEW (this->data_,
           CosNotification::StructuredEvent (*structured_event.data_));

  this->is_owner_ = 1;
  this->event_type_ = structured_event.data_->header.fixed_header.event_type;
}

CORBA::Boolean
TAO_Notify_StructuredEvent::is_special_event_type (void) const
{
  return this->event_type_.is_special ();
}

const TAO_Notify_EventType&
TAO_Notify_StructuredEvent::event_type (void) const
{
  return this->event_type_;
}

CORBA::Boolean
TAO_Notify_StructuredEvent::do_match (CosNotifyFilter::Filter_ptr filter, CORBA::Environment &ACE_TRY_ENV) const
{
  return filter->match_structured (*this->data_, ACE_TRY_ENV);
}

void
TAO_Notify_StructuredEvent::do_push (CosEventComm::PushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const
{
  // translation pg. 28
  CORBA::Any any;
  any <<= *this->data_;
  // is the typecode set by this operation or do we need to set it explicity.

  consumer->push (any, ACE_TRY_ENV);
}

void
TAO_Notify_StructuredEvent::do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer, CORBA::Environment &ACE_TRY_ENV) const
{
  consumer->push_structured_event (*this->data_, ACE_TRY_ENV);
}

// = EVENTTYPE_LIST
void
EVENTTYPE_LIST::populate (CosNotification::EventTypeSeq& event_type_seq)
{
  event_type_seq.length (this->size ());

  EVENTTYPE_LIST::ITERATOR iter (*this);

  TAO_Notify_EventType* event_type;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    event_type_seq[i] = event_type->event_type_;
}

void
EVENTTYPE_LIST::insert_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::insert (event_type);
    }
}

void
EVENTTYPE_LIST::remove_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::remove (event_type);
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set <TAO_Notify_EventType>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>;
template class ACE_Node<TAO_Notify_EventType>;

template class ACE_Unbounded_Set<TAO_Notify_Event_Listener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_Event_Listener*>;
template class ACE_Node<TAO_Notify_Event_Listener*>;

template class ACE_Unbounded_Set<TAO_Notify_Update_Listener*>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_Update_Listener*>;
template class ACE_Node<TAO_Notify_Update_Listener*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_EventType>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_EventType>
#pragma instantiate ACE_Node<TAO_Notify_EventType>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_Event_Listener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_Event_Listener*>
#pragma instantiate ACE_Node<TAO_Notify_Event_Listener*>

#pragma instantiate ACE_Unbounded_Set<TAO_Notify_Update_Listener*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_Update_Listener*>
#pragma instantiate ACE_Node<TAO_Notify_Update_Listener*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
