/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Timeout_Generator
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A new implementation of the Real Time Event Services.
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_TIMEOUT_GENERATOR_H
#define TAO_EC_TIMEOUT_GENERATOR_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

class TAO_EC_QOS_Info;
class TAO_EC_Timeout_Filter;

class TAO_ORBSVCS_Export TAO_EC_Timeout_Generator
{
  // = TITLE
  //   Define the interface for the generators of timeout events.
  //
  // = DESCRIPTION
  //   The Event Channel can use several strategies to dispatch
  //   timers, for instance, it can use the ORB reactor or a pool of
  //   reactors running at different priorities or a pool of
  //   Thread_Timer_Queue_Adapters running at different priorities
  //   also.
  //   This class is the abstract base class to abstract this
  //   strategies.
  //
public:
  virtual ~TAO_EC_Timeout_Generator (void);
  // destructor

  virtual void activate (void) = 0;
  // Activate any internal threads.

  virtual void shutdown (void) = 0;
  // Deactivate any internal threads, clients can destroy the object
  // after calling this method.

  virtual int schedule_timer (TAO_EC_Timeout_Filter* filter,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval) = 0;
  // Add a timer at the given priority, returns the timer ID.

  virtual int cancel_timer (const TAO_EC_QOS_Info& info,
                            int id) = 0;
  // Cancel a timer at the given priority.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Timeout_Adapter : public ACE_Event_Handler
{
  // = TITLE
  //   Adapt the EC_Filter interface as an Event_Handler.
  //
  // = DESCRIPTION
  //   ACE timer queues (including the reactor) use Event_Handlers to
  //   dispatch events, but we want to receive them in EC_Filters,
  //   this class is and adaptor for that purpose.
  //
public:
  TAO_EC_Timeout_Adapter (void);
  // Default construction.

private:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *act);
  // Casts <act> to EC_Filter and dispatches an event to it.
};

#if defined (__ACE_INLINE__)
#include "EC_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_TIMEOUT_GENERATOR_H */
