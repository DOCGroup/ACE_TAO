// Task.cpp
// $Id$

#ifndef ACE_TASK_T_C
#define ACE_TASK_T_C

#define ACE_BUILD_DLL
#include "ace/Task_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Module.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Task_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Task_T, "$Id$")

template <ACE_SYNCH_DECL> void
ACE_Task<ACE_SYNCH_USE>::dump (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nthr_mgr_ = %x"), this->thr_mgr_));
  this->msg_queue_->dump ();
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("delete_msg_queue_ = %d\n"), this->delete_msg_queue_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nflags = %x"), this->flags_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nmod_ = %x"), this->mod_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nnext_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ngrp_id_ = %d"), this->grp_id_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nthr_count_ = %d"), this->thr_count_));
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  this->lock_.dump ();
#endif /* ACE_MT_SAFE */

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// If the user doesn't supply a ACE_Message_Queue pointer then we'll
// allocate one dynamically.  Otherwise, we'll use the one they give.

template<ACE_SYNCH_DECL>
ACE_Task<ACE_SYNCH_USE>::ACE_Task (ACE_Thread_Manager *thr_man,
                                   ACE_Message_Queue<ACE_SYNCH_USE> *mq)
  : ACE_Task_Base (thr_man),
    msg_queue_ (0),
    delete_msg_queue_ (0),
    mod_ (0),
    next_ (0)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::ACE_Task");

  if (mq == 0)
    {
      ACE_NEW (mq, ACE_Message_Queue<ACE_SYNCH_USE>);
      this->delete_msg_queue_ = 1;
    }

  this->msg_queue_ = mq;
}

template<ACE_SYNCH_DECL>
ACE_Task<ACE_SYNCH_USE>::~ACE_Task (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::~ACE_Task");
  if (this->delete_msg_queue_)
    delete this->msg_queue_;

  // These assignments aren't strickly necessary but they help guard
  // against odd race conditions...
  this->delete_msg_queue_ = 0;
}

template<ACE_SYNCH_DECL> ACE_Task<ACE_SYNCH_USE> *
ACE_Task<ACE_SYNCH_USE>::sibling (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::sibling");
  if (this->mod_ == 0)
    return 0;
  else
    return this->mod_->sibling (this);
}

template<ACE_SYNCH_DECL> const ASYS_TCHAR *
ACE_Task<ACE_SYNCH_USE>::name (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::name");
  if (this->mod_ == 0)
    return 0;
  else
    return this->mod_->name ();
}

template<ACE_SYNCH_DECL> ACE_Module<ACE_SYNCH_USE> *
ACE_Task<ACE_SYNCH_USE>::module (void) const
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_USE>::module");
  return this->mod_;
}

// Initialize the <ACE_Message_Block_Buffer> to keep track of the
// <high_water_mark> and the <timeout>, which are used to determine at
// what point to flush the buffer.

template <class PEER_STREAM, ACE_SYNCH_DECL>
ACE_Buffered_Task<PEER_STREAM, ACE_SYNCH_USE>::ACE_Buffered_Task (PEER_STREAM stream,
                                                                size_t high_water_mark,
                                                                ACE_Time_Value *timeout)
  : current_size_ (0),
    high_water_mark_ (high_water_mark),
    timeout_ (timeout == 0 ? ACE_Time_Value::zero : *timeout),
    timeoutp_ (timeout),
    stream_ (stream)
{
}

template <class PEER_STREAM, ACE_SYNCH_DECL> int
ACE_Buffered_Task<PEER_STREAM, ACE_SYNCH_USE>::put (ACE_Message_Block *mb,
                                            ACE_Time_Value *tv)
{
#if 0
  if (this->putq (mb) == -1)
    return -1;
  else 
    {
      this->current_size_ += mb->total_size ();

      if (this->current_size_ >= this->high_water_mark_)
        // @@ Need to add the "timed flush" feature...
        return this->flush ();

      return 0;
    }
#else
  return 0;
#endif
}

// Flush the buffer.

template <class PEER_STREAM, ACE_SYNCH_DECL> int
ACE_Buffered_Task<PEER_STREAM, ACE_SYNCH_USE>::flush (void)
{
#if 0
  iovec iov[ACE_DEFAULT_WRITEV_MAX];
  size_t i = 0;

  ACE_Message_Queue_Iterator<ACE_NULL_SYNCH> iterator (queue);

  // Iterate over all the <ACE_Message_Block>s in the
  // <ACE_Message_Queue>.
  for (ACE_Message_Block *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      // Iterate over all the continuations (if any) in the
      // <Message_Block>.
      for (ACE_Message_Block *temp = entry;
           entry != 0;
           entry = entry->cont ())
        {
          iov[i].iov_len = entry->size ();
          iov[i].iov_buf = entry->rd_ptr ();

          i++;
          if (i == ACE_DEFAULT_WRITEV_MAX)
            {
              // Send off the data.
              if (this->stream_.sendv_n (iov,
                                         i) == -1)
                return -1;
              i = 0;
            }
        }
    }

  if (i > 0)
    return this->stream_.sendv_n (iov,
                                  i);
  else
    return 0;
#else
  return 0;
#endif
}

#endif /* ACE_TASK_T_C */
