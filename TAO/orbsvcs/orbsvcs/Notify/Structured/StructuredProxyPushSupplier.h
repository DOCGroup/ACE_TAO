/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Event_ForwarderS.h"

#include "orbsvcs/Notify/ProxySupplier_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_Event_Forwarder::StructuredProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_StructuredProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::StructuredProxyPushSupplier methods.
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::StructuredProxyPushSupplier>
{
  typedef TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::StructuredProxyPushSupplier> SuperClass;
  friend class TAO_Notify_Builder;

public:
  /// Constructor
  TAO_Notify_StructuredProxyPushSupplier ();

  /// Destructor
  ~TAO_Notify_StructuredProxyPushSupplier () override;


  void load_attrs (const TAO_Notify::NVPList& attrs) override;
  void validate () override;

  /// = Servant methods
  // = interface methods
  CosNotifyChannelAdmin::ProxyType MyType () override;

  void connect_structured_push_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer
  ) override;

  void disconnect_structured_push_supplier (
  ) override;

  const char * get_proxy_type_name () const override;

  virtual void configure(TAO_Notify_ConsumerAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

private:
  /// Release
  void release () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H */
