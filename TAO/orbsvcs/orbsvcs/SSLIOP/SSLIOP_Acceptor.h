// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_Acceptor.h
 *
 *  $Id$
 *
 *  IIOP/SSL specific acceptor processing
 *
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_ACCEPTOR_H
#define TAO_SSLIOP_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/IIOP_SSL_Acceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"
#include "orbsvcs/SSLIOP/SSLIOP_Accept_Strategy.h"

#include "orbsvcs/SSLIOPC.h"  /* CSIv1 */
#include "orbsvcs/CSIIOPC.h"  /* CSIv2 */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @class Acceptor
     *
     * @brief The SSLIOP-specific bridge class for the concrete acceptor.
     */
    class Acceptor
      : public IIOP_SSL_Acceptor
    {
    public:

      /// Constructor.
      Acceptor (::Security::QOP qop,
                const ACE_Time_Value & timeout);

      /// Destructor.
      ~Acceptor (void);

      typedef TAO_Strategy_Acceptor<Connection_Handler, ACE_SSL_SOCK_Acceptor> BASE_ACCEPTOR;
      typedef TAO_Creation_Strategy<Connection_Handler> CREATION_STRATEGY;
      typedef TAO_Concurrency_Strategy<Connection_Handler> CONCURRENCY_STRATEGY;
      typedef Accept_Strategy ACCEPT_STRATEGY;

      /**
       * @name The TAO_Acceptor Methods
       *
       * Check the documentation in tao/Pluggable.h for details.
       */
      //@{
      virtual int open (TAO_ORB_Core *orb_core,
                        ACE_Reactor *reactor,
                        int version_major,
                        int version_minor,
                        const char *address,
                        const char *options = 0);
      virtual int open_default (TAO_ORB_Core *orb_core,
                                ACE_Reactor *reactor,
                                int version_major,
                                int version_minor,
                                const char *options = 0);
      virtual int close (void);
      virtual int create_profile (const TAO::ObjectKey &object_key,
                                  TAO_MProfile &mprofile,
                                  CORBA::Short priority);
      virtual int is_collocated (const TAO_Endpoint* endpoint);
      //@}

      /// Retrieve the CSIv1 SSLIOP::SSL component associated with the
      /// endpoints set up by this acceptor.
      const ::SSLIOP::SSL &ssl_component (void) const;

    private:

      /// Implement the common part of the open*() methods.
      int ssliop_open_i (TAO_ORB_Core *orb_core,
                         const ACE_INET_Addr& addr,
                         ACE_Reactor *reactor);

      /// Parse protocol specific options.
      virtual int parse_options_i (int &argc, ACE_CString ** argv);

      /// Ensure that neither the endpoint configuration nor the ORB
      /// configuration violate security measures.
      int verify_secure_configuration (TAO_ORB_Core *orb_core,
                                       int major,
                                       int minor);

      /// Helper method to add a new profile to the mprofile for
      /// each endpoint.
      int create_new_profile (const TAO::ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority);

      /// Helper method to create a profile that contains all of
      /// our endpoints.
      int create_shared_profile (const TAO::ObjectKey &object_key,
                                 TAO_MProfile &mprofile,
                                 CORBA::Short priority);

    private:

      /// The concrete acceptor, as a pointer to it's base class.
      BASE_ACCEPTOR ssl_acceptor_;

      /**
       * @name Acceptor Strategies
       *
       * Strategies used when accepting an incoming connection.
       */
      CREATION_STRATEGY *creation_strategy_;
      CONCURRENCY_STRATEGY *concurrency_strategy_;
      ACCEPT_STRATEGY *accept_strategy_;

      /// The CSIv1 SSL component.
      /**
       * This is the SSLIOP endpoint-specific tagged component that is
       * embedded in a given IOR.
       */
      ::SSLIOP::SSL ssl_component_;

      /// The SSLIOP CSIv2 tagged component.
      /**
       *
       */
      CSIIOP::TLS_SEC_TRANS csiv2_component_;

      /// The accept() timeout.
      /**
       * This timeout includes the overall time to complete the SSL
       * handshake.  This includes both the TCP handshake and the SSL
       * handshake.
       */
      ACE_Time_Value const timeout_;

    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_ACCEPTOR_H */
