/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushConsumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H
#define TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

#include "orbsvcs/Notify/ProxyConsumer_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_StructuredProxyPushConsumer
 *
 * @brief CosNotifyChannelAdmin::StructuredProxyPushConsumer implementation.
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>
{
  using SuperClass = TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>;
  friend class TAO_Notify_Builder;

public:
  /// Constructor
  TAO_Notify_StructuredProxyPushConsumer ();

  /// Destructor
  ~TAO_Notify_StructuredProxyPushConsumer () override = default;

  TAO_Notify_StructuredProxyPushConsumer (TAO_Notify_StructuredProxyPushConsumer const &) = delete;
  TAO_Notify_StructuredProxyPushConsumer (TAO_Notify_StructuredProxyPushConsumer &&) = delete;
  TAO_Notify_StructuredProxyPushConsumer &operator= (TAO_Notify_StructuredProxyPushConsumer const &) = delete;
  TAO_Notify_StructuredProxyPushConsumer &operator= (TAO_Notify_StructuredProxyPushConsumer &&) = delete;

  void load_attrs (TAO_Notify::NVPList const &attrs) override;
  void validate () override;
  virtual void configure(TAO_Notify_SupplierAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

protected:
  // = interface methods
  CosNotifyChannelAdmin::ProxyType MyType () override;

  void connect_structured_push_supplier (
    CosNotifyComm::StructuredPushSupplier_ptr push_supplier) override;

  void push_structured_event (CosNotification::StructuredEvent const &notification) override;

  void disconnect_structured_push_consumer () override;

  char const * get_proxy_type_name () const override;

private:
  /// Release
  void release () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_STRUCTUREDPROXYPUSHCONSUMER_H */
