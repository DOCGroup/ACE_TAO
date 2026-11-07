/* -*- C++ -*- */
/**
 *  @file PushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PUSHSUPPLIER_H
#define TAO_Notify_Tests_PUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"

#include "Supplier_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_ProxyPushConsumer_Traits
{
public:
  using INTERFACE = CosNotifyChannelAdmin::ProxyPushConsumer;
  using PTR = CosNotifyChannelAdmin::ProxyPushConsumer_ptr;
  using VAR = CosNotifyChannelAdmin::ProxyPushConsumer_var;
  using ID = CosNotifyChannelAdmin::ProxyID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushSupplier_Traits
{
public:
  TAO_Notify_Tests_PushSupplier_Traits ();

  using INTERFACE = CosNotifyComm::PushSupplier;
  using PTR = CosNotifyComm::PushSupplier_ptr;
  using VAR = CosNotifyComm::PushSupplier_var;
  using SKELETON = POA_CosNotifyComm::PushSupplier;

  using Proxy_Traits = TAO_Notify_Tests_ProxyPushConsumer_Traits;
  using Admin_Traits = TAO_Notify_Tests_SupplierAdmin_Traits;
  using Admin_Ext_Traits = TAO_Notify_Tests_SupplierAdmin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

using TAO_Notify_Tests_PushSupplier_Base = TAO_Notify_Tests_Supplier_T<TAO_Notify_Tests_PushSupplier_Traits>;

/**
 * @class TAO_Notify_Tests_PushSupplier
 *
 * @brief PushSupplier Implementation.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushSupplier : public TAO_Notify_Tests_PushSupplier_Base
{
  using Peer_Traits = TAO_Notify_Tests_PushSupplier_Traits;
  using Admin_Traits = Peer_Traits::Admin_Traits;
  using Admin_Ext_Traits = Peer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = Peer_Traits::Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_PushSupplier ();

  /// Destructor
  virtual ~TAO_Notify_Tests_PushSupplier ();

  /// Send one event.
  virtual void send_event (const CORBA::Any & data);

protected:
  /// Connect to Peer.
  void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr) override;

  /// Disconnect from proxy.
  void disconnect_from_proxy () override;

  // = PushSupplier method
  void disconnect_push_supplier () override;
};

#if defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PUSHSUPPLIER_H */
