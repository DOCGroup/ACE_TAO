/**
 * @file IIOP_Current_Impl.h
 *
 * @brief Provide implementation for the IIOPTraits interface
 *
 * $Id$
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

        /// Constructor.
        IIOP_Current_Impl (TAO_ORB_Core* core, size_t tss_slot_id);

        //@{
        virtual ::CORBA::Long id (void);

        virtual ::SSLIOP::Current_ptr ssliop_current (void);

        virtual ::CORBA::Long remote_port (void);

        virtual char* remote_host (void);

        virtual ::CORBA::Long local_port (void);

        virtual char* local_host (void);
        //@}

      protected:
        /// Destructor
        /**
         * Protected destructor to enforce the fact this class is reference
         * counted, and should not be destroyed using delete() by anything
         * other than the reference counting mechanism.
         */
        virtual ~IIOP_Current_Impl (void);

      private:

        /// Returns the IIOP connection handler associated with the
        /// Transport. Will throw NoContext if the (selected) transport
        /// () == 0, or if transport->connection_handler () == 0. Will
        /// throw NoContext, if no transport has been selected yet.
        TAO_IIOP_Connection_Handler* handler (void);

      private:

        /// Prevent copying through the copy constructor and
        /// assignment operator.
        //@{
        IIOP_Current_Impl (const IIOP_Current_Impl &);
        void operator= (const IIOP_Current_Impl &);
        //@}

      };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

#include /**/ "ace/post.h"

#endif /* IIOP_CURRENT_IMPL_H */
