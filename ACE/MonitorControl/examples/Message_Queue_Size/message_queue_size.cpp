// $Id$

#include "ace/OS_NS_unistd.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/examples/MC_Test_Utilities.h"

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class MonitorChecker : public ACE_Task_Base
{
private:
  void *addr_;

public:
  MonitorChecker (void *addr)
    : addr_ (addr)
  {
  }

  int svc (void)
  {
    /// Reconstruct the monitor's unique name using the queue's hex address.
    const int nibbles = 2 * sizeof (ptrdiff_t);
    char buf[nibbles + 1];
    ACE_OS::sprintf (buf, "%p", this->addr_);
    buf[nibbles] = '\0';
    ACE_CString name_str ("Message_Queue_");
    name_str += buf;

    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER* mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

    /// Call on the administrator class to look up the desired monitors.
    ACE::MonitorControl::Monitor_Base *mq_monitor =
      mgr->admin ().monitor_point (name_str.c_str ());

    if (mq_monitor != 0)
      {
        ACE_OS::sleep (1);

        /// Query each monitor for its data every 2 seconds, and call the
        /// appropriate display function.
        for (int i = 0; i < 10; ++i)
          {
            ACE_OS::sleep (1);

            Monitor_Control_Types::Data data;
            mq_monitor->retrieve (data);
            MC_Test_Utilities::display_mq_size (data);
          }

        mq_monitor->remove_ref ();
      }

    return 0;
  }
};

int main (int /* argc */, char * /* argv */ [])
{
  /// Create a message queue with a built-in monitor (since ACE was
  /// compiled with monitors enabled) and add the monitor to the
  /// registry (some ACE activities create a message queue under
  /// the hood, so we must make the registration explicit).
  ACE_Message_Queue<ACE_NULL_SYNCH> monitored_queue;

  /// The message string is 11 bytes long so the message queue will
  /// grow and shrink in 11-byte increments.
  ACE_Message_Block *mb = 0;
  const char *msg = "Hidely Ho!";

  /// Run the monitor checker in a separate thread.
  MonitorChecker monitor_checker (&monitored_queue);
  monitor_checker.activate ();

  for (int i = 0; i < 10; ++i)
    {
      ACE_OS::sleep (1);

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
    }

  /// This makes Purify much happier, but doesn't seem necessary for
  /// a regular run.
  monitor_checker.wait ();

  /// Clean up the remaining message queue resources.
  monitored_queue.flush ();

  return 0;
}
