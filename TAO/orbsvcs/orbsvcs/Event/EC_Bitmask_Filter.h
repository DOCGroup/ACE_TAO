/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Bitmask_Filter
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

#ifndef TAO_EC_BITMASK_FILTER_H
#include "ace/pre.h"
#define TAO_EC_BITMASK_FILTER_H

#include "EC_Filter.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTEvent_Export TAO_EC_Bitmask_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   The bitmask filter.
  //
  // = DESCRIPTION
  //   This filter quickly rejects events that do not match a given
  //   bitmask.
  //   If the event is not rejected based on the mask then the child
  //   is consulted to finally accept or reject the event.
  //   When composed with the Null_Filter it accepts any events that
  //   satisfy:
  //   (event.header.type & type_mask) != 0
  //   && (event.header.type & source_mask) != 0
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the child.
  //
public:
  TAO_EC_Bitmask_Filter (CORBA::ULong source_mask,
                         CORBA::ULong type_mask,
                         TAO_EC_Filter* child);
  // Constructor.
  // Events that do not satisfy:
  //
  // (e.header.source & source_mask) != 0 &&
  // (e.header.type & type_mask) != 0
  //
  // are immediately rejected, other events are recursively tested
  // using the child node.
  // It assumes ownership of the child.

  virtual ~TAO_EC_Bitmask_Filter (void);
  // Destructor

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;
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
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info,
                                CORBA::Environment &ACE_TRY_ENV);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Bitmask_Filter
                              (const TAO_EC_Bitmask_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Bitmask_Filter& operator=
                              (const TAO_EC_Bitmask_Filter&))

private:
  CORBA::ULong source_mask_;
  CORBA::ULong type_mask_;
  // The bitmasks

  TAO_EC_Filter* child_;
  // The children
};

#if defined (__ACE_INLINE__)
#include "EC_Bitmask_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_BITMASK_FILTER_H */
