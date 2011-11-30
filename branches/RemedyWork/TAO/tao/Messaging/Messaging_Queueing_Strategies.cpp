// $Id$

#include "tao/Messaging/Messaging_Queueing_Strategies.h"
#include "tao/Messaging/Buffering_Constraint_Policy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
// ****************************************************************

  #if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  bool
  Eager_Transport_Queueing_Strategy::must_queue (bool) const
  {
    return true;
  }

  bool
  Eager_Transport_Queueing_Strategy::buffering_constraints_reached (
    TAO_Stub *stub,
    size_t msg_count,
    size_t total_bytes,
    bool &must_flush,
    const ACE_Time_Value &current_deadline,
    bool &set_timer,
    ACE_Time_Value &new_deadline) const
  {
    must_flush = false;
    set_timer = false;

    TAO::BufferingConstraint buffering_constraint;

    try
      {
        CORBA::Policy_var bcp_policy =
          stub->get_cached_policy (TAO_CACHED_POLICY_BUFFERING_CONSTRAINT);

        TAO::BufferingConstraintPolicy_var bcpv =
          TAO::BufferingConstraintPolicy::_narrow (bcp_policy.in ());

        TAO_Buffering_Constraint_Policy* bcp =
          dynamic_cast<TAO_Buffering_Constraint_Policy *> (bcpv.in ());
        if (bcp == 0)
          {
            return true;
          }
        bcp->get_buffering_constraint (buffering_constraint);
      }
    catch (const ::CORBA::Exception&)
      {
        return true;
      }


    if (buffering_constraint.mode == TAO::BUFFER_FLUSH)
      {
        must_flush = true;
        return true;
      }

    bool constraints_reached = false;

    if (ACE_BIT_ENABLED (buffering_constraint.mode,
                         TAO::BUFFER_MESSAGE_COUNT)
        && msg_count >= buffering_constraint.message_count)
      {
        constraints_reached = true;
      }

    if (ACE_BIT_ENABLED (buffering_constraint.mode,
                         TAO::BUFFER_MESSAGE_BYTES)
        && total_bytes >= buffering_constraint.message_bytes)
      {
        constraints_reached = true;
      }

    if (this->timer_check (stub,
                           buffering_constraint,
                           current_deadline,
                           set_timer,
                           new_deadline))
      {
        constraints_reached = true;
      }

    return constraints_reached;
  }

  bool
  Eager_Transport_Queueing_Strategy::timer_check (
    TAO_Stub *stub,
    const TAO::BufferingConstraint &buffering_constraint,
    const ACE_Time_Value &current_deadline,
    bool &set_timer,
    ACE_Time_Value &new_deadline) const
  {
    set_timer = false;

    if (!ACE_BIT_ENABLED (buffering_constraint.mode,
                          TAO::BUFFER_TIMEOUT))
      {
        return false;
      }

    // Compute the next deadline...
    ACE_Reactor * const reactor = stub->orb_core ()->reactor ();
    ACE_Time_Value const now =
        reactor->timer_queue ()->gettimeofday ();
    ACE_Time_Value timeout =
      this->time_conversion (buffering_constraint.timeout);
    new_deadline = now + timeout;

    // Check if the new deadline is more stringent, or if the deadline
    // has expired and thus must be reset anyway.
    if (current_deadline > new_deadline
        || current_deadline < now)
      {
        set_timer = true;
      }

    // ... if there is no deadline we don't want to schedule output (the
    // deadline will be set because set_timer is set to 1 in that case).
    // If there is a deadline but but it has not been reached, we
    // don't want to schedule any output either...
    if (current_deadline == ACE_Time_Value::zero
        || current_deadline >= now)
      {
        return false;
      }

    if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - TAO_Eager_Buffering_Sync_Strategy::timer_check, "
                    "Now = %u, Current = %u, New = %u\n",
                    now.msec (), current_deadline.msec (),
                    new_deadline.msec ()));
      }

    return true;
  }

  ACE_Time_Value
  Eager_Transport_Queueing_Strategy::time_conversion (
    const TimeBase::TimeT &time) const
  {
    TimeBase::TimeT seconds = time / 10000000u;
    TimeBase::TimeT microseconds = (time % 10000000u) / 10;
    return ACE_Time_Value (ACE_U64_TO_U32 (seconds),
                           ACE_U64_TO_U32 (microseconds));
  }

// ****************************************************************

  bool
  Delayed_Transport_Queueing_Strategy::must_queue (bool queue_empty) const
  {
    // If the queue is empty we want to send immediately
    return !queue_empty;
  }

  #endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

}

TAO_END_VERSIONED_NAMESPACE_DECL
