/* -*- C++ -*- */
/**
 *  @file EC_RTCORBA_Dispatching.h
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

#ifndef TAO_EC_RTCORBA_DISPATCHING_H
#define TAO_EC_RTCORBA_DISPATCHING_H
#include "ace/pre.h"

#include "EC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_Dispatching_Task.h"
#include "tao/RTCORBA/RTCORBA.h"

class TAO_EC_Event_Channel;

/**
 * @class TAO_EC_RTCORBA_Dispatching
 *
 * @brief RTCORBA-based dispatching strategy
 *
 */
class TAO_RTEvent_Export TAO_EC_RTCORBA_Dispatching : public TAO_EC_Dispatching
{
public:
  /// Constructor
  TAO_EC_RTCORBA_Dispatching (const RTCORBA::ThreadpoolLanes &lanes,
                              RTCORBA::PriorityMapping *mapping,
                              RTCORBA::Current_ptr current);

  /// Destructor
  ~TAO_EC_RTCORBA_Dispatching (void);

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier* proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     TAO_ENV_ARG_DECL);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            TAO_ENV_ARG_DECL);

private:
  /// The thread pools...
  RTCORBA::ThreadpoolLanes lanes_;

  /// Used to map between CORBA and native priorities
  RTCORBA::PriorityMapping *priority_mapping_;

  /// Used to get the current RTCORBA priority
  RTCORBA::Current_var current_;

  /// Use our own thread manager.
  ACE_Thread_Manager thread_manager_;

  /// The dispatching task(s), one per lane
  TAO_EC_Dispatching_Task *tasks_;
};

#if defined (__ACE_INLINE__)
#include "EC_RTCORBA_Dispatching.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_RTCORBA_DISPATCHING_H */
