/* -*- C++ -*- */
/**
 *  @file   EC_Timeout_Generator.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_TIMEOUT_GENERATOR_H
#define TAO_EC_TIMEOUT_GENERATOR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"
#include "EC_Timeout_Filter.h"

class TAO_EC_QOS_Info;

/**
 * @class TAO_EC_Timeout_Generator
 *
 * @brief Define the interface for the generators of timeout events.
 *
 * The Event Channel can use several strategies to dispatch
 * timers, for instance, it can use the ORB reactor or a pool of
 * reactors running at different priorities or a pool of
 * Thread_Timer_Queue_Adapters running at different priorities
 * also.
 * This class is the abstract base class to abstract this
 * strategies.
 */
class TAO_RTEvent_Export TAO_EC_Timeout_Generator
{
public:
  /// destructor
  virtual ~TAO_EC_Timeout_Generator (void);

  /// Activate any internal threads.
  virtual void activate (void) = 0;

  /// Deactivate any internal threads, clients can destroy the object
  /// after calling this method.
  virtual void shutdown (void) = 0;

  /// Add a timer at the given priority, returns the timer ID.
  virtual int schedule_timer (TAO_EC_Timeout_Filter* filter,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval) = 0;

  /// Cancel a timer at the given priority.
  virtual int cancel_timer (const TAO_EC_QOS_Info& info,
                            int id) = 0;
};

// ****************************************************************

/**
 * @class TAO_EC_Timeout_Adapter
 *
 * @brief Adapt the EC_Filter interface as an Event_Handler.
 *
 * ACE timer queues (including the reactor) use Event_Handlers to
 * dispatch events, but we want to receive them in EC_Filters,
 * this class is and adaptor for that purpose.
 */
class TAO_RTEvent_Export TAO_EC_Timeout_Adapter : public ACE_Event_Handler
{
public:
  /// Default construction.
  TAO_EC_Timeout_Adapter (void);

private:
  /// Casts <act> to EC_Filter and dispatches an event to it.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *act);
};

#if defined (__ACE_INLINE__)
#include "EC_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_TIMEOUT_GENERATOR_H */
