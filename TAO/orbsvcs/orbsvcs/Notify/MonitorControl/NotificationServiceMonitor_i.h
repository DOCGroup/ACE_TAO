// $Id$
#ifndef NOTIFICATIONSERVICEMONITOR_I_H
#define NOTIFICATIONSERVICEMONITOR_I_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControl/notify_mc_export.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCS.h"
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export NotificationServiceMonitor_i:
    public virtual POA_CosNotification::NotificationServiceMonitorControl
{
public:
  /// Construct a monitor service.  If the orb parameter is provided,
  /// it is possible that shutdown() will be called on it.
  NotificationServiceMonitor_i (CORBA::ORB_ptr orb = 0);

  /// Get a list of statistic names
  virtual CosNotification::NotificationServiceMonitorControl::NameList*
    get_statistic_names (void);

  /// Get the data from a single statistic
  virtual CosNotification::NotificationServiceMonitorControl::Data*
    get_statistic (const char* name);

  /// Get the data from multiple statistic instances
  virtual CosNotification::NotificationServiceMonitorControl::DataList*
    get_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names);

  /// Get the data from multiple statistic instances and clear them
  /// directly afterward
  virtual CosNotification::NotificationServiceMonitorControl::DataList*
    get_and_clear_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names);

  /// Clear the data from multiple statistic instances
  virtual void
    clear_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names);

  /// Shut down an event channel by name
  virtual void shutdown_event_channel (const char* name);

  /// Remove a consumer by name
  virtual void remove_consumer (const char* name);

  /// Remove a supplier by name
  virtual void remove_supplier (const char* name);

  /// Remove a consumer admin by name
  virtual void remove_consumeradmin (const char* name);

  /// Remove a supplier admin by name
  virtual void remove_supplieradmin (const char* name);

  /// Call shutdown() on the ORB if one was provided during construction
  virtual void shutdown (void);

private:
  void send_control_command (const char* name,
                             const char* cmd);

  void get_data (TAO_Statistic_Registry* registry,
                 const char* name,
                 CosNotification::NotificationServiceMonitorControl::Data& data);

  void get_invalid_names (TAO_Statistic_Registry* registry,
                          const CosNotification::NotificationServiceMonitorControl::NameList& names,
                          CosNotification::NotificationServiceMonitorControl::NameList& invalid);

  CORBA::ORB_var orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* NOTIFICATIONSERVICEMONITOR_I_H */
