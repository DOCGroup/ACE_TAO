// $Id$

#include "ace/OS_NS_unistd.h"

#include "ace/MonitorControl/MonitorControl.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

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

            Monitor_Control_Types::Data data;
            memory_monitor->retrieve (data);
            MC_Test_Utilities::display_memory_usage (data);
          }

        memory_monitor->remove_ref ();
      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  /// Set the timer for memory usage check at 2 sec.
  Monitor_Base *memory_usage_monitor =
    create_os_monitor<MEMORY_USAGE_MONITOR> (0, ACE_Time_Value (2));

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

  memory_usage_monitor->remove_ref ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

