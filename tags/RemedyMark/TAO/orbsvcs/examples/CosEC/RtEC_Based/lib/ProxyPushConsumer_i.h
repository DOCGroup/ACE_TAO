/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   ProxyPushConsumer_i.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::ProxyPushConsumer interface.
 *
 *
 */
//=============================================================================


#ifndef _PROXYPUSHCONSUMER_I_H
#define _PROXYPUSHCONSUMER_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "rtec_cosec_export.h"

class TAO_CosEC_PushSupplierWrapper;

class TAO_RTEC_COSEC_Export TAO_CosEC_ProxyPushConsumer_i :
  public virtual POA_CosEventChannelAdmin::ProxyPushConsumer
{
  // = TITLE
  //   class TAO_CosEC_ProxyPushConsumer_i implements the ProxyPushConsumer
  //   interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushConsumer uses the
  //   RtecEventChannelAdmin::ProxyPushConsumer.
  //
  //   NOTE:  RtecEventChannelAdmin::ProxyPushConsumer::push method is
  //   passed a RtecEventComm::EventSet.The <EventHeader> field in that is
  //   initialized using the  the 1st <publications> from the <SupplierQOS>.
  //   so we assume that publications[0] is initialized.
  //
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_CosEC_ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos,
                                 RtecEventChannelAdmin::ProxyPushConsumer_ptr proxypushconsumer);

  /// Destructor.
  ~TAO_CosEC_ProxyPushConsumer_i (void);

  /// Suppliers call this method to pass data to connected consumers.
  virtual void push (const CORBA::Any &data);

  /// Disconnects the supplier from the event communication.
  virtual void disconnect_push_consumer (void);

  /// Connects a push supplier.
  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier);

private:
  /// Returns 0 if a push_supplier is already connected to this
  /// ProxyPushConsumer, otherwise it returns a 1.
  int connected (void);

  /// The SupplierQOS specified by the user of this class.
  const RtecEventChannelAdmin::SupplierQOS &qos_;

  /// The Rtec ProxyPushConsumer specified by the user of this class.
  RtecEventChannelAdmin::ProxyPushConsumer_var proxypushconsumer_;

  /// The Rtec PushSupplier wrapper used by the Rtec ProxyPushConsumer.
  TAO_CosEC_PushSupplierWrapper *wrapper_;
};

#include /**/ "ace/post.h"
#endif /* _PROXYPUSHCONSUMER_I_H */
