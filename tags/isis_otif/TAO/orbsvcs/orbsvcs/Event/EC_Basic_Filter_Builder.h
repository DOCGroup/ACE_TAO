/* -*- C++ -*- */
/**
 *  @file   EC_Basic_Filter_Builder.h
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

#ifndef TAO_EC_BASIC_FILTER_BUILDER_H
#define TAO_EC_BASIC_FILTER_BUILDER_H
#include "ace/pre.h"

#include "EC_Filter_Builder.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Filter;
class TAO_EC_Event_Channel;

/**
 * @class TAO_EC_Basic_Filter_Builder
 *
 * @brief Implement a builder for the fundamental filters.
 *
 * The basic filtering mechanisms in the Event channel
 * (source/type based filtering + disjunctions, conjunctions, logical ands,
 * negations, and bitmasks) are constructed using this class.
 */
class TAO_RTEvent_Export TAO_EC_Basic_Filter_Builder : public TAO_EC_Filter_Builder
{
public:
  /// constructor.
  TAO_EC_Basic_Filter_Builder (TAO_EC_Event_Channel* ec);

  /// destructor...
  virtual ~TAO_EC_Basic_Filter_Builder (void);

  // = The TAO_EC_Filter_Builder methods...
  TAO_EC_Filter* build (TAO_EC_ProxyPushSupplier *supplier,
                        RtecEventChannelAdmin::ConsumerQOS& qos
                        ACE_ENV_ARG_DECL) const;

private:
  /// Recursively build the filter tree.
  TAO_EC_Filter* recursive_build (TAO_EC_ProxyPushSupplier *supplier,
                                  RtecEventChannelAdmin::ConsumerQOS& qos,
                                  CORBA::ULong& pos) const;

  /// Count the number of children of the current node, i.e. until a
  /// conjunction, disjunction, logical and, bitmask, or negation occurs.
  CORBA::ULong count_children (RtecEventChannelAdmin::ConsumerQOS& qos,
                               CORBA::ULong pos) const;

private:
  /// The event channel.
  TAO_EC_Event_Channel* event_channel_;
};

#if defined (__ACE_INLINE__)
#include "EC_Basic_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_BASIC_FILTER_BUILDER_H */
