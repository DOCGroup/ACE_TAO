// $Id$

#include "ace/OS_NS_unistd.h"

#include "ace/MonitorControl/MonitorControl.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class MonitorChecker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

    /// Call on the administrator class to look up the desired monitors.
    Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("OS/Network/BytesSent");

    if (bytes_monitor != 0)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 15; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data;
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_sent (data);
          }

        bytes_monitor->remove_ref ();
      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  /// Set the timer for # of threads check at 2 sec.
  Monitor_Base *monitor =
    create_os_monitor<BYTES_SENT_MONITOR> (0, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  ACE_OS::sleep (30);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  monitor->remove_ref ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
