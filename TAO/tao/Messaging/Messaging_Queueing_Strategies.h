// -*- C++ -*-

//=============================================================================
/**
 *  @file    Messaging_Queueing_Strategies.h
 *
 *  $Id$
 *
 *  Queueing strategies for the ORB Messaging layer.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_MESSAGING_QUEUEING_STRATEGIES_H
#define TAO_MESSAGING_QUEUEING_STRATEGIES_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"
#include "tao/Transport_Queueing_Strategies.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  struct BufferingConstraint;
}

namespace TimeBase
{
  typedef CORBA::ULongLong TimeT;
}

namespace TAO
{
  #if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  class Eager_Transport_Queueing_Strategy : public Transport_Queueing_Strategy
  {
  public:
    virtual bool must_queue (bool queue_empty) const;

    virtual bool buffering_constraints_reached (
      TAO_Stub *stub,
      size_t msg_count,
      size_t total_bytes,
      bool &must_flush,
      const ACE_Time_Value &current_deadline,
      bool &set_timer,
      ACE_Time_Value &new_deadline) const;

  private:
    /// Check if the buffering constraint includes any timeouts and
    /// compute the right timeout interval if needed.
    /**
     * @param stub
     * @param buffering_constraint The constraints defined by the
     *        application
     * @param current_deadline The current deadline
     * @param set_timer Return true if the timer should be set
     * @param new_deadline Return the timer interval value
     *
     * @return Returns true if the deadline has already expired and
     *         flushing must commence immediately.  If the function
     *         returns false then flushing may need to be delayed, use @c
     *         set_timer and
     */
    bool timer_check (TAO_Stub *stub,
                      const TAO::BufferingConstraint &buffering_constraint,
                      const ACE_Time_Value &current_deadline,
                      bool &set_timer,
                      ACE_Time_Value &new_deadline) const;

    /// Convert from standard CORBA time units to seconds/microseconds.
    ACE_Time_Value time_conversion (const TimeBase::TimeT &time) const;
  };

  /// Delay the buffering decision until the transport blocks
  /**
   * If the queue is empty the transport will try to send immediately.
   */
  class Delayed_Transport_Queueing_Strategy
    : public Eager_Transport_Queueing_Strategy
  {
  public:
    virtual bool must_queue (bool queue_empty) const;
  };

  #endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_MESSAGING_QUEUEING_STRATEGIES_H */
