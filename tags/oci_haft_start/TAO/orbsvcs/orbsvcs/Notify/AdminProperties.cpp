// $Id$

#include "AdminProperties.h"

#if ! defined (__ACE_INLINE__)
#include "AdminProperties.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_AdminProperties, "$id$")

#include "orbsvcs/CosNotificationC.h"

TAO_Notify_AdminProperties::TAO_Notify_AdminProperties (void)
  : max_global_queue_length_ (CosNotification::MaxQueueLength, 0)
  , max_consumers_ (CosNotification::MaxConsumers, 0)
  , max_suppliers_ (CosNotification::MaxSuppliers, 0)
  , reject_new_events_ (CosNotification::RejectNewEvents, 0)
  , global_queue_length_ (0)
  , global_queue_not_full_condition_ (global_queue_lock_)
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

CORBA::Boolean
TAO_Notify_AdminProperties::queue_full (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->global_queue_lock_, 1);

  if (this->max_global_queue_length () == 0)
    return 0;
  else
    if (this->global_queue_length_ > this->max_global_queue_length ().value ())
      return 1;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Atomic_Op<TAO_SYNCH_MUTEX,int>;
template class ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX,int>;
template class ACE_Refcounted_Auto_Ptr<TAO_Notify_AdminProperties, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Atomic_Op<TAO_SYNCH_MUTEX,int>
#pragma instantiate ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX,int>
#pragma ACE_Refcounted_Auto_Ptr<TAO_Notify_AdminProperties, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
