/* -*- C++ -*- */
/**
 *  @file ProxyConsumer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROXYCONSUMER_T_H
#define TAO_Notify_PROXYCONSUMER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Proxy_T.h"
#include "ProxyConsumer.h"

/**
 * @class TAO_Notify_ProxyConsumer_T
 *
 * @brief
 *
 */
template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxyConsumer_T : public virtual TAO_Notify_Proxy_T <SERVANT_TYPE>, public virtual TAO_Notify_ProxyConsumer
{
public:
  /// Constuctor
  TAO_Notify_ProxyConsumer_T (void);

  /// Destructor
  ~TAO_Notify_ProxyConsumer_T ();

  /// Notification of subscriptions set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed
                                    ACE_ENV_ARG_DECL);

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void offer_change (
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
#include "ProxyConsumer_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ProxyConsumer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ProxyConsumer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_Notify_PROXYCONSUMER_T_H */
