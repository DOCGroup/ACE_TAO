// $Id$

#include "ace/INET_Addr.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/IIOP_Transport.h"
#include "tao/Transport_Selection_Guard.h"

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "IIOP_Current_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


namespace TAO
{
  namespace Transport
  {


    /// Obtains the IIOP_Connection_Handler associated with the
    /// Transport. Will throw NO_IMPLEMENT if the (selected) transport
    /// () == 0, or if transport->connection_handler () == 0. Will
    /// throw NoContext, if no transport has been selected yet.

    TAO_IIOP_Connection_Handler*
    IIOP_Current_Impl::handler (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      const TAO_Transport* t = this->transport ();
      if (t == 0)
        ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);

      TAO_Connection_Handler *ch = const_cast<TAO_Transport*>(t)->connection_handler ();
      if (ch == 0)
        ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);

      return dynamic_cast <TAO_IIOP_Connection_Handler*> (ch);
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }


    /// Ctor

    IIOP_Current_Impl::IIOP_Current_Impl (TAO_ORB_Core* core, size_t tss_slot_id)
      : Current_Impl (core, tss_slot_id)
    {
    }


    /// Dtor

    IIOP_Current_Impl::~IIOP_Current_Impl (void)
    {
    }

    CORBA::Long
    IIOP_Current_Impl::id (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      // Need to use cast to pacify windows compilers complaining
      // about the implicit HANDLE -> CORBA::Long conversion.
      return (CORBA::Long) this->handler ()->get_handle ();
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }

    ::SSLIOP::Current_ptr
    IIOP_Current_Impl::ssliop_current (void)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
        ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
    }


    CORBA::Long
    IIOP_Current_Impl::remote_port (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_remote_addr (a);

      return a.get_port_number ();
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }

    char*
    IIOP_Current_Impl::remote_host (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_remote_addr (a);

      return CORBA::string_dup (a.get_host_addr ());
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }

    CORBA::Long
    IIOP_Current_Impl::local_port (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_local_addr (a);

      return a.get_port_number ();
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }

    char*
    IIOP_Current_Impl::local_host (void)
      ACE_THROW_SPEC ((::CORBA::SystemException, NoContext))
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_local_addr (a);

      return CORBA::string_dup (a.get_host_addr ());
#else
      ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
#endif
    }

  }
}


TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */
