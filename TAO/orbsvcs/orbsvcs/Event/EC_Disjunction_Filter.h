/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Disjunction_Filter
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A per-consumer filter that accepts events from a set of
//   children.
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

#ifndef TAO_EC_DISJUNCTION_FILTER_H
#define TAO_EC_DISJUNCTION_FILTER_H

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Disjunction_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   The disjunction filter.
  //
  // = DESCRIPTION
  //   This filter has a set of children (fixed at creation time), if
  //   any of the children accepts an event then it also does.
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the children.
  //
public:
  TAO_EC_Disjunction_Filter (TAO_EC_Filter* children[],
                             size_t n);
  // Constructor. It assumes ownership of both the array and the
  // children.

  virtual ~TAO_EC_Disjunction_Filter (void);
  // Destructor

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
  virtual int can_match (const RtecEventComm::EventHeader& header) const;

  typedef TAO_EC_Filter* value_type;
  typedef TAO_EC_Filter* const const_value_type;
  typedef const_value_type* ChildrenIterator;
  ChildrenIterator begin (void) const;
  ChildrenIterator end (void) const;
  // STL-like iterators...

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Disjunction_Filter
                              (const TAO_EC_Disjunction_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Disjunction_Filter& operator=
                              (const TAO_EC_Disjunction_Filter&))

private:
  TAO_EC_Filter** children_;
  // The children

  size_t n_;
  // The number of children.
};

#if defined (__ACE_INLINE__)
#include "EC_Disjunction_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_DISJUNCTION_FILTER_H */
