#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Message_Block.h"
#include <chrono>
#include <cstring>
#include <thread>

// Listing 2 code/ch16
class Workers : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Workers () = default;
  Workers (Workers const &) = delete;
  Workers (Workers &&) = delete;
  Workers &operator= (Workers const &) = delete;
  Workers &operator= (Workers &&) = delete;

  int svc () override
  {
    while (true)
      {
        ACE_Message_Block *raw_message_block {};
        if (this->getq (raw_message_block) == -1)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%t) Shutting down\n")));
            break;
          }
        ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
        if (!message_block)
          break;

        // Process the message.
        process_message (message_block);
      }

    return 0;
  }
  // Listing 2

private:
  void process_message (ACE_Message_Block_Unique_Ptr const &message_block)
  {
    ACE_TRACE ("Workers::process_message");
    int msgId {};
    std::memcpy (&msgId, message_block->rd_ptr (), sizeof (int));

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Started processing message %d\n"),
                msgId));
    std::this_thread::sleep_for (std::chrono::seconds (3));
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Finished processing message %d\n"),
                msgId));
  }
};

// Listing 1 code/ch16
class Manager : public ACE_Task<ACE_MT_SYNCH>
{
public:
  static constexpr int POOL_SIZE = 5;
  static constexpr int MAX_TIMEOUT = 5;

  Manager ()
  {
    ACE_TRACE ("Manager::Manager");
  }
  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

  int svc () override
  {
    ACE_TRACE ("Manager::svc");

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started\n")));

    // Create pool.
    Workers pool;
    pool.activate (THR_NEW_LWP | THR_JOINABLE, POOL_SIZE);

    while (!done ())
      {
        ACE_Message_Block *raw_message_block {};
        ACE_Time_Value tv {static_cast<long> (MAX_TIMEOUT)};
        tv += ACE_OS::time (nullptr);

        // Get a message request.
        if (this->getq (raw_message_block, &tv) < 0)
          {
            pool.msg_queue ()->deactivate ();
            pool.wait ();
            break;
          }
        ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
        if (!message_block)
          continue;

        // On success, the worker queue owns the message block and releases it.
        if (pool.putq (message_block.get ()) != -1)
          message_block.release ();
      }

    return 0;
  }

private:
  bool done () const;

  bool shutdown_ {};
};
// Listing 1

bool Manager::done () const
{
  return shutdown_;
}


int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Manager tp;
  tp.activate ();

  // Wait for a moment every time you send a message.
  ACE_Message_Block *raw_message_block {};
  for (int i = 0; i < 30; ++i)
    {
      ACE_NEW_RETURN
        (raw_message_block, ACE_Message_Block (sizeof (int)), -1);
      ACE_Message_Block_Unique_Ptr message_block {raw_message_block};

      std::memcpy (message_block->wr_ptr (), &i, sizeof (int));

      std::this_thread::sleep_for (std::chrono::milliseconds (100));

      // On success, the manager queue owns the message block.
      if (tp.putq (message_block.get ()) == -1)
        return -1;
      message_block.release ();
    }

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
