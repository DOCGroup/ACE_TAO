// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_WAIT_ON_LEADER_FOLLOWER_H
#define TAO_WAIT_ON_LEADER_FOLLOWER_H
#include "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Wait_On_Leader_Follower : public TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Wait according to the Leader-Follower model. Leader does the
  //    event loop of the Reactor and the Followers wait on the
  //    condition variable.
  //
  // = DESCRIPTION
  //
  //    This impelementation is to work with the Muxed Transport
  //    Mechanism. Here the state variables such as <Condition
  //    Variable> etc cannot be kept in the Wait Strategy, since the
  //    Wait Strategy is per Transport object and here the Transport
  //    is Muxed and hence there are multiple threads running in the
  //    same Transport context.

public:
  TAO_Wait_On_Leader_Follower (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_On_Leader_Follower (void);
  // Destructor.

  // = Documented in TAO_Wait_Strategy.
  virtual int sending_request (TAO_ORB_Core *orb_core,
                               int two_way);
  virtual int wait (ACE_Time_Value *max_wait_time,
                    int &reply_received);
  virtual int register_handler (void);
  virtual ACE_SYNCH_CONDITION *leader_follower_condition_variable (void);
  virtual int reply_dispatched (int &, ACE_SYNCH_CONDITION *);
  virtual void connection_closed (int &, ACE_SYNCH_CONDITION *);
};

#include "ace/post.h"
#endif /* TAO_WAIT_ON_LEADER_FOLLOWER_H */
