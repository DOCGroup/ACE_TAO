// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = DESCRIPTION
//     Define the interface for the Reply_Dispatcher strategies.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_REPLY_DISPATCHER_H
#define TAO_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward Declarations.
class TAO_GIOP_Message_State;
class TAO_GIOP_Version;
class TAO_Transport;

class TAO_Export TAO_Reply_Dispatcher
{
  // = TITLE
  //
  // = DESCRIPTION
  //   Different invocation modes process the Reply messages in
  //   different ways.  Traditional synchronous replies simply receive
  //   the message and wake up the waiting thread (if any).
  //   Asynchronous Method Invocation (Callbacks) must process the
  //   message in the thread that receives it.
  //   Deferred Synchronous (DII) and AMI in the Poller mode save the
  //   reply for later processing in the application.
  //   The lower level components in the ORB only deal with this
  //   abstract interface, when the invocation is made the right type
  //   of Reply Dispatcher is instantiated and registered with the
  //   Transport object.
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
  // @@ TODO Pluggable Messaging:  this method has too many arguments,
  //    the "Right Thing"[tm] is for the Transport Object to create a
  //    "ClientReply" that encapsulates all we need to process a
  //    reply.  Naturally it is possible that different messaging
  //    protocols implement different variants of such ClientReply
  //    class.

  CORBA::ULong reply_status (void) const;
  // Get the reply status.

  virtual TAO_GIOP_Message_State *message_state (void) = 0;
  // Get the Message State into which the reply has been read.

  virtual void dispatcher_bound (TAO_Transport*) = 0;
  // The dispatcher has been bound.
  // Some dispatchers need to retain state to cooperate with other
  // components, such as the waiting strategy.

  virtual void connection_closed (void) = 0;
  // The used for the pending reply has been closed.
  // No reply is expected.
  // @@ TODO: If the connection was closed due to a CloseConnection
  //    message then we could re-issue the request instead of raising
  //    the exception, it would a matter of simply adding a boolean
  //    argument to this function.

protected:
  CORBA::ULong reply_status_;
  // Reply or LocateReply status.
};

#if defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
