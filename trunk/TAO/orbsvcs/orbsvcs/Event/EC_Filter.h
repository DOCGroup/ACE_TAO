/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Filter
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   The per-consumer filtering mechanisms.
//   The EC needs to filter data passed to the consumers, so it can
//   correctly satisfy its subscription requirements.
//   This filtering can include correlations, sequences, timeouts,
//   etc. each consumer can request different filtering criteria.
//
//   Different filtering objects are associated with each consumer,
//   the filters are organized in a hierarchical structure,
//   corresponding to the subscription "expression" that the events
//   must satisfy.
//   The hierarchy is constructed using the "Builder" pattern.
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

#ifndef TAO_EC_FILTER_H
#define TAO_EC_FILTER_H

#include "ace/RB_Tree.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommC.h"

class TAO_EC_QOS_Info;

class TAO_EC_Filter
{
  // = TITLE
  //   Abstract base class for the filter hierarchy.
  //
  // = DESCRIPTION
  //   Defines the filter interface.
  //
  // = MEMORY MANAGMENT
  //   It does *not* assume ownership of its parent.
  //
public:
  TAO_EC_Filter (void);
  // constructor...

  virtual ~TAO_EC_Filter (void);
  // destructor...

  TAO_EC_Filter* parent (void) const;
  // Obtain the parent of this filter.

  void adopt_child (TAO_EC_Filter* child);
  // Become the parent of <child>

  static int matches (const RtecEventComm::EventHeader& rhs,
                      const RtecEventComm::EventHeader& lhs);
  // matches two event headers.
  // @@ TODO: strategize this...

  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment& env) = 0;
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment& env) = 0;
  // Filter this event, returns 1 if the event is accepted, 0
  // otherwise.
  // Notice that there are two versions of the method, if the event is
  // not const then filter can take ownership of the event.

  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment& env) = 0;
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& env) = 0;
  // This is called by the children when they accept an event and
  // which to pass it up.
  // Notice that there are two versions of the method, if the event is
  // not const then filter can take ownership of the event.

  virtual void clear (void) = 0;
  // Clear any saved state, must reset and assume no events have been
  // received.

  virtual CORBA::ULong max_event_size (void) const = 0;
  // Returns the maximum size of the events pushed by this filter.

  struct Header_Compare {
    int operator () (const RtecEventComm::EventHeader& lhs,
                     const RtecEventComm::EventHeader& rhs) const;
  };

  typedef ACE_RB_Tree<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> Headers;
  typedef ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> HeadersIterator;

  virtual void event_ids (Headers& headers) = 0;
  // Compute the disjunction of all the event types that could be of
  // interest for this filter (and its children).

private:
  TAO_EC_Filter* parent_;
  // The parent...
};

// ****************************************************************

class TAO_EC_Null_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   A null filter
  //
  // = DESCRIPTION
  //   This filter accepts any kind of event, it is useful for the
  //   implementation:
  //   a) Consumers that accept all events
  //   b) Consumers that trust the filtering done at the Supplier
  //      layer.
  //   c) Event Channels that don't do filtering (such as CosEC
  //      backends)
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Null_Filter (void);
  // constructor.

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment& env);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment& env);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment& env);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& env);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual void event_ids (TAO_EC_Filter::Headers& headers);
};

// ****************************************************************

// @@ Add more types of filters like:
// - Events in a sequence.
// - Events in a sequence with timeouts.
// - Conjunction with timeout [as opposed to disjunction of
//     conjunction and a timeout]
// - etc.

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_FILTER_H */
