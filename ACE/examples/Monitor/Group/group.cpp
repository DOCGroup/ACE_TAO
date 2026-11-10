#include <array>
#include <memory>

#include "ace/OS_NS_unistd.h"
#include "ace/streams.h"
#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Admin_Manager.h"

#include "ace/Monitor_Control/Monitor_Control.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

struct Monitor_Reference_Releaser
{
  void operator() (ACE::Monitor_Control::Monitor_Base *monitor) const
  {
    if (monitor != nullptr)
      monitor->remove_ref ();
  }
};

// Monitor references from the registry are released with remove_ref().
using Monitor_Reference = std::unique_ptr<ACE::Monitor_Control::Monitor_Base,
                                          Monitor_Reference_Releaser>;
using Monitor_Group_Reference = std::unique_ptr<Monitor_Group,
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
    // Get the borrowed MC service singleton.
    // ACE_Dynamic_Service returns a borrowed pointer from the service repository.
    MC_ADMINMANAGER * const mgr =
      ACE_Dynamic_Service<Monitor_Admin_Manager>::instance ("MC_ADMINMANAGER");

    Monitor_Reference cpu_monitor (
      mgr->admin ().monitor_point ("OS/Processor/CPULoad"));

    Monitor_Reference memory_monitor (
      mgr->admin ().monitor_point ("OS/Memory/TotalUsage"));

    Monitor_Reference bytes_monitor (
      mgr->admin ().monitor_point ("OS/Network/BytesSent"));

    // Query each monitor every 2 seconds and display its data.
    for (int i = 0; i < 10; ++i)
      {
        ACE_OS::sleep (2);

        Monitor_Control_Types::Data cpu_data (cpu_monitor->type ());
        cpu_monitor->retrieve (cpu_data);
        MC_Test_Utilities::display_cpu_load (cpu_data);

        Monitor_Control_Types::Data mem_data (memory_monitor->type ());
        memory_monitor->retrieve (mem_data);
        MC_Test_Utilities::display_memory_usage (mem_data);

        Monitor_Control_Types::Data bytes_data (bytes_monitor->type ());
        bytes_monitor->retrieve (bytes_data);
        MC_Test_Utilities::display_bytes_sent (bytes_data);

        cout << endl;
      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  // Create these future group members without automatic update.
  Monitor_Reference cpu_load_monitor (create_os_monitor<CPU_LOAD_MONITOR> ());
  Monitor_Reference bytes_sent_monitor (create_os_monitor<BYTES_SENT_MONITOR> ());
  Monitor_Reference memory_usage_monitor (create_os_monitor<MEMORY_USAGE_MONITOR> ());

  Monitor_Group_Reference group (new Monitor_Group ("Test_Group"));

  group->add_member (cpu_load_monitor.get ());
  group->add_member (bytes_sent_monitor.get ());
  group->add_member (memory_usage_monitor.get ());

  // Register the group as an auto-updated monitor point.
  // ACE_Dynamic_Service returns a borrowed pointer from the service repository.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
  bool const good_add = mgr->admin ().monitor_point (group.get (), ACE_Time_Value (2));

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
      // The iterator provides a borrowed pointer into mc_names.
      ACE_CString *item {};
      i.next (item);

      cout << item->c_str () << endl;
    }

  cout << endl;

  /// Runs the reactor's event loop in a separate thread so the timer(s)
  /// can run concurrently with the application.
  START_PERIODIC_MONITORS;

  /// Run the monitor checker in a separate thread.
  Monitor_Checker monitor_checker;
  monitor_checker.activate ();

  std::array<std::unique_ptr<char[]>, 5> str_array {};

  for (int i = 0; i < 10; ++i)
    {
      ACE_OS::sleep (1);

      // Alternate between letting the CPU sleep and keeping it busy.
      if (i % 2 == 0)
        {
          ACE_OS::sleep (1);
        }
      else
        {
          for (unsigned long j = 0; j < 5050505; j++)
            {
              (void) ACE::gcd (2419233733UL, 567715713UL);
            }
        }

      // Allocate a large string in the first five loops and free it later.
      if (i < 5)
        {
          str_array[i] = std::make_unique<char[]> (1024 * 1024 * 10);
        }
      else
        {
          str_array[i - 5].reset ();
        }
    }

  /// End the reactor's event loop, stopping the timer(s).
  STOP_PERIODIC_MONITORS;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
