/**
 * @file Loopback_Consumer.h
 *
 * $Id$
 *
 */

#ifndef TAO_PERF_RTEC_LOOPBACK_CONSUMER_H
#define TAO_PERF_RTEC_LOOPBACK_CONSUMER_H

#include "Loopback_Supplier.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Loopback_Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class TAO_RTEC_Perf_Export Loopback_Consumer
  : public virtual POA_RtecEventComm::PushConsumer

{
public:
  /// Constructor
  Loopback_Consumer (CORBA::Long experiment_id,
                     CORBA::Long event_type,
                     Loopback_Supplier *supplier,
                     PortableServer::POA_ptr poa);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Disconnect from the event channel
  void disconnect (void);

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

  /// The experiment id
  CORBA::Long experiment_id_;

  /// The event type this consumer subscribes to
  CORBA::Long event_type_;

  /// The supplier used to close the loopback
  PortableServer::Servant_var<Loopback_Supplier> supplier_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;

  /// The default poa
  PortableServer::POA_var default_POA_;
};

#endif /* ECFL_LOOPBACK_CONSUMER_H */
