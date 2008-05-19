/**
 * @file Supplier.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_SUPPLIER_H
#define TAO_PERF_RTEC_SUPPLIER_H

#include "rtec_perf_export.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Implement a simple supplier to keep track of the latency
 *
 */
class TAO_RTEC_Perf_Export Supplier
  : public virtual POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  /**
   * The experiment ID is used to configure the supplier ID on the
   * publication.
   */
  Supplier (CORBA::Long experiment_id,
            CORBA::Long event_type,
            CORBA::Long event_range,
            PortableServer::POA_ptr poa);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Disconnect from the event channel
  void disconnect (void);

  void push (const RtecEventComm::EventSet &events);

  //@{
  /** @name The RtecEventComm::PushSupplier methods
   */
  virtual void disconnect_push_supplier (void);
  virtual PortableServer::POA_ptr _default_POA (void);
  //@}

private:
  /// The experiment id
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The experiment id
  CORBA::Long experiment_id_;

  /// The event type
  CORBA::Long event_type_;

  /// The event type
  CORBA::Long event_range_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;

  /// The default poa
  PortableServer::POA_var default_POA_;
};

#endif /* TAO_PERF_RTEC_SUPPLIER_H */
