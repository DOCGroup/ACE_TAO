// -*- C++ -*-
// $Id$

#include "tao/Sync_Strategies.h"
#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Sync_Strategies.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Sync_Strategies, "$Id$")

TAO_Sync_Strategy::~TAO_Sync_Strategy (void)
{
}

// ****************************************************************

int
TAO_Transport_Sync_Strategy::
    must_queue (int)
{
  return 0;
}

int
TAO_Transport_Sync_Strategy::
    buffering_constraints_reached (TAO_Stub *,
                                   size_t ,
                                   size_t ,
                                   int &must_flush,
                                   const ACE_Time_Value &,
                                   int &set_timer,
                                   ACE_Time_Value &)
{
  set_timer = 0;
  must_flush = 1;
  return 1;
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

// ****************************************************************

int
TAO_Eager_Buffering_Sync_Strategy::
    must_queue (int)
{
  return 1;
}

int
TAO_Eager_Buffering_Sync_Strategy::
    buffering_constraints_reached (TAO_Stub *stub,
                                   size_t msg_count,
                                   size_t total_bytes,
                                   int &must_flush,
                                   const ACE_Time_Value &current_deadline,
                                   int &set_timer,
                                   ACE_Time_Value &new_deadline)
{
  must_flush = 0;
  set_timer = 0;

  CORBA::Policy_var bcp_policy = stub->buffering_constraint ();
  TAO::BufferingConstraintPolicy_var bcp = 
    TAO::BufferingConstraintPolicy::_narrow (bcp_policy.in());

  TAO_Buffering_Constraint_Policy *buffering_constraint_policy =
    ACE_dynamic_cast (TAO_Buffering_Constraint_Policy *, bcp.in ());

  if (buffering_constraint_policy == 0)
    return 1;

  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint_policy->get_buffering_constraint (buffering_constraint);

  if (buffering_constraint.mode == TAO::BUFFER_FLUSH)
    {
      must_flush = 1;
      return 1;
    }

  int constraints_reached = 0;
  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_COUNT)
      && msg_count >= buffering_constraint.message_count)
    constraints_reached = 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_BYTES)
      && total_bytes >= buffering_constraint.message_bytes)
    constraints_reached = 1;

  if (this->timer_check (buffering_constraint,
                         current_deadline,
                         set_timer,
                         new_deadline) != 0)
    constraints_reached = 1;

  return constraints_reached;
}

int
TAO_Eager_Buffering_Sync_Strategy::
    timer_check (const TAO::BufferingConstraint &buffering_constraint,
                 const ACE_Time_Value &current_deadline,
                 int &set_timer,
                 ACE_Time_Value &new_deadline)
{
  set_timer = 0;
  if (!ACE_BIT_ENABLED (buffering_constraint.mode,
                        TAO::BUFFER_TIMEOUT))
    {
      return 0;
    }

  // Compute the next deadline...
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  ACE_Time_Value timeout =
    this->time_conversion (buffering_constraint.timeout);
  new_deadline = now + timeout;

  // Check if the new deadline is more stringent, or if the deadline
  // has expired and thus must be reset anyway.
  if (current_deadline > new_deadline
      || current_deadline < now)
    {
      set_timer = 1;
    }

  // ... if there is no deadline we don't want to schedule output (the
  // deadline will be set because set_timer is set to 1 in that case).
  // If there is a deadline but but it has not been reached, we
  // don't want to schedule any output either...
  if (current_deadline == ACE_Time_Value::zero
      || current_deadline >= now)
    {
      return 0;
    }

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_Eager_Buffering_Sync_Strategy::timer_check, "
                  "Now = %u, Current = %u, New = %u\n",
                  now.msec (), current_deadline.msec (),
                  new_deadline.msec ()));
    }

  return 1;
}

ACE_Time_Value
TAO_Eager_Buffering_Sync_Strategy::
    time_conversion (const TimeBase::TimeT &time)
{
  TimeBase::TimeT seconds = time / 10000000u;
  TimeBase::TimeT microseconds = (time % 10000000u) / 10;
  return ACE_Time_Value (ACE_U64_TO_U32 (seconds),
                         ACE_U64_TO_U32 (microseconds));
}

// ****************************************************************

int
TAO_Delayed_Buffering_Sync_Strategy::
    must_queue (int queue_empty)
{
  // If the queue is empty we want to send immediately
  return !queue_empty;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
