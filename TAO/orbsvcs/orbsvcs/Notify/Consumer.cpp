// $Id$

#include "Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_Consumer, "$Id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "tao/debug.h"

TAO_Notify_Consumer::TAO_Notify_Consumer (TAO_Notify_ProxySupplier* proxy)
  :proxy_ (proxy), event_collection_ (0), is_suspended_ (0)
{
  this->event_collection_ = new TAO_Notify_Event_Collection ();
}

TAO_Notify_Consumer::~TAO_Notify_Consumer ()
{
  delete this->event_collection_;
}

TAO_Notify_Proxy*
TAO_Notify_Consumer::proxy (void)
{
  return this->proxy_supplier ();
}

void
TAO_Notify_Consumer::dispatch_pending (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->is_suspended_ == 1)
    return; // Do nothing if we're suspended.

  TAO_Notify_Event_Collection event_collection_copy;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
    event_collection_copy = *this->event_collection_; // Payload is never copied, this is a collection of _vars.
    this->event_collection_->reset ();
  }

  TAO_Notify_ProxySupplier* proxy_supplier = this->proxy_supplier ();

  TAO_Notify_Event_var event;

  while (!event_collection_copy.is_empty ())
    {
      if (event_collection_copy.dequeue_head (event) == 0)
        {
          proxy_supplier->push_no_filtering (event ACE_ENV_ARG_PARAMETER);
        }
    }
}

void
TAO_Notify_Consumer::resume (ACE_ENV_SINGLE_ARG_DECL)
{
  this->is_suspended_ = 0;

  this->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Consumer::dispatch_updates_i (const CosNotification::EventTypeSeq& added, const CosNotification::EventTypeSeq& removed
                                     ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (this->publish_.in ()))
    this->publish_->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
}
