/* -*- C++ -*- */
/**
 *  @file PushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PUSHCONSUMER_H
#define TAO_NS_PUSHCONSUMER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"

#include "Consumer_T.h"

class TAO_NS_ProxyPushSupplier_Traits
{
public:
  typedef CosNotifyChannelAdmin::ProxyPushSupplier INTERFACE;
  typedef CosNotifyChannelAdmin::ProxyPushSupplier_ptr PTR;
  typedef CosNotifyChannelAdmin::ProxyPushSupplier_var VAR;
  typedef CosNotifyChannelAdmin::ProxyID ID;
};

class TAO_NS_PushConsumer_Traits
{
public:
  TAO_NS_PushConsumer_Traits (void);

  typedef CosNotifyComm::PushConsumer INTERFACE;
  typedef CosNotifyComm::PushConsumer_ptr PTR;
  typedef CosNotifyComm::PushConsumer_var VAR;
  typedef POA_CosNotifyComm::PushConsumer SKELETON;

  typedef TAO_NS_ProxyPushSupplier_Traits Proxy_Traits;
  typedef TAO_NS_ConsumerAdmin_Traits Admin_Traits;
  typedef TAO_NS_ConsumerAdmin_Ext_Traits Admin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

typedef TAO_NS_Consumer_T<TAO_NS_PushConsumer_Traits> TAO_NS_PushConsumer_Base;

/**
 * @class TAO_NS_PushConsumer
 *
 * @brief
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_PushConsumer : public TAO_NS_PushConsumer_Base
{
  typedef TAO_NS_PushConsumer_Traits Peer_Traits;
  typedef Peer_Traits::Admin_Traits Admin_Traits;
  typedef Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef Peer_Traits::Proxy_Traits Proxy_Traits;

public:
  /// Constuctor
  TAO_NS_PushConsumer (void);

  /// Destructor
  virtual ~TAO_NS_PushConsumer ();

protected:
  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL);

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL);

  // = PushConsumer methods
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

 /// Default does nothing.
    void push (
        const CORBA::Any & data
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ));
};

#if defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PUSHCONSUMER_H */
