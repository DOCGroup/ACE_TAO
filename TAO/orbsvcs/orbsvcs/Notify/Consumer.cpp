// $Id$

#include "Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Consumer, "$Id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Dispatch_Observer_T.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

TAO_NS_Consumer::TAO_NS_Consumer (TAO_NS_ProxySupplier* proxy)
  :proxy_ (proxy), event_dispatch_observer_ (0), event_collection_ (0), is_suspended_ (0)
{
  this->event_collection_ = new TAO_NS_Event_Collection ();
}

TAO_NS_Consumer::~TAO_NS_Consumer ()
{
  delete this->event_collection_;
}

TAO_NS_Proxy*
TAO_NS_Consumer::proxy (void)
{
  return this->proxy_supplier ();
}

void
TAO_NS_Consumer::push (const TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
{
  if (this->is_suspended_ == 1) // If we're suspended, queue for later delivery.
    {
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
        this->event_collection_->enqueue_head (event);
      }
    }

  ACE_TRY
    {
      this->push_i (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->event_dispatch_observer_ != 0)
        {
          this->event_dispatch_observer_->dispatch_success (this ACE_ENV_ARG_PARAMETER);

          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
          this->retry_count_ = 0;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_NS_Consumer::push: error sending event. informing dispatch observer\n ");
        }
      //ACE_RE_THROW;

      if (this->event_dispatch_observer_ != 0)
        {
          {
            ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());

            ++this->retry_count_;
            this->event_collection_->enqueue_head (event);
          }

          this->event_dispatch_observer_->dispatch_failure (this, this->retry_count_ ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_ENDTRY;
}

void
TAO_NS_Consumer::dispatch_pending (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  if (this->is_suspended_ == 1)
    return; // Do nothing if we're suspended.

  TAO_NS_Event_Collection event_collection_copy;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
    event_collection_copy = *this->event_collection_; // Payload is never copied, this is a collection of _vars.
    this->event_collection_->reset ();
  }

  TAO_NS_ProxySupplier* proxy_supplier = this->proxy_supplier ();

  TAO_NS_Event_var event;

  while (!event_collection_copy.is_empty ())
    {
      if (event_collection_copy.dequeue_head (event) == 0)
        {
          proxy_supplier->push_no_filtering (event);
        }
    }
}

void
TAO_NS_Consumer::resume (ACE_ENV_SINGLE_ARG_DECL)
{
  this->is_suspended_ = 0;

  this->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_Consumer::dispatch_updates_i (const TAO_NS_EventTypeSeq & added, const TAO_NS_EventTypeSeq & removed
                                     ACE_ENV_ARG_DECL)
{
  CosNotification::EventTypeSeq cos_added;
  CosNotification::EventTypeSeq cos_removed;

  added.populate (cos_added);
  removed.populate (cos_removed);

  if (!CORBA::is_nil (this->publish_.in ()))
    this->publish_->offer_change (cos_added, cos_removed ACE_ENV_ARG_PARAMETER);
}
