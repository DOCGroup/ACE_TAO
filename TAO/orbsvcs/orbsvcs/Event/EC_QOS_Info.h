/* -*- C++ -*- */
/**
 *  @file   EC_QOS_Info.h
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

#ifndef TAO_EC_QOS_INFO_H
#define TAO_EC_QOS_INFO_H
#include "ace/pre.h"

#include "orbsvcs/RtecBaseC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_QOS_Info
 *
 * @brief A representation of QoS information for the event channel
 * filters.
 *
 * Filters compute QOS information for real-time dispatching, this
 * class encapsulates that information.
 * This first implementation is just a place-holder.
 */
class TAO_RTEvent_Export TAO_EC_QOS_Info
{
public:
  /// Constructor
  TAO_EC_QOS_Info (void);

  /// Copy constructor, it does the obvious thing, but if it is not
  /// here the HP/aCC compiler breaks.
  TAO_EC_QOS_Info (const TAO_EC_QOS_Info &rhs);

  RtecBase::handle_t rt_info;
  RtecBase::Preemption_Priority_t preemption_priority;

  /**
   * Timer ids propagate their identity using this field, notice that
   * they cannot use the event type because there could be multiple
   * timeouts for the same consumer.
   */
  long timer_id_;
};

#if defined (__ACE_INLINE__)
#include "EC_QOS_Info.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_EVENT_CHANNEL_H */
