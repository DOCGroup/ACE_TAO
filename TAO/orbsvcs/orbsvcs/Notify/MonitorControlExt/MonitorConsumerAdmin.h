// $Id$
#ifndef MONITORCONSUMERADMIN_H
#define MONITORCONSUMERADMIN_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Statistic;
class TAO_MonitorEventChannel;

class TAO_Notify_MC_Ext_Export TAO_MonitorConsumerAdmin:
                  public virtual TAO_Notify_ConsumerAdmin,
                  public virtual POA_NotifyMonitoringExt::ConsumerAdmin,
                  public virtual TAO_Notify_Buffering_Strategy::Tracker
{
public:
  /// Construct a monitor supplier admin
  TAO_MonitorConsumerAdmin (void);

  /// Remove the statistics for this event channel
  virtual ~TAO_MonitorConsumerAdmin (void);

  /// Register our statistic and control objects
  void register_stats_controls (TAO_MonitorEventChannel* mec,
                                const ACE_CString& name);

  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
      obtain_named_notification_push_supplier (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name);

  virtual CosNotifyChannelAdmin::ProxySupplier_ptr
      obtain_notification_push_supplier (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id);

  /// Receive the queue count and store the approximated queue size in
  /// our statistic object.
  virtual void update_queue_count (size_t count);

private:
  ACE_CString stat_name_;
  ACE_CString control_name_;
  TAO_Statistic* queue_size_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* MONITORCONSUMERADMIN_H */
