// $Id$

#include "Peer.h"

#if ! defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Peer, "$Id$")

#include "tao/debug.h"
#include "Proxy.h"

TAO_Notify_Peer::TAO_Notify_Peer (void)
{
}

TAO_Notify_Peer::~TAO_Notify_Peer ()
{
}

void
TAO_Notify_Peer::qos_changed (const TAO_Notify_QoSProperties& /*qos_properties*/)
{
  // NOP.
}

void
TAO_Notify_Peer::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // NOP.
}

void
TAO_Notify_Peer::handle_dispatch_exception (ACE_ENV_SINGLE_ARG_DECL)
{
  // Sever all association when a remote client misbehaves. Other strategies like reties are possible but not implemented.
  this->proxy ()->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Peer::dispatch_updates (const TAO_Notify_EventTypeSeq & added, const TAO_Notify_EventTypeSeq & removed ACE_ENV_ARG_DECL)
{
  TAO_Notify_EventTypeSeq subscribed_types ;
  this->proxy ()->subscribed_types (subscribed_types ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      CosNotification::EventTypeSeq cos_added;
      CosNotification::EventTypeSeq cos_removed;

      const TAO_Notify_EventType& special = TAO_Notify_EventType::special ();

      // Don;t inform of types that we already know about.
      // E.g. if we're subscribed for {A,B,C,F}
      // and we receive an update with added list {A,B,G}
      // then, we should only send {G} because peer already knows about {A, B}
      // However if we're subscribed for everything, send all kinds of adds.

      // Don;t inform of removed types that we don;t care about.
      // e.g. if we're currently subscribed for {A,B,C,F}
      // and we receive an update with removed list {A, B, D}
      // then, we should only send {A,B} because the peer is not interested in D.
      // However if we're subscribed for everything, send all kinds of removes.

      TAO_Notify_EventTypeSeq added_result = added;
      TAO_Notify_EventTypeSeq removed_result;

      if (subscribed_types.find (special) != 0)
        {
          added_result.remove_seq (subscribed_types);
          removed_result.intersection (subscribed_types, removed);
        }
      else
        {
          removed_result = removed;
        }

      added_result.populate_no_special (cos_added);
      removed_result.populate_no_special (cos_removed);

      if (cos_added.length () != 0 || cos_removed.length () != 0)
        {
          TAO_Notify_Proxy_Guard proxy_guard(this->proxy ()); // Protect this object from being destroyed in this scope.

          this->dispatch_updates_i (cos_added, cos_removed ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_exist)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::NO_IMPLEMENT, no_impl)
    {
      // The peer does not implement the offer/subscription_change method
      // Do nothing. Later, perhaps set a flag that helps us decide if we should dispatch_updates_i.
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      this->handle_dispatch_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Do nothing
    }
  ACE_ENDTRY;
}
