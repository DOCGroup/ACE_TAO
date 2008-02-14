
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
display_cpu_load (const MonitorControl_Types::Data &data)
{
  cout << "% CPU load:         ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
}

/// Display the message queue size as an unsigned integer.
void
display_mq_size (const MonitorControl_Types::Data &data)
{
  cout << "Message queue size: ";
  display_timestamp (data);
  cout << static_cast<size_t> (data.value_) << endl;
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
    ACE::MonitorControl::Monitor_Base *cpu_monitor =
      mgr->admin ().monitor_point ("CPULoad");
    ACE::MonitorControl::Monitor_Base *mq_monitor =
      mgr->admin ().monitor_point ("MQ monitor");
      
    /// Query each monitor for its data every 2 seconds, and call the
    /// appropriate display function.
    for (int i = 0; i < 10; ++i)
      {
        ACE_OS::sleep (2);
        
        MonitorControl_Types::Data data = cpu_monitor->retrieve ();
        display_cpu_load (data);
                   
        data = mq_monitor->retrieve ();
        display_mq_size (data);               
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
    
    /// The Admin class will own the reactor and destroy it. We are
    /// passing a vanilla reactor to show how it works, but in real
    /// life it could be some specialized reactor.
    ACE_Reactor* new_reactor = new ACE_Reactor;   
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    mgr->admin ().reactor (new_reactor);
    
    /// Set the timer for CPU load check at 2000 msecs (2 sec).
    ADD_PERIODIC_MONITOR (CPU_LOAD_MONITOR, 2000);
    
    /// Runs the reactor's event loop in a separate thread so the timer(s)
    /// can run concurrently with the application.
    START_PERIODIC_MONITORS;
    
    /// Create a message queue with a built-in monitor (since ACE was
    /// compiled with monitors enabled) and add the monitor to the
    /// registry (some ACE activities create a message queue under
    /// the hood, so we must make the registration explicit).
    ACE_Message_Queue<ACE_NULL_SYNCH> monitored_queue;
    monitored_queue.register_monitor ();
    
    /// The message string is 11 bytes long so the message queue will
    /// grow and shrink in 11-byte increments.
    ACE_Message_Block *mb = 0;
    const char *msg = "Hidely Ho!";
    
    /// Run the monitor checker in a separate thread.
    MonitorChecker monitor_checker;
    monitor_checker.activate ();
    
    /// Make sure the monitor checker is spawned before growing the queue.
    ACE_OS::sleep (1);
    
    for (int i = 0; i < 10; ++i)
      {
        /// Add 6 message blocks to the queue, then remove
        /// 4 of them.
        if (i < 6)
          {
            mb = new ACE_Message_Block (ACE_OS::strlen (msg) + 1);
            mb->copy (msg);
            monitored_queue.enqueue_tail (mb);
          }
        else
          {
            monitored_queue.dequeue_head (mb);
            mb->release ();
          }
          
        /// Alternate between letting the CPU sleep and keeping it
        /// busy. 
        if (i % 2 == 0)
          {
            ACE_OS::sleep (1);
          }
        else
          {
            for (unsigned long j = 0; j < 500000; j++)
              {
                (void) ACE_OS::fopen ("non existent file", "r");
              }
          }
      }
     
    /// Clean up the remaining message queue resources. 
    monitored_queue.flush ();
  
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