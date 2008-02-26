
#include "ace/OS_NS_unistd.h"
#include "ace/Date_Time.h"
#include "ace/streams.h"
#include "ace/Auto_Ptr.h"
#include "ace/Monitor_Point_Registry.h"

#include "MonitorControl/MonitorControl.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

/// At this point in the development, a monitor's 'data' is
/// a double value plus a timestamp. The method below is a
/// utlity function to display the timestamp, formatted as
/// mm-dd-yyyy hr:min:sec.usec
void
display_timestamp (const MonitorControl_Types::Data &data)
{
  ACE_Date_Time dt (data.timestamp_);
  cout << setfill ('0')
       << setw (2) << dt.month () << '-'
       << setw (2) << dt.day () << '-'
       << dt.year () << ' '
       << setw (2) << dt.hour () << ':'
       << setw (2) << dt.minute () << ':'
       << setw (2) << dt.second () << '.'
       << setw (6) << dt.microsec () << ":   ";
}

/// Display the CPU load as a floating point percentage, to
/// 2 decimal places.
void
display_cpu_load (const MonitorControl_Types::Data &data)
{
  cout << "% CPU load:               ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
}

/// Display the CPU load as a floating point percentage, to
/// 2 decimal places.
void
display_memory_usage (const MonitorControl_Types::Data &data)
{
  cout << "% memory used:            ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
}

/// Display the CPU load as a floating point percentage, to
/// 2 decimal places.
void
display_bytes_sent (const MonitorControl_Types::Data &data)
{
  cout << "total bytes sent:         ";
  display_timestamp (data);
  cout << static_cast<ACE_UINT64> (data.value_) << endl;
}

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
      ACE_Dynamic_Service<MC_Admin_Manager>::instance ("MC_ADMINMANAGER");

    ACE::MonitorControl::Monitor_Base *cpu_monitor =
      mgr->admin ().monitor_point ("CPULoad");

    ACE::MonitorControl::Monitor_Base *memory_monitor =
      mgr->admin ().monitor_point ("MemoryUsage");

    ACE::MonitorControl::Monitor_Base *bytes_monitor =
      mgr->admin ().monitor_point ("BytesSent");

    /// Query the monitor for its data every 2 seconds, and call the
    /// appropriate display function.
    for (int i = 0; i < 10; ++i)
      {
        ACE_OS::sleep (2);

        MonitorControl_Types::Data data = cpu_monitor->retrieve ();
        display_cpu_load (data);

        data = memory_monitor->retrieve ();
        display_memory_usage (data);

        data = bytes_monitor->retrieve ();
        display_bytes_sent (data);
        
        cout << endl;
      }

    return 0;
  }
};

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

int main (int argc, char *argv [])
{
  try
  {
    /// Start up the MonitorControl service before doing anything else.
    START_MC_SERVICE;

    /// Creates these future group members without automatic update.
    ADD_MANUAL_MONITOR (BYTES_SENT_MONITOR);
    ADD_MANUAL_MONITOR (CPU_LOAD_MONITOR);
    ADD_MANUAL_MONITOR (MEMORY_USAGE_MONITOR);
    
    MonitorGroup<true> *group = new MonitorGroup<true> ("Test_Group");
    ACE_Auto_Ptr<MonitorGroup<true> > safety (group);
    
    MC_Admin_Manager *mgr =
      ACE_Dynamic_Service<MC_Admin_Manager>::instance ("MC_ADMINMANAGER");
      
    /// Fetch monitors from the registry and add them to the group.
      
    Monitor_Base *m_base = mgr->admin ().monitor_point ("CPULoad");
    group->add_member (m_base);
    
    m_base = mgr->admin ().monitor_point ("BytesSent");
    group->add_member (m_base);
      
    m_base = mgr->admin ().monitor_point ("MemoryUsage");
    group->add_member (m_base);
      
    /// Register the group as an auto-updated monitor point.  
    bool good_add = mgr->admin ().monitor_point (group, 2000);
    
    if (!good_add)
      {
        ACE_ERROR ((LM_ERROR,
                    "Group monitor \"%s\" registration failed.\n",
                    group->name ()));
      }
      
    /// A quick test of the registry's name cache.  
    MonitorControl_Types::NameList &mc_names =
      Monitor_Point_Registry::instance ()->names ();
      
    cout << "stored in monitor point registry:" << endl << endl;
      
    for (MonitorControl_Types::NameList::Iterator i (mc_names);
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
                (void) ACE::gcd (2419233733, 567715713);
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
  }
  catch (const MC_Generic_Registry::MapError &e)
  {
    /// Catch possible errors in monitor registration. Other exceptions
    /// will be implemented later on.
    switch (e.why_)
    {
      case MC_Generic_Registry::MapError::MAP_ERROR_BIND_FAILURE:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Monitor add failed\n"),
                          -1);
        break;
      case MC_Generic_Registry::MapError::MAP_ERROR_INVALID_VALUE:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid monitor\n"),
                          -1);
        break;
      default:
        break;
    }
  }

  return 0;
}
