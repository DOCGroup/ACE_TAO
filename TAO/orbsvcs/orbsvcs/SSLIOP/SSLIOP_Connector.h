// -*- C++ -*-

//=============================================================================
/**
 *  @file     SSLIOP_Connector.h
 *
 *  $Id$
 *
 *  SSLIOP specific connector processing
 *
 *  @author Carlos O'Ryan
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef TAO_SSLIOP_CONNECTOR_H
#define TAO_SSLIOP_CONNECTOR_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/IIOP_SSL_Connector.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"

#include "ace/SSL/SSL_SOCK_Connector.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations.
class TAO_Base_Transport_Property;

class TAO_SSLIOP_Endpoint;

namespace TAO
{
  namespace SSLIOP
  {
    class OwnCredentials;

    /**
     * @class Connector
     *
     * @brief SSLIOP-specific Connector bridge for pluggable protocols.
     *
     * Concrete instance of the TAO_Connector class.  Responsible
     * for establishing a connection with a server and is called from
     * the Connector_Registry.
     */
    class Connector : public TAO::IIOP_SSL_Connector
    {
    public:

      /// Constructor.
      Connector (::Security::QOP qop);

      /**
       * @name The TAO_Connector methods
       *
       * Please check the documentation in Transport_Connector.h
       */
      //@{
      virtual int open (TAO_ORB_Core *orb_core);
      virtual int close (void);
      virtual TAO_Transport *connect (TAO::Profile_Transport_Resolver *r,
                                      TAO_Transport_Descriptor_Interface *desc,
                                      ACE_Time_Value *timeout);

      virtual TAO_Profile *create_profile (TAO_InputCDR& cdr);
      virtual int check_prefix (const char *endpoint);
      virtual TAO_Profile * corbaloc_scan (const char *ior, size_t &len);
      //@}

    protected:

      /**
       * @name @c TAO_Connector Methods
       *
       * Methods required by the @c TAO_Connector base class.
       *
       * @see @c TAO_Connector
       */
      //@{
      virtual TAO_Profile * make_profile (void);
      virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);
      //@}

      /// SSL-specific profile
      TAO_Profile * make_secure_profile (void);

      /// IIOP-specific connection establishment.
      /**
       * @note The IIOP endpoint is extracted from the SSLIOP endpoint.
       */
      TAO_Transport* iiop_connect (TAO_SSLIOP_Endpoint *ssliop_endpoint,
                                   TAO::Profile_Transport_Resolver *r,
                                   ACE_Time_Value *timeout);

      /// SSLIOP-specific connection establishment.
      TAO_Transport* ssliop_connect (TAO_SSLIOP_Endpoint *ssliop_endpoint,
                                     ::Security::QOP qop,
                                     const ::Security::EstablishTrust &trust,
                                     TAO::Profile_Transport_Resolver *r,
                                     TAO_Transport_Descriptor_Interface *desc,
                                     ACE_Time_Value *timeout);

      /// Retrieve SSLIOP credentials from the policy overrides list
      /// and set up the underlying SSL connection to use the X.509
      /// certificates stored within them.
      TAO::SSLIOP::OwnCredentials * retrieve_credentials (TAO_Stub *stub,
                                                          SSL *ssl);

    public:

      typedef TAO_Connect_Concurrency_Strategy<Connection_Handler>
          CONNECT_CONCURRENCY_STRATEGY;

      typedef TAO_Connect_Creation_Strategy<Connection_Handler>
          CONNECT_CREATION_STRATEGY;

      typedef ACE_Connect_Strategy<Connection_Handler,
                                   ACE_SSL_SOCK_Connector>
          CONNECT_STRATEGY;

      typedef ACE_Strategy_Connector<Connection_Handler,
                                     ACE_SSL_SOCK_Connector>
          BASE_CONNECTOR;

    private:

      /// If zero, connect to IIOP over SSL port by default.
      /// Otherwise, connect to the insecure IIOP port.
      ::Security::QOP qop_;

      /// Our connect strategy
      CONNECT_STRATEGY connect_strategy_;

      /// The connector initiating connection requests for IIOP.
      BASE_CONNECTOR base_connector_;

    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CONNECTOR_H */
