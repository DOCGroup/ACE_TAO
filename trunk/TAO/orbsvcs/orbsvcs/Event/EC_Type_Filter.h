/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Type_Filter
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A per-consumer filter that only accept events whose type/source
//   match.
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

#ifndef TAO_EC_TYPE_FILTER_H
#define TAO_EC_TYPE_FILTER_H

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Type_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   A filter based on event type/source
  //
  // = DESCRIPTION
  //   This filter only accept events with a predefined type/source,
  //   both the source and the type can be wildcards.
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Type_Filter (const RtecEventComm::EventHeader& header);
  // Constructor.

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

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Type_Filter
                              (const TAO_EC_Type_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Type_Filter& operator=
                              (const TAO_EC_Type_Filter&))

private:
  RtecEventComm::EventHeader header_;
  // Encapsulate the type/source that we must match.
};

#if defined (__ACE_INLINE__)
#include "EC_Type_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_TYPE_FILTER_H */
