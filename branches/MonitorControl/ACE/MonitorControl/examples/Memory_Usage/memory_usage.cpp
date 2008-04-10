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
    ACE::MonitorControl::Monitor_Base *memory_monitor =
      mgr->admin ().monitor_point ("OS/Memory/TotalUsage");

    if (memory_monitor != 0)
      {
        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            MonitorControl_Types::Data data;
            memory_monitor->retrieve (data);
            MC_Test_Utilities::display_memory_usage (data);
          }
      }

    return 0;
  }
};

int main (int /* argc */, char * /* argv */ [])
{
  /// Set the timer for memory usage check at 2 sec.
  ADD_PERIODIC_MONITOR (MEMORY_USAGE_MONITOR, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  char * str_array[5] = {0};

  for (int i = 0; i < 10; ++i)
    {
      ACE_OS::sleep (2);

      /// Allocate a large string in each of the first 5 loops,
      /// free them in the last 5 loops.
      if (i < 5)
        {
          str_array[i] = new char[1024*1024*10];
        }
      else
        {
          delete [] str_array[i - 5];
        }
    }

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  return 0;
}
