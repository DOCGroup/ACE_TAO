// -*- C++ -*-
// $Id$

#include "tao/Sync_Strategies.h"
#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/Sync_Strategies.i"
#endif /* ! __ACE_INLINE__ */

TAO_Sync_Strategy::~TAO_Sync_Strategy (void)
{
}

// ****************************************************************

int
TAO_Transport_Sync_Strategy::
    must_queue (TAO_Stub *, int)
{
  return 0;
}

int
TAO_Transport_Sync_Strategy::
    buffering_constraints_reached (TAO_Stub *,
                                   size_t ,
                                   size_t ,
                                   int &,
                                   ACE_Time_Value &)
{
  return 0;
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

// ****************************************************************

int
TAO_Eager_Buffering_Sync_Strategy::
    must_queue (TAO_Stub *, int)
{
  return 1;
}

int
TAO_Eager_Buffering_Sync_Strategy::
    buffering_constraints_reached (TAO_Stub *stub,
                                   size_t msg_count,
                                   size_t total_bytes,
                                   int &set_timer,
                                   ACE_Time_Value &interval)
{
  TAO_Buffering_Constraint_Policy *buffering_constraint_policy =
    stub->buffering_constraint ();

  if (buffering_constraint_policy == 0)
    return 1;

  // Automatically release the policy
  CORBA::Object_var auto_release = buffering_constraint_policy;

  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint_policy->get_buffering_constraint (buffering_constraint);

  this->timer_check (buffering_constraint, set_timer, interval);

  if (buffering_constraint.mode == TAO::BUFFER_FLUSH)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_COUNT)
      && msg_count >= buffering_constraint.message_count)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_BYTES)
      && total_bytes >= buffering_constraint.message_bytes)
    return 1;

  return 0;
}

void
TAO_Eager_Buffering_Sync_Strategy::
    timer_check (const TAO::BufferingConstraint &buffering_constraint,
                 int &set_timer,
                 ACE_Time_Value &interval)
{
  if (!ACE_BIT_ENABLED (buffering_constraint.mode,
                        TAO::BUFFER_TIMEOUT))
    {
      set_timer = 0;
      return;
    }

  ACE_Time_Value timeout =
    this->time_conversion (buffering_constraint.timeout);

  if (interval == timeout)
    {
      set_timer = 0;
      return;
    }

  set_timer = 1;
  interval = timeout;
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
    must_queue (TAO_Stub *,
                int queue_empty)
{
  // If the queue is empty we want to send immediately
  return !queue_empty;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
