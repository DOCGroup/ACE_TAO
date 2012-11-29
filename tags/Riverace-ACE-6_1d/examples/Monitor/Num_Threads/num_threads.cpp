// $Id$

#include "ace/OS_NS_unistd.h"

#include "ace/Monitor_Control/Monitor_Control.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

class Worker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    ACE_OS::sleep (5);
    return 0;
  }
};

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class Monitor_Checker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

    /// Call on the administrator class to look up the desired monitors.
    ACE::Monitor_Control::Monitor_Base *thread_monitor =
      mgr->admin ().monitor_point ("OS/System/NumThreads");

    if (thread_monitor != 0)
      {
        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 5; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data (thread_monitor->type ());
            thread_monitor->retrieve (data);
            MC_Test_Utilities::display_num_threads (data);
          }

        thread_monitor->remove_ref ();
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
  Monitor_Base *num_threads_monitor =
    create_os_monitor<NUM_THREADS_MONITOR> (0, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  Monitor_Checker monitor_checker;
  monitor_checker.activate ();

  /// Spawn 100 threads, sleep until they finish.
  Worker worker;
  worker.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 100);

  ACE_OS::sleep (6);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  num_threads_monitor->remove_ref ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
