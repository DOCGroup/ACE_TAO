// Task.cpp
// $Id$

#if !defined (ACE_TASK_C)
#define ACE_TASK_C

#define ACE_BUILD_DLL
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) 
// Lock the creation of the Singleton.
template <ACE_SYNCH_1>
ACE_Thread_Mutex ACE_Task_Exit<ACE_SYNCH_2>::ace_task_lock_;
#endif /* defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

template<ACE_SYNCH_1> ACE_Task_Exit<ACE_SYNCH_2> *
ACE_Task_Exit<ACE_SYNCH_2>::instance (void)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::instance");

#if defined (ACE_MT_SAFE) && defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) 
  // Lock the creation of the Singleton.  This should be inside of
  // ACE_Svc_Handler, but GNU G++ is too lame to handle this...
  static ACE_Thread_Mutex ace_task_lock_;
#endif /* defined (ACE_MT_SAFE) && defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

  // Determines if we were dynamically allocated.  
  static ACE_TSS_TYPE (ACE_Task_Exit<ACE_SYNCH_2>) *instance_;

  // Implement the Double Check pattern.

  if (instance_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ace_task_lock_, 0));

      if (instance_ == 0)
	ACE_NEW_RETURN (instance_, ACE_TSS_TYPE (ACE_Task_Exit<ACE_SYNCH_2>), 0);
    }

  return ACE_TSS_GET (instance_, ACE_Task_Exit<ACE_SYNCH_2>);

}

// Grab hold of the Task * so that we can close() it in the
// destructor.

template<ACE_SYNCH_1>
ACE_Task_Exit<ACE_SYNCH_2>::ACE_Task_Exit (void)
  : t_ (0), 
    status_ ((void *) -1)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::ACE_Task_Exit");
}

// Returns the pointer to the ACE_Task.

template<ACE_SYNCH_1> ACE_Task<ACE_SYNCH_2>*
ACE_Task_Exit<ACE_SYNCH_2>::get_task (void)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::get_task");

  return this->t_;
}

// Set the this pointer...

template<ACE_SYNCH_1> void
ACE_Task_Exit<ACE_SYNCH_2>::set_task (ACE_Task<ACE_SYNCH_2> *t)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::set_task");
  this->t_ = t;

  if (t != 0)
    this->tc_.insert (t->thr_mgr ());
}

// Set the thread exit status value.

template<ACE_SYNCH_1> void *
ACE_Task_Exit<ACE_SYNCH_2>::status (void *s)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::status");
  return this->status_ = s;
}

template<ACE_SYNCH_1> void *
ACE_Task_Exit<ACE_SYNCH_2>::status (void)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::status");
  return this->status_;
}

// When this object is destroyed the Task is automatically closed
// down!

template<ACE_SYNCH_1>
ACE_Task_Exit<ACE_SYNCH_2>::~ACE_Task_Exit (void)
{
  ACE_TRACE ("ACE_Task_Exit<ACE_SYNCH_2>::~ACE_Task_Exit");

  if (this->t_ != 0)
    {
      // The thread count must be decremented first in case the
      // close() hook does something crazy like "delete this".
      this->t_->thr_count_dec ();
      this->t_->close (u_long (this->status_));
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Task)

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Task<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nthr_mgr_ = %x", this->thr_mgr_));
  this->msg_queue_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_msg_queue_ = %d\n", this->delete_msg_queue_));
  ACE_DEBUG ((LM_DEBUG, "\nflags = %x", this->flags_));
  ACE_DEBUG ((LM_DEBUG, "\nmod_ = %x", this->mod_));
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\ngrp_id_ = %d", this->grp_id_));
  ACE_DEBUG ((LM_DEBUG, "\nthr_count_ = %d", this->thr_count_));
#if defined (ACE_MT_SAFE)
  this->lock_.dump ();
#endif /* ACE_MT_SAFE */

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// If the user doesn't supply a ACE_Message_Queue pointer then we'll
// allocate one dynamically.  Otherwise, we'll use the one they give.

template<ACE_SYNCH_1>
ACE_Task<ACE_SYNCH_2>::ACE_Task (ACE_Thread_Manager *thr_man, 
				 ACE_Message_Queue<ACE_SYNCH_2> *mq)
  : thr_count_ (0),
    thr_mgr_ (thr_man),
    msg_queue_ (0),
    delete_msg_queue_ (0),
    flags_ (0),
    mod_ (0), 
    next_ (0),
    grp_id_ (0)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::ACE_Task");

  if (mq == 0)
    {
      ACE_NEW (mq, ACE_Message_Queue<ACE_SYNCH_2>);
      this->delete_msg_queue_ = 1;
    }

  this->msg_queue_ = mq;
}

template<ACE_SYNCH_1>
ACE_Task<ACE_SYNCH_2>::~ACE_Task (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::~ACE_Task");
  if (this->delete_msg_queue_)
    delete this->msg_queue_;

  // These assignments aren't strickly necessary but they help guard
  // against odd race conditions...
  this->delete_msg_queue_ = 0;
  this->msg_queue_ = 0;
  this->thr_mgr_ = 0;
  this->mod_ = 0;
}

// Note that this routine often does not return since the thread that
// is executing it will do an ACE_Thread::exit() first!

template<ACE_SYNCH_1> void *
ACE_Task<ACE_SYNCH_2>::svc_run (ACE_Task<ACE_SYNCH_2> *t)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::svc_run");

  // Obtain our thread-specific exit hook and make sure that it knows
  // how to clean us up!
  ACE_Task_Exit<ACE_SYNCH_2> *exit_hook = 
    ACE_Task_Exit<ACE_SYNCH_2>::instance ();

  exit_hook->set_task (t);

  // Call the Task's svc() method.
  void *status = (void *) t->svc ();

  return exit_hook->status (status);
  /* NOTREACHED */
}

template<ACE_SYNCH_1> ACE_Task<ACE_SYNCH_2> *
ACE_Task<ACE_SYNCH_2>::sibling (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::sibling");
  if (this->mod_ == 0)
    return 0;
  else
    return this->mod_->sibling (this);
}

template<ACE_SYNCH_1> const char *
ACE_Task<ACE_SYNCH_2>::name (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::name");
  if (this->mod_ == 0)
    return 0;
  else
    return this->mod_->name ();
}

template<ACE_SYNCH_1> ACE_Module<ACE_SYNCH_2> *
ACE_Task<ACE_SYNCH_2>::module (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::module");
  return this->mod_;
}

// Get the current group id.
template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::grp_id (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::grp_id");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  return this->grp_id_;
}

// Set the current group id.
template <ACE_SYNCH_1> ACE_INLINE void
ACE_Task<ACE_SYNCH_2>::grp_id (int id)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::grp_id");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_));
  this->grp_id_ = id;
}

// Suspend a task.
template<ACE_SYNCH_1> int 
ACE_Task<ACE_SYNCH_2>::suspend (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::suspend");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (this->thr_count_ > 0)
    return this->thr_mgr_->suspend_task (this);
  else
    return 0;
}

// Resume a suspended task.
template<ACE_SYNCH_1> int 
ACE_Task<ACE_SYNCH_2>::resume (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::resume");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  if (this->thr_count_ > 0)
    return this->thr_mgr_->resume_task (this);
  else
    return 0;
}

template<ACE_SYNCH_1> int
ACE_Task<ACE_SYNCH_2>::activate (long flags, 
				 int n_threads, 
				 int force_active,
				 u_int priority,
				 int grp_id,
				 ACE_Task<ACE_SYNCH_2> *task)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::activate");

#if defined (ACE_MT_SAFE)
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  
  if (this->thr_count_ > 0 && force_active == 0)
    return 1; // Already active.
  else
    this->thr_count_ = n_threads;

  // Use the ACE_Thread_Manager singleton if we're running as an
  // active object and the caller didn't supply us with a
  // Thread_Manager.
  if (this->thr_mgr_ == 0)   
    this->thr_mgr_ = ACE_Service_Config::thr_mgr ();

  this->grp_id_ = this->thr_mgr_->spawn_n (n_threads, 
					   ACE_THR_FUNC (&ACE_Task<ACE_SYNCH_2>::svc_run),
					   (void *) this, 
					   flags,
					   priority,
					   grp_id,
					   task);
  if (this->grp_id_ == -1)
    return -1;
  else
    return 0;
#else
  {
    // Keep the compiler from complaining.
    n_threads = n_threads;
    force_active = force_active;
    flags = flags; 
    errno = EINVAL;
    return -1;
  }
#endif /* ACE_MT_SAFE */
}

#endif /* ACE_TASK_C */
