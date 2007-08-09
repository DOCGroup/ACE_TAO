// $Id$
#ifndef MONITORSUPPLIERADMIN_H
#define MONITORSUPPLIERADMIN_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Ext_Export TAO_MonitorSupplierAdmin:
                  public virtual TAO_Notify_SupplierAdmin,
                  public virtual POA_NotifyMonitoringExt::SupplierAdmin
{
public:
  /// Construct a monitor supplier admin
  TAO_MonitorSupplierAdmin (void);

  /// Remove the statistics for this event channel
  ~TAO_MonitorSupplierAdmin (void);

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
    obtain_named_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name);

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
    obtain_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* MONITORSUPPLIERADMIN_H */
