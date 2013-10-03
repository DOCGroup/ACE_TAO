/* -*- C++ -*- */

/**
 * @file Current_Impl.cpp
 *
 * $Id$
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 *
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

    /// ctor
    Current_Impl::Current_Impl (TAO_ORB_Core* core, size_t tss_slot_id)
      : core_ (core)
      , tss_slot_id_ (tss_slot_id)
    {
    }

    /// dtor
    Current_Impl::~Current_Impl (void)
    {
    }

    /// Obtains the current transport. Throws a NoContext exception
    /// if, there was no "current" transport selected on the current
    /// thread.
    const TAO_Transport*
    Current_Impl::transport (void) const
    {
      Transport_Selection_Guard* topguard =
        Transport_Selection_Guard::current (this->core_, this->tss_slot_id_);

      if (topguard == 0)
        throw NoContext();

      return topguard->get ();
    }

    static const TAO::Transport::Stats dummy_transport_stats;

    /// Obtains the current transport's stats
    const TAO::Transport::Stats*
    Current_Impl::transport_stats (void) const
    {
      static const TAO::Transport::Stats dummy;

      const TAO_Transport* t = this->transport ();

      return (t==0 || t->stats () == 0) ? &dummy_transport_stats : t->stats ();
    }

    CORBA::Long Current_Impl::id (void)
    {
      const TAO_Transport* t = this->transport ();

      return (t==0) ? 0 : t->id ();
    }

    CounterT Current_Impl::bytes_sent (void)
    {
      return transport_stats ()->bytes_sent ();
    }

    CounterT Current_Impl::bytes_received (void)
    {
      return transport_stats ()->bytes_received ();
    }

    CounterT Current_Impl::messages_sent (void)
    {
      return transport_stats ()->messages_sent ();
    }

    CounterT Current_Impl::messages_received (void)
    {
      return transport_stats ()->messages_received ();
    }

    TimeBase::TimeT Current_Impl::open_since (void)
    {
      TimeBase::TimeT msecs = 0;
      transport_stats ()->opened_since ().msec (msecs);
      return msecs;
    }

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

