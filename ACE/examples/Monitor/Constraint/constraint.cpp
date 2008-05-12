// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Monitor_Control_Action.h"

#include "ace/MonitorControl/MonitorControl.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

/// Two control actions that will be associated with two different
/// constraints on the same monitor.

class Trigger8k : public Control_Action
{
  virtual void execute (const char* /* command */)
  {
    ACE_DEBUG ((LM_DEBUG, "Total bytes received is above 8k\n"));
  }
};

class Trigger16k : public Control_Action
{
  virtual void execute (const char* /* command */)
  {
    ACE_DEBUG ((LM_DEBUG, "Total bytes received is above 16k\n"));
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
    Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("OS/Network/BytesReceived");

    if (bytes_monitor != 0)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            Monitor_Control_Types::Data data;
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_received (data);
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
  Monitor_Base *bytes_monitor =
    create_os_monitor<BYTES_RECEIVED_MONITOR> (0, ACE_Time_Value (2));

  /// Add two constraints, each with its own triggered action.

  Trigger8k *trigger8k = new Trigger8k;
  long id8 = bytes_monitor->add_constraint ("value > 8192", trigger8k);

  ACE_DEBUG ((LM_DEBUG, "trigger8k id = %d\n", id8));

  Trigger16k *trigger16k = new Trigger16k;
  long id16 = bytes_monitor->add_constraint ("value > 16384", trigger16k);

  ACE_DEBUG ((LM_DEBUG, "trigger16k id = %d\n", id16));

  /// Create a query and register it to be called periodically.
  MonitorQuery query ("OS/Network/BytesReceived");
  MonitorPointAutoQuery *auto_query = new MonitorPointAutoQuery;
  auto_query->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
  ACE_Event_Handler_var safety (auto_query);
  ADD_PERIODIC_QUERY (auto_query, &query, ACE_Time_Value (2));

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread. This class will
  /// fetch the monitor's value directly, and its output will be
  /// separate from the output from triggered actions.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  ACE_OS::sleep (20);

  /// Can do a remove_ref() on this returned value or on the original
  /// control action 'trigger8k', but not on both, since they point to
  /// the same thing.
  Control_Action *removed_action = bytes_monitor->remove_constraint (id8);
  ACE_DEBUG ((LM_DEBUG, "8k trigger removed\n"));

  ACE_OS::sleep (5);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  /// Do this instead of 'delete' since they are refcounted.
  removed_action->remove_ref ();
  trigger16k->remove_ref ();
  bytes_monitor->remove_ref ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
