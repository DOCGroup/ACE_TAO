/**
 * @file Consumer.h
 *
 * $Id$
 *
 */

#ifndef ECFL_CONSUMER_H
#define ECFL_CONSUMER_H

#include "ControlC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFL_Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class ECFL_Consumer
  : public virtual POA_RtecEventComm::PushConsumer
  , public virtual PortableServer::RefCountServantBase

{
public:
  /// Constructor
  ECFL_Consumer (CORBA::Long experiment_id,
                 CORBA::ULong iterations);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec
                TAO_ENV_ARG_DECL);

  /// Disconnect from the event channel
  void disconnect (TAO_ENV_SINGLE_ARG_DECL);

  /// Get the samples
  const Control::Samples &samples (void) const;

  //@{
  /** @name The RtecEventComm::PushConsumer methods
   */
  virtual void push (const RtecEventComm::EventSet& events
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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

#endif /* ECFL_CONSUMER_H */
