/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   ProxyPushConsumer_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the
//   CosEventChannelAdmin::ProxyPushConsumer interface.
//
// ======================================================================================

#if !defined (_PROXYPUSHCONSUMER_H)
#define _PROXYPUSHCONSUMER_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

class PushSupplierWrapper;

class TAO_ORBSVCS_Export ProxyPushConsumer_i : public POA_CosEventChannelAdmin::ProxyPushConsumer
{
  // = TITLE
  //   class ProxyPushConsumer_i implements the ProxyPushConsumer
  //   interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushConsumer uses the
  //   RtecEventChannelAdmin::ProxyPushConsumer.
  //
public:
  // = Initialization and termination methods.
  ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos,
                       RtecEventChannelAdmin::ProxyPushConsumer_ptr ppc);
  // Constructor.

  ~ProxyPushConsumer_i (void);
  // Destructor.

  virtual void push (const CORBA::Any &data,
                     CORBA::Environment &TAO_TRY_ENV);
  // Suppliers call this method to pass data to connected consumers.

  virtual void disconnect_push_consumer (CORBA::Environment &TAO_TRY_ENV);
  // Disconnects the supplier from the event communication.

  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier,
                                     CORBA::Environment &TAO_TRY_ENV);
  // Connects a push supplier.

private:
  int connected (void);
  // Returns 0 if a push_supplier is already connected to this
  // ProxyPushConsumer, otherwise it returns a 1.

  const RtecEventChannelAdmin::SupplierQOS &qos_;
  // The SupplierQOS specified by the user of this class.

  RtecEventChannelAdmin::ProxyPushConsumer_ptr ppc_;
  // The Rtec ProxyPushConsumer specified by the user of this class.

  PushSupplierWrapper *wrapper_;
  // The Rtec PushSupplier wrapper used by the Rtec proxyPushConsumer.
};

// @@ Pradeep, please don't use // here, but use /* ... */ instead.
// Please fix all uses of this in your code since it's not portable.
#endif // _PROXYPUSHCONSUMER_H
