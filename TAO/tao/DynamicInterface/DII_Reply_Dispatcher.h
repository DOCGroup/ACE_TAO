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

#ifndef TAO_DII_REPLY_DISPATCHER_H
#define TAO_DII_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dynamicinterface_export.h"
#include "tao/Reply_Dispatcher.h"

class TAO_DynamicInterface_Export TAO_DII_Deferred_Reply_Dispatcher
  : public TAO_Reply_Dispatcher
{
  // = TITLE
  //    TAO_DII_Deferred_Reply_Dispatcher
  //
  // = DESCRIPTION
  //    Reply dispatcher for DII deferred requests.
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

  void transport (TAO_Transport *t);
  // Sets the transport for this invocation.

  // = The Reply_Dispatcher methods
  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  virtual TAO_GIOP_Message_State *message_state (void);
  virtual void dispatcher_bound (TAO_Transport*);
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

  TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.
  // @@ Carlos : message_state should go away. All we need is the reply
  //    cdr. Is that right? (Alex).

  const CORBA::Request_ptr req_;
  // Where the reply needs to go.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...
};

#if defined (__ACE_INLINE__)
#  include "DII_Reply_Dispatcher.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
