// -*- C++ -*-

//=============================================================================
/**
 *  @file    SHMIOP_Connector.h
 *
 *  $Id$
 *
 *  SHMIOP specific connector processing
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SHMIOP_CONNECTOR_H
#define TAO_SHMIOP_CONNECTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "ace/MEM_Connector.h"
#include "ace/Connector.h"
#include "tao/Transport_Connector.h"
#include "tao/Strategies/SHMIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"
#include "tao/Connector_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_SHMIOP_Endpoint;

// ****************************************************************

/**
 * @class TAO_SHMIOP_Connector
 *
 * @brief SHMIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_Strategies_Export TAO_SHMIOP_Connector : public TAO_Connector
{
public:
  /// Constructor.
  TAO_SHMIOP_Connector (void);

  /// Default destructor
  ~TAO_SHMIOP_Connector (void);

  /**
   * @name The TAO_Connector Methods
   *
   * Please check the documentation in Transport_Connector.h for details.
   */
  //@{
  int open (TAO_ORB_Core *orb_core);
  int close (void);

  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;
  //@}

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler,
                               ACE_MEM_CONNECTOR>
          TAO_SHMIOP_CONNECT_STRATEGY;

  typedef ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler,
                                 ACE_MEM_CONNECTOR>
          TAO_SHMIOP_BASE_CONNECTOR;

protected:
  /**
   * @name More TAO_Connector Methods
   *
   * Please check the documentation in Transport_Connector.h for details.
   */
  //@{
  int set_validate_endpoint (TAO_Endpoint *endpoint);

  TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  virtual TAO_Profile *make_profile (void);

  /// Cancel the passed cvs handler from the connector
  int cancel_svc_handler (TAO_Connection_Handler * svc_handler);
  //@}

private:

  /// Return the remote endpoint, a helper function
  TAO_SHMIOP_Endpoint *remote_endpoint (TAO_Endpoint *ep);

private:

  /// Local address.
  ACE_MEM_Addr address_;

  /// Our connect strategy.
  TAO_SHMIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for SHMIOP.
  TAO_SHMIOP_BASE_CONNECTOR base_connector_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_SHMIOP_CONNECTOR_H */
