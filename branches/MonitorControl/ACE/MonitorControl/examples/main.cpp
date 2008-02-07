
#include "MonitorControl/MonitorControl.h"

#include "ace/OS_NS_unistd.h"

class MonitorChecker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
      
    ACE::MonitorControl::Monitor_Base *cpu_monitor =
      mgr->admin ().monitor_point ("CPULoad");

    ACE::MonitorControl::Monitor_Base *mq_monitor =
      mgr->admin ().monitor_point ("MQ monitor");
      
    for (int i = 0; i < 10; ++i)
      {
        MonitorControl_Types::Data data = cpu_monitor->retrieve ();
        ACE_DEBUG ((LM_DEBUG,
                    "%% CPU load:         %f\n",
//                    data.timestamp_,
                    data.value_));
                   
        data = mq_monitor->retrieve ();
        ACE_DEBUG ((LM_DEBUG,
                    "Message queue size: %u\n",
//                    data.timestamp_,
                    static_cast<size_t> (data.value_)));
                  
        ACE_OS::sleep (2);
      }

    return 0;
  }
};

int main (int argc, char *argv [])
{
  try
  {
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
    
    START_PERIODIC_MONITORS;
    
    ACE_Message_Queue<ACE_NULL_SYNCH> monitored_queue;
    monitored_queue.register_monitor ();
    ACE_Message_Block *mb = 0;
    const char *msg = "Hidely Ho!";
    
    MonitorChecker monitor_checker;
    monitor_checker.activate ();
    
    for (int i = 0; i < 10; ++i)
      {
        /// Add some message blocks to the queue, then remove
        /// some of them.
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
          
        /// Since the CPU load is checked every 2 seconds, the output
        /// of the monitor will vary between about 50% and 100%.
        if (i % 2 == 0)
          ACE_OS::sleep (1);
        else
          /// Do busy work to occupy the CPU.
          for (int j = 0; j < 500000000; j++);
      }
      
    monitored_queue.flush ();
  
    STOP_PERIODIC_MONITORS;
  }
  catch (const ACE::MonitorControl::MC_Generic_Registry::MapError &e)
  {
    switch (e.why_)
    {
      case ACE::MonitorControl::MC_Generic_Registry::MapError::MAP_ERROR_BIND_FAILURE:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Monitor add failed\n"),
                          -1);
        break;
      case ACE::MonitorControl::MC_Generic_Registry::MapError::MAP_ERROR_INVALID_VALUE:
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