// $Id$

#include "Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Consumer, "$Id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Dispatch_Observer_T.h"

TAO_NS_Consumer::TAO_NS_Consumer (TAO_NS_ProxySupplier* proxy)
  :proxy_ (proxy), event_dispatch_observer_ (0)
{
}

TAO_NS_Consumer::~TAO_NS_Consumer ()
{
}

TAO_NS_Proxy*
TAO_NS_Consumer::proxy (void)
{
  return this->proxy_supplier ();
}

void
TAO_NS_Consumer::push (const TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->push_i (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->event_dispatch_observer_ != 0)
        {
          this->event_dispatch_observer_->dispatch_success (this ACE_ENV_ARG_PARAMETER);

          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
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
            ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

            ++this->retry_count_;
            this->event_collection_.enqueue_head (event);
          }

          this->event_dispatch_observer_->dispatch_failure (this, this->retry_count_ ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_ENDTRY;
}

void
TAO_NS_Consumer::dispatch_pending (void)
{
  TAO_NS_Event_Collection event_collection_copy;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    event_collection_copy = this->event_collection_; // Payload is never copied, this is a collections of _vars.
    this->event_collection_.reset ();
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
