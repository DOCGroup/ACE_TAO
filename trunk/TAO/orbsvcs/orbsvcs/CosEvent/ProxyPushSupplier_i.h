/* -*- C++ -*- */
// $Id$

// ================================================================
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
// ================================================================

#if !defined (_PROXYPUSHSUPPLIER_H)
#define _PROXYPUSHSUPPLIER_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class PushConsumerWrapper;

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
  // Ends the event communication and disposes this object.

  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer,
                                     CORBA::Environment &TAO_TRY_ENV);
  // Connects the <push_consumer> to the Event Channel.

private:
  int connected (void);
  // Returns 0 if a push_consumer is already connected to this
  // ProxyPushSupplier, otherwise it returns a 1.

  const RtecEventChannelAdmin::ConsumerQOS &qos_;
  // The ConsumerQOS specified by the user of this class.

  RtecEventChannelAdmin::ProxyPushSupplier_ptr pps_;
  // The Rtec ProxyPushSupplier specified by the user of this class.

  PushConsumerWrapper *wrapper_;
  // The Rtec PushConsumer wrapper used by the Rtec ProxyPushSupplier.
};

#endif /* _PROXYPUSHSUPPLIER_H */
