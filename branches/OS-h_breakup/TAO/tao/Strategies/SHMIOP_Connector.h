// -*- C++ -*-

//=============================================================================
/**
 *  @file    SHMIOP_Connector.h
 *
 *  $Id$
 *
 *  SHMIOP specific connector processing
 *
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SHMIOP_CONNECTOR_H
#define TAO_SHMIOP_CONNECTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "ace/MEM_Connector.h"
#include "ace/Connector.h"
#include "tao/Pluggable.h"
#include "SHMIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"
#include "tao/Connector_Impl.h"

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
  // = Initialization and termination methods.
  /// Constructor.
  TAO_SHMIOP_Connector (CORBA::Boolean flag = 0);

  /// Default destructor
  ~TAO_SHMIOP_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_GIOP_Invocation *invocation,
               TAO_Transport_Descriptor_Interface *desc
               ACE_ENV_ARG_DECL);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

protected:

  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual TAO_Profile *make_profile (ACE_ENV_SINGLE_ARG_DECL);

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler,
                               ACE_MEM_CONNECTOR>
          TAO_SHMIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler,
                                 ACE_MEM_CONNECTOR>
          TAO_SHMIOP_BASE_CONNECTOR;
private:

  /// local address
  ACE_MEM_Addr address_;

  /// Our connect strategy
  TAO_SHMIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for SHMIOP.
  TAO_SHMIOP_BASE_CONNECTOR base_connector_;

  /// Are we using GIOP lite??
  CORBA::Boolean lite_flag_;
};

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_SHMIOP_CONNECTOR_H */
