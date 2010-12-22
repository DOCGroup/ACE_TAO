// $Id$

#include "ace/config-lite.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Method_Request.h"
#include "ace/Future.h"
#include "ace/Activation_Queue.h"
#include "ace/Condition_T.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

class Worker;

class IManager
{
public:
  virtual ~IManager (void) { }

  virtual int return_to_work (Worker *worker) = 0;
};

// Listing 2 code/ch16
class Worker : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Worker (IManager *manager) : manager_(manager) { }

  virtual int svc (void)
  {
    ACE_Thread_ID id;
    thread_id_ = id;
    while (1)
      {
        ACE_Message_Block *mb = 0;
        if (this->getq (mb) == -1)
          ACE_ERROR_BREAK
            ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("getq")));
        if (mb->msg_type () == ACE_Message_Block::MB_HANGUP)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%t) Shutting down\n")));
            mb->release ();
            break;
          }
        // Process the message.
        process_message (mb);
        // Return to work.
        this->manager_->return_to_work (this);
      }

    return 0;
  }

  const ACE_Thread_ID& thread_id (void)
  {
    return thread_id_;
  }

private:
  void process_message (ACE_Message_Block *mb)
  {
    ACE_TRACE (ACE_TEXT ("Worker::process_message"));
    int msgId;
    ACE_OS::memcpy (&msgId, mb->rd_ptr (), sizeof(int));
    mb->release ();

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Started processing message %d\n"),
                msgId));
    ACE_OS::sleep (3);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Finished processing message %d\n"),
                msgId));
  }

  IManager *manager_;
  ACE_Thread_ID thread_id_;
};

// Listing 1 code/ch16
class Manager: public ACE_Task<ACE_MT_SYNCH>, private IManager
{
public:
  enum {POOL_SIZE = 5, MAX_TIMEOUT = 5};

  Manager ()
    : shutdown_(0), workers_lock_(), workers_cond_(workers_lock_)
  {
    ACE_TRACE (ACE_TEXT ("Manager::Manager"));
  }

  int svc (void)
  {
    ACE_TRACE (ACE_TEXT ("Manager::svc"));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started\n")));

    // Create pool.
    create_worker_pool ();

    while (!done ())
      {
        ACE_Message_Block *mb = 0;
        ACE_Time_Value tv ((long)MAX_TIMEOUT);
        tv += ACE_OS::time (0);

        // Get a message request.
        if (this->getq (mb, &tv) < 0)
          {
            shut_down ();
            break;
          }

        // Choose a worker.
        Worker *worker = 0;
        {
          ACE_GUARD_RETURN (ACE_Mutex,
                            worker_mon, this->workers_lock_, -1);

          while (this->workers_.is_empty ())
            workers_cond_.wait ();

          this->workers_.dequeue_head (worker);
        }

        // Ask the worker to do the job.
        worker->putq (mb);
      }

    return 0;
  }

  int shut_down (void);

  const ACE_Thread_ID& thread_id (Worker *worker);

  virtual int return_to_work (Worker *worker)
  {
    ACE_GUARD_RETURN (ACE_Mutex,
                      worker_mon, this->workers_lock_, -1);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Worker %t returning to work.\n")));
    this->workers_.enqueue_tail (worker);
    this->workers_cond_.signal ();

    return 0;
  }

private:
  int create_worker_pool (void)
  {
    ACE_GUARD_RETURN (ACE_Mutex,
                      worker_mon,
                      this->workers_lock_,
                      -1);
    for (int i = 0; i < POOL_SIZE; i++)
      {
        Worker *worker;
        ACE_NEW_RETURN (worker, Worker (this), -1);
        this->workers_.enqueue_tail (worker);
        worker->activate ();
      }

    return 0;
  }

  int done (void);

private:
  int shutdown_;
  ACE_Mutex workers_lock_;
  ACE_Condition<ACE_Mutex> workers_cond_;
  ACE_Unbounded_Queue<Worker* > workers_;
};
// Listing 1

int Manager::done (void)
{
  return (shutdown_ == 1);
}

int
Manager::shut_down (void)
{
  ACE_TRACE (ACE_TEXT ("Manager::shut_down"));
  ACE_Unbounded_Queue<Worker* >::ITERATOR iter =
    this->workers_.begin ();
  Worker **worker_ptr = 0;
  do
    {
      iter.next (worker_ptr);
      Worker *worker = (*worker_ptr);
      ACE_Thread_ID id = thread_id (worker);
      char buf [65];
      id.to_string (buf);
      ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("(%t) Attempting shutdown of %C\n"),
                 buf));

      // Send the hangup message.
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN
        (mb,
         ACE_Message_Block(0,
                           ACE_Message_Block::MB_HANGUP),
         -1);
      worker->putq (mb);

      // Wait for the exit.
      worker->wait ();

      ACE_ASSERT (worker->msg_queue ()->is_empty ());
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Worker %C shut down.\n"),
                  buf));
      delete worker;
    }
  while (iter.advance ());

  shutdown_ = 1;

  return 0;
}

const ACE_Thread_ID&
Manager::thread_id (Worker *worker)
{
  return worker->thread_id ();
}
#endif

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3878_Regression_Test"));

#if defined (ACE_HAS_THREADS)
  Manager tp;
  tp.activate ();

  // Wait for a moment every time you send a message.
  ACE_Time_Value tv;
  tv.msec (100);

  ACE_Message_Block *mb = 0;
  for (int i = 0; i < 3; i++)
    {
      ACE_NEW_RETURN
        (mb, ACE_Message_Block(sizeof(int)), -1);

      ACE_OS::memcpy (mb->wr_ptr (), &i, sizeof(int));

      ACE_OS::sleep (tv);

      // Add a new work item.
      tp.putq (mb);
    }

  ACE_Thread_Manager::instance ()->wait ();
#else /* ACE_HAS_THREADS */
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads are not supported on this platform\n")));
#endif

  ACE_END_TEST;

  return 0;
}

