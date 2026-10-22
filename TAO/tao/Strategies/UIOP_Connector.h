// -*- C++ -*-

//=============================================================================
/**
 *  @file    UIOP_Connector.h
 *
 *  UIOP specific connector processing
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
 */
class TAO_Strategies_Export TAO_UIOP_Connector : public TAO_Connector
{
public:
  /**
   * Constructor.
   * @@ Do we want to pass in the tag here or should it be statically
   * defined?
   */
  TAO_UIOP_Connector ();

  /// Destructor
  ~TAO_UIOP_Connector () override;

  /**
   * @name The TAO_Connector Methods
   *
   * Please check the documentation in Transport_Connector.h for details.
   */
  //@{
  int open (TAO_ORB_Core *orb_core) override;
  int close () override;

  TAO_Profile *create_profile (TAO_InputCDR &cdr) override;

  int check_prefix (char const *endpoint) override;

  TAO_Profile *corbaloc_scan (char const *str, size_t &len) override;

  char object_key_delimiter () const override;

  /// Cancel the passed cvs handler from the connector
  int cancel_svc_handler (TAO_Connection_Handler *svc_handler) override;
  //@}

public:
  using TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY =
    TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>;

  using TAO_UIOP_CONNECT_CREATION_STRATEGY =
    TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>;

  using TAO_UIOP_CONNECT_STRATEGY =
    ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;

  using TAO_UIOP_BASE_CONNECTOR =
    ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;

protected:
  /**
   * @name More TAO_Connector methods
   *
   * Please check the documentation in Transport_Connector.h.
   */
  //@{
  int set_validate_endpoint (TAO_Endpoint *endpoint) override;

  TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = nullptr) override;

  TAO_Profile *make_profile () override;

  //@}

private:
  TAO_UIOP_Connector (TAO_UIOP_Connector const &) = delete;
  TAO_UIOP_Connector &operator= (TAO_UIOP_Connector const &) = delete;
  TAO_UIOP_Connector (TAO_UIOP_Connector &&) = delete;
  TAO_UIOP_Connector &operator= (TAO_UIOP_Connector &&) = delete;

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
