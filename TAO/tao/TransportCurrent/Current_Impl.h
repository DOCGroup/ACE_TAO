#ifndef CURRENT_IMPL_H
#define CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "tao/TransportCurrent/TCC.h"
#include "tao/LocalObject.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

namespace TAO
{
  namespace Transport
  {
    // Forward decl
    class Stats;

    /**
     * @class Current_impl
     *
     * @brief Implementation of the TAO::Transport::Current
     * interface.
     *
     * Current_Impl is useful for obtaining information about the
     * Transport, associated with the calling thread.
     */
    class TAO_Transport_Current_Export Current_Impl
      : public virtual Current
      , public virtual ::CORBA::LocalObject
    {
    public:
      Current_Impl (TAO_ORB_Core *core, size_t tss_slot_id);

      Current_Impl (Current_Impl const &) = delete;
      Current_Impl (Current_Impl &&) = delete;
      Current_Impl &operator= (Current_Impl const &) = delete;
      Current_Impl &operator= (Current_Impl &&) = delete;

      /// Methods exposed by the PortableInterceptor::Current interface.
      CORBA::Long id () override;

      CounterT bytes_sent () override;

      CounterT bytes_received () override;

      CounterT messages_sent () override;

      CounterT messages_received () override;

      ::TimeBase::TimeT open_since () override;

    protected:
      /// A (strategy) method used to obtain the transport pointer.
      TAO_Transport const *transport () const;

      /// A Stats instance. If protocol is unavailable (0) or the
      /// TAO_HAS_TRANSPORT_CURRENT macro is defined as anything but
      /// 1, a single static instance will be used.
      TAO::Transport::Stats const *transport_stats () const;

      /// Protected because instances are reference counted.
      ~Current_Impl () override = default;

    private:
      // Borrowed from the ORB that owns this Current object.
      TAO_ORB_Core * const core_;

      // The ORB's TSS slot id for the Transport pointer.
      size_t const tss_slot_id_;
    };
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */


#include /**/ "ace/post.h"

#endif /* CURRENT_IMPL_H */
