// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Reply_Dispatcher.h
//
// = DESCRIPTION
//     Dispatch the reply appropriately.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_REPLY_DISPATCHER_H
#include "ace/pre.h"
#define TAO_REPLY_DISPATCHER_H

#include "tao/Request.h"

#include "tao/GIOP_Message_State.h"
// This should not be here. Please see below for reasons

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MessagingC.h"

// Forward Declarations.

class TAO_Stub;
class TAO_InputCDR;


class TAO_Export TAO_Reply_Dispatcher
{
  // = TITLE
  //
  // = DESCRIPTION
  //

public:
  TAO_Reply_Dispatcher (void);
  // Constructor.

  virtual ~TAO_Reply_Dispatcher (void);
  // Destructor.

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state) = 0;
  // Dispatch the reply. Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void);
  // Get the Message State into which the reply has been read.

  virtual int leader_follower_condition_variable (TAO_Transport *);
  // Obtain the condition variable used in the Leader Follower Wait
  // Strategy. This is valid only for the synchronous reply dispatcher
  // and only when the Leader Follower wait strategy is used.
};

// *********************************************************************

class TAO_Export TAO_Synch_Reply_Dispatcher : public TAO_Reply_Dispatcher
{
  // = TITLE
  //
  //     Reply dispatcher for Synchoronous Method Invocation (SMI)s.
  //
  // = DESCRIPTION
  //

public:
  TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core,
                              IOP::ServiceContextList &sc);
  // Constructor.

  virtual ~TAO_Synch_Reply_Dispatcher (void);
  // Destructor.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  const TAO_GIOP_Version& version (void) const;
  // Get the GIOP version.

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply. Copy the buffers and return. Since the
  // invocation is synchronous demarshalling will take place on the
  // stack.
  // Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void);
  // Return the message state of this invocation.

  virtual TAO_InputCDR &reply_cdr (void);
  // Return the reply CDR.

  virtual int &reply_received (void);
  // Return the reference to the reply received flag. This will not
  // make sense in the Asynch Reply Dispatcher case, since the
  // reply will be dispatched as soon as it is available and the
  // dispatcher will go away immediately after that.

  virtual int leader_follower_condition_variable (TAO_Transport *);
  // Obtain the condition variable used in the Leader Follower Wait
  // Strategy.

protected:
  IOP::ServiceContextList &reply_service_info_;
  // The service context list

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  // TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_Message_State message_state_;
  // All the state required to receive the input...
  // @@ Having members of type TAO_GIOP* indicates that we
  // (Reply_despatcher) are aware of the underlying messaging
  // protocol. But for the present let us close our eyes till we are
  // able to iterate on a use case - Bala.
  
  int reply_received_;
  // Flag that indicates the reply  has been received.

  ACE_SYNCH_CONDITION *leader_follower_condition_variable_;
  // Condition variable used by the leader to notify the follower
  // about the availability of the response.

  TAO_ORB_Core *orb_core_;
  // Cache the ORB Core pointer.
};

// *********************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

#  if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

class TAO_Export TAO_Asynch_Reply_Dispatcher : public TAO_Reply_Dispatcher
{
  // = TITLE
  //
  //     Reply dispatcher for Asynchoronous Method Invocation (AMI)s.
  //
  // = DESCRIPTION
  //

public:
  TAO_Asynch_Reply_Dispatcher (const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                               Messaging::ReplyHandler_ptr reply_handler_ptr);
 // Constructor.

  virtual ~TAO_Asynch_Reply_Dispatcher (void);
  // Destructor.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  const TAO_GIOP_Version& version (void) const;
  // Get the GIOP version

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply. This involves demarshalling the reply and
  // calling the appropriate call back hook method on the reply
  // handler.
  // Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void);
  // Return the message state.

  void transport (TAO_Transport *t);
  // Sets the transport for this invocation.

protected:
  IOP::ServiceContextList reply_service_info_;
  // The service context list
  // Note, that this is not a reference as in
  // the synchronous case. We own the reply_service_info
  // because our TAO_Asynch_Invocation will go out
  // of scope before we are done.

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  //  TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.
  // @@ Carlos : message_state should go away. All we need is the reply
  //    cdr. Is that right? (Alex).

  const TAO_Reply_Handler_Skeleton reply_handler_skel_;
  // Skeleton for the call back method in the Reply Handler.

  Messaging::ReplyHandler_ptr reply_handler_;
  // Reply Handler passed in the Asynchronous Invocation.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...
};

#  endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

// *********************************************************************

#if !defined (TAO_HAS_MINIMUM_CORBA)

class TAO_Export TAO_DII_Deferred_Reply_Dispatcher : public TAO_Reply_Dispatcher
{
  // = TITLE
  //
  //     Reply dispatcher for DII deferred requests.
  //
  // = DESCRIPTION
  //

public:
  TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req);
 // Constructor.

  virtual ~TAO_DII_Deferred_Reply_Dispatcher (void);
  // Destructor.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  const TAO_GIOP_Version& version (void) const;
  // Get the GIOP version

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply. This involves demarshalling the reply and
  // calling the appropriate call back hook method on the reply
  // handler.
  // Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void);
  // Return the message state.

  void transport (TAO_Transport *t);
  // Sets the transport for this invocation.

protected:
  IOP::ServiceContextList reply_service_info_;
  // The service context list
  // Note, that this is not a reference as in
  // the synchronous case. We own the reply_service_info
  // because our TAO_Asynch_Invocation will go out
  // of scope before we are done.

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  //TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.
  // @@ Carlos : message_state should go away. All we need is the reply
  //    cdr. Is that right? (Alex).

  const CORBA::Request_ptr req_;
  // Where the reply needs to go.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...
};

#endif /* TAO_HAS_MINIMUM_CORBA */

// *********************************************************************

#if defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
