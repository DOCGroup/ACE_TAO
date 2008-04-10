// $Id$

#include "ace/OS_NS_unistd.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/examples/MC_Test_Utilities.h"

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
    ACE::MonitorControl::Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("OS/Network/BytesSent");

    if (bytes_monitor != 0)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 15; ++i)
          {
            ACE_OS::sleep (2);

            MonitorControl_Types::Data data;
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_sent (data);
          }
      }

    return 0;
  }
};

int main (int /* argc */, char * /* argv */ [])
{
  /// Set the timer for # of threads check at 2 sec.
  ADD_PERIODIC_MONITOR (BYTES_SENT_MONITOR, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  ACE_OS::sleep (30);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  return 0;
}
