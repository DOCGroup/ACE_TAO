/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   ProxyPushConsumer_i.h
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::ProxyPushConsumer interface.
 */
//=============================================================================

#ifndef _PROXYPUSHCONSUMER_I_H
#define _PROXYPUSHCONSUMER_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "rtec_cosec_export.h"

class TAO_CosEC_PushSupplierWrapper;

/**
 * @brief Implements the Cos ProxyPushConsumer interface using the Rtec
 * ProxyPushConsumer.
 *
 * The Rtec push method receives an EventSet whose EventHeader is initialized
 * from the first publication in the SupplierQOS; this implementation assumes
 * that publications[0] is initialized.
 */
class TAO_RTEC_COSEC_Export TAO_CosEC_ProxyPushConsumer_i :
  public virtual POA_CosEventChannelAdmin::ProxyPushConsumer
{
public:
  TAO_CosEC_ProxyPushConsumer_i (RtecEventChannelAdmin::SupplierQOS const &qos,
                                 RtecEventChannelAdmin::ProxyPushConsumer_ptr proxypushconsumer);

  ~TAO_CosEC_ProxyPushConsumer_i () override = default;
  TAO_CosEC_ProxyPushConsumer_i (TAO_CosEC_ProxyPushConsumer_i const &) = delete;
  TAO_CosEC_ProxyPushConsumer_i (TAO_CosEC_ProxyPushConsumer_i &&) = delete;
  TAO_CosEC_ProxyPushConsumer_i &operator= (TAO_CosEC_ProxyPushConsumer_i const &) = delete;
  TAO_CosEC_ProxyPushConsumer_i &operator= (TAO_CosEC_ProxyPushConsumer_i &&) = delete;

  /// Suppliers call this method to pass data to connected consumers.
  void push (CORBA::Any const &data) override;

  /// Disconnects the supplier from the event communication.
  void disconnect_push_consumer () override;

  /// Connects a push supplier.
  void connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier) override;

private:
  /// Returns 0 if a push_supplier is already connected to this
  /// ProxyPushConsumer, otherwise it returns a 1.
  bool connected () const;

  /// The SupplierQOS specified by the user of this class.
  RtecEventChannelAdmin::SupplierQOS const &qos_;

  /// The Rtec ProxyPushConsumer specified by the user of this class.
  RtecEventChannelAdmin::ProxyPushConsumer_var proxypushconsumer_;

  /// The Rtec PushSupplier wrapper used by the Rtec ProxyPushConsumer.
  TAO_CosEC_PushSupplierWrapper *wrapper_ {};
};

#include /**/ "ace/post.h"
#endif /* _PROXYPUSHCONSUMER_I_H */
