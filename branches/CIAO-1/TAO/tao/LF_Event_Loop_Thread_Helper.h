// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Event_Loop_Thread_Helper.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_EVENT_LOOP_THREAD_HELPER_H
#define TAO_LF_EVENT_LOOP_THREAD_HELPER_H
#include "ace/pre.h"

#include "tao/orbconf.h"
#include "tao/LF_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Helper class to enter and exit the Leader/Followers event
 * loop.
 *
 * Uses the Guard idiom to enter and exit the Leader/Followers event
 * loop.
 */
class TAO_Export TAO_LF_Event_Loop_Thread_Helper
{
public:
  /// Constructor
  TAO_LF_Event_Loop_Thread_Helper (TAO_Leader_Follower &leader_follower,
                                   TAO_LF_Strategy &lf_strategy,
                                   ACE_Time_Value *max_wait_time);

  /// Destructor
  ~TAO_LF_Event_Loop_Thread_Helper (void);

  /// Calls <set_event_loop_thread> on the leader/followers object.
  int event_loop_return (void) const;

private:
  /// Reference to leader/followers object.
  TAO_Leader_Follower &leader_follower_;

  /// The Leader/Follower Strategy used by this ORB.
  TAO_LF_Strategy &lf_strategy_;

  /// Remembers the status returned while trying to enter the event
  /// loop.
  int event_loop_return_;
};

#if defined (__ACE_INLINE__)
# include "tao/LF_Event_Loop_Thread_Helper.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_LF_EVENT_LOOP_THREAD_HELPER_H */
