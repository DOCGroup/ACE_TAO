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
    ACE::MonitorControl::Monitor_Base *cpu_monitor =
      mgr->admin ().monitor_point ("OS/Processor/CPULoad");

    if (cpu_monitor != 0)
      {
        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            MonitorControl_Types::Data data;
            cpu_monitor->retrieve (data);
            MC_Test_Utilities::display_cpu_load (data);
          }
      }

    return 0;
  }
};

int main (int /* argc */, char * /* argv */ [])
{
  /// The Admin class will own the reactor and destroy it. We are
  /// passing a vanilla reactor to show how it works, but in real
  /// life it could be some specialized reactor.
  ACE_Reactor* new_reactor = new ACE_Reactor;
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  mgr->admin ().reactor (new_reactor);

  /// Set the timer for CPU load check at 2 sec.
  ADD_PERIODIC_MONITOR (CPU_LOAD_MONITOR, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  /// Make sure the monitor checker is spawned before doing anything.
  ACE_OS::sleep (1);

  for (int i = 0; i < 10; ++i)
    {
      /// Alternate between letting the CPU sleep and keeping it
      /// busy.
      if (i % 2 == 0)
        {
          ACE_OS::sleep (1);
        }
      else
        {
          for (unsigned long j = 0; j < 5050505; j++)
            {
              (void) ACE::gcd (2419233733, 567715713);
            }
        }
    }

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  return 0;
}
