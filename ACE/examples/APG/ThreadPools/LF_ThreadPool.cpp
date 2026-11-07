#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/Condition_T.h"
#include "ace/Message_Block.h"
#include <atomic>
#include <chrono>
#include <memory>
#include <cstring>
#include <thread>

constexpr int MESSAGE_COUNT = 30;

// Listing 4 code/ch16
class Follower
{
public:
  explicit Follower (ACE_Thread_Mutex &leader_lock)
    : cond_ (leader_lock), owner_ (ACE_Thread::self ())
  {
  }
  Follower (Follower const &) = delete;
  Follower (Follower &&) = delete;
  Follower &operator= (Follower const &) = delete;
  Follower &operator= (Follower &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  int wait ()
  {
    return this->cond_.wait ();
  }

  int signal ()
  {
    return this->cond_.signal ();
  }
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_thread_t owner () const
  {
    return this->owner_;
  }

private:
  ACE_Condition<ACE_Thread_Mutex> cond_;
  ACE_thread_t owner_ {};
};
// Listing 4
// Listing 1 code/ch16
class LF_ThreadPool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  LF_ThreadPool ()
  {
    ACE_TRACE ("LF_ThreadPool::TP");
  }
  LF_ThreadPool (LF_ThreadPool const &) = delete;
  LF_ThreadPool (LF_ThreadPool &&) = delete;
  LF_ThreadPool &operator= (LF_ThreadPool const &) = delete;
  LF_ThreadPool &operator= (LF_ThreadPool &&) = delete;

  int svc () override;

  void shut_down ()
  {
    ACE_GUARD (ACE_Thread_Mutex, leader_mon, this->leader_lock_);
    this->shutdown_.store (true);
    leader_active (0);

    ACE_GUARD (ACE_Thread_Mutex, follower_mon, this->followers_lock_);
    while (!this->followers_.is_empty ())
      {
        Follower *follower {};
        if (this->followers_.dequeue_head (follower) != 0)
          break;
        follower->signal ();
      }

    this->msg_queue ()->deactivate ();
  }

  int processed_messages () const
  {
    return this->processed_messages_.load ();
  }

private:
  int become_leader ();

  Follower *make_follower ();

  int elect_new_leader ();

  bool leader_active () const
  {
    ACE_TRACE ("LF_ThreadPool::leader_active");
    return this->current_leader_ != 0;
  }

  void leader_active (ACE_thread_t const leader)
  {
    ACE_TRACE ("LF_ThreadPool::leader_active");
    this->current_leader_ = leader;
  }

  void process_message (ACE_Message_Block_Unique_Ptr const &message_block);

  bool done () const
  {
    return this->shutdown_.load ();
  }

private:
  std::atomic<bool> shutdown_ {};
  std::atomic<int> processed_messages_ {};
  ACE_thread_t current_leader_ {};
  ACE_Thread_Mutex leader_lock_;
  ACE_Unbounded_Queue<Follower *> followers_; // Borrowed; each waiting thread owns its follower.
  ACE_Thread_Mutex followers_lock_;
  static constexpr long LONG_TIME = 5L;
};
// Listing 1
// Listing 2 code/ch16
int
LF_ThreadPool::svc ()
{
  ACE_TRACE ("LF_ThreadPool::svc");
  while (!done ())
    {
      if (become_leader () == -1)
        break;

      ACE_Message_Block *raw_message_block {};
      ACE_Time_Value tv {LONG_TIME};
      tv += ACE_OS::gettimeofday ();

      // Get a message, elect new leader, then process message.
      if (this->getq (raw_message_block, &tv) < 0)
        {
          if (done ())
            break;
          if (elect_new_leader () == 0)
            break;
          continue;
        }
      ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
      if (!message_block)
        continue;

      elect_new_leader ();
      process_message (message_block);
    }

  return 0;
}
// Listing 2
// Listing 3 code/ch16
int
LF_ThreadPool::become_leader ()
{
  ACE_TRACE ("LF_ThreadPool::become_leader");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, leader_mon, this->leader_lock_, -1);
  if (leader_active ())
    {
      std::unique_ptr<Follower> fw {make_follower ()};
      if (!fw)
        return -1;
      {
        // Wait until told to do so.
        while (leader_active () && !done ())
          {
            fw->wait ();
            if (leader_active () && !done ())
              {
                // Another follower became leader first; register again before waiting.
                ACE_GUARD_RETURN (ACE_Thread_Mutex,
                                  follower_mon,
                                  this->followers_lock_,
                                  -1);
                if (this->followers_.enqueue_tail (fw.get ()) == -1)
                  return -1;
              }
          }
      }

    }

  if (done ())
    return -1;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Becoming the leader\n")));

  // Mark yourself as the active leader.
  leader_active (ACE_Thread::self ());
  return 0;
}

Follower*
LF_ThreadPool::make_follower ()
{
  ACE_TRACE ("LF_ThreadPool::make_follower");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, follower_mon, this->followers_lock_, nullptr);
  Follower *follower_raw {};
  ACE_NEW_RETURN (follower_raw, Follower (this->leader_lock_), nullptr);
  std::unique_ptr<Follower> follower {follower_raw};
  if (this->followers_.enqueue_tail (follower.get ()) == -1)
    return nullptr;
  return follower.release ();
}
// Listing 3
// Listing 5 code/ch16
int
LF_ThreadPool::elect_new_leader ()
{
  ACE_TRACE ("LF_ThreadPool::elect_new_leader");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, leader_mon, this->leader_lock_, -1);
  leader_active (0);

  // Wake up a follower
  if (!followers_.is_empty ())
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex,
                        follower_mon,
                        this->followers_lock_,
                        -1);
      // Get the old follower.
      Follower *fw {};
      if (this->followers_.dequeue_head (fw) != 0)
        return -1;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Resigning and Electing %d\n"),
                  fw->owner ()));
      return (fw->signal () == 0) ? 0 : -1;
    }
  else
    {
      ACE_DEBUG
        ((LM_ERROR, ACE_TEXT ("(%t) Oops no followers left\n")));
      return -1;
    }
}
// Listing 5

void
LF_ThreadPool::process_message (ACE_Message_Block_Unique_Ptr const &message_block)
{
  ACE_TRACE ("LF_ThreadPool::process_message");
  int msgId {};
  std::memcpy (&msgId, message_block->rd_ptr (), sizeof (int));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Started processing message:%d\n"),
              msgId));
  std::this_thread::sleep_for (std::chrono::seconds (1));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Finished processing message:%d\n"),
              msgId));
  ++this->processed_messages_;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  LF_ThreadPool tp;
  tp.activate (THR_NEW_LWP| THR_JOINABLE, 5);

  // Wait for a few seconds...
  std::this_thread::sleep_for (std::chrono::seconds (2));

  ACE_Message_Block *raw_message_block {};
  for (int i = 0; i < MESSAGE_COUNT; ++i)
    {
      ACE_NEW_RETURN (raw_message_block, ACE_Message_Block (sizeof (int)), -1);
      ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
      std::memcpy (message_block->wr_ptr (), &i, sizeof (int));
      std::this_thread::sleep_for (std::chrono::seconds (1));

      // Add a new work item.
      if (tp.putq (message_block.get ()) == -1)
        return -1;
      message_block.release ();
    }

  while (tp.processed_messages () < MESSAGE_COUNT)
    std::this_thread::sleep_for (std::chrono::milliseconds (10));

  tp.shut_down ();
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
