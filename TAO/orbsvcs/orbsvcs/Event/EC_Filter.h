/* -*- C++ -*- */
/**
 *  @file   EC_Filter.h
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

#ifndef TAO_EC_FILTER_H
#define TAO_EC_FILTER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventCommC.h"
#include /**/ "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_QOS_Info;

/**
 * @class TAO_EC_Filter
 *
 * @brief Abstract base class for the filter hierarchy.
 *
 * The per-consumer filtering mechanisms.
 * The EC needs to filter data passed to the consumers, so it can
 * correctly satisfy its subscription requirements.
 * This filtering can include correlations, sequences, timeouts,
 * etc. each consumer can request different filtering criteria.
 * Different filtering objects are associated with each consumer,
 * the filters are organized in a hierarchical structure,
 * corresponding to the subscription "expression" that the events
 * must satisfy.
 * The hierarchy is constructed using the "Builder" pattern.
 *
 * <H2>Memory Management</H2>
 * It does *not* assume ownership of its parent.
 */
class TAO_RTEvent_Export TAO_EC_Filter
{
public:
  /// constructor...
  TAO_EC_Filter (void);

  /// destructor...
  virtual ~TAO_EC_Filter (void);

  /// Obtain the parent of this filter.
  TAO_EC_Filter* parent (void) const;

  /// Become the parent of <child>
  void adopt_child (TAO_EC_Filter* child);

  /// Matches two event headers.
  /// @todo Should we strategize the algorithm used to match headers?
  static int matches (const RtecEventComm::EventHeader& rhs,
                      const RtecEventComm::EventHeader& lhs);

  typedef TAO_EC_Filter* value_type;
  typedef TAO_EC_Filter* const const_value_type;
  typedef const_value_type* ChildrenIterator;

  /**
   * STL-like iterators
   * Filters follow the Composite pattern. All filters expose the same
   * interface as if they all had children, but for simple filters the
   * iterators return an empty range.
   */
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;

  /**
   * Filter this event, returns 1 if the event is accepted, 0
   * otherwise.
   * Notice that there are two versions of the method, if the event is
   * not const then filter can take ownership of the event.
   *
   * @attention There seems to be a disparity in interfaces: Supplier
   * always push event sets of size 1 to the EC_ProxyPushSupplier, and
   * EC_Filters do not implement handling of sets of more than 1
   * event.  Then, why is this not enforced by the interface by having
   * EC_ProxyPushSupplier take an event rather than a set?
   */
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info
                      ACE_ENV_ARG_DECL) = 0;
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info
                             ACE_ENV_ARG_DECL) = 0;

  /**
   * This is called by the children when they accept an event and
   * which to pass it up.
   * Notice that there are two versions of the method, if the event is
   * not const then filter can take ownership of the event.
   */
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     ACE_ENV_ARG_DECL) = 0;
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            ACE_ENV_ARG_DECL) = 0;

  /// Clear any saved state, must reset and assume no events have been
  /// received.
  virtual void clear (void) = 0;

  /// Returns the maximum size of the events pushed by this filter.
  virtual CORBA::ULong max_event_size (void) const = 0;

  /**
   * Returns 0 if an event with that header could never be accepted.
   * This can used by the suppliers to filter out consumers that
   * couldn't possibly be interested in their events.
   * The rt_info and
   */
  virtual int can_match (const RtecEventComm::EventHeader& header) const = 0;

  /**
   * This is used for computing the scheduling dependencies:
   *
   * Leaf filters check if the header could be matched, similar to the
   * can_match() method; if it does they return 1, and 0 otherwise.
   * Intermediate nodes always return 0.
   *
   * This is used to build precise dependencies between the suppliers
   * and the leaf of the filters that accept that event.  Notice that
   * only the nodes doing scheduling recurse through the list, so in
   * configurations that do no require scheduling the recursion stops
   * fairly soon.
   */
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info& qos_info
                                ACE_ENV_ARG_DECL) = 0;

  /**
   * Obtain the QOS information for this filter, the default
   * implementation returns an invalid QOS.  Only the filters that
   * support scheduling information implement this method.
   * @return Returns 0 on success and -1 on failure
   */
  virtual void get_qos_info (TAO_EC_QOS_Info& qos_info
                             ACE_ENV_ARG_DECL);

private:
  /// The parent...
  TAO_EC_Filter* parent_;
};

// ****************************************************************

/**
 * @class TAO_EC_Null_Filter
 *
 * @brief A null filter
 *
 * This filter accepts any kind of event, it is useful for the
 * implementation:
 * a) Consumers that accept all events
 * b) Consumers that trust the filtering done at the Supplier
 * layer.
 * c) Event Channels that don't do filtering (such as CosEC
 * backends)
 */
class TAO_RTEvent_Export TAO_EC_Null_Filter : public TAO_EC_Filter
{
public:
  /// Constructor.
  TAO_EC_Null_Filter (void);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info
                      ACE_ENV_ARG_DECL);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info
                             ACE_ENV_ARG_DECL);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     ACE_ENV_ARG_DECL);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            ACE_ENV_ARG_DECL);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info
                                ACE_ENV_ARG_DECL);
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

#include /**/ "ace/post.h"
#endif /* TAO_EC_FILTER_H */
