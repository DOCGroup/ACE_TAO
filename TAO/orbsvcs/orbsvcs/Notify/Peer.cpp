// $Id$

#include "Peer.h"

#if ! defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Peer, "$id$")

#include "tao/debug.h"
#include "Dispatch_Observer_T.h"

TAO_NS_Peer::TAO_NS_Peer (void)
{
}

TAO_NS_Peer::~TAO_NS_Peer ()
{
}

void
TAO_NS_Peer::type_added (const TAO_NS_EventType& added)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->added_.insert (added);
  this->removed_.remove (added);
}

void
TAO_NS_Peer::type_removed (const TAO_NS_EventType& removed)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->removed_.insert (removed);
  this->added_.remove (removed);
}

void
TAO_NS_Peer::dispatch_pending (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->added_.size () == 0 && this->removed_.size () == 0)
    return; // Return if nothing to send.

  TAO_NS_EventTypeSeq added_copy;
  TAO_NS_EventTypeSeq removed_copy;
  TAO_NS_Reverse_Lock unlock (this->lock_);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  added_copy = this->added_;
  removed_copy = this->removed_;
  this->added_.reset ();
  this->removed_.reset ();

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
          this->added_.insert_seq (added_copy);
          this->removed_.insert_seq (removed_copy);

          ACE_GUARD (TAO_NS_Reverse_Lock, ace_mon, unlock);

          this->updates_dispatch_observer_->dispatch_failure (this, this->retry_count_ ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_ENDTRY;
}
