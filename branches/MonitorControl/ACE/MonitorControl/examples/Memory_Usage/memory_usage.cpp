
#include "ace/OS_NS_unistd.h"
#include "ace/Date_Time.h"
#include "ace/streams.h"

#include "MonitorControl/MonitorControl.h"

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
display_memory_usage (const MonitorControl_Types::Data &data)
{
  cout << "% memory used:         ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
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
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
      
    /// Call on the administrator class to look up the desired monitors.  
    ACE::MonitorControl::Monitor_Base *memory_monitor =
      mgr->admin ().monitor_point ("MemoryUsage");
      
    /// Query each monitor for its data every 2 seconds, and call the
    /// appropriate display function.
    for (int i = 0; i < 10; ++i)
      {
        ACE_OS::sleep (2);
        
        MonitorControl_Types::Data data = memory_monitor->retrieve ();
        display_memory_usage (data);
      }

    return 0;
  }
};

int main (int argc, char *argv [])
{
  try
  {
    /// Start up the MonitorControl service before doing anything else.
    START_MC_SERVICE;
    
    /// Set the timer for memory usage check at 2000 msecs (2 sec).
    ADD_PERIODIC_MONITOR (MEMORY_USAGE_MONITOR, 2000);
    
    /// Runs the reactor's event loop in a separate thread so the timer(s)
    /// can run concurrently with the application.
    START_PERIODIC_MONITORS;
    
    /// Run the monitor checker in a separate thread.
    MonitorChecker monitor_checker;
    monitor_checker.activate ();
    
    char * str_array[5] = {0};
    
    for (int i = 0; i < 10; ++i)
      { 
        ACE_OS::sleep (2);
        
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