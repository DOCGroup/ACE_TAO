// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_Accept_Strategy.h
 *
 *  $Id$
 *
 *  IIOP/SSL specific accept strategy
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_ACCEPT_STRATEGY_H
#define TAO_SSLIOP_ACCEPT_STRATEGY_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"

#include "tao/Acceptor_Impl.h"

#include "ace/SSL/SSL_SOCK_Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @class Accept_Strategy
     *
     * @brief SSLIOP-specific accept strategy that builds on the
     *        TAO_Accept_Strategy implementation.
     *
     * This accept strategy builds on on the TAO_Accept_Strategy
     * implementation.  It sub-classes that class, and overrides the
     * accept_svc_handler() method so that a timeout value may be
     * passed to the underlying peer acceptor.  This is necessary to
     * defend against a simple Denial-of-Service attack.
     * @par
     * Since SSL requires two handshakes, one TCP and one SSL, it is
     * possible for a malicious client to establish a TCP connection
     * to the SSL port, and never complete the SSL handshake.  The
     * underlying SSL passive connection code would block/hang waiting
     * for the SSL handshake to complete.  Given enough incomplete
     * connections where only the TCP handshake is completed, a server
     * process could potentially run out of available file
     * descriptors, thus preventing legitimate client connections from
     * being established.
     * @par.
     * The timeout defense alluded to above bounds the time this sort of
     * DoS attack lasts.
     */
    class Accept_Strategy
      : public TAO_Accept_Strategy<TAO::SSLIOP::Connection_Handler,
                                   ACE_SSL_SOCK_Acceptor>
    {
    public:

      /// Constructor.
      Accept_Strategy (TAO_ORB_Core * orb_core,
                       const ACE_Time_Value & timeout);

      /// Overridden method that forces a passive connection timeout value
      /// to be passed to the underlying acceptor.
      virtual int accept_svc_handler (handler_type * svc_handler);

    private:

      /// The accept() timeout.
      /**
       * This timeout includes the overall time to complete the SSL
       * handshake.  This includes both the TCP handshake and the SSL
       * handshake.
       */
      const ACE_Time_Value timeout_;
    };

  }  // End SSLIOP namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_ACCEPT_STRATEGY_H */
