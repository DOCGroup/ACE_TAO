#include <cstddef>
#include <memory>

#include "ace/OS_NS_unistd.h"
#include "ace/Monitor_Control_Action.h"
#include "ace/Monitor_Control/Monitor_Control.h"
#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

struct Control_Action_Releaser
{
  void operator() (Control_Action *action) const
  {
    action->remove_ref ();
  }
};

using Control_Action_Reference = std::unique_ptr<Control_Action,
                                                Control_Action_Releaser>;

/// Two control actions that will be associated with two different
/// constraints on the same monitor.

class Trigger8k : public Control_Action
{
public:
  Trigger8k () = default;
  Trigger8k (Trigger8k const &) = delete;
  Trigger8k (Trigger8k &&) = delete;
  Trigger8k &operator= (Trigger8k const &) = delete;
  Trigger8k &operator= (Trigger8k &&) = delete;

  void execute (char const * /* command */) override
  {
    ACE_DEBUG ((LM_DEBUG, "Total bytes received is above 8k\n"));
  }
};

class Trigger16k : public Control_Action
{
public:
  Trigger16k () = default;
  Trigger16k (Trigger16k const &) = delete;
  Trigger16k (Trigger16k &&) = delete;
  Trigger16k &operator= (Trigger16k const &) = delete;
  Trigger16k &operator= (Trigger16k &&) = delete;

  void execute (char const * /* command */) override
  {
    ACE_DEBUG ((LM_DEBUG, "Total bytes received is above 16k\n"));
  }
};

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
    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER * const mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    if (mgr == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), -1);

    /// Call on the administrator class to look up the desired monitors.
    Monitor_Base_Unique_Ptr const bytes_monitor (
      mgr->admin ().monitor_point ("OS/Network/BytesReceived"));

    if (bytes_monitor != nullptr)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (size_t i {}; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data (bytes_monitor->type ());
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_received (data);
          }
      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */[])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)
  /// Set the timer for # of threads check at 2 sec.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  if (mgr == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), 1);

  Monitor_Base_Unique_Ptr const bytes_monitor (
    create_os_monitor<BYTES_RECEIVED_MONITOR> (nullptr, ACE_Time_Value (2)));
  if (bytes_monitor == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to create bytes-received monitor\n"), 1);

  /// Add two constraints, each with its own triggered action.

  Control_Action_Reference const trigger8k (new Trigger8k);
  long const id8 = bytes_monitor->add_constraint ("value > 8192", trigger8k.get ());

  ACE_DEBUG ((LM_DEBUG, "trigger8k id = %q\n", static_cast<ACE_INT64> (id8)));

  Control_Action_Reference const trigger16k (new Trigger16k);
  long const id16 = bytes_monitor->add_constraint ("value > 16384", trigger16k.get ());

  ACE_DEBUG ((LM_DEBUG, "trigger16k id = %q\n", static_cast<ACE_INT64> (id16)));

  /// Create a query and register it to be called periodically.
  Monitor_Query query ("OS/Network/BytesReceived");
  ACE_Event_Handler_var const auto_query =
    ACE::make_event_handler<Monitor_Point_Auto_Query> ();
  auto_query->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
  ADD_PERIODIC_QUERY (auto_query.handler (), &query, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread. This class will
  /// fetch the monitor's value directly, and its output will be
  /// separate from the output from triggered actions.
  Monitor_Checker monitor_checker {};
  if (monitor_checker.activate () == -1)
    {
      ACE_ERROR ((LM_ERROR, "Unable to start monitor checker\n"));
      STOP_PERIODIC_MONITORS;
      return 1;
    }

  ACE_OS::sleep (20);

  /// Removing the constraint transfers its reference to this smart pointer.
  Control_Action_Reference const removed_action (
    bytes_monitor->remove_constraint (id8));
  ACE_DEBUG ((LM_DEBUG, "8k trigger removed\n"));

  ACE_OS::sleep (5);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
