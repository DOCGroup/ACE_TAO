/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * Defines a simple Push Supplier.
 *
 *
 */
//=============================================================================


#ifndef COSECSUPPLIER_H_
#define COSECSUPPLIER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

/**
 * @class Supplier
 *
 * @brief Supplier
 *
 * The Supplier is a simple Push Supplier that connects to
 * the CosEC and sends events to it.
 */
class Supplier : public POA_CosEventComm::PushSupplier
{
public:
  /// This method connects the supplier to the EC.
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel);

  /// Disconnect from the EC.
  void close (void);

  void connect (void);

  /// Disconnect from the EC, but do not forget about it or close it.
  void disconnect (void);

  /// Send one event.
  void send_event (const CORBA::Any &data);

  /// The POA_CosEventComm::PushSupplier method.
  virtual void disconnect_push_supplier (void);

private:
  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;
};

#endif /* COSECSUPPLIER_H_ */
