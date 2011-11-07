// $Id$

#ifndef MONITOREVENTCHANNEL_H
#define MONITOREVENTCHANNEL_H

#include /**/ "ace/pre.h"

#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Control_Types.h"

#include "orbsvcs/Notify/EventChannel.h"

#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"

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

class TAO_Notify_ThreadPool_Task;

class TAO_Notify_MC_Ext_Export TAO_MonitorEventChannel
  :  public virtual TAO_Notify_EventChannel,
     public virtual POA_NotifyMonitoringExt::EventChannel
{
public:
  /// Construct a named event channel and associate various
  /// statistic objects with it in the statistic registry.
  TAO_MonitorEventChannel (const char* name);

  /// Remove the statistics for this event channel.
  ~TAO_MonitorEventChannel (void);

  /// Return the name of this event channel.
  const ACE_CString& name (void) const;

  /// Register a single statistic with the EC and statistic registry.
  bool register_statistic (const ACE_CString& name,
                           Monitor_Base* stat);

  /// Unregister a single statistic from the EC and statistic registry.
  bool unregister_statistic (const ACE_CString& name);

  /// Keep track of the supplier name associated with the proxy id.
  void map_supplier_proxy (CosNotifyChannelAdmin::ProxyID id,
                           const ACE_CString& name);

  /// Keep track of the consumer name associated with the proxy id.
  void map_consumer_proxy (CosNotifyChannelAdmin::ProxyID id,
                           const ACE_CString& name);

  /// Remove the supplier/consumer proxy id/name mapping.
  virtual void cleanup_proxy (CosNotifyChannelAdmin::ProxyID id,
                              bool is_supplier,
                              bool experienced_timeout);

  /// Remove the consumer admin id/name mapping.
  virtual void remove_consumeradmin (CosNotifyChannelAdmin::AdminID id);

  /// Remove the supplier admin id/name mapping.
  virtual void remove_supplieradmin (CosNotifyChannelAdmin::AdminID id);

  /// Add the statistics for this event channel.  If the ec was
  /// constructed with a non-zero length string, this method will
  /// do nothing.
  void add_stats (const char* name = 0);

  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr
    new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                       CosNotifyChannelAdmin::AdminID_out id);

  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr
    named_new_for_consumers (
                       CosNotifyChannelAdmin::InterFilterGroupOperator op,
                       CosNotifyChannelAdmin::AdminID_out id,
                       const char* name);

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr
    new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                       CosNotifyChannelAdmin::AdminID_out id);

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr
    named_new_for_suppliers (
                       CosNotifyChannelAdmin::InterFilterGroupOperator op,
                       CosNotifyChannelAdmin::AdminID_out id,
                       const char* name);

private:
  size_t get_consumers (Monitor_Control_Types::NameList* names);
  size_t get_suppliers (Monitor_Control_Types::NameList* names);
  void get_timedout_consumers (Monitor_Control_Types::NameList* names);
  size_t get_consumeradmins (Monitor_Control_Types::NameList* names);
  size_t get_supplieradmins (Monitor_Control_Types::NameList* names);
  TAO_Notify_ThreadPool_Task* get_threadpool_task (
                                CosNotifyChannelAdmin::AdminID id);
  size_t calculate_queue_size (bool count);
  double get_oldest_event (void);
  void determine_slowest_consumer (Monitor_Control_Types::NameList* names);
  bool destroy_consumer (CosNotifyChannelAdmin::ProxyID id);
  bool destroy_supplier (CosNotifyChannelAdmin::ProxyID id);


  friend class RemoveConsumerSupplierControl;
  friend class EventChannelConsumersSuppliers;
  friend class EventChannelTimedoutConsumers;
  friend class EventChannelConsumerSupplierAdmins;
  friend class SlowestConsumers;
  friend class QueuedEvents;
  friend class OldestEvent;

  typedef ACE_Hash_Map_Manager<CORBA::Long,
                               ACE_CString,
                               ACE_SYNCH_NULL_MUTEX> Map;

  size_t get_admins (Map& map,
                     const CosNotifyChannelAdmin::AdminIDSeq& ids,
                     Monitor_Control_Types::NameList* names);
  bool is_duplicate_name (const Map& map,
                          const ACE_CString& name) const;

  void remove_list_name (Monitor_Control_Types::NameList& list,
                         const ACE_CString& name);

  ACE_CString name_;

  mutable TAO_SYNCH_MUTEX names_mutex_;
  Monitor_Control_Types::NameList stat_names_;
  Monitor_Control_Types::NameList control_names_;

  mutable TAO_SYNCH_RW_MUTEX supplier_mutex_;
  Map supplier_map_;

  mutable TAO_SYNCH_RW_MUTEX consumer_mutex_;
  Map consumer_map_;

  mutable TAO_SYNCH_RW_MUTEX timedout_supplier_mutex_;
  Map timedout_supplier_map_;

  mutable TAO_SYNCH_RW_MUTEX supplieradmin_mutex_;
  Map supplieradmin_map_;

  mutable TAO_SYNCH_RW_MUTEX consumeradmin_mutex_;
  Map consumeradmin_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* MONITOREVENTCHANNEL_H */
