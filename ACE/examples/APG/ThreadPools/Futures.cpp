#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Method_Request.h"
#include "ace/Future.h"
#include "ace/Activation_Queue.h"
#include "ace/Condition_T.h"
#include <array>
#include <chrono>
#include <memory>
#include <thread>

constexpr int OUTSTANDING_REQUESTS = 20;

// Listing 2 code/ch16
class CompletionCallBack : public ACE_Future_Observer<ACE_CString *>
{
public:
  CompletionCallBack () = default;
  CompletionCallBack (CompletionCallBack const &) = delete;
  CompletionCallBack (CompletionCallBack &&) = delete;
  CompletionCallBack &operator= (CompletionCallBack const &) = delete;
  CompletionCallBack &operator= (CompletionCallBack &&) = delete;

  void update (ACE_Future<ACE_CString *> const &future) override
  {
    ACE_CString *result {};

    // Block for the result.
    future.get (result);
    std::unique_ptr<ACE_CString> result_owner {result};
    if (result_owner)
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("%C\n"), result_owner->c_str ()));
  }
};
// Listing 2
// Listing 1 code/ch16
class LongWork : public ACE_Method_Request
{
public:
  LongWork () = default;
  LongWork (LongWork const &) = delete;
  LongWork (LongWork &&) = delete;
  LongWork &operator= (LongWork const &) = delete;
  LongWork &operator= (LongWork &&) = delete;

  int call () override
  {
    ACE_TRACE ("LongWork::call");
    ACE_DEBUG
      ((LM_INFO, ACE_TEXT ("(%t) Attempting long work task\n")));
    std::this_thread::sleep_for (std::chrono::seconds (1));

    ACE_CString *msg {};
    ACE_NEW_RETURN (msg, ACE_CString ("Completed assigned task\n"), -1);
    std::unique_ptr<ACE_CString> msg_owner {msg};
    if (result_.set (msg_owner.get ()) == -1)
      return -1;
    msg_owner.release ();
    return 0;
  }

  void attach (CompletionCallBack *cb)
  {
    result_.attach (cb);
  }

private:
  ACE_Future<ACE_CString *> result_;
};
// Listing 1

class Exit : public ACE_Method_Request
{
public:
  Exit () = default;
  Exit (Exit const &) = delete;
  Exit (Exit &&) = delete;
  Exit &operator= (Exit const &) = delete;
  Exit &operator= (Exit &&) = delete;

  int call () override
  {
    ACE_TRACE ("Exit::call");
    return -1;
  }
};

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

// Listing 3 code/ch16
class Worker : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit Worker (IManager *manager)
    : manager_ (manager), queue_ (msg_queue ())
  { }
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int perform (ACE_Method_Request *req)
  {
    ACE_TRACE ("Worker::perform");
    // The caller retains ownership of the request until it has been processed.
    return this->queue_.enqueue (req);
  }

  int svc () override
  {
    this->thread_id_ = ACE_Thread::self ();
    while (true)
      {
        ACE_Method_Request *request = this->queue_.dequeue ();
        if (request == nullptr)
          return -1;

        // Requests are borrowed; the caller keeps them alive until processing finishes.
        int const result = request->call ();
        if (result == -1)
          break;

        // Return to work.
        this->manager_->return_to_work (this);
      }

    return 0;
  }

  ACE_thread_t thread_id () const;

private:
  IManager *const manager_; // Borrowed; Manager owns the pool and outlives its workers.
  ACE_thread_t thread_id_ {};
  ACE_Activation_Queue queue_;
};
// Listing 3

ACE_thread_t Worker::thread_id () const
{
  return thread_id_;
}

// Listing 4 code/ch16
class Manager : public ACE_Task_Base, private IManager
{
public:
  static constexpr int POOL_SIZE = 5;
  static constexpr int MAX_TIMEOUT = 5;

  Manager ()
    : workers_cond_ (workers_lock_)
  {
    ACE_TRACE ("Manager");
  }
  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

  int perform (ACE_Method_Request *req)
  {
    ACE_TRACE ("perform");
    // Requests remain caller-owned while they pass through both queues.
    return this->queue_.enqueue (req);
  }

  int svc () override
  {
    ACE_TRACE ("svc");

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started\n")));

    // Create pool when you get in the first time.
    create_worker_pool ();

    while (!done ())
      {
        ACE_Time_Value tv {static_cast<long> (MAX_TIMEOUT)};
        tv += ACE_OS::time (nullptr);

        // Get the next message
        ACE_Method_Request *request = this->queue_.dequeue (&tv);
        if (request == nullptr)
          {
            shut_down ();
            break;
          }

        // Choose a worker.
        Worker *const worker = choose_worker ();

        // Ask the worker to do the job.
        worker->perform (request);
      }

    return 0;
  }

  int shut_down ();

  int return_to_work (Worker *worker) override
  {
    ACE_GUARD_RETURN
      (ACE_Thread_Mutex, worker_mon, this->workers_lock_, -1);
    ACE_DEBUG
      ((LM_DEBUG, ACE_TEXT ("(%t) Worker returning to work.\n")));
    this->workers_.enqueue_tail (worker);
    this->workers_cond_.signal ();

    return 0;
  }

private:
  Worker *choose_worker ()
  {
    ACE_GUARD_RETURN
      (ACE_Thread_Mutex, worker_mon, this->workers_lock_, nullptr)

      while (this->workers_.is_empty ())
        workers_cond_.wait ();

    Worker *worker {};
    this->workers_.dequeue_head (worker);
    return worker;
  }

  int create_worker_pool ()
  {
    ACE_GUARD_RETURN
      (ACE_Thread_Mutex, worker_mon, this->workers_lock_, -1);
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

  bool done () const
  {
    return shutdown_;
  }

  ACE_thread_t thread_id (Worker *worker)
  {
    return worker->thread_id ();
  }

private:
  bool shutdown_ {};
  ACE_Thread_Mutex workers_lock_;
  ACE_Condition<ACE_Thread_Mutex> workers_cond_;
  ACE_Unbounded_Queue<Worker *> workers_; // Manager owns workers until their threads stop.
  ACE_Activation_Queue queue_;
};
// Listing 4

int
Manager::shut_down ()
{
  ACE_TRACE ("Manager::shut_down");
  ACE_Unbounded_Queue<Worker *>::ITERATOR iter = this->workers_.begin ();
  Worker **worker_ptr {};
  do
    {
      iter.next (worker_ptr);
      Worker *const worker = *worker_ptr;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Attempting shutdown of %d\n"),
                  thread_id (worker)));

      Exit *req {};
      ACE_NEW_RETURN (req, Exit (), -1);
      std::unique_ptr<Exit> request {req};

      // Send the hangup message
      worker->perform (request.get ());

      // Wait for the exit.
      worker->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Worker %d shut down.\n"),
                  thread_id (worker)));
      std::unique_ptr<Worker> worker_owner {worker};

    }
  while (iter.advance ());

  shutdown_ = true;

  return 0;
}

// Listing 5 code/ch16
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Manager tp;
  tp.activate ();

  // Wait for a few seconds every time you send a message.
  CompletionCallBack cb;
  std::array<LongWork, OUTSTANDING_REQUESTS> work_array {};
  for (int i = 0; i < OUTSTANDING_REQUESTS; ++i)
    {
      work_array[i].attach (&cb);
      std::this_thread::sleep_for (std::chrono::milliseconds (100));
      tp.perform (&work_array[i]);
    }

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
// Listing 5

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
