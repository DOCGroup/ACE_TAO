// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_On_Leader_Follower.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_WAIT_ON_LEADER_FOLLOWER_H
#define TAO_WAIT_ON_LEADER_FOLLOWER_H
#include /**/ "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Wait_On_Leader_Follower
 *
 * @brief Wait according to the Leader-Follower model. Leader does the
 * event loop of the Reactor and the Followers wait on the
 * condition variable.
 *
 * This impelementation is to work with the Muxed Transport
 * Mechanism. Here the state variables such as <Condition
 * Variable> etc cannot be kept in the Wait Strategy, since the
 * Wait Strategy is per Transport object and here the Transport
 * is Muxed and hence there are multiple threads running in the
 * same Transport context.
 */
class TAO_Export TAO_Wait_On_Leader_Follower : public TAO_Wait_Strategy
{

public:
  /// Constructor.
  TAO_Wait_On_Leader_Follower (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Wait_On_Leader_Follower (void);

  // = Documented in TAO_Wait_Strategy.
  virtual int sending_request (TAO_ORB_Core *orb_core,
                               int two_way);
  virtual int wait (ACE_Time_Value *max_wait_time,
                    TAO_Synch_Reply_Dispatcher &rd);
  virtual int register_handler (void);
  virtual int non_blocking (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_WAIT_ON_LEADER_FOLLOWER_H */
