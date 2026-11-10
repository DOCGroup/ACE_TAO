// -*- C++ -*-

/**
 *  @file ProxySupplier_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROXYSUPPLIER_T_H
#define TAO_Notify_PROXYSUPPLIER_T_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Proxy_T.h"
#include "orbsvcs/Notify/ProxySupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ProxySupplier_T
 *
 * @brief
 */
template <class SERVANT_TYPE>
class TAO_Notify_ProxySupplier_T : public virtual TAO_Notify_Proxy_T <SERVANT_TYPE>, public virtual TAO_Notify_ProxySupplier
{
public:
  TAO_Notify_ProxySupplier_T () = default;

  TAO_Notify_ProxySupplier_T (TAO_Notify_ProxySupplier_T const &) = delete;
  TAO_Notify_ProxySupplier_T (TAO_Notify_ProxySupplier_T &&) = delete;
  TAO_Notify_ProxySupplier_T &operator= (TAO_Notify_ProxySupplier_T const &) = delete;
  TAO_Notify_ProxySupplier_T &operator= (TAO_Notify_ProxySupplier_T &&) = delete;

  ~TAO_Notify_ProxySupplier_T () override = default;

  /// Notification of subscriptions set at the admin.
  void admin_types_changed (const CosNotification::EventTypeSeq & added,
                            const CosNotification::EventTypeSeq & removed) override;

  ///= POA_Notify_Internal methods
  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_structured (const CosNotification::StructuredEvent & event);

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_structured_no_filtering (const CosNotification::StructuredEvent & event);

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_any (const CORBA::Any & event);

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_any_no_filtering (const CORBA::Any & event);
protected:
  //= Data Members
  CORBA::Boolean is_suspended_ {};

  // = Interface methods
  CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin () override;

  void suspend_connection () override;

  void resume_connection () override;

  CosNotifyFilter::MappingFilter_ptr priority_filter () override;

  void priority_filter (CosNotifyFilter::MappingFilter_ptr priority_filter) override;

  CosNotifyFilter::MappingFilter_ptr lifetime_filter () override;

  void lifetime_filter (CosNotifyFilter::MappingFilter_ptr lifetime_filter) override;

  CosNotification::EventTypeSeq *obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode) override;

  void subscription_change (CosNotification::EventTypeSeq const &added,
                            CosNotification::EventTypeSeq const &removed) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/Notify/ProxySupplier_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYSUPPLIER_T_H */
