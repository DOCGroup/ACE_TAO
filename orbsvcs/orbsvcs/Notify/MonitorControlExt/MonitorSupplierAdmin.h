// $Id$

#ifndef MONITORSUPPLIERADMIN_H
#define MONITORSUPPLIERADMIN_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/SupplierAdmin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_MonitorEventChannel;

class TAO_Notify_MC_Ext_Export TAO_MonitorSupplierAdmin:
                  public virtual TAO_Notify_SupplierAdmin,
                  public virtual POA_NotifyMonitoringExt::SupplierAdmin
{
public:
  /// Construct a monitor supplier admin
  TAO_MonitorSupplierAdmin (void);

  /// Remove the statistics for this event channel
  ~TAO_MonitorSupplierAdmin (void);

  /// Register our statistic and control objects
  void register_stats_controls (TAO_MonitorEventChannel* mec,
                                const ACE_CString& name);

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
    obtain_named_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name);

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr
    obtain_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id);

private:
  ACE_CString control_name_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* MONITORSUPPLIERADMIN_H */
