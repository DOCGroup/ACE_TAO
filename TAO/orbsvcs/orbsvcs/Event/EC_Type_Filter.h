// -*- C++ -*-

/**
 *  @file   EC_Type_Filter.h
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
  explicit TAO_EC_Type_Filter (RtecEventComm::EventHeader const &header);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  int filter (RtecEventComm::EventSet const &event,
              TAO_EC_QOS_Info& qos_info) override;
  int filter_nocopy (RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info) override;
  void push (RtecEventComm::EventSet const &event,
             TAO_EC_QOS_Info& qos_info) override;
  void push_nocopy (RtecEventComm::EventSet& event,
                    TAO_EC_QOS_Info& qos_info) override;
  void clear () override;
  CORBA::ULong max_event_size () const override;
  int can_match (RtecEventComm::EventHeader const &header) const override;
  int add_dependencies (RtecEventComm::EventHeader const &header,
                        TAO_EC_QOS_Info const &qos_info) override;

private:
  TAO_EC_Type_Filter (TAO_EC_Type_Filter const &) = delete;
  TAO_EC_Type_Filter (TAO_EC_Type_Filter &&) = delete;
  TAO_EC_Type_Filter& operator= (TAO_EC_Type_Filter const &) = delete;
  TAO_EC_Type_Filter& operator= (TAO_EC_Type_Filter &&) = delete;

  /// Filter an EventSet that contains more than one event.
  int filter_set (RtecEventComm::EventSet const &event,
                  TAO_EC_QOS_Info& qos_info);

private:
  /// Encapsulate the type/source that we must match.
  RtecEventComm::EventHeader header_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_TYPE_FILTER_H */
