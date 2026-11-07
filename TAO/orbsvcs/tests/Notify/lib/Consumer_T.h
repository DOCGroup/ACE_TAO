/* -*- C++ -*- */
/**
 *  @file Consumer_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_CONSUMER_T_H
#define TAO_Notify_Tests_CONSUMER_T_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer_T.h"

class TAO_Notify_Tests_ConsumerAdmin_Traits
{
 public:
  using INTERFACE = CosNotifyChannelAdmin::ConsumerAdmin;
  using PTR = CosNotifyChannelAdmin::ConsumerAdmin_ptr;
  using VAR = CosNotifyChannelAdmin::ConsumerAdmin_var;
};

class TAO_Notify_Tests_ConsumerAdmin_Ext_Traits
{
 public:
  using INTERFACE = NotifyExt::ConsumerAdmin;
  using PTR = NotifyExt::ConsumerAdmin_ptr;
  using VAR = NotifyExt::ConsumerAdmin_var;
};

/**
 * @class TAO_Notify_Tests_Consumer_T
 *
 * @brief Base template to implement Consumers.
 */
template <class Consumer_Traits>
class TAO_Notify_Tests_Consumer_T : public TAO_Notify_Tests_Peer_T <Consumer_Traits>
{
  using Admin_Traits = typename Consumer_Traits::Admin_Traits;
  using Admin_Ext_Traits = typename Consumer_Traits::Admin_Ext_Traits;
  using Proxy_Traits = typename Consumer_Traits::Proxy_Traits;

  using Proxy_Traits_INTERFACE = typename Proxy_Traits::INTERFACE;
  using Proxy_Traits_PTR = typename Proxy_Traits::PTR;
  using Admin_Traits_PTR = typename Admin_Traits::PTR;
  using Admin_Ext_Traits_PTR = typename Admin_Ext_Traits::PTR;

public:
  /// Constructor
  TAO_Notify_Tests_Consumer_T ();

  /// Destructor
  ~TAO_Notify_Tests_Consumer_T ();

  /// For backward compatibility. use <get_proxy> instead.
  Proxy_Traits_PTR get_proxy_supplier ();

  /// Send subscription_change
  virtual void subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed);

protected:
  /// Obtain Proxy.
  Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr) override;

  /// Obtain Proxy with QoS.
  Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr,
                                 CosNotification::QoSProperties &qos) override;

  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;
};

#include "Consumer_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_CONSUMER_T_H */
