// $Id$

#ifndef CURRENT_IMPL_H
#define CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
      , public virtual TAO_Local_RefCounted_Object
      {
      public:

        /// Constructor.
        Current_Impl (TAO_ORB_Core* core, size_t tss_slot_id);

        /**
         * These are methods exposed by the
         * PortableInterceptor::Current interface.
         */
        //@{
        virtual CORBA::Long id (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));

        virtual CounterT bytes_sent (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));

        virtual CounterT bytes_received (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));

        virtual CounterT messages_sent (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));

        virtual CounterT messages_received (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));

        virtual ::TimeBase::TimeT open_since (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((::CORBA::SystemException, NoContext));
        //@}

      protected:

        /**
         * Some helper methods
         */
        //@{
        /// A (strategy) method used to obtain the transport ptr
        const TAO_Transport* transport (ACE_ENV_SINGLE_ARG_DECL) const
          ACE_THROW_SPEC ((NoContext));

        /// A Stats instance. If protocol is unavailable (0) or the
        /// TAO_HAS_TRANSPORT_CURRENT macro is defined as anything but
        /// 1, a single static instance will be used.
        const TAO::Transport::Stats* transport_stats (ACE_ENV_SINGLE_ARG_DECL) const
          ACE_THROW_SPEC ((NoContext));
        //@}

        /// Destructor is protected to enforce the fact this class is
        /// reference counted, and should not be destroyed using
        /// delete() by anything other than the reference counting
        /// mechanism.
        virtual ~Current_Impl (void);

      private:

        /// Prevent copying through the copy constructor and the
        /// assignment operator.
        //@{
        Current_Impl (const Current_Impl &);
        void operator= (const Current_Impl &);
        //@}

      private:

        // The ORB (core) that owes us.
        TAO_ORB_Core* core_;

        // The ORB's TSS slot id for the Transport ptr
        size_t tss_slot_id_;

      };

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CURRENT_IMPL_H */
