/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Masked_Type_Filter
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

#ifndef TAO_EC_MASKED_TYPE_FILTER_H
#include "ace/pre.h"
#define TAO_EC_MASKED_TYPE_FILTER_H

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Masked_Type_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   A masked type filter.
  //
  // = DESCRIPTION
  //   This filter only accepts events whose type and/or source have
  //   a given value when a bitmask is applied to them.
  //   In short the filter checks that:
  //
  //   (event.header.type & type_mask) == type_value
  //
  //   and that:
  //
  //   (event.header.source & source_mask) == source_value
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Masked_Type_Filter (CORBA::ULong source_mask,
                             CORBA::ULong type_mask,
                             CORBA::ULong source_value,
                             CORBA::ULong type_value);
  // Constructor.

  virtual ~TAO_EC_Masked_Type_Filter (void);
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
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Masked_Type_Filter
                              (const TAO_EC_Masked_Type_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Masked_Type_Filter& operator=
                              (const TAO_EC_Masked_Type_Filter&))

private:
  CORBA::ULong source_mask_;
  CORBA::ULong type_mask_;
  // The bitmasks

  CORBA::ULong source_value_;
  CORBA::ULong type_value_;
  // The values
};

#if defined (__ACE_INLINE__)
#include "EC_Masked_Type_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_MASKED_TYPE_FILTER_H */
