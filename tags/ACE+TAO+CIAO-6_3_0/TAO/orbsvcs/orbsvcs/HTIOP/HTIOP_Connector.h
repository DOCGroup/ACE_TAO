// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTIOP_Connector.h
 *
 *  $Id$
 *
 *  HTIOP specific connector processing
 *
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef HTIOP_CONNECTOR_H
#define HTIOP_CONNECTOR_H
#include /**/ "ace/pre.h"

#include "tao/Connector_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/HTIOP/HTIOP_Connection_Handler.h"

#include "tao/Transport_Connector.h"

#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
namespace ACE
{
  namespace HTBP
  {
    class Environment;
  }
}
ACE_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {
    class Endpoint;

    /**
     * @class HTIOP_Connector
     *
     * @brief HTIOP-specific Connector bridge for pluggable protocols.
     *
     * Concrete instance of the TAO_Connector class.  Responsible
     * for establishing a connection with a server and is called from the
     * Connector_Registory.
     */
    class HTIOP_Export Connector : public TAO_Connector
    {
    public:
      typedef TAO_Connect_Concurrency_Strategy<Connection_Handler>
      CONNECT_CONCURRENCY_STRATEGY;

      typedef TAO_Connect_Creation_Strategy<Connection_Handler>
      CONNECT_CREATION_STRATEGY;

      /// Initialization and termination methods.
      /// Constructor.
      /// {@
      Connector (ACE::HTBP::Environment *ht_env);

      /// Destructor.
      ~Connector (void);

      /// @}


      /// The TAO_Connector methods, please check the documentation on
      /// Transport_Connector.h
      /// {@
      int open (TAO_ORB_Core *orb_core);
      int close (void);

      TAO_Profile *create_profile (TAO_InputCDR& cdr);

      virtual int check_prefix (const char *endpoint);

      virtual char object_key_delimiter (void) const;
      /// @}

    protected:

      // = The TAO_Connector methods, please check the documentation on
      // Transport_Connector.h
      int set_validate_endpoint (TAO_Endpoint *ep);

      TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                      TAO_Transport_Descriptor_Interface &desc,
                                      ACE_Time_Value *timeout = 0);


      /// More TAO_Connector methods, please check the documentation on
      /// Transport_Connector.h
      virtual TAO_Profile *make_profile (void);

      /// Cancel the passed cvs handler from the connector
      /// Its not clear what it means to cancel in HTIOP, since there's no
      /// base connector.
      virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);

    private:

      /// Return the remote endpoint, a helper function
      Endpoint *remote_endpoint (TAO_Endpoint *ep);

      /// Our creation strategy
      CONNECT_CREATION_STRATEGY *connect_creation_strategy_;

      /// Our activation strategy
      CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy_;

      /// reference to the configuration parameters
      ACE::HTBP::Environment *ht_env_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* HTIOP_CONNECTOR_H */
