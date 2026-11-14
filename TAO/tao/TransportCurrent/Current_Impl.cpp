/* -*- C++ -*- */

/**
 * @file Current_Impl.cpp
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */

#include "tao/Transport.h"

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "tao/Transport_Selection_Guard.h"
#include "tao/TransportCurrent/Current_Loader.h"
#include "tao/TransportCurrent/Current_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {
    Current_Impl::Current_Impl (TAO_ORB_Core *core, size_t tss_slot_id)
      : core_ (core)
      , tss_slot_id_ (tss_slot_id)
    {
    }

    /// Obtains the current transport. Throws a NoContext exception
    /// if, there was no "current" transport selected on the current
    /// thread.
    TAO_Transport const *
    Current_Impl::transport () const
    {
      Transport_Selection_Guard * const topguard =
        Transport_Selection_Guard::current (this->core_, this->tss_slot_id_);

      if (topguard == nullptr)
        throw NoContext ();

      return topguard->get ();
    }

    static TAO::Transport::Stats const dummy_transport_stats;

    /// Obtains the current transport's stats
    TAO::Transport::Stats const *
    Current_Impl::transport_stats () const
    {
      TAO_Transport const * const t = this->transport ();

      TAO::Transport::Stats const * const stats =
        t == nullptr ? nullptr : t->stats ();
      return stats == nullptr ? &dummy_transport_stats : stats;
    }

    CORBA::Long Current_Impl::id ()
    {
      TAO_Transport const * const t = this->transport ();

      return t == nullptr ? CORBA::Long {} : static_cast<CORBA::Long> (t->id ());
    }

    CounterT Current_Impl::bytes_sent ()
    {
      return transport_stats ()->bytes_sent ();
    }

    CounterT Current_Impl::bytes_received ()
    {
      return transport_stats ()->bytes_received ();
    }

    CounterT Current_Impl::messages_sent ()
    {
      return transport_stats ()->messages_sent ();
    }

    CounterT Current_Impl::messages_received ()
    {
      return transport_stats ()->messages_received ();
    }

    TimeBase::TimeT Current_Impl::open_since ()
    {
      TimeBase::TimeT msecs {};
      transport_stats ()->opened_since ().msec (msecs);
      return msecs;
    }

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

