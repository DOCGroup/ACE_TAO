/* -*- C++ -*- */
/**
 *  @file PushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
  typedef CosNotifyChannelAdmin::ProxyPushConsumer INTERFACE;
  typedef CosNotifyChannelAdmin::ProxyPushConsumer_ptr PTR;
  typedef CosNotifyChannelAdmin::ProxyPushConsumer_var VAR;
  typedef CosNotifyChannelAdmin::ProxyID ID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushSupplier_Traits
{
public:
  TAO_Notify_Tests_PushSupplier_Traits (void);

  typedef CosNotifyComm::PushSupplier INTERFACE;
  typedef CosNotifyComm::PushSupplier_ptr PTR;
  typedef CosNotifyComm::PushSupplier_var VAR;
  typedef POA_CosNotifyComm::PushSupplier SKELETON;

  typedef TAO_Notify_Tests_ProxyPushConsumer_Traits Proxy_Traits;
  typedef TAO_Notify_Tests_SupplierAdmin_Traits Admin_Traits;
  typedef TAO_Notify_Tests_SupplierAdmin_Ext_Traits Admin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

typedef TAO_Notify_Tests_Supplier_T<TAO_Notify_Tests_PushSupplier_Traits> TAO_Notify_Tests_PushSupplier_Base;

/**
 * @class TAO_Notify_Tests_PushSupplier
 *
 * @brief PushSupplier Implementation.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_PushSupplier : public TAO_Notify_Tests_PushSupplier_Base
{
  typedef TAO_Notify_Tests_PushSupplier_Traits Peer_Traits;
  typedef Peer_Traits::Admin_Traits Admin_Traits;
  typedef Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef Peer_Traits::Proxy_Traits Proxy_Traits;

public:
  /// Constructor
  TAO_Notify_Tests_PushSupplier (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_PushSupplier ();

  /// Send one event.
  virtual void send_event (const CORBA::Any & data);

protected:
  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr);

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (void);

  // = PushSupplier method
  virtual void disconnect_push_supplier (void);
};

#if defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PUSHSUPPLIER_H */
