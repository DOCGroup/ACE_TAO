/**
 * @file Consumer.h
 *
 * $Id$
 *
 */

#ifndef TAO_PERF_RTEC_CONSUMER_H
#define TAO_PERF_RTEC_CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#include "ace/Sample_History.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class Consumer
  : public virtual POA_RtecEventComm::PushConsumer
  , public virtual PortableServer::RefCountServantBase

{
public:
  /// Constructor
  Consumer (CORBA::Long experiment_id,
            CORBA::Long event_type,
            CORBA::ULong iterations,
            CORBA::Long workload_in_usecs,
            ACE_UINT32 gsf);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);

  /// Disconnect from the event channel
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Access the history of samples
  ACE_Sample_History &sample_history (void);

  //@{
  /** @name The RtecEventComm::PushConsumer methods
   */
  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;

  /// The experiment id
  CORBA::Long experiment_id_;

  /// The event type
  CORBA::Long event_type_;

  /// The history of latency samples
  ACE_Sample_History sample_history_;

  /// The time spent processing each event, in microseconds
  CORBA::ULong workload_in_usecs_;

  /// The global scale factor for the high resolution timers
  ACE_UINT32 gsf_;
};

#endif /* TAO_PERF_RTEC_CONSUMER_H */
