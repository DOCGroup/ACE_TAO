/* -*- C++ -*- */
/**
 *  @file ProxySupplier_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROXYSUPPLIER_T_H
#define TAO_Notify_PROXYSUPPLIER_T_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Proxy_T.h"
#include "ProxySupplier.h"

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
  /// Constuctor
  TAO_Notify_ProxySupplier_T (void);

  /// Destructor
  ~TAO_Notify_ProxySupplier_T ();

  /// Notification of subscriptions set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed
                                    ACE_ENV_ARG_DECL);

  ///= POA_Notify_Internal methods
  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_structured (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_structured_no_filtering (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_any (const CORBA::Any & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_any_no_filtering (const CORBA::Any & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
protected:
  //= Data Members
  CORBA::Boolean is_suspended_;

  // = Interface methods
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void suspend_connection (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual void resume_connection (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

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
#include "ProxySupplier_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ProxySupplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ProxySupplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYSUPPLIER_T_H */
