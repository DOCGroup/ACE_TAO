// Task.cpp
// $Id$

#if !defined (ACE_TASK_T_C)
#define ACE_TASK_T_C

#define ACE_BUILD_DLL
#include "ace/Task_T.h"
#include "ace/Module.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task_T.i"
#endif /* __ACE_INLINE__ */

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
  : ACE_Task_Base (thr_man), 
    msg_queue_ (0),
    delete_msg_queue_ (0),
    mod_ (0),
    next_ (0)
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

#endif /* ACE_TASK_T_C */
