// $Id$
#ifndef MONITORCONSUMERADMIN_H
#define MONITORCONSUMERADMIN_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Ext_Export TAO_MonitorConsumerAdmin:
                  public virtual TAO_Notify_ConsumerAdmin,
                  public virtual POA_NotifyMonitoringExt::ConsumerAdmin
{
public:
  /// Construct a monitor supplier admin
  TAO_MonitorConsumerAdmin (void);

  /// Remove the statistics for this event channel
  ~TAO_MonitorConsumerAdmin (void);

    virtual CosNotifyChannelAdmin::ProxySupplier_ptr
      obtain_named_notification_push_supplier (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name);

    virtual CosNotifyChannelAdmin::ProxySupplier_ptr
      obtain_notification_push_supplier (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* MONITORCONSUMERADMIN_H */
