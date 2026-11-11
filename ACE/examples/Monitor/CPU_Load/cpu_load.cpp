#include <cstddef>
#include <memory>

#include "ace/OS_NS_unistd.h"

#include "ace/Monitor_Control/Monitor_Control.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

struct Monitor_Reference_Releaser
{
  void operator() (ACE::Monitor_Control::Monitor_Base *monitor) const
  {
    if (monitor != nullptr)
      monitor->remove_ref ();
  }
};

using Monitor_Reference = std::unique_ptr<ACE::Monitor_Control::Monitor_Base,
                                          Monitor_Reference_Releaser>;

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class Monitor_Checker : public ACE_Task_Base
{
public:
  Monitor_Checker () = default;
  Monitor_Checker (const Monitor_Checker &) = delete;
  Monitor_Checker (Monitor_Checker &&) = delete;
  Monitor_Checker &operator= (const Monitor_Checker &) = delete;
  Monitor_Checker &operator= (Monitor_Checker &&) = delete;

  int svc () override
  {
    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER * const mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    if (mgr == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), -1);

    /// Call on the administrator class to look up the desired monitors.
    Monitor_Reference const cpu_monitor (
      mgr->admin ().monitor_point ("OS/Processor/CPULoad"));

    if (cpu_monitor != nullptr)
      {
        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (std::size_t i {}; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data (cpu_monitor->type ());
            cpu_monitor->retrieve (data);
            MC_Test_Utilities::display_cpu_load (data);
          }

      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  /// The Admin class will own the reactor and destroy it. We are
  /// passing a vanilla reactor to show how it works, but in real
  /// life it could be some specialized reactor.
  ACE_Reactor new_reactor;
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  if (mgr == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), 1);
  mgr->admin ().reactor (&new_reactor);

  /// Set the timer for CPU load check at 2 sec.
  Monitor_Reference const cpu_monitor (
    create_os_monitor<CPU_LOAD_MONITOR> (nullptr, ACE_Time_Value (2)));
  if (cpu_monitor == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to create CPU monitor\n"), 1);

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  Monitor_Checker monitor_checker {};
  monitor_checker.activate ();

  /// Make sure the monitor checker is spawned before doing anything.
  ACE_OS::sleep (1);

  for (std::size_t i {}; i < 10; ++i)
    {
      /// Alternate between letting the CPU sleep and keeping it
      /// busy.
      if (i % 2 == 0)
        {
          ACE_OS::sleep (1);
        }
      else
        {
          for (std::size_t j {}; j < 5050505; ++j)
            {
              (void) ACE::gcd (2419233733UL, 567715713UL);
            }
        }
    }

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
