/* -*- C++ -*- */
/**
 *  @file SequencePushConsumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H
#define TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Consumer_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequenceProxyPushSupplier_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::SequenceProxyPushSupplier;
  using PTR = CosNotifyChannelAdmin::SequenceProxyPushSupplier_ptr;
  using VAR = CosNotifyChannelAdmin::SequenceProxyPushSupplier_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushConsumer_Traits
{
public:
  TAO_Notify_Tests_SequencePushConsumer_Traits ();

  using INTERFACE = CosNotifyComm::SequencePushConsumer;
  using PTR = CosNotifyComm::SequencePushConsumer_ptr;
  using VAR = CosNotifyComm::SequencePushConsumer_var;
  using SKELETON = POA_CosNotifyComm::SequencePushConsumer;

  using Proxy_Traits = TAO_Notify_Tests_SequenceProxyPushSupplier_Traits;
  using Admin_Traits = TAO_Notify_Tests_ConsumerAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_ConsumerAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_SequencePushConsumer_Base =
  TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_SequencePushConsumer_Traits>;

/**
 * @class TAO_Notify_Tests_SequencePushConsumer
 *
 * @brief SequencePushConsumer Implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushConsumer : public TAO_Notify_Tests_SequencePushConsumer_Base
{
  using Peer_Traits = TAO_Notify_Tests_SequencePushConsumer_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_SequencePushConsumer ();

  /// Destructor
  virtual ~TAO_Notify_Tests_SequencePushConsumer ();

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = SequencePushConsumer methods
  void disconnect_sequence_push_consumer () override;

 /// Default does nothing.
  void push_structured_events (
        CosNotification::EventBatch const &notifications) override;
};

#if defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H */
