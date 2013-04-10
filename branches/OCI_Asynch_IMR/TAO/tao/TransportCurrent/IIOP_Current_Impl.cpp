// $Id$

#include "ace/INET_Addr.h"
#include "ace/Truncate.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/IIOP_Transport.h"
#include "tao/Transport_Selection_Guard.h"
#include "tao/SystemException.h"

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "IIOP_Current_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


namespace TAO
{
  namespace Transport
  {


    /// Obtains the IIOP_Connection_Handler associated with the
    /// Transport. Will throw NoContext if the (selected) transport
    /// () == 0, or if transport->connection_handler () == 0. Will
    /// throw NoContext, if no transport has been selected yet.

    TAO_IIOP_Connection_Handler*
    IIOP_Current_Impl::handler (void)
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      const TAO_Transport* t = this->transport ();
      if (t == 0)
        throw NoContext ();

      TAO_Connection_Handler *ch = const_cast<TAO_Transport*>(t)->connection_handler ();
      if (ch == 0)
        throw NoContext ();

      // Make sure that this connection handler is for IIOP.  This
      // implementation is not intended to operate under SSLIOP.
      TAO_IIOP_Connection_Handler* iiop_ch =
        dynamic_cast <TAO_IIOP_Connection_Handler*> (ch);
      if (iiop_ch == 0)
        throw NoContext ();

      return iiop_ch;
#else
      throw ::CORBA::NO_IMPLEMENT ();
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
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      // Need to use cast to pacify windows compilers complaining
      // about the implicit HANDLE -> CORBA::Long conversion.
      return ACE_Utils::truncate_cast<CORBA::Long> ((intptr_t)this->handler ()->get_handle ());
#else
      throw ::CORBA::NO_IMPLEMENT ();
#endif
    }

    ::SSLIOP::Current_ptr
    IIOP_Current_Impl::ssliop_current (void)
    {
        throw ::CORBA::NO_IMPLEMENT ();
    }


    CORBA::Long
    IIOP_Current_Impl::remote_port (void)
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_remote_addr (a);

      return a.get_port_number ();
#else
      throw ::CORBA::NO_IMPLEMENT ();
#endif
    }

    char*
    IIOP_Current_Impl::remote_host (void)
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_remote_addr (a);

      return CORBA::string_dup (a.get_host_addr ());
#else
      throw ::CORBA::NO_IMPLEMENT ();
#endif
    }

    CORBA::Long
    IIOP_Current_Impl::local_port (void)
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_local_addr (a);

      return a.get_port_number ();
#else
      throw ::CORBA::NO_IMPLEMENT ();
#endif
    }

    char*
    IIOP_Current_Impl::local_host (void)
    {
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
      TAO_IIOP_Connection_Handler *iiopch =
        this->handler ();

      ACE_INET_Addr a;
      iiopch->peer ().get_local_addr (a);

      return CORBA::string_dup (a.get_host_addr ());
#else
      throw ::CORBA::NO_IMPLEMENT ();
#endif
    }

  }
}


TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */
