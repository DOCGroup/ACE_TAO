/* -*- C++ -*- */
/**
 *  @file   EC_Timeout_Filter.h
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

#ifndef TAO_EC_TIMEOUT_FILTER_H
#define TAO_EC_TIMEOUT_FILTER_H
#include "ace/pre.h"

#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_QOS_Info.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_Timeout_Filter
 *
 * @brief A filter based on event type/source
 *
 * This filter only accept events with a predefined type/source,
 * both the source and the type can be wildcards.
 */
class TAO_RTEvent_Export TAO_EC_Timeout_Filter : public TAO_EC_Filter
{
public:
  /// Constructor.
  TAO_EC_Timeout_Filter (TAO_EC_Event_Channel *event_channel,
                         TAO_EC_ProxyPushSupplier *supplier,
                         const TAO_EC_QOS_Info& qos_info,
                         RtecEventComm::EventType type,
                         RtecEventComm::Time period);

  /// Destructor.
  virtual ~TAO_EC_Timeout_Filter (void);

  /// Return the QOS_Info for this Timeout filter.
  const TAO_EC_QOS_Info& qos_info (void) const;

  /// The type of timeout event that we generate.
  RtecEventComm::EventType type (void) const;

  /// Callback from the Timeout_Generator
  void push_to_proxy (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info
                      TAO_ENV_ARG_DECL);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info
                      TAO_ENV_ARG_DECL);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info
                             TAO_ENV_ARG_DECL);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     TAO_ENV_ARG_DECL);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            TAO_ENV_ARG_DECL);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info
                                TAO_ENV_ARG_DECL);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Timeout_Filter
                              (const TAO_EC_Timeout_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Timeout_Filter& operator=
                              (const TAO_EC_Timeout_Filter&))

private:
  /// The event channel.
  TAO_EC_Event_Channel* event_channel_;

  /// The supplier that finally receives the timeout event.
  TAO_EC_ProxyPushSupplier *supplier_;

  /// Events "generated" by this filter use this QOS_Info.
  TAO_EC_QOS_Info qos_info_;

  /// The type of timeout event...
  RtecEventComm::EventType type_;

  /// The period for deadline timeouts...
  RtecEventComm::Time period_;

  /// The ID of the timeout in the Timeout_Generator, for
  /// cancellation.
  long id_;
};

#if defined (__ACE_INLINE__)
#include "EC_Timeout_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_TIMEOUT_FILTER_H */
