/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   ProxyPushSupplier_i.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::ProxyPushSupplier interface.
 */
//=============================================================================


#ifndef _PROXYPUSHSUPPLIER_H
#define _PROXYPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "rtec_cosec_export.h"

class TAO_CosEC_PushConsumerWrapper;

class TAO_RTEC_COSEC_Export TAO_CosEC_ProxyPushSupplier_i :
  public POA_CosEventChannelAdmin::ProxyPushSupplier
{
  // = TITLE
  //   class ProxyPushSupplier-i implements the ProxyPushConsumer interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushSupplier uses the
  //   RtecEventChannelAdmin::ProxyPushSupplier.
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_CosEC_ProxyPushSupplier_i (const RtecEventChannelAdmin::ConsumerQOS &qos,
                                 RtecEventChannelAdmin::ProxyPushSupplier_ptr pps);

  /// Destructor.
  ~TAO_CosEC_ProxyPushSupplier_i (void);

  /// Ends the event communication and disposes this object.
  virtual void disconnect_push_supplier (void);

  /// Connects the <push_consumer> to the Event Channel.
  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer);

private:
  /// Returns 0 if a push_consumer is already connected to this
  /// ProxyPushSupplier, otherwise it returns a 1.
  int connected (void);

  /// The ConsumerQOS specified by the user of this class.
  const RtecEventChannelAdmin::ConsumerQOS &qos_;

  /// The Rtec ProxyPushSupplier specified by the user of this class.
  RtecEventChannelAdmin::ProxyPushSupplier_var pps_;

  /// The Rtec PushConsumer wrapper used by the Rtec ProxyPushSupplier.
  TAO_CosEC_PushConsumerWrapper *wrapper_;
};

#include /**/ "ace/post.h"
#endif /* _PROXYPUSHSUPPLIER_H */
