/* -*- C++ -*- */
/**
 *  @file Supplier_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
  typedef CosNotifyChannelAdmin::SupplierAdmin INTERFACE;
  typedef CosNotifyChannelAdmin::SupplierAdmin_ptr PTR;
  typedef CosNotifyChannelAdmin::SupplierAdmin_var VAR;
};

class TAO_Notify_Tests_SupplierAdmin_Ext_Traits
{
 public:
  typedef NotifyExt::SupplierAdmin INTERFACE;
  typedef NotifyExt::SupplierAdmin_ptr PTR;
  typedef NotifyExt::SupplierAdmin_var VAR;
};

/**
 * @class TAO_Notify_Tests_Supplier_T
 *
 * @brief Base template to implement Suppliers.
 *
 */
template <class Supplier_Traits>
class TAO_Notify_Tests_Supplier_T : public TAO_Notify_Tests_Peer_T <Supplier_Traits>
{
  typedef typename Supplier_Traits::Admin_Traits Admin_Traits;
  typedef typename Supplier_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef typename Supplier_Traits::Proxy_Traits Proxy_Traits;

  typedef typename Proxy_Traits::INTERFACE Proxy_Traits_INTERFACE;
  typedef typename Proxy_Traits::PTR Proxy_Traits_PTR;
  typedef typename Admin_Traits::PTR Admin_Traits_PTR;
  typedef typename Admin_Ext_Traits::PTR Admin_Ext_Traits_PTR;

public:
  /// Constuctor
  TAO_Notify_Tests_Supplier_T (void);

  /// Destructor
  ~TAO_Notify_Tests_Supplier_T ();

  /// For backward compatibility. use <get_proxy> instead.
  Proxy_Traits_PTR get_proxy_consumer (void);

  /// Send offer change.
  void offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed);

protected:
  /// Obtain Proxy.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr);

  /// Obtain Proxy with QoS.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr
                                         , CosNotification::QoSProperties& qos);

    // = NotifySubscribe
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Supplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Supplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_SUPPLIER_T_H */
