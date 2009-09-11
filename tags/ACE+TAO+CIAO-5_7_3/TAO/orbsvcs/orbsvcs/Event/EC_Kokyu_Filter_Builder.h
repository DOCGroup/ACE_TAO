// -*- C++ -*-
/**
 *  @file   EC_Kokyu_Filter_Builder.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall (thrall@cs.wustl.edu)
 *
 * Based on previous work by Carlos O'Ryan (coryan@cs.wustl.edu) and
 * Tim Harrison (harrison@cs.wustl.edu) and other members of the DOC group.
 * More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_KOKYU_FILTER_BUILDER_H
#define TAO_EC_KOKYU_FILTER_BUILDER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Filter_Builder.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/rtkokyu_event_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Filter;
class TAO_EC_Event_Channel_Base;

/**
 * @class TAO_EC_Kokyu_Filter_Builder
 *
 * @brief Implement a builder for the fundamental filters.
 *
 * The sched filtering mechanisms in the Event channel
 * (source/type based filtering + disjunctions and conjunctions)
 * are constructed using this class.
 */
class TAO_RTKokyuEvent_Export TAO_EC_Kokyu_Filter_Builder : public TAO_EC_Filter_Builder
{
public:
  /// constructor.
  TAO_EC_Kokyu_Filter_Builder (TAO_EC_Event_Channel_Base* ec);

  /// destructor...
  virtual ~TAO_EC_Kokyu_Filter_Builder (void);

  // = The TAO_EC_Filter_Builder methods...
  TAO_EC_Filter* build (TAO_EC_ProxyPushSupplier *supplier,
                        RtecEventChannelAdmin::ConsumerQOS& qos) const;

private:
  /// Recursively build the filter tree.
  TAO_EC_Filter* recursive_build (TAO_EC_ProxyPushSupplier *supplier,
                                  RtecEventChannelAdmin::ConsumerQOS& qos,
                                  CORBA::ULong& pos,
                                  RtecScheduler::Scheduler_ptr scheduler,
                                  RtecScheduler::handle_t parent_info) const;

  /// Build the name recursively...
  void recursive_name (RtecEventChannelAdmin::ConsumerQOS& qos,
                       CORBA::ULong& pos,
                       RtecScheduler::Scheduler_ptr scheduler,
                       ACE_CString &name) const;

  /// Count the number of children of the current node, i.e. until a
  /// conjunction or disjunction starts.
  CORBA::ULong count_children (RtecEventChannelAdmin::ConsumerQOS& qos,
                               CORBA::ULong pos) const;

private:
  /// The event channel.
  TAO_EC_Event_Channel_Base* event_channel_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Kokyu_Filter_Builder.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_KOKYU_FILTER_BUILDER_H */
