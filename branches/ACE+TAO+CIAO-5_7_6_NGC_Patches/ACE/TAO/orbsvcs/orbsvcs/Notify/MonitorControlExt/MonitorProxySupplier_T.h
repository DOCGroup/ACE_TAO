// $Id$
#ifndef MONITORPROXYSUPPLIER_H
#define MONITORPROXYSUPPLIER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/SupplierAdmin.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    class Monitor_Base;
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_MonitorEventChannel;
class TAO_MonitorConsumerAdmin;

template <typename ProxyPushSupplier>
class TAO_MonitorProxySupplier_T
  : public TAO_Notify_Buffering_Strategy::Tracker
  , public ProxyPushSupplier
{
public:
  /// Construct a monitor supplier admin
  TAO_MonitorProxySupplier_T (void);

  /// Remove the statistics for this event channel
  virtual ~TAO_MonitorProxySupplier_T (void);

  /// Configure the statistics after construction
  /// @param admin the parent admin for this proxy
  /// @proxy_id the ID assigned to this proxy
  virtual void configure(
    TAO_Notify_ConsumerAdmin & admin,
    CosNotifyChannelAdmin::ProxyID_out proxy_id);

  // Implement Tracer interface
  virtual void update_queue_count (size_t count);
  virtual void count_queue_overflow (bool local_overflow, bool global_overflow);

  /// Access to Statistics Name
  ACE_CString & queue_item_stat_name (void);

  /// Access to Statistics Name
  ACE_CString & overflow_stat_name (void);

private:
  ACE_CString base_stat_name_;
  ACE_CString queue_item_stat_name_;
  ACE_CString overflow_stat_name_;

  Monitor_Base * queue_item_count_;
  Monitor_Base * overflows_;

  TAO_MonitorEventChannel* event_channel_;
  TAO_MonitorConsumerAdmin * admin_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "orbsvcs/Notify/MonitorControlExt/MonitorProxySupplier_T.cpp"
#pragma implementation ("MonitorProxySupplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif // TAO_HAS_MONITOR_FRAMEWORK == 1

#include /**/ "ace/post.h"
#endif /* MONITORPROXYSUPPLIER_H */

