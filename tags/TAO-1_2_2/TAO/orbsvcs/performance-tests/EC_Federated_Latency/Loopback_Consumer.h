/**
 * @file Loopback_Consumer.h
 *
 * $Id$
 *
 */

#ifndef ECFL_LOOPBACK_CONSUMER_H
#define ECFL_LOOPBACK_CONSUMER_H

#include "Loopback_Supplier.h"
#include "Servant_var.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFL_Loopback_Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class ECFL_Loopback_Consumer
  : public virtual POA_RtecEventComm::PushConsumer
  , public virtual PortableServer::RefCountServantBase

{
public:
  /// Constructor
  ECFL_Loopback_Consumer (CORBA::Long experiment_id,
                          ECFL_Loopback_Supplier *supplier);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);

  /// Disconnect from the event channel
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

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

  /// The experiment id
  CORBA::Long experiment_id_;

  /// The supplier used to close the loopback
  Servant_var<ECFL_Loopback_Supplier> supplier_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;
};

#endif /* ECFL_LOOPBACK_CONSUMER_H */
