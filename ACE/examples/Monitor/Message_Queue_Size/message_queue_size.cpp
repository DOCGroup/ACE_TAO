#include "ace/OS_NS_unistd.h"
#include "ace/Message_Block.h"

#include "ace/Monitor_Control/Monitor_Control.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#include <cstddef>
#include <cstring>
#include <sstream>
#include <string>

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

/// Subclass of ACE_Task_Base, meaning that the override of
/// the svc() method below will run in a new thread when
/// activate() is called on a class instance.
class Monitor_Checker : public ACE_Task_Base
{
private:
  void const * const addr_;

public:
  explicit Monitor_Checker (void const *addr)
    : addr_ (addr)
  {
  }

  Monitor_Checker (Monitor_Checker const &) = delete;
  Monitor_Checker (Monitor_Checker &&) = delete;
  Monitor_Checker &operator= (Monitor_Checker const &) = delete;
  Monitor_Checker &operator= (Monitor_Checker &&) = delete;

  int svc () override
  {
    constexpr size_t monitor_samples = 10;
    std::ostringstream name_stream;
    name_stream << "Message_Queue_" << ACE_OS::getpid () << '_' << this->addr_;
    std::string const name_str = name_stream.str ();

    /// Get an instance of the MC service singleton.
    MC_ADMINMANAGER * const mgr =
      ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    if (mgr == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR, "Monitor service is unavailable\n"), -1);

    /// Call on the administrator class to look up the desired monitors.
    ACE::Monitor_Control::Monitor_Base_Unique_Ptr const mq_monitor (
      mgr->admin ().monitor_point (name_str.c_str ()));

    if (mq_monitor != nullptr)
      {
        ACE_OS::sleep (1);

        /// Query the monitor for its data every second, and call the
        /// appropriate display function.
        for (size_t i {}; i < monitor_samples; ++i)
          {
            ACE_OS::sleep (1);

            Monitor_Control_Types::Data data (mq_monitor->type ());
            mq_monitor->retrieve (data);
            MC_Test_Utilities::display_mq_size (data);
          }

      }

    return 0;
  }
};

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int /* argc */, ACE_TCHAR * /* argv */ [])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)
  constexpr size_t iterations = 10;
  constexpr size_t enqueue_iterations = 6;

  /// Create a message queue with a built-in monitor (since ACE was
  /// compiled with monitors enabled) and add the monitor to the
  /// registry (some ACE activities create a message queue under
  /// the hood, so we must make the registration explicit).
  ACE_Message_Queue<ACE_NULL_SYNCH> monitored_queue;

  /// The message string is 11 bytes long so the message queue will
  /// grow and shrink in 11-byte increments.
  char const * const msg = "Hidely Ho!";

  /// Run the monitor checker in a separate thread.
  Monitor_Checker monitor_checker (&monitored_queue);
  monitor_checker.activate ();

  for (size_t i {}; i < iterations; ++i)
    {
      ACE_OS::sleep (1);

      /// Add 6 message blocks to the queue, then remove
      /// 4 of them.
      if (i < enqueue_iterations)
        {
          ACE_Message_Block_Unique_Ptr mb (
            new ACE_Message_Block (std::strlen (msg) + 1));
          mb->copy (msg);
          if (monitored_queue.enqueue_tail (mb.get ()) != -1)
            {
              mb.release ();
            }
        }
      else
        {
          ACE_Message_Block *mb {};
          if (monitored_queue.dequeue_head (mb) != -1)
            {
              ACE_Message_Block_Unique_Ptr dequeued_mb (mb);
            }
        }
    }

  /// This makes Purify much happier, but doesn't seem necessary for
  /// a regular run.
  monitor_checker.wait ();

  /// Clean up the remaining message queue resources.
  monitored_queue.flush ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

