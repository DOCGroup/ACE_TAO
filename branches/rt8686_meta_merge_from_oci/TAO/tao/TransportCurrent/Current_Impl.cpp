// $Id:$

#include "tao/Transport.h"
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
    Current_Impl::transport (ACE_ENV_SINGLE_ARG_DECL) const
      ACE_THROW_SPEC ((NoContext))
    {
      Transport_Selection_Guard* topguard =
        Transport_Selection_Guard::current (this->core_, this->tss_slot_id_);

      if (topguard == 0)
        ACE_THROW (NoContext());
      ACE_CHECK;

      return topguard->get ();
    }

    /// Obtains the current transport's stats
    const TAO::Transport::Stats*
    Current_Impl::transport_stats (ACE_ENV_SINGLE_ARG_DECL) const
      ACE_THROW_SPEC ((NoContext))
    {
      static const TAO::Transport::Stats dummy;

      const TAO_Transport* t =
        this->transport (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return (t==0 || t->stats () == 0) ? &dummy : t->stats ();
    }

    CORBA::Long Current_Impl::id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      const TAO_Transport* t =
        this->transport (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return (t==0) ? 0 : t->id ();
    }

    CounterT Current_Impl::bytes_sent (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      return transport_stats ()->bytes_sent ();
    }

    CounterT Current_Impl::bytes_received (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      return transport_stats ()->bytes_received ();
    }

    CounterT Current_Impl::messages_sent (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      return transport_stats ()->messages_sent ();
    }

    CounterT Current_Impl::messages_received (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      return transport_stats ()->messages_received ();
    }

    TimeBase::TimeT Current_Impl::open_since (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
      TimeBase::TimeT msecs = 0;
      transport_stats ()->opened_since ().msec (msecs);
      return msecs;
    }

  };

};

TAO_END_VERSIONED_NAMESPACE_DECL

