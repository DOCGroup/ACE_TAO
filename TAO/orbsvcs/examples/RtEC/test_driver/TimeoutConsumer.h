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
//#include "ace/Task.h"
//#include "ace/Synch.h"
#include "ace/RW_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TimeoutConsumer
{
  // = TITLE
  //   Simple consumer object which responds to timeout events.
  //
  // = DESCRIPTION
  //   This class is a consumer of timeout events.
  //   For each timeout event it consumes, it pushes a specified EventSet into the EC.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  TimeoutConsumer (void);
  // Default Constructor.

  virtual ~TimeoutConsumer (void);

  void connect (ACE_RW_Mutex* done,
                RtecScheduler::Scheduler_ptr scheduler,
                const char *entry_prefix,
                TimeBase::TimeT period,
                RtecScheduler::Importance_t importance,
                RtecScheduler::Criticality_t criticality,
                RtecEventComm::EventSourceID supplier_id,
                size_t to_send,
                const RtecEventComm::EventSet& events,
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

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  size_t _to_send; //number of times to push on timeout
  size_t _num_sent; //number of pushes so far
  int _hold_mtx; //1 when hold _done mutex; 0 else
  ACE_RW_Mutex* _done; //release read lock when _num_sent >= _to_send

  RtecEventComm::EventSourceID _supplier_id;
  // We generate an id based on the name....

  RtecEventChannelAdmin::ProxyPushConsumer_var _consumer_proxy;
  // We talk to the EC (as a supplier) using this proxy.

  ACE_PushSupplier_Adapter<TimeoutConsumer> _supplier;
  // We connect to the EC as a supplier so we can push events
  // every time we receive a timeout event.

  RtecEventChannelAdmin::ProxyPushSupplier_var _supplier_proxy;
  // We talk to the EC (as a consumer) using this proxy.

  ACE_PushConsumer_Adapter<TimeoutConsumer> _consumer;
  // We connect to the EC as a consumer so we can receive the
  // timeout events.

  RtecEventComm::EventSet _events;
  // set of events to push when a timeout event is received.
};

#endif /* CONSUMER_H */
