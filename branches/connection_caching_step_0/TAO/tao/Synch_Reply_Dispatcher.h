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

#ifndef TAO_SYNCH_REPLY_DISPATCHER_H
#define TAO_SYNCH_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/Reply_Dispatcher.h"
#include "tao/GIOP_Message_State.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Wait_Strategy;

class TAO_Export TAO_Synch_Reply_Dispatcher : public TAO_Reply_Dispatcher
{
  // = TITLE
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

  TAO_InputCDR &reply_cdr (void);
  // Return the reply CDR.

  int &reply_received (void);
  // A flag to check if the reply

  virtual int dispatch_reply (CORBA::ULong reply_status,
                              const TAO_GIOP_Version& version,
                              IOP::ServiceContextList& reply_ctx,
                              TAO_GIOP_Message_State* message_state);
  virtual TAO_GIOP_Message_State *message_state (void);
  virtual void dispatcher_bound (TAO_Transport *);
  virtual void connection_closed (void);

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

  TAO_ORB_Core *orb_core_;
  // Cache the ORB Core pointer.

  TAO_Wait_Strategy *wait_strategy_;
  // Save the wait strategy to signal the waiting threads (if
  // appropriate).

  ACE_SYNCH_CONDITION *leader_follower_condition_variable_;
  // The condition variable used to signal the waiting thread in the
  // Leader/Followers model.  The variable is acquired in the thread
  // that binds the Reply_Dispatcher to its transport, and then passed
  // to the Waiting_Strategy to do the signalling, if needed.
};

#if defined (__ACE_INLINE__)
#include "tao/Synch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
