/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Timeout_Filter
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

#ifndef TAO_EC_TIMEOUT_FILTER_H
#define TAO_EC_TIMEOUT_FILTER_H

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_QOS_Info.h"

class TAO_EC_Event_Channel;

class TAO_ORBSVCS_Export TAO_EC_Timeout_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   A filter based on event type/source
  //
  // = DESCRIPTION
  //   This filter only accept events with a predefined type/source,
  //   both the source and the type can be wildcards.
  //
public:
  TAO_EC_Timeout_Filter (TAO_EC_Event_Channel *event_channel,
                         const TAO_EC_QOS_Info& qos_info,
                         RtecEventComm::EventType type,
                         RtecEventComm::Time period);
  // Constructor.

  virtual ~TAO_EC_Timeout_Filter (void);
  // Destructor.

  const TAO_EC_QOS_Info& qos_info (void) const;
  // Return the QOS_Info for this Timeout filter.

  RtecEventComm::EventType type (void) const;
  // The type of timeout event that we generate.

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
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Timeout_Filter
                              (const TAO_EC_Timeout_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Timeout_Filter& operator=
                              (const TAO_EC_Timeout_Filter&))

private:
  TAO_EC_Event_Channel* event_channel_;
  // The event channel.

  TAO_EC_QOS_Info qos_info_;
  // Events "generated" by this filter use this QOS_Info.

  RtecEventComm::EventType type_;
  // The type of timeout event...

  int id_;
  // The ID of the timeout in the Timeout_Generator, for
  // cancellation.
};

#if defined (__ACE_INLINE__)
#include "EC_Timeout_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_TIMEOUT_FILTER_H */
