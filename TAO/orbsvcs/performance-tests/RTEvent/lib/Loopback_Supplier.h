/**
 * @file Loopback_Supplier.h
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_RTEC_LOOPBACK_SUPPLIER_H
#define TAO_RTEC_LOOPBACK_SUPPLIER_H

#include "rtec_perf_export.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Loopback_Supplier
 *
 * @brief Implement a simple supplier to keep track of the latency
 */
class TAO_RTEC_Perf_Export Loopback_Supplier
  : public virtual POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  /**
   * The experiment ID is used to configure the supplier ID on the
   * publication.
   */
  Loopback_Supplier (CORBA::Long experiment_id,
                     CORBA::Long response_type,
                     PortableServer::POA_ptr poa);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Disconnect from the event channel
  void disconnect ();

  void push (const RtecEventComm::EventSet &events);

  //@{
  /** @name The RtecEventComm::PushSupplier methods
   */
  virtual void disconnect_push_supplier ();
  virtual PortableServer::POA_ptr _default_POA ();
  //@}

private:
  /// The experiment id
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The experiment id
  CORBA::Long experiment_id_;

  /// The response event type
  CORBA::Long response_type_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;

  /// The default poa
  PortableServer::POA_var default_POA_;

  /// Count the number of events processed
  CORBA::ULong counter_;
};

#endif /* TAO_RTEC_LOOPBACK_SUPPLIER_H */
