// $Id$

#include "JAWS/JAWS.h"

#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Pipeline.h"
#include "JAWS/Pipeline_Tasks.h"
#include "JAWS/Policy.h"
#include "JAWS/Data_Block.h"

JAWS_Concurrency_Base::JAWS_Concurrency_Base (void)
  : mb_acquired_ (0),
    mb_ (0)
{
}

JAWS_Concurrency_Base::~JAWS_Concurrency_Base (void)
{
}

ACE_Message_Block *
JAWS_Concurrency_Base::singleton_mb (void)
{
  if (this->mb_acquired_ == 0)
    {
      ACE_Guard<ACE_Thread_Mutex> g(this->lock_);

      if (this->mb_acquired_ == 0)
        {
          int result;
          ACE_Message_Block *mb;

          result = this->getq (mb);
          this->mb_acquired_ = 1;

          if (result == -1 || mb == 0)
            return 0;

          this->mb_ = mb;
        }
    }

  return this->mb_;
}

int
JAWS_Concurrency_Base::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

int
JAWS_Concurrency_Base::svc (void)
{
  ACE_Message_Block *mb;         // The message queue element

  JAWS_Data_Block *db;           // Contains the task list
  JAWS_Dispatch_Policy *policy;  // Contains task policies
  JAWS_IO_Handler *handler;      // Keeps the state of the task
  JAWS_Pipeline_Handler *task;   // The task itself

  // Thread specific message block and data block
  JAWS_Data_Block ts_db;
  ACE_Message_Block ts_mb (&ts_db);

  int result = 0;

  mb = this->singleton_mb ();

  if (mb == 0)
    {
      JAWS_TRACE ("JAWS_Concurrency_Base::svc, empty message block");
      return -1;
    }

  for (;;)
    {
      // A NULL data block indicates that the thread should shut
      // itself down
      db = ACE_dynamic_cast (JAWS_Data_Block *, mb->data_block ());
      if (db == 0)
        {
          JAWS_TRACE ("JAWS_Concurrency_Base::svc, empty data block");
          break;
        }

      policy = db->policy ();

      // Each time we iterate, we create a handler to maintain
      // our state for us.
      handler = policy->ioh_factory ()->create_io_handler ();
      if (handler == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "JAWS_Server::open, can't create handler\n"));
          return -1;
        }

      // Set the initial task in the handler
      handler->task (db->task ());

      ts_db.task (db->task ());
      ts_db.policy  (db->policy ());
      ts_db.io_handler (handler);

      do
        {
          //  handler maintains the state of the protocol
          task = handler->task ();

          // Use a NULL task to make the thread recycle now
          if (task == 0)
            break;

          // the task should set the handler to the appropriate next step
          result = task->put (&ts_mb);

          if (result == 1)
            {
              JAWS_TRACE ("JAWS_Concurrency_Base::svc, waiting");
              // need to wait for an asynchronous event
              // I don't know how to do this yet, so pretend it's
              // an error

              // handler = wait for completion ();
              // db->io_handler (handler);
              // result = 0;
              result = -1;
            }

          if (result == -1)
            {
              // definately something wrong.
              JAWS_TRACE ("JAWS_Concurrency_Base::svc, negative result");
              ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Concurrency_Base::svc"));
              break;
            }

        }
      while (result == 0);

      policy->ioh_factory ()->destroy_io_handler (handler);
    }

  JAWS_TRACE ("JAWS_Concurrency_Base::svc, shutting down");

  return 0;
}

JAWS_Dispatcher::JAWS_Dispatcher (void)
  : policy_(0)
{
}

int
JAWS_Dispatcher::dispatch (ACE_Message_Block *mb)
{
  return this->policy ()->concurrency ()->put (mb);
}

JAWS_Dispatch_Policy *
JAWS_Dispatcher::policy (void)
{
  return this->policy_;
}

JAWS_Dispatch_Policy *
JAWS_Dispatcher::policy (JAWS_Dispatch_Policy *p)
{
  this->policy_ = p;
  return this->policy_;
}

int
JAWS_Thread_Pool_Task::open (long flags, int nthreads, int maxthreads)
{
  this->nthreads_ = nthreads;
  this->maxthreads_ = maxthreads;

  if (this->activate (flags, nthreads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Thread_Pool_Task::activate"),
                      -1);

  return 0;
}

int
JAWS_Thread_Per_Task::open (long flags, int maxthreads)
{
  this->flags_ = flags;
  this->maxthreads_ = maxthreads;
  return 0;
}

int
JAWS_Thread_Per_Task::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  const int force_active = 1;
  const int nthreads = 1;

  if (this->activate (this->flags_, nthreads, force_active) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Thread_Pool_Task::activate"),
                      -1);

  this->putq (mb, tv);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_Dispatcher, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<JAWS_Thread_Pool_Task, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<JAWS_Thread_Per_Task, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_Dispatcher, ACE_SYNCH_MUTEX>
#pragma instantiate  ACE_Singleton<JAWS_Thread_Pool_Task, ACE_SYNCH_MUTEX>
#pragma instantiate  ACE_Singleton<JAWS_Thread_Per_Task, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
