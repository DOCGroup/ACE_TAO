/**
 * @file IIOP_Current_Impl.h
 *
 * @brief Provide implementation for the IIOPTraits interface
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */

#ifndef IIOP_CURRENT_IMPL_H
#define IIOP_CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "tao/TransportCurrent/TC_IIOPC.h"
#include "tao/TransportCurrent/Current_Impl.h"

#include <cstddef>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;
class TAO_IIOP_Connection_Handler;

namespace TAO
{
  namespace Transport
  {
    /**
     * @class IIOP_Current_Impl
     *
     * @brief Implementation of the TAO::Transport::IIOPCurrent
     * interface.
     *
     * IIOP_Current_Impl is useful for obtaining information about the
     * IIOP Transport, associated with the calling thread.
     */
    class TAO_Transport_Current_Export IIOP_Current_Impl
      : public virtual IIOP::Current
      , public virtual Current_Impl
    {
    public:
      IIOP_Current_Impl (TAO_ORB_Core *core, std::size_t tss_slot_id);

      IIOP_Current_Impl (IIOP_Current_Impl const &) = delete;
      IIOP_Current_Impl (IIOP_Current_Impl &&) = delete;
      IIOP_Current_Impl &operator= (IIOP_Current_Impl const &) = delete;
      IIOP_Current_Impl &operator= (IIOP_Current_Impl &&) = delete;

      //@{
      ::CORBA::Long id () override;

      ::SSLIOP::Current_ptr ssliop_current () override;

      ::CORBA::Long remote_port () override;

      char* remote_host () override;

      ::CORBA::Long local_port () override;

      char* local_host () override;
      //@}

    protected:
      /// Protected because instances are reference counted.
      ~IIOP_Current_Impl () override = default;

    private:
      /// Borrowed from the active transport; throws NoContext if unavailable.
      TAO_IIOP_Connection_Handler *handler () const;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

#include /**/ "ace/post.h"

#endif /* IIOP_CURRENT_IMPL_H */
