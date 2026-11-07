/* -*- C++ -*- */
/**
 *  @file PushConsumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PUSHCONSUMER_H
#define TAO_Notify_Tests_PUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Consumer_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_ProxyPushSupplier_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::ProxyPushSupplier;
  using PTR = CosNotifyChannelAdmin::ProxyPushSupplier_ptr;
  using VAR = CosNotifyChannelAdmin::ProxyPushSupplier_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushConsumer_Traits
{
public:
  TAO_Notify_Tests_PushConsumer_Traits ();

  using INTERFACE = CosNotifyComm::PushConsumer;
  using PTR = CosNotifyComm::PushConsumer_ptr;
  using VAR = CosNotifyComm::PushConsumer_var;
  using SKELETON = POA_CosNotifyComm::PushConsumer;

  using Proxy_Traits = TAO_Notify_Tests_ProxyPushSupplier_Traits;
  using Admin_Traits = TAO_Notify_Tests_ConsumerAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_ConsumerAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_PushConsumer_Base =
  TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_PushConsumer_Traits>;

/**
 * @class TAO_Notify_Tests_PushConsumer
 *
 * @brief PushConsumer implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushConsumer : public TAO_Notify_Tests_PushConsumer_Base
{
  using Peer_Traits = TAO_Notify_Tests_PushConsumer_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_PushConsumer ();

  /// Destructor
  virtual ~TAO_Notify_Tests_PushConsumer ();

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = PushConsumer methods
  void disconnect_push_consumer () override;

  /// Default does nothing.
  void push (CORBA::Any const &data) override;
};

#if defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PUSHCONSUMER_H */
