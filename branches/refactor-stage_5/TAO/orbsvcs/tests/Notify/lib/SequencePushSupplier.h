/* -*- C++ -*- */
/**
 *  @file SequencePushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H
#define TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"

#include "Supplier_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequenceProxyPushConsumer_Traits
{
public:
  typedef CosNotifyChannelAdmin::SequenceProxyPushConsumer INTERFACE;
  typedef CosNotifyChannelAdmin::SequenceProxyPushConsumer_ptr PTR;
  typedef CosNotifyChannelAdmin::SequenceProxyPushConsumer_var VAR;
  typedef CosNotifyChannelAdmin::ProxyID ID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushSupplier_Traits
{
public:
  TAO_Notify_Tests_SequencePushSupplier_Traits (void);

  typedef CosNotifyComm::SequencePushSupplier INTERFACE;
  typedef CosNotifyComm::SequencePushSupplier_ptr PTR;
  typedef CosNotifyComm::SequencePushSupplier_var VAR;
  typedef POA_CosNotifyComm::SequencePushSupplier SKELETON;

  typedef TAO_Notify_Tests_SequenceProxyPushConsumer_Traits Proxy_Traits;
  typedef TAO_Notify_Tests_SupplierAdmin_Traits Admin_Traits;
  typedef TAO_Notify_Tests_SupplierAdmin_Ext_Traits Admin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

typedef TAO_Notify_Tests_Supplier_T<TAO_Notify_Tests_SequencePushSupplier_Traits> TAO_Notify_Tests_SequencePushSupplier_Base;

/**
 * @class TAO_Notify_Tests_SequencePushSupplier
 *
 * @brief SequencePushSupplier Implementation.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushSupplier : public TAO_Notify_Tests_SequencePushSupplier_Base
{
  typedef TAO_Notify_Tests_SequencePushSupplier_Traits Peer_Traits;
  typedef Peer_Traits::Admin_Traits Admin_Traits;
  typedef Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef Peer_Traits::Proxy_Traits Proxy_Traits;

public:
  /// Constuctor
  TAO_Notify_Tests_SequencePushSupplier (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_SequencePushSupplier ();

  /// Send one event.
  virtual void send_events (const CosNotification::EventBatch& events
                            ACE_ENV_ARG_DECL);

protected:
  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL);

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL);

  // = SequencePushSupplier method
  virtual void disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
};

#if defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SEQUENCEPUSHSUPPLIER_H */
