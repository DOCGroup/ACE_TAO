/* -*- C++ -*- */
/**
 *  @file SequenceProxyPushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H
#define TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

#include "orbsvcs/Notify/ProxySupplier_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_SequenceProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::SequenceProxyPushSupplier methods.
 */
class TAO_Notify_Serv_Export TAO_Notify_SequenceProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
{
  using SuperClass = TAO_Notify_ProxySupplier_T <POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
  friend class TAO_Notify_Builder;

public:
  /// Constructor
  TAO_Notify_SequenceProxyPushSupplier ();

  /// Destructor
  ~TAO_Notify_SequenceProxyPushSupplier () override = default;

  TAO_Notify_SequenceProxyPushSupplier (TAO_Notify_SequenceProxyPushSupplier const &) = delete;
  TAO_Notify_SequenceProxyPushSupplier (TAO_Notify_SequenceProxyPushSupplier &&) = delete;
  TAO_Notify_SequenceProxyPushSupplier &operator= (TAO_Notify_SequenceProxyPushSupplier const &) = delete;
  TAO_Notify_SequenceProxyPushSupplier &operator= (TAO_Notify_SequenceProxyPushSupplier &&) = delete;


  char const * get_proxy_type_name () const override;

  void load_attrs (TAO_Notify::NVPList const &attrs) override;
  void validate () override;

  virtual void configure(TAO_Notify_ConsumerAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

  /// = Servant methods
  CosNotifyChannelAdmin::ProxyType MyType () override;

  void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer) override;

  void disconnect_sequence_push_supplier () override;

  /// TAO_Notify_Destroy_Callback methods
  void release () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_SEQUENCEPROXYPUSHSUPPLIER_H */
