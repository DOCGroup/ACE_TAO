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

class TAO_CosEC_PushConsumerWrapper;

class TAO_ORBSVCS_Export TAO_CosEC_ProxyPushSupplier_i :
  public POA_CosEventChannelAdmin::ProxyPushSupplier,
  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   class ProxyPushSupplier-i implements the ProxyPushConsumer interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushSupplier uses the
  //   RtecEventChannelAdmin::ProxyPushSupplier.
public:
  // = Initialization and termination methods.
  TAO_CosEC_ProxyPushSupplier_i (const RtecEventChannelAdmin::ConsumerQOS &qos,
                                 RtecEventChannelAdmin::ProxyPushSupplier_ptr pps);
  // Constructor.

  ~TAO_CosEC_ProxyPushSupplier_i (void);
  // Destructor.

  virtual void disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV);
  // Ends the event communication and disposes this object.

  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer,
                                     CORBA::Environment &ACE_TRY_ENV);
  // Connects the <push_consumer> to the Event Channel.

private:
  int connected (void);
  // Returns 0 if a push_consumer is already connected to this
  // ProxyPushSupplier, otherwise it returns a 1.

  const RtecEventChannelAdmin::ConsumerQOS &qos_;
  // The ConsumerQOS specified by the user of this class.

  RtecEventChannelAdmin::ProxyPushSupplier_var pps_;
  // The Rtec ProxyPushSupplier specified by the user of this class.

  TAO_CosEC_PushConsumerWrapper *wrapper_;
  // The Rtec PushConsumer wrapper used by the Rtec ProxyPushSupplier.
};

#endif /* _PROXYPUSHSUPPLIER_H */
