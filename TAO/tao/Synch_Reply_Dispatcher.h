// This may look like C, but it's really -*- C++ -*-


//=============================================================================
/**
 *  @file    Synch_Reply_Dispatcher.h
 *
 *  $Id$
 *
 *   Dispatch the reply appropriately.
 *
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SYNCH_REPLY_DISPATCHER_H
#define TAO_SYNCH_REPLY_DISPATCHER_H
#include "ace/pre.h"

#include "tao/Reply_Dispatcher.h"
#include "tao/GIOP_Message_Version.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Wait_Strategy;
class TAO_Pluggable_Reply_Params;

/**
 * @class TAO_Synch_Reply_Dispatcher
 *
 * @brief Reply dispatcher for Synchoronous Method Invocation (SMI)s.
 *
 */
class TAO_Export TAO_Synch_Reply_Dispatcher : public TAO_Reply_Dispatcher
{

public:
  /// Constructor.
  TAO_Synch_Reply_Dispatcher (TAO_ORB_Core *orb_core,
                              IOP::ServiceContextList &sc);

  /// Destructor.
  virtual ~TAO_Synch_Reply_Dispatcher (void);

  /// Return the reply CDR.
  TAO_InputCDR &reply_cdr (void);

  /// A flag to check if the reply
  int &reply_received (void);

  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  // Commented for the time being - Bala
  // virtual TAO_GIOP_Message_State *message_state (void);

  virtual void dispatcher_bound (TAO_Transport *);

  virtual void connection_closed (void);

protected:
  /// The service context list
  IOP::ServiceContextList &reply_service_info_;

private:
  //  TAO_GIOP_Message_State message_state_;
  // All the state required to receive the input...
  // @@ Having members of type TAO_GIOP* indicates that we
  // (Reply_despatcher) are aware of the underlying messaging
  // protocol. But for the present let us close our eyes till we are
  // able to iterate on a use case - Bala.

  /// Flag that indicates the reply  has been received.
  int reply_received_;

  /// Cache the ORB Core pointer.
  TAO_ORB_Core *orb_core_;

  /// Save the wait strategy to signal the waiting threads (if
  /// appropriate).
  TAO_Wait_Strategy *wait_strategy_;

  /// CDR stream which has the reply information that needs to be
  /// demarshalled by the stubs
  TAO_InputCDR reply_cdr_;

  /**
   * The condition variable used to signal the waiting thread in the
   * Leader/Followers model.  The variable is acquired in the thread
   * that binds the Reply_Dispatcher to its transport, and then passed
   * to the Waiting_Strategy to do the signalling, if needed.
   */
  TAO_SYNCH_CONDITION *leader_follower_condition_variable_;
};

#include "ace/post.h"
#endif /* TAO_REPLY_DISPATCHER_H */
