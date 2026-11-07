/* -*- C++ -*- */
/**
 *  @file StructuredPushConsumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_STRUCTUREDPUSHCONSUMER_H
#define TAO_Notify_Tests_STRUCTUREDPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Consumer_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredProxyPushSupplier_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::StructuredProxyPushSupplier;
  using PTR = CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr;
  using VAR = CosNotifyChannelAdmin::StructuredProxyPushSupplier_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredPushConsumer_Traits
{
public:
  TAO_Notify_Tests_StructuredPushConsumer_Traits ();

  using INTERFACE = CosNotifyComm::StructuredPushConsumer;
  using PTR = CosNotifyComm::StructuredPushConsumer_ptr;
  using VAR = CosNotifyComm::StructuredPushConsumer_var;
  using SKELETON = POA_CosNotifyComm::StructuredPushConsumer;

  using Proxy_Traits = TAO_Notify_Tests_StructuredProxyPushSupplier_Traits;
  using Admin_Traits = TAO_Notify_Tests_ConsumerAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_ConsumerAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_StructuredPushConsumer_Base =
  TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_StructuredPushConsumer_Traits>;

/**
 * @class TAO_Notify_Tests_StructuredPushConsumer
 *
 * @brief StructuredPushConsumer Implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_StructuredPushConsumer
  : public TAO_Notify_Tests_StructuredPushConsumer_Base
{
  using Peer_Traits = TAO_Notify_Tests_StructuredPushConsumer_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_StructuredPushConsumer ();

  /// Destructor
  virtual ~TAO_Notify_Tests_StructuredPushConsumer ();

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = StructuredPushConsumer methods
  void disconnect_structured_push_consumer () override;

 /// Default does nothing.
  void push_structured_event (
        CosNotification::StructuredEvent const &notification) override;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_STRUCTUREDPUSHCONSUMER_H */
