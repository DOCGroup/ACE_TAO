/**
 * @file Supplier.h
 *
 * $Id$
 *
 */

#ifndef ECL_SUPPLIER_H
#define ECL_SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECL_Supplier
 *
 * @brief Implement a simple supplier to keep track of the latency
 *
 */
class ECL_Supplier
  : public virtual POA_RtecEventComm::PushSupplier
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  ECL_Supplier (void);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::EventChannel_ptr ec,
                CORBA::Environment &ACE_TRY_ENV);

  /// Disconnect from the event channel
  void disconnect (CORBA::Environment &ACE_TRY_ENV);

  void push (const RtecEventComm::EventSet &events,
             CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@{
  /** @name The RtecEventComm::PushSupplier methods
   */
  virtual void disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// The proxy this object is connected to
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;
};

#endif /* ECL_SUPPLIER_H */
