// $Id$

#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Pipeline.h"
#include "JAWS/Pipeline_Tasks.h"
#include "JAWS/Policy.h"
#include "JAWS/Data_Block.h"

JAWS_Concurrency_Base::JAWS_Concurrency_Base (void)
{
}

int
JAWS_Concurrency_Base::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

int
JAWS_Concurrency_Base::svc (void)
{
  int result = 0;

  for (;;)
    {
      ACE_Message_Block *mb;  // The message queue element

      JAWS_Data_Block *db;           // Contains the task list
      JAWS_Dispatch_Policy *policy;  // Contains task policies
      JAWS_IO_Handler *handler;      // Keeps the state of the task
      JAWS_Pipeline_Handler *task;   // The task itself

      // At this point we could set a timeout value so that the
      // threading strategy can delete a thread if there is nothing to
      // do.  Carefully think how to implement it so you don't leave
      // yourself with 0 threads.
      result = this->getq (mb);

      // Use a NULL message block to indicate that the thread should shut
      // itself down
      if (result == -1 || mb == 0)
        break;

      db = ACE_dynamic_cast (JAWS_Data_Block *, mb->data_block ());
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

      // Thread specific message block
      JAWS_Data_Block ts_db;
      ACE_Message_Block ts_mb (&ts_db);

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
              ACE_TRACE (("%p\n", "JAWS_Concurrency_Base::svc, waiting"));
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
              ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Concurrency_Base::svc"));
              break;
            }

        }
      while (result == 0);

      policy->ioh_factory ()->destroy_io_handler (handler);
    }

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
