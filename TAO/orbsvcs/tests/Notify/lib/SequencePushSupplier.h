/* -*- C++ -*- */
/**
 *  @file SequencePushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H
#define TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Supplier_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequenceProxyPushConsumer_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::SequenceProxyPushConsumer;
  using PTR = CosNotifyChannelAdmin::SequenceProxyPushConsumer_ptr;
  using VAR = CosNotifyChannelAdmin::SequenceProxyPushConsumer_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushSupplier_Traits
{
public:
  TAO_Notify_Tests_SequencePushSupplier_Traits ();

  using INTERFACE = CosNotifyComm::SequencePushSupplier;
  using PTR = CosNotifyComm::SequencePushSupplier_ptr;
  using VAR = CosNotifyComm::SequencePushSupplier_var;
  using SKELETON = POA_CosNotifyComm::SequencePushSupplier;

  using Proxy_Traits = TAO_Notify_Tests_SequenceProxyPushConsumer_Traits;
  using Admin_Traits = TAO_Notify_Tests_SupplierAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_SupplierAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_SequencePushSupplier_Base =
  TAO_Notify_Tests_Supplier_T<TAO_Notify_Tests_SequencePushSupplier_Traits>;

/**
 * @class TAO_Notify_Tests_SequencePushSupplier
 *
 * @brief SequencePushSupplier Implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushSupplier : public TAO_Notify_Tests_SequencePushSupplier_Base
{
  using Peer_Traits = TAO_Notify_Tests_SequencePushSupplier_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_SequencePushSupplier ();

  /// Destructor
  virtual ~TAO_Notify_Tests_SequencePushSupplier ();

  /// Send one event.
  virtual void send_events (const CosNotification::EventBatch& events);

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = SequencePushSupplier method
  void disconnect_sequence_push_supplier () override;
};

#if defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H */
