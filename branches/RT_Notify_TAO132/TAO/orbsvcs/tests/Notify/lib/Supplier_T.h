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

#ifndef TAO_NS_SUPPLIER_T_H
#define TAO_NS_SUPPLIER_T_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer_T.h"

class TAO_NS_SupplierAdmin_Traits
{
 public:
  typedef CosNotifyChannelAdmin::SupplierAdmin INTERFACE;
  typedef CosNotifyChannelAdmin::SupplierAdmin_ptr PTR;
  typedef CosNotifyChannelAdmin::SupplierAdmin_var VAR;
};

class TAO_NS_SupplierAdmin_Ext_Traits
{
 public:
  typedef NotifyExt::SupplierAdmin INTERFACE;
  typedef NotifyExt::SupplierAdmin_ptr PTR;
  typedef NotifyExt::SupplierAdmin_var VAR;
};

/**
 * @class TAO_NS_Supplier_T
 *
 * @brief
 *
 */
template <class Supplier_Traits>
class TAO_NOTIFY_TEST_Export TAO_NS_Supplier_T : public TAO_NS_Peer_T <Supplier_Traits>
{
  typedef ACE_TYPENAME Supplier_Traits::Admin_Traits Admin_Traits;
  typedef ACE_TYPENAME Supplier_Traits::Admin_Ext_Traits Admin_Ext_Traits;
  typedef ACE_TYPENAME Supplier_Traits::Proxy_Traits Proxy_Traits;

  typedef ACE_TYPENAME Proxy_Traits::INTERFACE Proxy_Traits_INTERFACE;
  typedef ACE_TYPENAME Proxy_Traits::PTR Proxy_Traits_PTR;
  typedef ACE_TYPENAME Admin_Traits::PTR Admin_Traits_PTR;
  typedef ACE_TYPENAME Admin_Ext_Traits::PTR Admin_Ext_Traits_PTR;

public:
  /// Constuctor
  TAO_NS_Supplier_T (void);

  /// Destructor
  ~TAO_NS_Supplier_T ();

  /// For backward compatibility. use <get_proxy> instead.
  Proxy_Traits_PTR get_proxy_consumer (void);

  /// Send offer change.
  void offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL);

protected:
  /// Obtain Proxy.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL);

  /// Obtain Proxy with QoS.
  virtual Proxy_Traits_PTR obtain_proxy (Admin_Ext_Traits_PTR admin_ptr
                                         , CosNotification::QoSProperties& qos
                                          ACE_ENV_ARG_DECL);

    // = NotifySubscribe
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));
};

#if defined (__ACE_INLINE__)
#include "Supplier_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Supplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Supplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIER_T_H */
