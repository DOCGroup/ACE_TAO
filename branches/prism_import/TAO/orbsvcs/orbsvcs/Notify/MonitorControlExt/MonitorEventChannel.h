// $Id$
#ifndef MONITOREVENTCHANNEL_H
#define MONITOREVENTCHANNEL_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "ace/Vector_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ThreadPool_Task;

class TAO_Notify_MC_Ext_Export TAO_MonitorEventChannel:
                  public TAO_Notify_EventChannel
{
public:
  /// Construct a named event channel and associate various
  /// statistic objects with it in the statistic registry
  TAO_MonitorEventChannel (const char* name);

  /// Remove the statistics for this event channel
  ~TAO_MonitorEventChannel (void);

  /// Return the name of this event channel
  const ACE_CString& name (void) const;

  void map_supplier_proxy (CosNotifyChannelAdmin::ProxyID id,
                           const ACE_CString& name);
  void map_consumer_proxy (CosNotifyChannelAdmin::ProxyID id,
                           const ACE_CString& name);

  virtual void cleanup_proxy (CosNotifyChannelAdmin::ProxyID id
			      , bool is_supplier);

  /// Add the statistics for this event channel.  If the ec was
  /// constructed with a non-zero length string, this method will
  /// do nothing.
  void add_stats (const char* name = 0);

private:
  size_t get_consumers (TAO_Statistic::List* names);
  size_t get_suppliers (TAO_Statistic::List* names);
  TAO_Notify_ThreadPool_Task* get_threadpool_task (
                                CosNotifyChannelAdmin::AdminID id);
  size_t calculate_queue_size (bool count);
  double get_oldest_event (void);
  void determine_slowest_consumer (TAO_Statistic::List* names);

  void unmap_supplier_proxy (CosNotifyChannelAdmin::ProxyID id);
  void unmap_consumer_proxy (CosNotifyChannelAdmin::ProxyID id);

  friend class EventChannelConsumers;
  friend class EventChannelSuppliers;
  friend class SlowestConsumers;
  friend class QueuedEvents;
  friend class OldestEvent;

  typedef ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ProxyID,
                               ACE_CString,
                               ACE_SYNCH_NULL_MUTEX> Map;

  bool is_duplicate_name (const Map& map,
                          const ACE_CString& name) const;

  ACE_CString name_;
  ACE_Vector<ACE_CString> stat_names_;
  ACE_Vector<ACE_CString> control_names_;

  mutable ACE_SYNCH_RW_MUTEX supplier_mutex_;
  Map supplier_map_;

  mutable ACE_SYNCH_RW_MUTEX consumer_mutex_;
  Map consumer_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* MONITOREVENTCHANNEL_H */
