/* -*- C++ -*- */
/**
 *  @file SequencePushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H
#define TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"

#include "Consumer_T.h"

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequenceProxyPushSupplier_Traits
{
public:
  typedef CosNotifyChannelAdmin::SequenceProxyPushSupplier INTERFACE;
  typedef CosNotifyChannelAdmin::SequenceProxyPushSupplier_ptr PTR;
  typedef CosNotifyChannelAdmin::SequenceProxyPushSupplier_var VAR;
  typedef CosNotifyChannelAdmin::ProxyID ID;
};

class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushConsumer_Traits
{
public:
  TAO_Notify_Tests_SequencePushConsumer_Traits (void);

  typedef CosNotifyComm::SequencePushConsumer INTERFACE;
  typedef CosNotifyComm::SequencePushConsumer_ptr PTR;
  typedef CosNotifyComm::SequencePushConsumer_var VAR;
  typedef POA_CosNotifyComm::SequencePushConsumer SKELETON;

  typedef TAO_Notify_Tests_SequenceProxyPushSupplier_Traits Proxy_Traits;
  typedef TAO_Notify_Tests_ConsumerAdmin_Traits Admin_Traits;
  typedef TAO_Notify_Tests_ConsumerAdmin_Ext_Traits Admin_Ext_Traits;

  const CosNotifyChannelAdmin::ClientType type_;
};

typedef TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_SequencePushConsumer_Traits> TAO_Notify_Tests_SequencePushConsumer_Base;

/**
 * @class TAO_Notify_Tests_SequencePushConsumer
 *
 * @brief SequencePushConsumer Implementation.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SequencePushConsumer : public TAO_Notify_Tests_SequencePushConsumer_Base
{
  typedef TAO_Notify_Tests_SequencePushConsumer_Traits Peer_Traits;
  typedef Peer_Traits::Admin_Traits Admin_Traits;
  typedef Peer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef Peer_Traits::Proxy_Traits Proxy_Traits;

public:
  /// Constuctor
  TAO_Notify_Tests_SequencePushConsumer (void);

  /// Destructor
  virtual ~TAO_Notify_Tests_SequencePushConsumer ();

protected:
  /// Connect to Peer.
  virtual void connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL);

  /// Disconnect from proxy.
  virtual void disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL);

  // = SequencePushConsumer methods
  virtual void disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

 /// Default does nothing.
  virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));
};

#if defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SEQUENCEPUSHCONSUMER_H */
