/**
 * @file Consumer.h
 *
 * $Id$
 *
 */

#ifndef ECFS_CONSUMER_H
#define ECFS_CONSUMER_H

#include "ControlC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFS_Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class ECFS_Consumer
  : public virtual POA_RtecEventComm::PushConsumer
  , public virtual PortableServer::RefCountServantBase

{
public:
  /// Constructor
  ECFS_Consumer (CORBA::Long experiment_id,
                 CORBA::ULong iterations);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec
                ACE_ENV_ARG_DECL);

  /// Disconnect from the event channel
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Get the samples
  const Control::Samples &samples (void) const;

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

  /// The samples recorded so far
  Control::Samples samples_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;
};

#endif /* ECFS_CONSUMER_H */
