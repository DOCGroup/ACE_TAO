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
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "ace/LSOCK_Connector.h"
#include "ace/Connector.h"
#include "UIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"
#include "tao/Connector_Impl.h"


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
  TAO_UIOP_Connector (CORBA::Boolean flag = 0);

  /// Destructor
  ~TAO_UIOP_Connector (void);

  /**
   * @name The TAO_Connector Methods
   *
   * Please check the documentation in Pluggable.h.
   */
  //@{
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_GIOP_Invocation *invocation,
               TAO_Transport_Descriptor_Interface *desc
               ACE_ENV_ARG_DECL);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;
  //@}

protected:

  /**
   * @name More TAO_Connector methods
   *
   * Please check the documentation in Pluggable.h.
   */
  //@{
  virtual TAO_Profile *make_profile (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtains uiop properties that must be used by this connector, i.e.,
  /// initializes <uiop_properties_>.
  int init_uiop_properties (void);
  //@}

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>
          TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>
          TAO_UIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_UIOP_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>
          TAO_UIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_UIOP_Connection_Handler,
                                 ACE_LSOCK_CONNECTOR>
          TAO_UIOP_BASE_CONNECTOR;

private:

  /// Our connect strategy
  TAO_UIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for UIOP.
  TAO_UIOP_BASE_CONNECTOR base_connector_;

  /// Do we need to use a GIOP_Lite for sending messages?
  CORBA::Boolean lite_flag_;

  /// UIOP configuration properties for all connections established by
  /// this connector.
  TAO_UIOP_Properties uiop_properties_;
};

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_CONNECTOR_H */
