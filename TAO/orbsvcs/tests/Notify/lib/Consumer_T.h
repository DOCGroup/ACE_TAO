/* -*- C++ -*- */
/**
 *  @file Consumer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_CONSUMER_T_H
#define TAO_NS_CONSUMER_T_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer_T.h"

class TAO_NS_ConsumerAdmin_Traits
{
 public:
  typedef CosNotifyChannelAdmin::ConsumerAdmin INTERFACE;
  typedef CosNotifyChannelAdmin::ConsumerAdmin_ptr PTR;
  typedef CosNotifyChannelAdmin::ConsumerAdmin_var VAR;
};

class TAO_NS_ConsumerAdmin_Ext_Traits
{
 public:
  typedef NotifyExt::ConsumerAdmin INTERFACE;
  typedef NotifyExt::ConsumerAdmin_ptr PTR;
  typedef NotifyExt::ConsumerAdmin_var VAR;
};

/**
 * @class TAO_NS_Consumer_T
 *
 * @brief Base template to implement Consumers.
 *
 */
template <class Consumer_Traits>
class TAO_NOTIFY_TEST_Export TAO_NS_Consumer_T : public TAO_NS_Peer_T <Consumer_Traits>
{
  typedef ACE_TYPENAME Consumer_Traits::Admin_Traits Admin_Traits;
  typedef ACE_TYPENAME Consumer_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef ACE_TYPENAME Consumer_Traits::Proxy_Traits Proxy_Traits;

  typedef ACE_TYPENAME Proxy_Traits::INTERFACE Proxy_Traits_INTERFACE;
  typedef ACE_TYPENAME Proxy_Traits::PTR Proxy_Traits_PTR;
  typedef ACE_TYPENAME Admin_Traits::PTR Admin_Traits_PTR;
  typedef ACE_TYPENAME Admin_Ext_Traits::PTR Admin_Ext_Traits_PTR;

public:
  /// Constuctor
  TAO_NS_Consumer_T (void);

  /// Destructor
  ~TAO_NS_Consumer_T ();

  /// For backward compatibility. use <get_proxy> instead.
  Proxy_Traits_PTR get_proxy_supplier (void);

  /// Send subscription_change
  virtual void subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL);

protected:
  /// Obtain Proxy.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL);

  /// Obtain Proxy with QoS.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr
                                         , CosNotification::QoSProperties& qos
                                          ACE_ENV_ARG_DECL);

  // = NotifyPublish method
  virtual void offer_change (const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));
};

#if defined (__ACE_INLINE__)
#include "Consumer_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Consumer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Consumer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_CONSUMER_T_H */
