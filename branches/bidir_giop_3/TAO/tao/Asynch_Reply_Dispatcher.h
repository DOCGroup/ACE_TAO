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
//     Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ASYNCH_REPLY_DISPATCHER_H
#define TAO_ASYNCH_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Reply_Dispatcher.h"

class TAO_Pluggable_Reply_Params;
class TAO_ORB_Core ;



#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)
#include "tao/MessagingC.h"
#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */

class TAO_Export TAO_Asynch_Reply_Dispatcher_Base
  : public TAO_Reply_Dispatcher
{
  // = TITLE
  //    TAO_Asynch_Reply_Dispatcher_Base
  //
  // = DESCRIPTION
  //    Base class for TAO_Asynch_Reply_Dispatcher and
  //    TAO_DII_Deferred_Reply_Dispatcher.
public:
  TAO_Asynch_Reply_Dispatcher_Base (TAO_ORB_Core *orb_core);
  // Default constructor.

  virtual ~TAO_Asynch_Reply_Dispatcher_Base (void);
  // Destructor.

  void transport (TAO_Transport *t);
  // Sets the transport for this invocation.

  // = The Reply Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  // virtual TAO_GIOP_Message_State *message_state (void);

  virtual void dispatcher_bound (TAO_Transport *t);

  virtual void connection_closed (void);

protected:
  IOP::ServiceContextList reply_service_info_;
  // The service context list.
  // Note, that this is not a reference as in
  // the synchronous case. We own the reply_service_info
  // because our TAO_Asynch_Invocation or TAO_DII_Deferred_Invocation
  // will go out of scope before we are done.

  // TAO_GIOP_Message_State *message_state_;
  // CDR stream for reading the input.

  TAO_InputCDR reply_cdr_;
  // CDR stream which has the reply information that needs to be
  // demarshalled by the stubs

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...
};

// *********************************************************************

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

class TAO_Export TAO_Asynch_Reply_Dispatcher
  : public TAO_Asynch_Reply_Dispatcher_Base
{
  // = TITLE
  //    TAO_Asynch_Reply_Dispatcher
  //
  // = DESCRIPTION
  //    Reply dispatcher for Asynchoronous Method Invocation (AMI)s.
public:
  TAO_Asynch_Reply_Dispatcher (
      const TAO_Reply_Handler_Skeleton &reply_handler_skel,
      Messaging::ReplyHandler_ptr reply_handler_ptr,
      TAO_ORB_Core *orb_core
    );
 // Constructor.

  virtual ~TAO_Asynch_Reply_Dispatcher (void);
  // Destructor.

  // = The Reply Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  virtual void connection_closed (void);

private:
  const TAO_Reply_Handler_Skeleton reply_handler_skel_;
  // Skeleton for the call back method in the Reply Handler.

  Messaging::ReplyHandler_var reply_handler_;
  // Reply Handler passed in the Asynchronous Invocation.
};

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

#if defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ASYNCH_REPLY_DISPATCHER_H */
