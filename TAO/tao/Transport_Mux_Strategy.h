// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Transport_Mux_Strategy.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TRANSPORT_MUX_STRATEGY_H
#define TAO_TRANSPORT_MUX_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Reply_Dispatcher;
class TAO_Transport;
class TAO_Pluggable_Reply_Params;

/**
 * @class TAO_Transport_Mux_Strategy
 *
 * @brief Strategy to determine whether the connection should be
 * multiplexed for multiple requests or it is exclusive for a
 * single request at a time.
 *
 */
class TAO_Export TAO_Transport_Mux_Strategy
{

public:
  /// Base class constructor.
  TAO_Transport_Mux_Strategy (TAO_Transport *transport);

  /// Base class destructor.
  virtual ~TAO_Transport_Mux_Strategy (void);

  /// Generate and return an unique request id for the current
  /// invocation.
  virtual CORBA::ULong request_id (void)  = 0;

  // = Bind and Find methods for the <Request ID, ReplyDispatcher>
  //   pairs.

  /// Bind the dispatcher with the request id. Commonalities in the
  /// derived class implementations is kept here.
  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rd) = 0;

  /**
   * Unbind the dispatcher, the client is no longer waiting for the
   * request, for example, because the request timedout.
   * The strategy can (must) cleanup any resources associated with the
   * request.
   * A later reply for that request should be ignored.
   */
  virtual int unbind_dispatcher (CORBA::ULong request_id) = 0;

  /// Dispatch the reply for <request_id>, cleanup any resources
  /// allocated for that request.
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params) = 0;

  /// Request has been just sent, but the reply is not received. Idle
  /// the transport now.
  virtual int idle_after_send (void) = 0;

  /// Request is sent and the reply is received. Idle the transport
  /// now.
  virtual int idle_after_reply (void) = 0;

  /// The transport object has closed the connection, inform all Reply
  /// dispatchers and waiting strategies.
  virtual void connection_closed (void) = 0;

protected:
  /// Cache the transport reference.
  TAO_Transport *transport_;

  /// Lock to protect the state of the object
  ACE_Lock *lock_;
};

#include /**/ "ace/post.h"
#endif /* TRANSPORT_MUX_STRATEGY_H */
