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
#define TAO_REPLY_DISPATCHER_H

#include "tao/GIOP.h"
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
                              TAO_GIOP_ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state) = 0;
  // Dispatch the reply. Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void) const;
  // Get the Message State into which the reply has been read.

  virtual void leader_follower_condition_variable (TAO_Transport *);
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
  TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core);
  // Constructor.

  virtual ~TAO_Synch_Reply_Dispatcher (void);
  // Destructor.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  const TAO_GIOP_Version& version (void) const;
  // Get the GIOP version.

  TAO_GIOP_ServiceContextList& reply_ctx (void);
  // Get the reply context

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              TAO_GIOP_ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply. Copy the buffers and return. Since the
  // invocation is synchronous demarshalling will take place on the
  // stack.
  // Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void) const;
  // Return the message state of this invocation.

  virtual TAO_InputCDR &reply_cdr (void);
  // Return the reply CDR.

  virtual int &reply_received (void);
  // Return the reference to the reply received flag. This will not
  // make sense in the Asynch Reply Dispatcher case, since the
  // reply will be dispatched as soon as it is available and the
  // dispatcher will go away immediately after that. 

  virtual void leader_follower_condition_variable (TAO_Transport *);
  // Obtain the condition variable used in the Leader Follower Wait
  // Strategy.

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_ServiceContextList reply_ctx_;
  // The service context list

  TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.
  // @@ Carlos : message_state should go away. All we need is the
  //    reply cdr. Is that rite? (Alex).

  TAO_InputCDR reply_cdr_;
  // CDR where the reply message is placed.

  int reply_received_;
  // Flag that indicates the reply  has been received.

  ACE_SYNCH_CONDITION *leader_follower_condition_variable_;
  // Condition variable used by the leader to notify the follower
  // about the availability of the response.
};

// *********************************************************************
#if defined (TAO_HAS_CORBA_MESSAGING) && defined (TAO_POLLER)
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

  TAO_GIOP_ServiceContextList& reply_ctx (void);
  // Get the reply context

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              TAO_GIOP_ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  // Dispatch the reply. This involves demarshalling the reply and
  // calling the appropriate call back hook method on the reply
  // handler.
  // Return 1 on sucess, -1 on error.

  virtual TAO_GIOP_Message_State *message_state (void) const;
  // Return the message state.

private:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.

  TAO_GIOP_Version version_;
  // The version

  TAO_GIOP_ServiceContextList reply_ctx_;
  // The service context list

  TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.
  // @@ Carlos : message_state should go away. All we need is the reply
  //    cdr. Is that rite? (Alex).

  const TAO_Reply_Handler_Skeleton reply_handler_skel_;
  // Skeleton for the call back method in the Reply Handler.

  Messaging::ReplyHandler_ptr reply_handler_;
  // Reply Handler passed in the Asynchronous Invocation.
};
#endif /* TAO_HAS_CORBA_MESSAGING && TAO_POLLER */
// *********************************************************************

#if defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_REPLY_DISPATCHER_H */
