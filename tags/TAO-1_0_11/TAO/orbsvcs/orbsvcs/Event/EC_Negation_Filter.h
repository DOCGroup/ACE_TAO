/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Negation_Filter
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

#ifndef TAO_EC_NEGATION_FILTER_H
#define TAO_EC_NEGATION_FILTER_H

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Negation_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   The negation filter.
  //
  // = DESCRIPTION
  //   This filter has a set of children (fixed at creation time), if
  //   any of the children accepts an event then it also does.
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the children.
  //
public:
  TAO_EC_Negation_Filter (TAO_EC_Filter* child);
  // Constructor. It assumes ownership of both the array and the
  // children.

  virtual ~TAO_EC_Negation_Filter (void);
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
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Negation_Filter
                              (const TAO_EC_Negation_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Negation_Filter& operator=
                              (const TAO_EC_Negation_Filter&))

private:
  TAO_EC_Filter* child_;
  // The children
};

#if defined (__ACE_INLINE__)
#include "EC_Negation_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_NEGATION_FILTER_H */
