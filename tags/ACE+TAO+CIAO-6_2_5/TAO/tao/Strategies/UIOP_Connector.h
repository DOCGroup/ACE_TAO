// -*- C++ -*-

//=============================================================================
/**
 *  @file    UIOP_Connector.h
 *
 *  $Id$
 *
 *  UIOP specific connector processing
 *
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_UIOP_CONNECTOR_H
#define TAO_UIOP_CONNECTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "ace/LSOCK_Connector.h"
#include "ace/Connector.h"
#include "tao/Transport_Connector.h"
#include "tao/Strategies/UIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"
#include "tao/Connector_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_UIOP_Endpoint;
class TAO_Endpoint;

/**
 * @class TAO_UIOP_Connector
 *
 * @brief UIOP-specific Connector bridge for pluggable protocols.
 *
 */
class TAO_Strategies_Export TAO_UIOP_Connector : public TAO_Connector
{
public:

  /**
   * Constructor.
   * @@ Do we want to pass in the tag here or should it be statically
   * defined?
   */
  TAO_UIOP_Connector (void);

  /// Destructor
  ~TAO_UIOP_Connector (void);

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

  virtual TAO_Profile *corbaloc_scan (const char *str, size_t &len
                                     );

  virtual char object_key_delimiter (void) const;

  /// Cancel the passed cvs handler from the connector
  virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);
  //@}

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>
          TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>
          TAO_UIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_UIOP_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>
          TAO_UIOP_CONNECT_STRATEGY;

  typedef ACE_Strategy_Connector<TAO_UIOP_Connection_Handler,
                                 ACE_LSOCK_CONNECTOR>
          TAO_UIOP_BASE_CONNECTOR;

protected:

  /**
   * @name More TAO_Connector methods
   *
   * Please check the documentation in Transport_Connector.h.
   */
  //@{
  int set_validate_endpoint (TAO_Endpoint *endpoint);

  TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  virtual TAO_Profile *make_profile (void);

  //@}

private:

  /// Return the remote endpoint, a helper function
  TAO_UIOP_Endpoint *remote_endpoint (TAO_Endpoint *ep);

private:

  /// Our connect strategy
  TAO_UIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for UIOP.
  TAO_UIOP_BASE_CONNECTOR base_connector_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

# endif  /* TAO_HAS_UIOP == 1 */

#include /**/ "ace/post.h"

#endif  /* TAO_UIOP_CONNECTOR_H */
