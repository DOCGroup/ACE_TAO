// -*- C++ -*-

/**
 *  @file   EC_Type_Filter.h
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

#ifndef TAO_EC_TYPE_FILTER_H
#define TAO_EC_TYPE_FILTER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Filter.h"
#include /**/ "orbsvcs/Event/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_Type_Filter
 *
 * @brief A filter based on event type/source
 *
 * This filter only accept events with a predefined type/source,
 * both the source and the type can be wildcards.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Type_Filter : public TAO_EC_Filter
{
public:
  /// Constructor.
  TAO_EC_Type_Filter (const RtecEventComm::EventHeader& header);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
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
  TAO_EC_Type_Filter (const TAO_EC_Type_Filter&);
  TAO_EC_Type_Filter& operator= (const TAO_EC_Type_Filter&);

  /// Filter an EventSet that contains more than one event.
  int filter_set (const RtecEventComm::EventSet& event,
                  TAO_EC_QOS_Info& qos_info);

private:
  /// Encapsulate the type/source that we must match.
  RtecEventComm::EventHeader header_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_TYPE_FILTER_H */
