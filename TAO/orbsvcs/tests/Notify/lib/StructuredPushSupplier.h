/* -*- C++ -*- */
/**
 *  @file StructuredPushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_STRUCTUREDPUSHSUPPLIER_H
#define TAO_Notify_Tests_STRUCTUREDPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Supplier_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredProxyPushConsumer_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::StructuredProxyPushConsumer;
  using PTR = CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr;
  using VAR = CosNotifyChannelAdmin::StructuredProxyPushConsumer_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredPushSupplier_Traits
{
public:
  TAO_Notify_Tests_StructuredPushSupplier_Traits ();

  using INTERFACE = CosNotifyComm::StructuredPushSupplier;
  using PTR = CosNotifyComm::StructuredPushSupplier_ptr;
  using VAR = CosNotifyComm::StructuredPushSupplier_var;
  using SKELETON = POA_CosNotifyComm::StructuredPushSupplier;

  using Proxy_Traits = TAO_Notify_Tests_StructuredProxyPushConsumer_Traits;
  using Admin_Traits = TAO_Notify_Tests_SupplierAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_SupplierAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_StructuredPushSupplier_Base =
  TAO_Notify_Tests_Supplier_T<TAO_Notify_Tests_StructuredPushSupplier_Traits>;

/**
 * @class TAO_Notify_Tests_StructuredPushSupplier
 *
 * @brief StructuredPushSupplier Implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredPushSupplier
  : public TAO_Notify_Tests_StructuredPushSupplier_Base
{
  using Peer_Traits = TAO_Notify_Tests_StructuredPushSupplier_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_StructuredPushSupplier ();

  /// Destructor
  virtual ~TAO_Notify_Tests_StructuredPushSupplier ();

  /// Send one event.
  virtual void send_event (const CosNotification::StructuredEvent& event);

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = StructuredPushSupplier method
  void disconnect_structured_push_supplier () override;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_STRUCTUREDPUSHSUPPLIER_H */
