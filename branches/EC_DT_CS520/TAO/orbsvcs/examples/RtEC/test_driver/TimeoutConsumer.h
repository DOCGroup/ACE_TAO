/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   TimeoutConsumer
//
// = AUTHOR
//   Bryan Thrall (thrall@cse.wustl.edu)
//
// ============================================================================

#ifndef TIMEOUTCONSUMER_H
#define TIMEOUTCONSUMER_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "TestConfig.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// TODO: Convert to Observer (or other similar) pattern.

class Timeout_Observer {
  // = TITLE
  //   Interface for all TimeoutConsumer observers.
  //
  // = DESCRIPTION
  //   Any class which wants to receive notifications of timeout events
  //   needs to be a subclass of this interface.

public:
  virtual void update(ACE_ENV_SINGLE_ARG_DECL) = 0;
  // Called by the TimeoutConsumer when a timeout occurs.
  //
  // For now, the notification is binary ("A timeout occurred"), but
  // in the future, it should be useful to pass the event type(s) or
  // even the events themselves.
};

class TimeoutConsumer
{
  // = TITLE
  //   Simple consumer object of timeout events.
  //
  // = DESCRIPTION This class is a consumer of timeout events.  For
  //   each timeout event it consumes, it notifies its observer
  //   (specified in its constructor).
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  TimeoutConsumer (Timeout_Observer* obs);
  // For now, only handle a single observer. In the future, handle any number.
  // Note that the TimeoutConsumer does NOT take ownership of the observer.

  virtual ~TimeoutConsumer (void);

  RtecScheduler::handle_t get_RT_Info(void);

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char *entry_prefix,
                TimeBase::TimeT period,
                RtecScheduler::Importance_t importance,
                RtecScheduler::Criticality_t criticality,
                RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);
  // This method connects the supplier to the EC.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC.

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // pushes _events as a supplier until _to_send pushes, then calls resume() on this task.

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  Timeout_Observer* _observer;

  RtecScheduler::handle_t _rt_info;

  RtecScheduler::Scheduler_ptr _scheduler;

  RtecEventChannelAdmin::ProxyPushSupplier_var _supplier_proxy;
  // We talk to the EC (as a consumer) using this proxy.

  ACE_PushConsumer_Adapter<TimeoutConsumer> _consumer;
  // We connect to the EC as a consumer so we can receive the
  // timeout events.

  RtecEventComm::EventSet _events;
  // set of events to push when a timeout event is received.
};

#endif /* CONSUMER_H */
