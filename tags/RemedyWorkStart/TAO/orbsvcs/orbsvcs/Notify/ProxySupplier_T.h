// -*- C++ -*-

/**
 *  @file ProxySupplier_T.h
 *
 *  $Id$
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
 *
 */
template <class SERVANT_TYPE>
class TAO_Notify_ProxySupplier_T : public virtual TAO_Notify_Proxy_T <SERVANT_TYPE>, public virtual TAO_Notify_ProxySupplier
{
public:
  /// Constructor
  TAO_Notify_ProxySupplier_T (void);

  /// Destructor
  ~TAO_Notify_ProxySupplier_T ();

  /// Notification of subscriptions set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed);

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
  CORBA::Boolean is_suspended_;

  // = Interface methods
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
  );

  virtual void suspend_connection (
  );

  virtual void resume_connection (
  );

  virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
  );

  virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
  );

  virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
  );

  virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
  );

  virtual CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
  );

  virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/ProxySupplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ProxySupplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYSUPPLIER_T_H */
