// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/streams.h"
#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Admin_Manager.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/examples/MC_Test_Utilities.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

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
      ACE_Dynamic_Service<Monitor_Admin_Manager>::instance ("MC_ADMINMANAGER");

    ACE::MonitorControl::Monitor_Base *cpu_monitor =
      mgr->admin ().monitor_point ("OS/Processor/CPULoad");

    ACE::MonitorControl::Monitor_Base *memory_monitor =
      mgr->admin ().monitor_point ("OS/Memory/TotalUsage");

    ACE::MonitorControl::Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("OS/Network/BytesSent");

    /// Query the monitor for its data every 2 seconds, and call the
    /// appropriate display function.
    for (int i = 0; i < 10; ++i)
      {
        ACE_OS::sleep (2);

        Monitor_Control_Types::Data data;
        cpu_monitor->retrieve (data);
        MC_Test_Utilities::display_cpu_load (data);

        memory_monitor->retrieve (data);
        MC_Test_Utilities::display_memory_usage (data);

        bytes_monitor->retrieve (data);
        MC_Test_Utilities::display_bytes_sent (data);

        cout << endl;
      }

    cpu_monitor->remove_ref ();
    memory_monitor->remove_ref ();
    bytes_monitor->remove_ref ();

    return 0;
  }
};

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
  /// Creates these future group members without automatic update.
  Monitor_Base *cpu_load_monitor =
    create_os_monitor<CPU_LOAD_MONITOR> ();
  Monitor_Base *bytes_sent_monitor =
    create_os_monitor<BYTES_SENT_MONITOR> ();
  Monitor_Base *memory_usage_monitor =
    create_os_monitor<MEMORY_USAGE_MONITOR> ();

  MonitorGroup *group = new MonitorGroup ("Test_Group");

  group->add_member (cpu_load_monitor);
  group->add_member (bytes_sent_monitor);
  group->add_member (memory_usage_monitor);

  /// Register the group as an auto-updated monitor point.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  bool good_add = mgr->admin ().monitor_point (group, ACE_Time_Value (2));

  if (!good_add)
    {
      ACE_ERROR ((LM_ERROR,
                  "Group monitor \"%s\" registration failed.\n",
                  group->name ()));
    }

  /// A quick test of the registry's name cache.
  Monitor_Control_Types::NameList mc_names =
    Monitor_Point_Registry::instance ()->names ();

  cout << "stored in monitor point registry:" << endl << endl;

  for (Monitor_Control_Types::NameList::Iterator i (mc_names);
       !i.done ();
       i.advance ())
    {
      ACE_CString *item = 0;
      i.next (item);

      cout << item->c_str () << endl;
    }

  cout << endl;

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker;
  monitor_checker.activate ();

  char * str_array[5] = {0};

  for (int i = 0; i < 10; ++i)
    {
      ACE_OS::sleep (1);

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
              (void) ACE::gcd (static_cast<u_long> (2419233733), 567715713);
            }
        }

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

  cpu_load_monitor->remove_ref ();
  bytes_sent_monitor->remove_ref ();
  memory_usage_monitor->remove_ref ();
  group->remove_ref ();

  return 0;
}
