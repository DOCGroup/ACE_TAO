/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Sched_Filter_Builder
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
//
// ============================================================================

#ifndef TAO_EC_SCHED_FILTER_BUILDER_H
#define TAO_EC_SCHED_FILTER_BUILDER_H

#include "EC_Filter_Builder.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Filter;
class TAO_EC_Event_Channel;

class TAO_ORBSVCS_Export TAO_EC_Sched_Filter_Builder : public TAO_EC_Filter_Builder
{
  // = TITLE
  //   Implement a builder for the fundamental filters.
  //
  // = DESCRIPTION
  //   The sched filtering mechanisms in the Event channel
  //   (source/type based filtering + disjunctions and conjunctions)
  //   are constructed using this class.
  //
public:
  TAO_EC_Sched_Filter_Builder (TAO_EC_Event_Channel* ec);
  // constructor.

  virtual ~TAO_EC_Sched_Filter_Builder (void);
  // destructor...

  // = The TAO_EC_Filter_Builder methods...
  TAO_EC_Filter* build (TAO_EC_ProxyPushSupplier *supplier,
                        RtecEventChannelAdmin::ConsumerQOS& qos,
                        CORBA::Environment &env) const;

private:
  TAO_EC_Filter* recursive_build (TAO_EC_ProxyPushSupplier *supplier,
                                  RtecEventChannelAdmin::ConsumerQOS& qos,
                                  CORBA::ULong& pos,
                                  RtecScheduler::Scheduler_ptr scheduler,
                                  const char* base_name,
                                  CORBA::Environment& env) const;
  // Recursively build the filter tree.

  CORBA::ULong count_children (RtecEventChannelAdmin::ConsumerQOS& qos,
                               CORBA::ULong pos) const;
  // Count the number of children of the current node, i.e. until a
  // conjunction or disjunction starts.

private:
  TAO_EC_Event_Channel* event_channel_;
  // The event channel.
};

#if defined (__ACE_INLINE__)
#include "EC_Sched_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_SCHED_FILTER_BUILDER_H */
