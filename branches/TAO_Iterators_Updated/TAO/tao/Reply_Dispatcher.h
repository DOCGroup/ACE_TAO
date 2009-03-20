// -*- C++ -*-

//=============================================================================
/**
 *  @file     Reply_Dispatcher.h
 *
 *  $Id$
 *
 *   Define the interface for the Reply_Dispatcher strategies.
 *
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_REPLY_DISPATCHER_H
#define TAO_REPLY_DISPATCHER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/GIOPC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward Declarations.
class TAO_Pluggable_Reply_Params;

/**
 * @class TAO_Reply_Dispatcher
 *
 * Different invocation modes process the Reply messages in
 * different ways.  Traditional synchronous replies simply receive
 * the message and wake up the waiting thread (if any).
 * Asynchronous Method Invocation (Callbacks) must process the
 * message in the thread that receives it.
 * Deferred Synchronous (DII) and AMI in the Poller mode save the
 * reply for later processing in the application.
 * The lower level components in the ORB only deal with this
 * abstract interface, when the invocation is made the right type
 * of Reply Dispatcher is instantiated and registered with the
 * Transport object.
 */
class TAO_Export TAO_Reply_Dispatcher
{

public:
  /// Constructor.
  TAO_Reply_Dispatcher (void);

  /// Destructor.
  virtual ~TAO_Reply_Dispatcher (void);

  /**
   * Dispatch the reply. Return 1 on sucess, -1 on error.
   * @todo Pluggable Messaging:  this method has too many arguments,
   *    the "Right Thing"[tm] is for the Transport Object to create a
   *    "ClientReply" that encapsulates all we need to process a
   *    reply.  Naturally it is possible that different messaging
   *    protocols implement different variants of such ClientReply
   *    class.
   */
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params) = 0;

  /// Inform that the reply timed out
  virtual void reply_timed_out (void) = 0;

  /**
   * The used for the pending reply has been closed.
   * No reply is expected.
   * @todo If the connection was closed due to a CloseConnection
   *    message then we could re-issue the request instead of raising
   *    the exception, it would a matter of simply adding a boolean
   *    argument to this function.
   */
  virtual void connection_closed (void) = 0;

  /// Get the locate reply status.
  GIOP::LocateStatusType locate_reply_status (void) const;

  GIOP::ReplyStatusType reply_status (void) const;

protected:
  /// LocateReply status.
  GIOP::LocateStatusType locate_reply_status_;

  // RequestReply status
  GIOP::ReplyStatusType reply_status_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_REPLY_DISPATCHER_H */
