// $Id$

#include "Peer.h"

#if ! defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Peer, "$id$")

#include "tao/debug.h"
#include "Dispatch_Observer_T.h"
#include "Proxy.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

TAO_NS_Peer::TAO_NS_Peer (void)
  :updates_dispatch_observer_ (0), retry_count_ (1)
{
}

TAO_NS_Peer::~TAO_NS_Peer ()
{
}

void
TAO_NS_Peer::qos_changed (TAO_NS_QoSProperties& /*qos_properties*/)
{
  // NOP.
}

void
TAO_NS_Peer::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // NOP.
}

void
TAO_NS_Peer::dispatch_pending (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Proxy* proxy = this->proxy ();
  TAO_NS_EventTypeSeq& added = proxy->added_;
  TAO_NS_EventTypeSeq& removed = proxy->removed_;

  if (added.size () == 0 && removed.size () == 0)
    return; // Return if nothing to send.

  TAO_NS_EventTypeSeq added_copy;
  TAO_NS_EventTypeSeq removed_copy;
  TAO_NS_Reverse_Lock unlock (proxy->lock_);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, proxy->lock_);

  added_copy = added;
  removed_copy = removed;
  added.reset ();
  removed.reset ();

  ACE_TRY
    {
      {
        ACE_GUARD (TAO_NS_Reverse_Lock, ace_mon, unlock);

        this->dispatch_updates_i (added_copy, removed_copy ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (this->updates_dispatch_observer_ != 0)
          this->updates_dispatch_observer_->dispatch_success (this ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      this->retry_count_ = 0;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Peer:dispatch_pending serror sending updates\n ");
        }
      //ACE_RE_THROW;

      ++this->retry_count_;

      if (this->updates_dispatch_observer_ != 0)
        {
          /// Restore the lists.
          added.insert_seq (added_copy);
          removed.insert_seq (removed_copy);

          ACE_GUARD (TAO_NS_Reverse_Lock, ace_mon, unlock);

          this->updates_dispatch_observer_->dispatch_failure (this, this->retry_count_ ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_ENDTRY;
}
