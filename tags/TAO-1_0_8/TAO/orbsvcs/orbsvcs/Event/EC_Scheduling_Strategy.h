/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Scheduling_Strategy
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

#ifndef TAO_EC_SCHEDULING_STRATEGY_H
#define TAO_EC_SCHEDULING_STRATEGY_H

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_QOS_Info;

class TAO_ORBSVCS_Export TAO_EC_Scheduling_Strategy
{
  // = TITLE
  //   Define the interface for the scheduling strategy
  //
  // = DESCRIPTION
  //   The scheduling strategy controls the actions that the event
  //   channel must take to update the dependency information in the
  //   scheduler and to query the scheduler for the priority of each
  //   event pushed by a supplier.
  //
  //   The base
  //
  // = MEMORY MANAGMENT
  //
public:
  virtual ~TAO_EC_Scheduling_Strategy (void);
  // Destructor

  virtual void add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer,
      CORBA::Environment &ACE_TRY_ENV) = 0;
  // Add all the dependencies between <supplier> and <consumer>

  virtual void init_event_qos (
      const RtecEventComm::EventHeader& header,
      TAO_EC_ProxyPushConsumer *consumer,
      TAO_EC_QOS_Info& qos_info,
      CORBA::Environment &ACE_TRY_ENV) = 0;
  // Initializes <qos_info> based on the QoS information for
  // <consumer> and the event header.

};

#if defined (__ACE_INLINE__)
#include "EC_Scheduling_Strategy.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_SCHEDULING_STRATEGY_H */
