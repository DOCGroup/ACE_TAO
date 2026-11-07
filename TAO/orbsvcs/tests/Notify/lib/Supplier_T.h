/* -*- C++ -*- */
/**
 *  @file Supplier_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_SUPPLIER_T_H
#define TAO_Notify_Tests_SUPPLIER_T_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer_T.h"

class TAO_Notify_Tests_SupplierAdmin_Traits
{
 public:
  using INTERFACE = CosNotifyChannelAdmin::SupplierAdmin;
  using PTR = CosNotifyChannelAdmin::SupplierAdmin_ptr;
  using VAR = CosNotifyChannelAdmin::SupplierAdmin_var;
};

class TAO_Notify_Tests_SupplierAdmin_Ext_Traits
{
 public:
  using INTERFACE = NotifyExt::SupplierAdmin;
  using PTR = NotifyExt::SupplierAdmin_ptr;
  using VAR = NotifyExt::SupplierAdmin_var;
};

/**
 * @class TAO_Notify_Tests_Supplier_T
 *
 * @brief Base template to implement Suppliers.
 */
template <class Supplier_Traits>
class TAO_Notify_Tests_Supplier_T : public TAO_Notify_Tests_Peer_T <Supplier_Traits>
{
  using Admin_Traits = typename Supplier_Traits::Admin_Traits;
  using Admin_Ext_Traits = typename Supplier_Traits::Admin_Ext_Traits;
  using Proxy_Traits = typename Supplier_Traits::Proxy_Traits;

  using Proxy_Traits_INTERFACE = typename Proxy_Traits::INTERFACE;
  using Proxy_Traits_PTR = typename Proxy_Traits::PTR;
  using Admin_Traits_PTR = typename Admin_Traits::PTR;
  using Admin_Ext_Traits_PTR = typename Admin_Ext_Traits::PTR;

public:
  /// Constructor
  TAO_Notify_Tests_Supplier_T ();

  /// Destructor
  ~TAO_Notify_Tests_Supplier_T ();

  /// For backward compatibility. use <get_proxy> instead.
  Proxy_Traits_PTR get_proxy_consumer ();

  /// Send offer change.
  void offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed);

protected:
  /// Obtain Proxy.
  Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr) override;

  /// Obtain Proxy with QoS.
  Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr,
                                 CosNotification::QoSProperties &qos) override;

  // = NotifySubscribe
  void subscription_change (
      CosNotification::EventTypeSeq const &added,
      CosNotification::EventTypeSeq const &removed) override;
};

#include "Supplier_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SUPPLIER_T_H */
