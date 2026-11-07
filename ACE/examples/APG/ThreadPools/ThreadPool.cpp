#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Method_Request.h"
#include "ace/Future.h"
#include "ace/Activation_Queue.h"
#include "ace/Condition_T.h"
#include "ace/Message_Block.h"
#include <array>
#include <chrono>
#include <memory>
#include <cstring>
#include <thread>

class Worker;

class IManager
{
public:
  IManager () = default;
  IManager (IManager const &) = delete;
  IManager (IManager &&) = delete;
  IManager &operator= (IManager const &) = delete;
  IManager &operator= (IManager &&) = delete;

  virtual ~IManager () = default;

  virtual int return_to_work (Worker *worker) = 0;
};

// Listing 2 code/ch16
class Worker : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit Worker (IManager *manager) : manager_ (manager) { }
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override
  {
    ACE_Thread_ID const id {};
    this->thread_id_ = id;
    while (true)
      {
        ACE_Message_Block *raw_message_block {};
        if (this->getq (raw_message_block) == -1)
          ACE_ERROR_BREAK
            ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("getq")));
        ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
        if (!message_block)
          break;
        if (message_block->msg_type () == ACE_Message_Block::MB_HANGUP)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%t) Shutting down\n")));
            break;
          }
        // Process the message.
        process_message (message_block);
        // Return to work.
        this->manager_->return_to_work (this);
      }

    return 0;
  }
  // Listing 2

  ACE_Thread_ID const &thread_id () const
  {
    return this->thread_id_;
  }

private:
  void process_message (ACE_Message_Block_Unique_Ptr const &message_block)
  {
    ACE_TRACE ("Worker::process_message");
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

  IManager *const manager_; // Borrowed; Manager owns the pool and outlives its workers.
  ACE_Thread_ID thread_id_ {};
};

// Listing 1 code/ch16
class Manager : public ACE_Task<ACE_MT_SYNCH>, private IManager
{
public:
  static constexpr int POOL_SIZE = 5;
  static constexpr int MAX_TIMEOUT = 5;

  Manager ()
    : workers_cond_ (workers_lock_)
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
    create_worker_pool ();

    while (!done ())
      {
        ACE_Message_Block *raw_message_block {};
        ACE_Time_Value tv {static_cast<long> (MAX_TIMEOUT)};
        tv += ACE_OS::time (nullptr);

        // Get a message request.
        if (this->getq (raw_message_block, &tv) < 0)
          {
            shut_down ();
            break;
          }
        ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
        if (!message_block)
          continue;

        // Choose a worker.
        Worker *worker {};
        {
          ACE_GUARD_RETURN (ACE_Thread_Mutex,
                            worker_mon, this->workers_lock_, -1);

          while (this->workers_.is_empty ())
            workers_cond_.wait ();

          this->workers_.dequeue_head (worker);
        }

        // On success, the worker queue owns the message block.
        if (worker->putq (message_block.get ()) != -1)
          message_block.release ();
      }

    return 0;
  }

  int shut_down ();

  ACE_Thread_ID const &thread_id (Worker *worker) const;

  int return_to_work (Worker *worker) override
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex,
                      worker_mon, this->workers_lock_, -1);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Worker %t returning to work.\n")));
    this->workers_.enqueue_tail (worker);
    this->workers_cond_.signal ();

    return 0;
  }

private:
  int create_worker_pool ()
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex,
                      worker_mon,
                      this->workers_lock_,
                      -1);
    for (int i = 0; i < POOL_SIZE; ++i)
      {
        Worker *worker_raw {};
        ACE_NEW_RETURN (worker_raw, Worker (this), -1);
        std::unique_ptr<Worker> worker {worker_raw};
        if (this->workers_.enqueue_tail (worker.get ()) == -1)
          return -1;
        worker.release ();
        worker_raw->activate ();
      }

    return 0;
  }

  bool done () const;

private:
  bool shutdown_ {};
  ACE_Thread_Mutex workers_lock_;
  ACE_Condition<ACE_Thread_Mutex> workers_cond_;
  ACE_Unbounded_Queue<Worker *> workers_; // Manager owns the workers while queued.
};
// Listing 1

bool Manager::done () const
{
  return shutdown_;
}

int
Manager::shut_down ()
{
  ACE_TRACE ("Manager::shut_down");
  ACE_Unbounded_Queue<Worker *>::ITERATOR iter =
    this->workers_.begin ();
  Worker **worker_ptr {};
  do
    {
      iter.next (worker_ptr);
      Worker *const worker = *worker_ptr;
      ACE_Thread_ID const &id = thread_id (worker);
      std::array<char, 65> buf {};
      id.to_string (buf.data (), buf.size ());
      ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("(%t) Attempting shutdown of %C\n"),
                 buf.data ()));

      // Send the hangup message.
      ACE_Message_Block *raw_message_block {};
      ACE_NEW_RETURN
        (raw_message_block,
         ACE_Message_Block (0, ACE_Message_Block::MB_HANGUP),
         -1);
      ACE_Message_Block_Unique_Ptr message_block {raw_message_block};
      if (worker->putq (message_block.get ()) == -1)
        return -1;
      message_block.release ();

      // Wait for the exit.
      worker->wait ();

      ACE_ASSERT (worker->msg_queue ()->is_empty ());
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Worker %C shut down.\n"),
                  buf.data ()));
      std::unique_ptr<Worker> worker_owner {worker};
    }
  while (iter.advance ());

  shutdown_ = true;

  return 0;
}

ACE_Thread_ID const &
Manager::thread_id (Worker *worker) const
{
  return worker->thread_id ();
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

      // Add a new work item.
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
