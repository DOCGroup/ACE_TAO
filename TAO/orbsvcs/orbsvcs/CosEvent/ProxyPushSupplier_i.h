/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   ProxyPushSupplier_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the
//   CosEventChannelAdmin::ProxyPushSupplier interface.
//
// ======================================================================================

#if !defined (_PROXYPUSHSUPPLIER_H)
#define _PROXYPUSHSUPPLIER_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class TAO_ORBSVCS_Export ProxyPushSupplier_i : public POA_CosEventChannelAdmin::ProxyPushSupplier
{
  // = TITLE
  //   class ProxyPushSupplier-i implements the ProxyPushConsumer interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushSupplier uses the
  //   RtecEventChannelAdmin::ProxyPushSupplier.
public:
  // = Initialization and termination methods.
  ProxyPushSupplier_i (const RtecEventChannelAdmin::ConsumerQOS &qos,
                       RtecEventChannelAdmin::ProxyPushSupplier_ptr pps);
  // Constructor.

  ~ProxyPushSupplier_i (void);
  // Destructor.

  virtual void disconnect_push_supplier (CORBA::Environment &TAO_TRY_ENV);
  // @@ Pradeep, please add comments.

  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer,
                                     CORBA::Environment &TAO_TRY_ENV);
  // @@ Pradeep, please add comments.

private:
  const RtecEventChannelAdmin::ConsumerQOS &qos_;
  RtecEventChannelAdmin::ProxyPushSupplier_ptr pps_;
};

// @@ Pradeep, please don't use // here, but use /* ... */ instead.
// Please fix all uses of this in your code since it's not portable.
#endif //_PROXYPUSHSUPPLIER_H
