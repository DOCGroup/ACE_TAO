// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Strategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_STRATEGY_H
#define TAO_LF_STRATEGY_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Time_Value;
class TAO_Leader_Follower;

/**
 * @brief Strategize Leader/Follower manipulations in the ORB event
 * loop.
 *
 * The ORB event loop must participate in the Leader/Followers
 * protocol, but only if that concurrency model is configured,
 * otherwise performance suffers.
 *
 * This class strategizes the ORB behavior in this respect.
 */
class TAO_Export TAO_LF_Strategy
{
public:
  /// Destructor
  virtual ~TAO_LF_Strategy (void);

  /// The current thread will handle an upcall
  /**
   * Threads that handle requests can block for long periods of time,
   * causing deadlocks if they don't elect a new leader before
   * starting the upcall the system can become non-responsive or
   * dead-lock.
   */
  virtual void set_upcall_thread (TAO_Leader_Follower &) = 0;

  /// The current thread is entering the reactor event loop
  /**
   * Threads that block in the reactor event loop become "server"
   * threads for the Leader/Follower set.  They must be flagged
   * specially because they do not wait for one specific event, but
   * for any event whatsoever.
   */
  virtual int set_event_loop_thread (ACE_Time_Value *max_wait_time,
                                     TAO_Leader_Follower &) = 0;

  /// The current thread is leaving the event loop
  /**
   * When the thread leaves the event loop a new leader must be
   * elected.
   */
  virtual void reset_event_loop_thread (int call_reset,
                                        TAO_Leader_Follower &) = 0;
};

#if defined (__ACE_INLINE__)
# include "tao/LF_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_LF_STRATEGY_H */
