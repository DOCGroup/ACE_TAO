// This may look like C, but it's really -*- C++ -*-
// $Id$



// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Transport_Mux_Strategy.h
//
// = DESCRIPTION
//     IIOP Transport Mux Strategies.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_TRANSPORT_MUX_STRATEGY_H
#include "ace/pre.h"
#define TAO_TRANSPORT_MUX_STRATEGY_H

// BALA should not be here But for the time being
#include "tao/GIOP_Message_State.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"

class TAO_Reply_Dispatcher;

class TAO_Export TAO_Transport_Mux_Strategy
{
  // = TITLE
  //
  //     Strategy to determine whether the connection should be
  //     multiplexed for multiple requests or it is exclusive for a
  //     single request at a time.
  //
  // = DESCRIPTION
  //

public:
  TAO_Transport_Mux_Strategy (TAO_Transport *transport);
  // Base class constructor.

  virtual ~TAO_Transport_Mux_Strategy (void);
  // Base class destructor.

  virtual CORBA::ULong request_id (void)  = 0;
  // Generate and return an unique request id for the current
  // invocation.

  // = Bind and Find methods for the <Request ID, ReplyDispatcher>
  //   pairs.

  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rd);
  // Bind the dispatcher with the request id. Commonalities in the
  // derived class implementations is kept here.

  virtual int dispatch_reply (CORBA::ULong request_id,
                              CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state) = 0;
  // Dispatch the reply for <request_id>, cleanup any resources
  // allocated for that request.

  // = "Factory methods" to obtain the CDR stream, in the Muxed case
  //    the factory simply allocates a new one, in the Exclusive case
  //    the factory returns a pointer to the pre-allocated CDR.

  virtual TAO_GIOP_Message_State *get_message_state (void) = 0;
  // Get a CDR stream.

  virtual void destroy_message_state (TAO_GIOP_Message_State *) = 0;
  // Destroy a CDR stream.

  virtual int idle_after_send (void) = 0;
  // Request has been just sent, but the reply is not received. Idle
  // the transport now.

  virtual int idle_after_reply (void) = 0;
  // Request is sent and the reply is received. Idle the transport
  // now.

  //  virtual int reply_received (const CORBA::ULong request_id) = 0;
  // Check whether the reply has been receieved for the request with
  // <request_id>. Return 0 if no, 1 on yes and -1 if the request_id
  // is invalid or there are some errors.

protected:
  TAO_Transport *transport_;
  // Cache the transport reference.
};

// *********************************************************************

class TAO_Export TAO_Exclusive_TMS : public TAO_Transport_Mux_Strategy
{
  // = TITLE
  //
  //    Connection exclusive for the request.
  //
  // = DESCRIPTION
  //

public:
  TAO_Exclusive_TMS (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Exclusive_TMS (void);
  // Destructor.

  virtual CORBA::ULong request_id (void);
  // Generate and return an unique request id for the current
  // invocation.

  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rh);
  // Bind the dispatcher with the request id.

  virtual int dispatch_reply (CORBA::ULong request_id,
                              CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply for <request_id>, cleanup any resources
  // allocated for that request.

  virtual TAO_GIOP_Message_State *get_message_state (void);
  // Return the pre-allocated message state.

  virtual void destroy_message_state (TAO_GIOP_Message_State *);
  // No op in this strategy.

  virtual int idle_after_send (void);
  // Request has been just sent, but the reply is not received. Idle
  // the transport now.

  virtual int idle_after_reply (void);
  // Request is sent and the reply is received. Idle the transport
  // now.

  // virtual int reply_received (const CORBA::ULong request_id);
  // Check whether the reply has been receieved for the request with
  // <request_id>. Return 0 if no, 1 on yes and -1 if the request_id
  // is invalid or there are errors.

protected:
  CORBA::ULong request_id_generator_;
  // Used to generate a different request_id on each call to
  // request_id().

  CORBA::ULong request_id_;
  // Request id for the current request.

  TAO_Reply_Dispatcher *rd_;
  // Reply Dispatcher corresponding to the request.

  TAO_GIOP_Message_State message_state_;
  // Message state to read the incoming message.
  
  // @@ Having members of type TAO_GIOP* indicates that we
  // (Transport_Mux_Strategy) are aware of the underlying messaging
  // protocol. But for the present let us close our eyes till we are
  // able to iterate on a use case - Bala.
};

// *********************************************************************

class TAO_Export TAO_Muxed_TMS : public  TAO_Transport_Mux_Strategy
{
  // = TITLE
  //
  //    Connection is multiplexed for many requests.
  //
  // = DESCRIPTION
  //

public:
  TAO_Muxed_TMS (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Muxed_TMS (void);
  // Destructor.

  virtual CORBA::ULong request_id (void);
  // Generate and return an unique request id for the current
  // invocation.

  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rh);
  // Bind the dispatcher with the request id.

  virtual int dispatch_reply (CORBA::ULong request_id,
                              CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply for <request_id>, cleanup any resources
  // allocated for that request.

  virtual TAO_GIOP_Message_State *get_message_state (void);
  // Return the message state.

  virtual void destroy_message_state (TAO_GIOP_Message_State *);
  // No op in this strategy.

  virtual int idle_after_send (void);
  // Request has been just sent, but the reply is not received. Idle
  // the transport now.

  virtual int idle_after_reply (void);
  // Request is sent and the reply is received. Idle the transport
  // now.

  //  virtual int reply_received (const CORBA::ULong request_id);
  // Check whether the reply has been receieved for the request with
  // <request_id>. Return 0 if no, 1 on yes and -1 if the request_id
  // is invalid or there are errors.

protected:
  CORBA::ULong request_id_generator_;
  // Used to generate a different request_id on each call to
  // request_id().

  ACE_SYNCH_MUTEX request_id_lock_;
  // Lock to protect the state of the request id generator.

  typedef ACE_Hash_Map_Manager_Ex <CORBA::ULong,
                                   TAO_Reply_Dispatcher *,
                                   ACE_Hash <CORBA::ULong>,
                                   ACE_Equal_To <CORBA::ULong>,
                                   ACE_SYNCH_MUTEX> REQUEST_DISPATCHER_TABLE;

  REQUEST_DISPATCHER_TABLE dispatcher_table_;
  // Table of <Request ID, Reply Dispatcher> pairs.

  TAO_ORB_Core *orb_core_;
  // Keep track of the orb core pointer. We need to this to create the
  // Reply Dispatchers.

  TAO_GIOP_Message_State *message_state_;
  // Message state where the current input message is being read. This
  // is created at start of each incoming message. When that message
  // is read, the message is processed and for the next message a new
  // message state is created.

  // @@ Having members of type TAO_GIOP* indicates that we
  // (Transport_Mux_Strategy) are aware of the underlying messaging
  // protocol. But for the present let us close our eyes till we are
  // able to iterate on a use case - Bala.
};

// *********************************************************************

#include "ace/post.h"
#endif /* TRANSPORT_MUX_STRATEGY_H */
