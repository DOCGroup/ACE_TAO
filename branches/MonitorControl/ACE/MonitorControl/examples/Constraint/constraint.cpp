// $Id$

#include "ace/OS_NS_unistd.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/examples/MC_Test_Utilities.h"

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
    ACE::MonitorControl::Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("BytesReceived");

    if (bytes_monitor != 0)
      {
        /// Query the monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 10; ++i)
          {
            ACE_OS::sleep (2);

            MonitorControl_Types::Data data;
            bytes_monitor->retrieve (data);
            MC_Test_Utilities::display_bytes_received (data);
          }
      }

    return 0;
  }
};

int main (int /* argc */, char * /* argv */ [])
{
  /// Set the timer for # of threads check at 2000 msecs (2 sec).
  ADD_PERIODIC_MONITOR (BYTES_RECEIVED_MONITOR, 2000);

  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_Admin_Manager>::instance ("MC_ADMINMANAGER");

  ACE::MonitorControl::Monitor_Base *bytes_monitor =
    mgr->admin ().monitor_point ("BytesReceived");

  /// Add two constraints, each with its own triggered action.

  Trigger8k trigger8k;
  bytes_monitor->constraints ("value > 8192", &trigger8k);

  Trigger16k trigger16k;
  bytes_monitor->constraints ("value > 16384", &trigger16k);

  /// Create a query and register it to be called periodically.
  MonitorQuery query ("BytesReceived");
  MonitorPointAutoQuery *auto_query = new MonitorPointAutoQuery;
  ACE_Event_Handler_var safety (auto_query);
  ADD_PERIODIC_QUERY (auto_query, &query, 2);

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread. This class will
  /// fetch the monitor's value directly, and its output will be
  /// separate from the output from triggered actions.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  ACE_OS::sleep (20);

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

  return 0;
}
