// -*- C++ -*-

//=============================================================================
/**
 *  @file    Sync_Strategies.h
 *
 *  $Id$
 *
 *   Sync Strategies for the ORB Messaging layer.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_SYNC_STRATEGIES_H
#define TAO_SYNC_STRATEGIES_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"
#include "tao/TAOC.h"

/// Define the interface for the Queueing Strategy
/**
 * The low-level I/O components in the ORB use this strategy to
 * determine when messages must be queued, immediately sent or
 * flushed.
 *
 * The strategy isolates this low-level components from the higher
 * level strategies used by the application developer.
 *
 * @todo The class name (Sync_Strategy) is inherited from the policies
 * (SyncScopePolicy), but Queueing_Strategy probably captures its
 * intent better.  It should be changed in a future revision of the
 * ORB.
 */
class TAO_Export TAO_Sync_Strategy
{
public:
  /// Destructor
  virtual ~TAO_Sync_Strategy (void);

  /// Return 1 if a message must be queued
  virtual int must_queue (int queue_empty) = 0;

  /// Return 1 if it is time to start
  /**
   * @param stub The object used to make the request, this is used to
   *        obtain the policies currently in effect for the request
   * @param msg_count The number of messages currently queued
   * @param total_bytes Number of bytes currently queued
   * @param set_timer Returns 1 if a timer should be set to drain the
   *        queue
   * @param interval If set_timer returns 1, this parameter contains
   *        the timer interval
   */
  virtual int buffering_constraints_reached (TAO_Stub *stub,
                                             size_t msg_count,
                                             size_t total_bytes,
                                             int &must_flush,
                                             const ACE_Time_Value &current_deadline,
                                             int &set_timer,
                                             ACE_Time_Value &interval) = 0;
};

class TAO_Export TAO_Transport_Sync_Strategy : public TAO_Sync_Strategy
{
public:
  virtual int must_queue (int queue_empty);

  virtual int buffering_constraints_reached (TAO_Stub *stub,
                                             size_t msg_count,
                                             size_t total_bytes,
                                             int &must_flush,
                                             const ACE_Time_Value &current_deadline,
                                             int &set_timer,
                                             ACE_Time_Value &interval);
};

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

class TAO_Export TAO_Eager_Buffering_Sync_Strategy : public TAO_Sync_Strategy
{
public:
  virtual int must_queue (int queue_empty);

  virtual int buffering_constraints_reached (TAO_Stub *stub,
                                             size_t msg_count,
                                             size_t total_bytes,
                                             int &must_flush,
                                             const ACE_Time_Value &current_deadline,
                                             int &set_timer,
                                             ACE_Time_Value &new_deadline);

private:
  /// Check if the buffering constraint includes any timeouts and
  /// compute the right timeout interval if needed.
  /**
   * @param buffering_constraint The constraints defined by the
   *        application
   * @param current_deadline The current deadline
   * @param set_timer Return 1 if the timer should be set
   * @param new_deadline Return the timer interval value
   *
   * @return Returns 1 if the deadline has already expired and
   *         flushing must commence immediately.  If the function
   *         returns 0 then flushing may need to be delayed, use @c
   *         set_timer and
   *
   */
  int timer_check (const TAO::BufferingConstraint &buffering_constraint,
                   const ACE_Time_Value &current_deadline,
                   int &set_timer,
                   ACE_Time_Value &new_deadline);

  /// Convert from standard CORBA time units to seconds/microseconds.
  ACE_Time_Value time_conversion (const TimeBase::TimeT &time);
};

/// Delay the buffering decision until the transport blocks
/**
 * If the queue is empty the transport will try to send immediately.
 */
class TAO_Export TAO_Delayed_Buffering_Sync_Strategy : public TAO_Eager_Buffering_Sync_Strategy
{
public:
  virtual int must_queue (int queue_empty);
};

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if defined (__ACE_INLINE__)
# include "tao/Sync_Strategies.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_SYNC_STRATEGIES_H */
