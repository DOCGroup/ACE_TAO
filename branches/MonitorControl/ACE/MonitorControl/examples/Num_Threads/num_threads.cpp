// $Id$

#include "ace/OS_NS_unistd.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/examples/MC_Test_Utilities.h"

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
class MonitorChecker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

    /// Call on the administrator class to look up the desired monitors.
    ACE::MonitorControl::Monitor_Base *thread_monitor =
      mgr->admin ().monitor_point ("NumThreads");

    if (thread_monitor != 0)
      {
        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 5; ++i)
          {
            ACE_OS::sleep (2);

            MonitorControl_Types::Data data = thread_monitor->retrieve ();
            MC_Test_Utilities::display_num_threads (data);
          }
      }

    return 0;
  }
};

int main (int argc, char *argv [])
{
  /// Set the timer for # of threads check at 2000 msecs (2 sec).
  ADD_PERIODIC_MONITOR (NUM_THREADS_MONITOR, 2000);

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  /// Spawn 100 threads, sleep until they finish.
  Worker worker;
  worker.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 100);

  ACE_OS::sleep (6);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  return 0;
}
