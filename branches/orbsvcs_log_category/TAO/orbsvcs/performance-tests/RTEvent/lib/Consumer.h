/**
 * @file Consumer.h
 *
 * $Id$
 *
 */

#ifndef TAO_PERF_RTEC_CONSUMER_H
#define TAO_PERF_RTEC_CONSUMER_H

#include "rtec_perf_export.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#include "ace/Sample_History.h"
#include "ace/High_Res_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class TAO_RTEC_Perf_Export Consumer
  : public virtual POA_RtecEventComm::PushConsumer

{
public:
  /// Constructor
  Consumer (CORBA::Long experiment_id,
            CORBA::Long event_type,
            CORBA::ULong iterations,
            CORBA::Long workload_in_usecs,
            ACE_High_Res_Timer::global_scale_factor_type gsf,
            PortableServer::POA_ptr poa);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Disconnect from the event channel
  void disconnect (void);

  /// Access the history of samples
  ACE_Sample_History &sample_history (void);

  //@{
  /** @name The RtecEventComm::PushConsumer methods
   */
  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);
  virtual PortableServer::POA_ptr _default_POA (void);
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
  ACE_High_Res_Timer::global_scale_factor_type gsf_;

  /// The default poa
  PortableServer::POA_var default_POA_;
};

#endif /* TAO_PERF_RTEC_CONSUMER_H */
