// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Client_Strategy_Factory.h
 *
 *  $Id$
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CLIENT_STRATEGY_FACTORY_H
#define TAO_CLIENT_STRATEGY_FACTORY_H
#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Export.h"

class ACE_Lock;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;
class TAO_Transport;
class TAO_ORB_Core;
class TAO_Connect_Strategy;

/**
 * @class TAO_Client_Strategy_Factory
 *
 * @brief Abstract factory used by the client to manufacture various
 * strategies used on the client-side.
 */
class TAO_Export TAO_Client_Strategy_Factory : public ACE_Service_Object
{
public:
  // = Intialization and termination methods.
  /// Constructor.
  TAO_Client_Strategy_Factory (void);

  /// Destructor
  virtual ~TAO_Client_Strategy_Factory (void);

  /**
   * Create the lock for the forwarding Profile used by the
   * <TAO_GIOP_Invocation::location_forward> and the
   * <TAO_GIOP_Invocation::start>.
   */
  virtual ACE_Lock* create_profile_lock (void);

  /// Create the correct client request muxing strategy.
  virtual TAO_Transport_Mux_Strategy *create_transport_mux_strategy (TAO_Transport *transport);

  /// Create the correct lock for request muxing strategy.
  virtual ACE_Lock *create_transport_mux_strategy_lock (void);

  /// Return the size of the reply dispatcher table
  virtual int reply_dispatcher_table_size (void) const;

  /// Create the correct client <wait_for_reply> strategy.
  virtual TAO_Wait_Strategy *create_wait_strategy (TAO_Transport *transport);

  /// Create the correct client <asynch_connect> strategy.
  virtual TAO_Connect_Strategy *create_connect_strategy (TAO_ORB_Core *);

  /// Does the client allow any form of callback?
  virtual int allow_callback (void);

  /// Create a lock to be used by the TAO_Fault_Tolerant_Service class
  /// to generate unique retention ids
  virtual ACE_Lock *create_ft_service_retention_id_lock (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_CLIENT_STRATEGY_FACTORY_H */
