// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Method_Request.h"
#include "ace/Future.h"
#include "ace/Activation_Queue.h"
#include "ace/Condition_T.h"

#define OUTSTANDING_REQUESTS 20

// Listing 2 code/ch16
class CompletionCallBack: public ACE_Future_Observer<ACE_CString*>
{
public:
  virtual void update (const ACE_Future<ACE_CString*> & future)
  {
    ACE_CString *result = 0;

    // Block for the result.
    future.get (result);
    ACE_DEBUG ((LM_INFO, ACE_TEXT("%C\n"), result->c_str ()));
    delete result;
  }
};
// Listing 2
// Listing 1 code/ch16
class LongWork : public ACE_Method_Request
{
public:
  virtual int call (void)
  {
    ACE_TRACE (ACE_TEXT ("LongWork::call"));
    ACE_DEBUG
      ((LM_INFO, ACE_TEXT ("(%t) Attempting long work task\n")));
    ACE_OS::sleep (1);

    char buf[1024];
    ACE_OS::strcpy (buf, "Completed assigned task\n");
    ACE_CString *msg;
    ACE_NEW_RETURN
      (msg, ACE_CString (buf, ACE_OS::strlen (buf) + 1), -1);
    result_.set (msg);
    return 0;
  }

  ACE_Future<ACE_CString*> &future (void)
  {
    ACE_TRACE (ACE_TEXT ("LongWork::future"));
    return result_;
  }

  void attach (CompletionCallBack *cb)
  {
    result_.attach (cb);
  }

private:
  ACE_Future<ACE_CString*> result_;
};
// Listing 1

class Exit : public ACE_Method_Request
{
public:
  virtual int call (void)
  {
    ACE_TRACE (ACE_TEXT ("Exit::call"));
    return -1;
  }
};

class Worker;

class IManager
{
public:
  virtual ~IManager (void) { }

  virtual int return_to_work (Worker *worker) = 0;
};

// Listing 3 code/ch16
class Worker: public ACE_Task<ACE_MT_SYNCH>
{
public:
  Worker (IManager *manager)
    : manager_(manager), queue_ (msg_queue ())
  { }

  int perform (ACE_Method_Request *req)
  {
    ACE_TRACE (ACE_TEXT ("Worker::perform"));
    return this->queue_.enqueue (req);
  }

  virtual int svc (void)
  {
    thread_id_ = ACE_Thread::self ();
    while (1)
      {
        ACE_Method_Request *request = this->queue_.dequeue();
        if (request == 0)
          return -1;

        // Invoke the request
        int result = request->call ();
        if (result == -1)
          break;

        // Return to work.
        this->manager_->return_to_work (this);
      }

    return 0;
  }

  ACE_thread_t thread_id (void);

private:
  IManager *manager_;
  ACE_thread_t thread_id_;
  ACE_Activation_Queue queue_;
};
// Listing 3

ACE_thread_t Worker::thread_id (void)
{
  return thread_id_;
}

// Listing 4 code/ch16
class Manager : public ACE_Task_Base, private IManager
{
public:
  enum {POOL_SIZE = 5, MAX_TIMEOUT = 5};

  Manager ()
    : shutdown_(0), workers_lock_(), workers_cond_(workers_lock_)
  {
    ACE_TRACE (ACE_TEXT ("Manager::TP"));
  }

  int perform (ACE_Method_Request *req)
  {
    ACE_TRACE (ACE_TEXT ("Manager::perform"));
    return this->queue_.enqueue (req);
  }

  int svc (void)
  {
    ACE_TRACE (ACE_TEXT ("Manager::svc"));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started\n")));

    // Create pool when you get in the first time.
    create_worker_pool ();

    while (!done ())
      {
        ACE_Time_Value tv ((long)MAX_TIMEOUT);
        tv += ACE_OS::time (0);

        // Get the next message
        ACE_Method_Request *request = this->queue_.dequeue (&tv);
        if (request == 0)
          {
            shut_down ();
            break;
          }

        // Choose a worker.
        Worker *worker = choose_worker ();

        // Ask the worker to do the job.
        worker->perform (request);
      }

    return 0;
  }

  int shut_down (void);

  virtual int return_to_work (Worker *worker)
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
  Worker *choose_worker (void)
  {
    ACE_GUARD_RETURN
      (ACE_Thread_Mutex, worker_mon, this->workers_lock_, 0)

      while (this->workers_.is_empty ())
        workers_cond_.wait ();

    Worker *worker = 0;
    this->workers_.dequeue_head (worker);
    return worker;
  }

  int create_worker_pool (void)
  {
    ACE_GUARD_RETURN
      (ACE_Thread_Mutex, worker_mon, this->workers_lock_, -1);
    for (int i = 0; i < POOL_SIZE; i++)
      {
        Worker *worker;
        ACE_NEW_RETURN (worker, Worker (this), -1);
        this->workers_.enqueue_tail (worker);
        worker->activate ();
      }

    return 0;
  }

  int done (void)
  {
    return (shutdown_ == 1);
  }

  ACE_thread_t thread_id (Worker *worker)
  {
    return worker->thread_id ();
  }

private:
  int shutdown_;
  ACE_Thread_Mutex workers_lock_;
  ACE_Condition<ACE_Thread_Mutex> workers_cond_;
  ACE_Unbounded_Queue<Worker* > workers_;
  ACE_Activation_Queue queue_;
};
// Listing 4

int
Manager::shut_down (void)
{
  ACE_TRACE (ACE_TEXT ("Manager::shut_down"));
  ACE_Unbounded_Queue<Worker* >::ITERATOR iter = this->workers_.begin ();
  Worker **worker_ptr = 0;
  do
    {
      iter.next (worker_ptr);
      Worker *worker = (*worker_ptr);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Attempting shutdown of %d\n"),
                  thread_id (worker)));

      Exit *req;
      ACE_NEW_RETURN (req, Exit(), -1);

      // Send the hangup message
      worker->perform (req);

      // Wait for the exit.
      worker->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Worker %d shut down.\n"),
                  thread_id (worker)));

      delete req;
      delete worker;

    }
  while (iter.advance ());

  shutdown_ = 1;

  return 0;
}

// Listing 5 code/ch16
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Manager tp;
  tp.activate ();

  ACE_Time_Value tv;
  tv.msec (100);

  // Wait for a few seconds every time you send a message.
  CompletionCallBack cb;
  LongWork workArray[OUTSTANDING_REQUESTS];
  for (int i = 0; i < OUTSTANDING_REQUESTS; i++)
    {
      workArray[i].attach (&cb);
      ACE_OS::sleep (tv);
      tp.perform (&workArray[i]);
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
