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
        /// Constructor.
        Current_Impl (TAO_ORB_Core* core, size_t tss_slot_id);

        /**
         * These are methods exposed by the
         * PortableInterceptor::Current interface.
         */
        //@{
        virtual CORBA::Long id ();

        virtual CounterT bytes_sent ();

        virtual CounterT bytes_received ();

        virtual CounterT messages_sent ();

        virtual CounterT messages_received ();

        virtual ::TimeBase::TimeT open_since ();
        //@}

      protected:
        /**
         * Some helper methods
         */
        //@{
        /// A (strategy) method used to obtain the transport ptr
        const TAO_Transport* transport () const;

        /// A Stats instance. If protocol is unavailable (0) or the
        /// TAO_HAS_TRANSPORT_CURRENT macro is defined as anything but
        /// 1, a single static instance will be used.
        const TAO::Transport::Stats* transport_stats () const;
        //@}

        /// Destructor is protected to enforce the fact this class is
        /// reference counted, and should not be destroyed using
        /// delete() by anything other than the reference counting
        /// mechanism.
        virtual ~Current_Impl (void);

      private:
        Current_Impl (const Current_Impl &) = delete;
        void operator= (const Current_Impl &) = delete;

      private:
        // The ORB (core) that owes us.
        TAO_ORB_Core* core_;

        // The ORB's TSS slot id for the Transport ptr
        size_t tss_slot_id_;
      };

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */


#include /**/ "ace/post.h"

#endif /* CURRENT_IMPL_H */
