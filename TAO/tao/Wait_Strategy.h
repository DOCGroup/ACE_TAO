// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_Strategy.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_WAIT_STRATEGY_H
#define TAO_WAIT_STRATEGY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;
class TAO_Transport;

/**
 * @class TAO_Wait_Strategy
 *
 * @brief Strategy for waiting for the reply.
 *
 */
class TAO_Export TAO_Wait_Strategy
{

public:
  /// Constructor.
  TAO_Wait_Strategy (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Wait_Strategy (void);

  /**
   * The user is going to send a request, prepare any internal
   * variables because the reply may arrive *before* the user calls
   * wait.
   */
  virtual int sending_request (TAO_ORB_Core *orb_core,
                               int two_way);

  /// Base class virtual method. Wait till the <reply_received> flag is
  /// true or the time expires.
  virtual int wait (ACE_Time_Value *max_wait_time,
                    int &reply_received) = 0;

  /// Register the handler needs with the reactor provided that it makes
  /// sense for the  strategy.
  virtual int register_handler (void) = 0;

  /// Returns a value to indicate whether the transport needs to set
  /// the socket on which it is waiting to non-blocking mode or not.
  virtual int non_blocking (void) = 0;

  /**
   * Return the TSS leader follower condition variable used in the
   * Wait Strategy. Muxed Leader Follower implementation returns a
   * valid condition variable, others return 0.
   * The condition variable is acquired by the Reply_Dispatcher (when
   * needed) in the thread that binds it to the right Transport.
   * Later (when the reply is finally received) the Reply_Dispatcher
   * passes this condition variable back to Waiting_Strategy, that can
   * then signal the waiting thread if needed.
   */
  virtual ACE_SYNCH_CONDITION *leader_follower_condition_variable (void);

  /**
   * This is the callback used by the Reply_Dispatcher to inform the
   * Waiting_Strategy that a reply has been completely received, that
   * it was already stored in the right place, and that the condition
   * variable should be signalled if needed.
   * The Waiting_Strategy must set the reply received flag, using
   * whatever locks it needs.
   */
  virtual int reply_dispatched (int &reply_received_flag,
                                ACE_SYNCH_CONDITION *);

  /**
   * The connection has been closed by the lower level components in
   * the ORB.
   * The wait has finished and must result in an error.
   */
  virtual void connection_closed (int &reply_received_flag,
                                  ACE_SYNCH_CONDITION*);

protected:
  /// Transport object.
  TAO_Transport *transport_;
};

#include "ace/post.h"
#endif /* TAO_WAIT_STRATEGY_H */
