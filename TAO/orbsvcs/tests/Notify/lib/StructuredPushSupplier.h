/* -*- C++ -*- */
/**
 *  @file StructuredPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_STRUCTUREDPUSHSUPPLIER_H
#define TAO_NS_STRUCTUREDPUSHSUPPLIER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"

#include "Supplier_T.h"

class TAO_NS_StructuredProxyPushConsumer_Traits
{
public:
  typedef CosNotifyChannelAdmin::StructuredProxyPushConsumer INTERFACE;
  typedef CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr PTR;
  typedef CosNotifyChannelAdmin::StructuredProxyPushConsumer_var VAR;
  typedef CosNotifyChannelAdmin::ProxyID ID;
};

class TAO_NS_StructuredPushSupplier_Traits
{
public:
  TAO_NS_StructuredPushSupplier_Traits (void);

  typedef CosNotifyComm::StructuredPushSupplier INTERFACE;
  typedef CosNotifyComm::StructuredPushSupplier_ptr PTR;
  typedef CosNotifyComm::StructuredPushSupplier_var VAR;
  typedef POA_CosNotifyComm::StructuredPushSupplier SKELETON;

  typedef TAO_NS_StructuredProxyPushConsumer_Traits Proxy_Traits;
  typedef TAO_NS_SupplierAdmin_Traits Admin_Traits;
  typedef TAO_NS_SupplierAdmin_Ext_Traits Admin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

typedef TAO_NS_Supplier_T<TAO_NS_StructuredPushSupplier_Traits> TAO_NS_StructuredPushSupplier_Base;

/**
 * @class TAO_NS_StructuredPushSupplier
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_StructuredPushSupplier : public TAO_NS_StructuredPushSupplier_Base
{
  typedef TAO_NS_StructuredPushSupplier_Traits Peer_Traits;
  typedef Peer_Traits::Admin_Traits Admin_Traits;
  typedef Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef Peer_Traits::Proxy_Traits Proxy_Traits;

public:
  /// Constuctor
  TAO_NS_StructuredPushSupplier (void);

  /// Destructor
  virtual ~TAO_NS_StructuredPushSupplier ();

  /// Send one event.
  virtual void send_event (const CosNotification::StructuredEvent& event
                           ACE_ENV_ARG_DECL);

protected:
  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL);

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL);

  // = StructuredPushSupplier method
  virtual void disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
};

#if defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPUSHSUPPLIER_H */
