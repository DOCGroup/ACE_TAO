// $Id$

#include "JAWS/JAWS.h"
#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Pipeline.h"
#include "JAWS/Pipeline_Tasks.h"
#include "JAWS/Policy.h"
#include "JAWS/Data_Block.h"
#include "JAWS/Waiter.h"
#include "JAWS/Reaper.h"

ACE_RCSID(JAWS, Concurrency, "$Id$")

JAWS_Concurrency_Base::JAWS_Concurrency_Base (void)
  : ACE_Task<ACE_MT_SYNCH> (new ACE_Thread_Manager),
    mb_acquired_ (0),
    mb_ (0),
    reaper_ (new JAWS_Reaper (this))
{
}

JAWS_Concurrency_Base::~JAWS_Concurrency_Base (void)
{
  delete this->thr_mgr_;
  delete this->reaper_;
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
  JAWS_TRACE ("JAWS_Concurrency_Base::svc");

  ACE_Message_Block *mb;         // The message queue element
  JAWS_Data_Block *db;           // Contains the task list

  mb = this->singleton_mb ();

  // A NULL data block indicates that the thread should shut
  // itself down
  if (mb == 0)
    {
      JAWS_TRACE ("JAWS_Concurrency_Base::svc, empty message block");
      return -1;
    }

  db = ACE_dynamic_cast (JAWS_Data_Block *, mb);

  this->svc_loop (db);

  return 0;
}

int
JAWS_Concurrency_Base::svc_loop (JAWS_Data_Block *db)
{
  JAWS_TRACE ("JAWS_Concurrency_Base::svc_loop");

  for (;;)
    if (this->svc_hook (db) != 0)
      break;

  return 0;
}

int
JAWS_Concurrency_Base::svc_hook (JAWS_Data_Block *db)
{
  JAWS_TRACE ("JAWS_Concurrency_Base::svc_hook");

  int result = 0;

  JAWS_Dispatch_Policy *policy;  // Contains task policies
  JAWS_IO_Handler *handler;      // Keeps the state of the task
  JAWS_Pipeline_Handler *task;   // The task itself
  JAWS_Data_Block *mb;     // The task message block

  // Thread specific message block and data block
  JAWS_Data_Block *ts_db = new JAWS_Data_Block (*db);
  if (ts_db == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Concurrency_Base::svc_hook"));
      return -1;
    }

  policy = db->policy ();
  task = db->task ();
  handler = 0;

  // Get the waiter index
  JAWS_Waiter *waiter = JAWS_Waiter_Singleton::instance ();
  int waiter_index = waiter->index ();

  mb = ts_db;
  do
    {
      JAWS_TRACE ("JAWS_Concurrency_Base::svc_hook, looping");

      // Use a NULL task to make the thread recycle now
      if (task == 0)
        {
          JAWS_TRACE ("JAWS_Concurrency_Base::svc_hook, recycling");
          break;
        }

      // the task should set the handler to the appropriate next step
      result = task->put (mb);

      // For when result == 0
      handler = mb->io_handler ();

      if (result == 1 || result == 2)
        {
          JAWS_TRACE ("JAWS_Concurrency_Base::svc_hook, waiting");
          // need to wait for an asynchronous event

          // We need a way to destroy all the handlers created by the
          // Asynch_Acceptor.  Figure this out later.
          handler = waiter->wait_for_completion (waiter_index);
          result = (handler == 0) ? -1 : 0;
        }

      if (result < 0)
        {
          // definately something wrong.
          JAWS_TRACE ("JAWS_Concurrency_Base::svc_hook, negative result");
          ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Concurrency_Base::svc_hook"));
          break;
        }

      mb = handler->message_block ();
      task = handler->task ();
    }
  while (result == 0);

  if (handler != 0)
    {
      handler->message_block ()->release ();
      delete handler; // handler->factory ()->destroy_io_handler (handler);
    }
  delete ts_db; // ts_db->release ();

  return result;
}

int
JAWS_Concurrency_Base::activate_hook (void)
{
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
  this->flags_ = flags;
  this->nthreads_ = nthreads;
  this->maxthreads_ = maxthreads;

  ACE_thread_t *thr_names = new ACE_thread_t[nthreads];

  if (this->activate (flags | THR_SUSPENDED,
                      nthreads,
                      0,          // force active
                      ACE_DEFAULT_THREAD_PRIORITY,
                      -1,         // group id
                      0,          // ACE_Task_Base
                      0,          // thread handles
                      0,          // stack
                      0,          // stack size
                      thr_names) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Thread_Pool_Task::activate"),
                      -1);

  for (int i = 0; i < nthreads; i++)
    {
      JAWS_Thread_ID thr_id(thr_names[i]);
      JAWS_IO_Handler *dummy = 0;

      JAWS_Waiter_Singleton::instance ()->insert (thr_id, dummy);
    }

  delete[] thr_names;

  this->thr_mgr_->resume_all ();

  if (this->reaper_->open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Reaper::open"),
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
  JAWS_TRACE ("JAWS_Thread_Per_Task::put");

  this->putq (mb, tv);
  return this->activate_hook ();
}

int
JAWS_Thread_Per_Task::svc_loop (JAWS_Data_Block *db)
{
  return this->svc_hook (db);
}

int
JAWS_Thread_Per_Task::activate_hook (void)
{
  const int force_active = 1;
  const int nthreads = 1;

  ACE_thread_t thr_name;

  if (this->activate (this->flags_ | THR_SUSPENDED,
                      nthreads,
                      force_active,
                      ACE_DEFAULT_THREAD_PRIORITY,
                      -1,         // group id
                      0,          // ACE_Task_Base
                      0,          // thread handle
                      0,          // stack
                      0,          // stack size
                      &thr_name) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Thread_Pool_Task::activate"),
                      -1);

  JAWS_Thread_ID thr_id(thr_name);
  JAWS_IO_Handler *dummy = 0;

  JAWS_Waiter_Singleton::instance ()->insert (thr_id, dummy);

  this->thr_mgr_->resume (thr_name);

  if (this->reaper_->open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Reaper::open"),
                      -1);

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
