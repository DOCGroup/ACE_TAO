/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Consumer
//
// = AUTHOR
//   Bryan A. Thrall (thrall@cse.wustl.edu)
//
// ============================================================================

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Channel_Clients_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of the events pushed by a TimeoutConsumer
  //   every timeout.
  //
  //   It simply registers for the event type specified in its connect()
  //   function.
  //
public:
  Consumer (void);
  // Default Constructor.

  virtual ~Consumer (void);

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char *entry_prefix,
                int consumer_id, //unique identifier
                long event_type,
                RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);
  // This method connects the consumer to the EC without setting anything
  //   in the RT_Info (such as period, criticality, etc.). The consumer
  //   subscribes to events with the specified event_type.

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char *entry_prefix,
                int consumer_id, //unique identifier
                long event_type,
                TimeBase::TimeT period,
                RtecScheduler::Importance_t importance,
                RtecScheduler::Criticality_t criticality,
                RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);
  // This method connects the consumer to the EC, setting RT_Info values
  //   for period, criticality, and importance. The consumer subscribes
  //   to events with the specified event_type.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC.

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

protected:
  void connect_impl (bool set_rtinfo, //true if should set RT_Info
                     RtecScheduler::Scheduler_ptr scheduler,
                     const char *entry_prefix,
                     int consumer_id, //unique identifier
                     long event_type,
                     TimeBase::TimeT period,
                     RtecScheduler::Importance_t importance,
                     RtecScheduler::Criticality_t criticality,
                     RtecEventChannelAdmin::EventChannel_ptr ec
                     ACE_ENV_ARG_DECL);
  // This method implements the Consumer::connect() methods; if the first
  //   parameter is false, then the RT_Info values are ignored. Otherwise,
  //   they are set.

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var _supplier_proxy;
  // We talk to the EC (as a consumer) using this proxy.

  ACE_PushConsumer_Adapter<Consumer> _consumer;
  // We connect to the EC as a consumer so we can receive the
  // timeout events.

  int _consumer_id;
};

#endif /* CONSUMER_H */
