// $Id$
#include "Notify_Event.h"

#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Notify_Event.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID(Notify, Notify_Event, "$Id$")

// @@ Pradeep: David is going to give you a hard time from having a
// static object
TAO_Notify_EventType
TAO_Notify_EventType::special_event_type_ ("*", "%ALL");

// @@ You can probably get away returning this stuff by value, and
// creating it on the fly, assuming there aren't too many calls to
// this.
TAO_Notify_EventType&
TAO_Notify_EventType::special_event_type (void)
{
  return special_event_type_;
}

TAO_Notify_EventType::TAO_Notify_EventType (void)
{
  // No-Op.
}

TAO_Notify_EventType::TAO_Notify_EventType (const char* domain_name,
                                            const char* type_name)
{
  this->event_type_.type_name = type_name;
  this->event_type_.domain_name = domain_name;
  this->recompute_hash ();
}

TAO_Notify_EventType::TAO_Notify_EventType (const CosNotification::EventType& event_type)
{
  this->event_type_.type_name = event_type.type_name.in ();
  this->event_type_.domain_name = event_type.domain_name.in ();
  this->recompute_hash ();
}

TAO_Notify_EventType::~TAO_Notify_EventType ()
{
}

u_long
TAO_Notify_EventType::hash (void) const
{
  // @@ Pradeep: this is an excellent candidate for an inline
  // function. Get in the habit of creating .h, .cpp and .i files,
  // even if initially the .i file is empty, that way it is easier to
  // do this stuff.
  return this->hash_value_;
}

void
TAO_Notify_EventType::recompute_hash (void)
{
  // @@ Pradeep: this code is bound to crash someday if the strings
  // are too long....  See if the hash_pjw () function can be modified
  // to take accumulate multiple strings, as in:
  //   hash = ACE::hash_pjw_accummulate (0, str1);
  //   hash = ACE::hash_pjw_accummulate (hash, str2);
  //
  // @@ Or use grow the buffer when needed, or just add the two hash
  // values or something, but fix this code!
  //
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
  if (this->hash () != rhs.hash ())
    return 0;
  else // compare the strings
    return (ACE_OS::strcmp (this->event_type_.type_name, rhs.event_type_.type_name) == 0  &&
            ACE_OS::strcmp (this->event_type_.domain_name, rhs.event_type_.domain_name) == 0
           );
}

CORBA::Boolean
TAO_Notify_EventType::is_special (void) const
{
  if ((this->event_type_.domain_name == 0 ||
             ACE_OS::strcmp (this->event_type_.domain_name, "") == 0 ||
             ACE_OS::strcmp (this->event_type_.domain_name, "*") == 0) &&
      (this->event_type_.type_name == 0 ||
             ACE_OS::strcmp (this->event_type_.domain_name, "") == 0 ||
             ACE_OS::strcmp (this->event_type_.type_name, "*") == 0 ||
             ACE_OS::strcmp (this->event_type_.type_name, "%ALL") == 0))
    return 1;
  else
    return 0;
}

const CosNotification::EventType&
TAO_Notify_EventType::get_native (void) const
{
  return event_type_;
}

// ****************************************************************

TAO_Notify_Event::TAO_Notify_Event (void)
  :lock_ (0),
   refcount_ (1),
   event_reliability_ (CosNotification::BestEffort),
   priority_ (CosNotification::DefaultPriority),
   //   start_time_ (0),
   //   stop_time_ (0),
   timeout_ (0)
{
  ACE_NEW (this->lock_, ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ());
}

TAO_Notify_Event::~TAO_Notify_Event ()
{
  delete this->lock_;
  this->lock_ = 0;
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "in ~TAO_Notify_Event %X\n", this));
}


void
TAO_Notify_Event::_incr_refcnt (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  this->refcount_++;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "in TAO_Notify_Event %X incr %d\n", this, this->refcount_));
}

void
TAO_Notify_Event::_decr_refcnt (void)
{
  int delete_me = 0;

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    this->refcount_--;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "in TAO_Notify_Event %X decr %d\n", this, this->refcount_));

    if (this->refcount_ == 0)
      delete_me = 1;
  }

  if (delete_me == 1)
    delete this;
}

// ****************************************************************

// = Any Event Type.

TAO_Notify_Any::TAO_Notify_Any (CORBA::Any * data)
  :data_ (data),
   is_owner_ (1)
{
}

TAO_Notify_Any::TAO_Notify_Any (const CORBA::Any * data)
  :data_ ((CORBA::Any*)data),
   is_owner_ (0)
{
}

TAO_Notify_Any::~TAO_Notify_Any ()
{
  if (this->is_owner_)
    delete this->data_;
}

TAO_Notify_Event*
TAO_Notify_Any::clone (void)
{
  TAO_Notify_Any* clone;

  if (this->is_owner_)
    {
      // @@ Are you sure this is the right way to clone?  You are
      // stealing the data from the original class...
      ACE_NEW_RETURN (clone, TAO_Notify_Any ((CORBA::Any const *)this->data_),
                      0);
      this->is_owner_ = 0;
    }
  else
    {
      CORBA::Any * data_copy;
      ACE_NEW_RETURN (data_copy, CORBA::Any (*this->data_), 0);
      ACE_NEW_RETURN (clone, TAO_Notify_Any (data_copy), 0);

      // Later: cleanup data_copy if this new fails.
    }

  return clone;
}

void
TAO_Notify_Any::operator=(const TAO_Notify_Any& notify_any)
{
  if (TAO_debug_level > 0)
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
TAO_Notify_Any::do_match (CosNotifyFilter::Filter_ptr filter
                          ACE_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_Any::do_match ()\n"));

  return filter->match (*this->data_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Any::do_push (CosEventComm::PushConsumer_ptr consumer
                         ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_Any::do_push ("
                          "CosEventComm::PushConsumer_ptr)\n"));

  consumer->push (*this->data_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Any::do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer
                         ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_Any::do_push ("
                          "CosNotifyComm::StructuredPushConsumer_ptr)\n"));

  // translation pg. 28
  CosNotification::StructuredEvent event;
  event.remainder_of_body <<= *this->data_;
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("%ANY");
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("");

  consumer->push_structured_event (event ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Any::do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                         const TAO_Notify_QoSAdmin_i& /*qos_admin*/,
                         CosNotification::EventBatch& /*unsent*/,
                         int /*flush_queue*/
                         ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_Any::do_push ("
                          "CosNotifyComm::SequencePushConsumer_ptr)\n"));

  // translation pg. 28
  CosNotification::StructuredEvent event;
  event.remainder_of_body <<= *this->data_;
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("%ANY");
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("");

  CosNotification::EventBatch events;
  events.length (1);
  events[0] = event;

  consumer->push_structured_events (events ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

// = TAO_Notify_StructuredEvent

// @@ Pradeep: many of the same comments that i made for
// TAO_Notify_Any apply here too.

TAO_Notify_StructuredEvent::TAO_Notify_StructuredEvent (CosNotification::StructuredEvent * notification)
  :data_ (notification),
   event_type_ (notification->header.fixed_header.event_type),
   is_owner_ (1)
{

  this->init_QoS ();
}

TAO_Notify_StructuredEvent::TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent * notification)
  :data_ ((CosNotification::StructuredEvent*)notification),
   event_type_ (notification->header.fixed_header.event_type),
   is_owner_ (0)
{
}

TAO_Notify_StructuredEvent::~TAO_Notify_StructuredEvent ()
{
  if (this->is_owner_)
    delete this->data_;
}

void
TAO_Notify_StructuredEvent::init_QoS (void)
{
  CosNotification::PropertySeq& qos = this->data_->header.variable_header;

  for (CORBA::ULong index = 0; index < qos.length (); ++index)
    {
      ACE_CString property_name (qos[index].name);

      if (property_name.compare (CosNotification::Priority) == 0)
        {
          qos[index].value >>= this->priority_;
        }
      else if (property_name.compare (CosNotification::StartTime) == 0)
        {
          // qos[index].value >>= this->start_time_;
        }
      else if (property_name.compare (CosNotification::StopTime) == 0)
        {
          // qos[index].value >>= this->stop_time_;
        }
      else if (property_name.compare (CosNotification::Timeout) == 0)
        {
          qos[index].value >>= this->timeout_;
        }
    }
}

TAO_Notify_Event*
TAO_Notify_StructuredEvent::clone (void)
{
  TAO_Notify_StructuredEvent* clone;

  if (this->is_owner_)
    {
      ACE_NEW_RETURN (clone, TAO_Notify_StructuredEvent ((CosNotification::StructuredEvent const *) this->data_), 0);
      this->is_owner_ = 0;
    }
  else
    {
      CosNotification::StructuredEvent *data_copy;
      ACE_NEW_RETURN (data_copy, CosNotification::StructuredEvent (*this->data_),
                      0);
      ACE_NEW_RETURN (clone, TAO_Notify_StructuredEvent (data_copy), 0);
      // Later: cleanup *data_copy if this new fails.
      clone->is_owner_ = 1;
    }

  return clone;
}

void
TAO_Notify_StructuredEvent::operator=(const TAO_Notify_StructuredEvent& structured_event)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_Notify_StructuredEvent::operator=\n"));

  if (this->is_owner_)
    delete this->data_;

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
TAO_Notify_StructuredEvent::do_match (CosNotifyFilter::Filter_ptr filter
                                      ACE_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_match ()\n"));

  return filter->match_structured (*this->data_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent::do_push (CosEventComm::PushConsumer_ptr consumer
                                     ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosEventComm::PushConsumer_ptr)\n"));

  // translation pg. 28
  CORBA::Any any;
  any <<= *this->data_;
  // is the typecode set by this operation or do we need to set it explicity.

  consumer->push (any ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent::do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosNotifyComm::StructuredPushConsumer_ptr)\n"));

  consumer->push_structured_event (*this->data_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent::do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                                     const TAO_Notify_QoSAdmin_i& /*qos_admin*/,
                                     CosNotification::EventBatch& /*unsent*/,
                                     int /*flush_queue*/
                                     ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosNotifyComm::SequencePushConsumer_ptr)\n"));

  CosNotification::EventBatch events;
  events.length (1);
  events[0] = *this->data_;

  consumer->push_structured_events (events ACE_ENV_ARG_PARAMETER);
}
