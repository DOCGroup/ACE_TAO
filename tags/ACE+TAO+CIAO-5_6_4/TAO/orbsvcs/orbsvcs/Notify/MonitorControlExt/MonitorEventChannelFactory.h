// $Id$
#ifndef MONITOREVENTCHANNELFACTORY_H
#define MONITOREVENTCHANNELFACTORY_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Vector_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Ext_Export TAO_MonitorEventChannelFactory:
                  public TAO_Notify_EventChannelFactory,
                  public virtual POA_NotifyMonitoringExt::EventChannelFactory
{
public:
  /// Construct a named event channel factory and associate various
  /// statistic objects with it in the statistic registry
  TAO_MonitorEventChannelFactory (const char* name);

  /// Remove the factory name from the factory names statistic
  ~TAO_MonitorEventChannelFactory (void);

  /// Create a named event channel and associate various statistic
  /// objects with it in the statistic registry
  virtual CosNotifyChannelAdmin::EventChannel_ptr
    create_named_channel(const CosNotification::QoSProperties& initial_qos,
                         const CosNotification::AdminProperties& initial_admin,
                         CosNotifyChannelAdmin::ChannelID_out id,
                         const char* name);

  /// Create an event channel and use the id as the name.
  virtual CosNotifyChannelAdmin::EventChannel_ptr
    create_channel(const CosNotification::QoSProperties& initial_qos,
                   const CosNotification::AdminProperties& initial_admin,
                   CosNotifyChannelAdmin::ChannelID_out id);

  /// Hook into the remove() call from the base class and remove it from
  /// our map before passing control back to the base.
  virtual void remove (TAO_Notify_EventChannel* channel);

  // This is public to allow the Unbinder class access
  // for SunCC 5.5 and above
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               CosNotifyChannelAdmin::ChannelID,
                               ACE_SYNCH_NULL_MUTEX> Map;

private:
  size_t get_consumers (CosNotifyChannelAdmin::ChannelID id);
  size_t get_suppliers (CosNotifyChannelAdmin::ChannelID id);
  size_t get_ecs (TAO_Statistic::List* names, bool active);

  friend class EventChannels;

  class Unbinder
  {
  public:
    Unbinder (Map& map,
              const ACE_CString& name);
    ~Unbinder (void);
    void release (void);

  private:
    Map& map_;
    const ACE_CString& name_;
    bool released_;
  };

  TAO_SYNCH_RW_MUTEX mutex_;
  ACE_CString name_;
  Map map_;
  ACE_Vector<ACE_CString> stat_names_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* MONITOREVENTCHANNELFACTORY_H */
