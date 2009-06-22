// -*- C++ -*-

/**
 *  @file   EC_And_Filter.h
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

#ifndef TAO_EC_AND_FILTER_H
#define TAO_EC_AND_FILTER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_And_Filter
 *
 * @brief The 'logical and' filter.
 *
 * This filter has a set of children (fixed at creation time),
 * only if all the children accept an event it does so too.
 *
 * <H2>Memory Management</H2>
 * It assumes ownership of the children.
 */
class TAO_RTEvent_Serv_Export TAO_EC_And_Filter : public TAO_EC_Filter
{
public:
  /// Constructor. It assumes ownership of both the array and the
  /// children.
  TAO_EC_And_Filter (TAO_EC_Filter* children[],
                             size_t n);

  /// Destructor
  virtual ~TAO_EC_And_Filter (void);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info);

private:
  TAO_EC_And_Filter (const TAO_EC_And_Filter&);
  TAO_EC_And_Filter& operator= (const TAO_EC_And_Filter&);

private:
  /// The children
  TAO_EC_Filter** children_;

  /// The number of children.
  size_t n_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_AND_FILTER_H */
