// -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_SSL_Connector.h
 *
 *  $Id$
 *
 *  IIOP specific connector processing -- SSL aware version
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_CONNECTOR_H
#define TAO_IIOP_SSL_CONNECTOR_H

#include /**/ "ace/pre.h"

#include "IIOP_SSL_Connection_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Connection_Handler.h"
#include "tao/IIOP_Connector.h"

namespace TAO
{
  /**
   * @class IIOP_SSL_Connector
   *
   * @brief IIOP-specific Connector (SSL aware) bridge for pluggable
   *        protocols.
   *
   * Concrete instance of the TAO_Connector class.  Responsible
   * for establishing a connection with a server and is called from the
   * Connector_Registry.
   */
  class IIOP_SSL_Connector : public TAO_IIOP_Connector
  {
  public:

    /// Constructor.
    IIOP_SSL_Connector (CORBA::Boolean flag = 0);

    /// Destructor.
    ~IIOP_SSL_Connector (void);

    int open (TAO_ORB_Core *orb_core);
    int close (void);

  public:

    typedef TAO_Connect_Concurrency_Strategy<IIOP_SSL_Connection_Handler>
            CONNECT_CONCURRENCY_STRATEGY;

    typedef TAO_Connect_Creation_Strategy<IIOP_SSL_Connection_Handler>
            CONNECT_CREATION_STRATEGY;

    typedef ACE_Connect_Strategy<IIOP_SSL_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
            CONNECT_STRATEGY ;

    typedef ACE_Strategy_Connector<IIOP_SSL_Connection_Handler,
                                   ACE_SOCK_CONNECTOR>
            BASE_CONNECTOR;

  protected:

    /**
     * @name The TAO_Connector Methods
     *
     * Check the documentation in tao/Transport_Connector.h for details.
     */
    //@{
    int set_validate_endpoint (TAO_Endpoint *ep);

    TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                    TAO_Transport_Descriptor_Interface &desc,
                                    ACE_Time_Value *timeout = 0);

  virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);
    //@}

  private:

    /// Our connect strategy
    CONNECT_STRATEGY connect_strategy_;

    /// The connector initiating connection requests for IIOP_SSL.
    BASE_CONNECTOR base_connector_;

    /// State that will be passed to each SSLIOP connection handler upon
    /// creation.
    TAO::SSLIOP::Connection_Handler_State handler_state_;

  };

} // End TAO namespace.

#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECTOR_H */
