// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Asynch_Reply_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ASYNCH_REPLY_DISPATCHER_H
#define TAO_ASYNCH_REPLY_DISPATCHER_H
#include /**/ "ace/pre.h"

#include "tao/Messaging/Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Messaging/Asynch_Timeout_Handler.h"
#include "tao/Asynch_Reply_Dispatcher_Base.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Reply dispatcher for Asynchoronous Method Invocation (AMI)s.
class TAO_Asynch_Reply_Dispatcher
  : public TAO_Asynch_Reply_Dispatcher_Base
{
public:
  /// Constructor.
  TAO_Asynch_Reply_Dispatcher (
      const TAO_Reply_Handler_Stub &reply_handler_stub,
      Messaging::ReplyHandler_ptr reply_handler_ptr,
      TAO_ORB_Core *orb_core,
      ACE_Allocator *allocator);

  /// Destructor.
  virtual ~TAO_Asynch_Reply_Dispatcher (void);

  /// @name The Reply Dispatcher methods
  //@{
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  virtual void connection_closed (void);
  //@}

  /// Inform that the reply timed out
  virtual void reply_timed_out (void);

  /// Install the timeout handler
  long schedule_timer (CORBA::ULong request_id,
                       const ACE_Time_Value &max_wait_time);

private:
  /// Stub for the call back method in the Reply Handler.
  TAO_Reply_Handler_Stub const reply_handler_stub_;

  /// Reply Handler passed in the Asynchronous Invocation.
  Messaging::ReplyHandler_var reply_handler_;

  /// Timeout Handler in case of AMI timeouts
  TAO_Asynch_Timeout_Handler *timeout_handler_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ASYNCH_REPLY_DISPATCHER_H */
