// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = DESCRIPTION
//     Dispatch the reply appropriately.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ASYNCH_REPLY_DISPATCHER_H
#define TAO_ASYNCH_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// *********************************************************************

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

#include "tao/Reply_Dispatcher.h"
#include "tao/MessagingC.h"

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

  void transport (TAO_Transport *t);
  // Sets the transport for this invocation.

  // = The Reply Dispatcher methods
  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  virtual TAO_GIOP_Message_State *message_state (void);
  virtual void dispatcher_bound (TAO_Transport *t);
  virtual void connection_closed (void);

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

  const TAO_Reply_Handler_Skeleton reply_handler_skel_;
  // Skeleton for the call back method in the Reply Handler.

  Messaging::ReplyHandler_var reply_handler_;
  // Reply Handler passed in the Asynchronous Invocation.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...
};

#if defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

#include "ace/post.h"
#endif /* TAO_ASYNCH_REPLY_DISPATCHER_H */
