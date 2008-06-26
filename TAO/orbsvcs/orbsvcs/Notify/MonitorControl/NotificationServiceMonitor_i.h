// $Id$

#ifndef NOTIFICATIONSERVICEMONITOR_I_H
#define NOTIFICATIONSERVICEMONITOR_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCS.h"

#include "tao/ORB.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    class Monitor_Point_Registry;
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export NotificationServiceMonitor_i
  : public virtual POA_CosNotification::NotificationServiceMonitorControl
{
public:
  /// Construct a monitor service.  If the orb parameter is provided,
  /// it is possible that shutdown() will be called on it.
  NotificationServiceMonitor_i (CORBA::ORB_ptr orb = 0);

  /// Get a list of statistic names
  virtual Monitor::NameList* get_statistic_names (void);

  /// Get the data from a single statistic
  virtual CosNotification::NotificationServiceMonitorControl::Data*
  get_statistic (const char* name);

  /// Get the data from multiple statistic instances
  virtual CosNotification::NotificationServiceMonitorControl::DataList*
  get_statistics (const Monitor::NameList& names);

  /// Get the data from multiple statistic instances and clear them
  /// directly afterward
  virtual CosNotification::NotificationServiceMonitorControl::DataList*
  get_and_clear_statistics (const Monitor::NameList& names);

  /// Clear the data from multiple statistic instances
  virtual void clear_statistics (const Monitor::NameList& names);

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

  void get_data (Monitor_Point_Registry* registry,
                 const char* name,
                 CosNotification::NotificationServiceMonitorControl::Data& data);

  void get_invalid_names (Monitor_Point_Registry* registry,
                          const Monitor::NameList& names,
                          Monitor::NameList& invalid);

  CORBA::ORB_var orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* NOTIFICATIONSERVICEMONITOR_I_H */
