// This may look like C, but it's really -*- C++ -*-
//=============================================================================
/**
 *  @file Asynch_Reply_Dispatcher_Base.h
 *
 *  $Id$
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ASYNCH_REPLY_DISPATCHER_BASE_H
#define TAO_ASYNCH_REPLY_DISPATCHER_BASE_H
#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"
#include "tao/Reply_Dispatcher.h"

class TAO_Pluggable_Reply_Params;
class TAO_ORB_Core ;

/// Base class for TAO_Asynch_Reply_Dispatcher and
/// TAO_DII_Deferred_Reply_Dispatcher
class TAO_Export TAO_Asynch_Reply_Dispatcher_Base : public TAO_Reply_Dispatcher
{
public:
  /// Default constructor.
  TAO_Asynch_Reply_Dispatcher_Base (TAO_ORB_Core *orb_core);

  /// Destructor.
  virtual ~TAO_Asynch_Reply_Dispatcher_Base (void);

  /// Sets the transport for this invocation.
  void transport (TAO_Transport *t);

  // = The Reply Dispatcher methods
  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  // virtual TAO_GIOP_Message_State *message_state (void);

  virtual void connection_closed (void);

    /// Inform that the reply timed out
  virtual void reply_timed_out (void);

  /// Install the timeout handler
  virtual long schedule_timer (CORBA::ULong request_id,
                               const ACE_Time_Value &max_wait_time);

protected:
  /// The service context list.
  /**
   * Note, that this is not a reference as in
   * the synchronous case. We own the reply_service_info
   * because our TAO_Asynch_Invocation or TAO_DII_Deferred_Invocation
   * will go out of scope before we are done.
   */
  IOP::ServiceContextList reply_service_info_;

  /// The buffer that is used to initialise the data block
  char buf_[ACE_CDR::DEFAULT_BUFSIZE];

  /// datablock that is created on the stack to initialise the CDR
  /// stream underneath.
  ACE_Data_Block db_;

  /// CDR stream which has the reply information that needs to be
  /// demarshalled by the stubs
  TAO_InputCDR reply_cdr_;

  /// This invocation is using this transport, may change...
  TAO_Transport *transport_;
};

#if defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher_Base.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ASYNCH_REPLY_DISPATCHER_BASE_H */
