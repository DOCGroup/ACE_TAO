// $Id$

#include "orbsvcs/Notify/AdminProperties.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/AdminProperties.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/CosNotificationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_AdminProperties::TAO_Notify_AdminProperties (void)
  : max_global_queue_length_ (CosNotification::MaxQueueLength, 0)
  , max_consumers_ (CosNotification::MaxConsumers, 0)
  , max_suppliers_ (CosNotification::MaxSuppliers, 0)
  , reject_new_events_ (CosNotification::RejectNewEvents, 0)
  , global_queue_length_ (0)
  , global_queue_not_full_ (global_queue_lock_)
{
}

TAO_Notify_AdminProperties::~TAO_Notify_AdminProperties ()
{
}

int
TAO_Notify_AdminProperties::init (const CosNotification::PropertySeq& prop_seq)
{
  if (TAO_Notify_PropertySeq::init (prop_seq) != 0)
    return -1;

  this->max_global_queue_length_.set (*this);
  this->max_consumers_.set (*this);
  this->max_suppliers_.set (*this);
  this->reject_new_events_.set (*this);

  //@@ check if unsupported property was set.
  // This will happen when number of successfull inits != numbers of items bound in map_.

  return 0;
}

void
TAO_Notify_AdminProperties::init ()
{
  // This method should only be called once, (during topo load)
  ACE_ASSERT(this->size() == 0);

  if (this->max_global_queue_length_.is_valid())
  {
    CORBA::Any a;
    a <<= this->max_global_queue_length_.value();
    this->add(this->max_global_queue_length_.name(), a);
  }
  if (this->max_consumers_.is_valid())
  {
    CORBA::Any a;
    a <<= this->max_consumers_.value();
    this->add(this->max_consumers_.name(), a);
  }
  if (this->max_suppliers_.is_valid())
  {
    CORBA::Any a;
    a <<= this->max_suppliers_.value();
    this->add(this->max_suppliers_.name(), a);
  }
  if (this->reject_new_events_.is_valid())
  {
    CORBA::Any a;
    a <<= CORBA::Any::from_boolean(this->reject_new_events_.value());
    this->add(this->reject_new_events_.name(), a);
  }
}

CORBA::Boolean
TAO_Notify_AdminProperties::queue_full (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, 1);

  if (this->max_global_queue_length () == 0)
    return 0;
  else
    if (this->global_queue_length_ >= this->max_global_queue_length ().value ())
      return 1;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
