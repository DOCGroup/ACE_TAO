#include "ace/OS_NS_unistd.h"

#include "ace/Monitor_Control/Monitor_Control.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#include <cstddef>

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class Monitor_Checker : public ACE_Task_Base
{
public:
  Monitor_Checker () = default;
  Monitor_Checker (Monitor_Checker const &) = delete;
  Monitor_Checker (Monitor_Checker &&) = delete;
  Monitor_Checker &operator= (Monitor_Checker const &) = delete;
  Monitor_Checker &operator= (Monitor_Checker &&) = delete;

  int svc () override
  {
    constexpr std::size_t monitor_samples = 15;

    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER * const mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    if (mgr == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), -1);

    /// Call on the administrator class to look up the desired monitors.
    ACE::Monitor_Control::Monitor_Base_Unique_Ptr const bytes_monitor (
      mgr->admin ().monitor_point ("OS/Network/BytesSent"));

    if (bytes_monitor != nullptr)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (std::size_t i {}; i < monitor_samples; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data (bytes_monitor->type ());
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_sent (data);
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

  /// Set the timer to sample network bytes sent every two seconds.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  if (mgr == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), 1);

  ACE::Monitor_Control::Monitor_Base_Unique_Ptr const monitor (
    create_os_monitor<BYTES_SENT_MONITOR> (nullptr, ACE_Time_Value (2)));
  if (monitor == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to create bytes-sent monitor\n"), 1);

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  Monitor_Checker monitor_checker;
  monitor_checker.activate ();

  ACE_OS::sleep (30);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
