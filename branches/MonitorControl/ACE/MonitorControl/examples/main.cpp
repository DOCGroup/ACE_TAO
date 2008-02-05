
#include "MonitorControl/MonitorControl.h"

#include "ace/OS_NS_unistd.h"

int main (int argc, char *argv [])
{
  try
  {
    ACE_Message_Queue<ACE_NULL_SYNCH> monitored_queue;
    ACE_Message_Block *mb = 0;
    const char *msg = "Hidely Ho!";
    ACE_Time_Value tv (0, 555555);
    
    for (int k = 0; k < 10; ++k)
      {
        if (k < 6)
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
          
        ACE_OS::sleep (tv);
      }
      
    monitored_queue.flush ();
  /*
    START_MC_SERVICE;
    
    /// The Admin class will own the reactor and destroy it.
    ACE_Reactor* new_reactor = new ACE_Reactor;
    
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    mgr->admin ().reactor (new_reactor);
    
    /// Set the timer for CPU load check at 2000 msecs (2 sec).
    ADD_PERIODIC_MONITOR (CPU_LOAD_MONITOR, 2000);
    
    START_PERIODIC_MONITORS;
    
    for (int i = 0; i < 10; ++i)
      {
        /// Since the CPU load is checked every 2 seconds, the output
        /// of the monitor will vary between about 50% and 100%.
        if (i % 2 == 0)
          ACE_OS::sleep (1);
        else
          for (int j = 0; j < 500000000; j++);
      }

    STOP_PERIODIC_MONITORS;
    */
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