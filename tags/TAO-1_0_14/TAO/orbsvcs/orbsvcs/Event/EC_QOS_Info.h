/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Event_Channel
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
// ============================================================================

#ifndef TAO_EC_QOS_INFO_H
#define TAO_EC_QOS_INFO_H

#include "orbsvcs/RtecSchedulerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_QOS_Info
{
  // = TITLE
  //   A representation of QoS information for the event channel
  //   filters.
  //
  // = DESCRIPTION
  //   Filters compute QOS information for real-time dispatching, this
  //   class encapsulates that information.
  //   This first implementation is just a place-holder.
  //
public:
  TAO_EC_QOS_Info (void);
  // constructor

  TAO_EC_QOS_Info (const TAO_EC_QOS_Info &rhs);
  // copy constructor, it does the obvious thing, but if it is not
  // here the HP/aCC compiler breaks. 

  RtecScheduler::handle_t rt_info;
  RtecScheduler::Preemption_Priority_t preemption_priority;

  long timer_id_;
  // Timer ids propagate their identity using this field, notice that
  // they cannot use the event type because there could be multiple
  // timeouts for the same consumer.
};

#if defined (__ACE_INLINE__)
#include "EC_QOS_Info.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_EVENT_CHANNEL_H */
