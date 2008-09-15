// -*- C++ -*-

/**
 *  @file ProxyConsumer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROXYCONSUMER_T_H
#define TAO_Notify_PROXYCONSUMER_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Proxy_T.h"
#include "orbsvcs/Notify/ProxyConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ProxyConsumer_T
 */
template <class SERVANT_TYPE>
class TAO_Notify_Serv_Export TAO_Notify_ProxyConsumer_T
  : public virtual TAO_Notify_Proxy_T <SERVANT_TYPE>,
    public virtual TAO_Notify_ProxyConsumer
{
public:
  /// Constructor
  TAO_Notify_ProxyConsumer_T (void);

  /// Destructor
  ~TAO_Notify_ProxyConsumer_T ();

  /// Notification of subscriptions set at the admin.
  virtual void admin_types_changed (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed);

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (void);

  virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode);

  virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  );

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/ProxyConsumer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ProxyConsumer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYCONSUMER_T_H */
