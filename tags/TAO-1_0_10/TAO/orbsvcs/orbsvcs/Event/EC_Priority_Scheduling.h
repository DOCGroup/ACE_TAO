/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Priority_Scheduling
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

#ifndef TAO_EC_PRIORITY_SCHEDULING_H
#define TAO_EC_PRIORITY_SCHEDULING_H

#include "EC_Scheduling_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Priority_Scheduling : public TAO_EC_Scheduling_Strategy
{
  // = TITLE
  //   A scheduling strategy that uses TAO's real-time scheduler
  //
  // = DESCRIPTION
  //   This implementation of the Scheduling_Strategy uses TAO's
  //   real-time scheduler.
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Priority_Scheduling (RtecScheduler::Scheduler_ptr scheduler);
  // Constructor.

  virtual ~TAO_EC_Priority_Scheduling (void);
  // Destructor

  virtual void add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer,
      CORBA::Environment &ACE_TRY_ENV);
  // Add all the dependencies between <supplier> and <consumer>

  virtual void init_event_qos (
      const RtecEventComm::EventHeader& header,
      TAO_EC_ProxyPushConsumer *consumer,
      TAO_EC_QOS_Info& qos_info,
      CORBA::Environment &ACE_TRY_ENV);
  // Initializes <qos_info> based on the QoS information for
  // <consumer> and the event header.

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Priority_Scheduling
                              (const TAO_EC_Priority_Scheduling&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Priority_Scheduling& operator=
                              (const TAO_EC_Priority_Scheduling&))

  void init_rt_info (CORBA::Environment& env);
  // Initialize our RT_Info handle and dependencies

private:
  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler we are going to use
};

#if defined (__ACE_INLINE__)
#include "EC_Priority_Scheduling.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PRIORITY_SCHEDULING_H */
