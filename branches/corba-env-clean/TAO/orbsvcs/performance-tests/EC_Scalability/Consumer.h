/**
 * @file Consumer.h
 *
 * $Id$
 *
 */

#ifndef ECS_CONSUMER_H
#define ECS_CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Sample_History.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECS_Consumer
 *
 * @brief Implement a simple consumer to keep track of the latency
 *
 */
class ECS_Consumer
  : public virtual POA_RtecEventComm::PushConsumer
  , public virtual PortableServer::RefCountServantBase

{
public:
  /// Constructor
  ECS_Consumer (int iterations);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec,
                CORBA::Environment &ACE_TRY_ENV);

  /// Disconnect from the event channel
  void disconnect (CORBA::Environment &ACE_TRY_ENV);

  /// Access the history of samples
  ACE_Sample_History &sample_history (void);

  //@{
  /** @name The RtecEventComm::PushConsumer methods
   */
  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;

  /// The history of latency samples
  ACE_Sample_History sample_history_;
};

#endif /* ECS_CONSUMER_H */
