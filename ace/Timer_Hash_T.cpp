// $Id$

#ifndef ACE_TIMER_HASH_T_C
#define ACE_TIMER_HASH_T_C

#include "ace/Timer_Hash_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"

ACE_RCSID(ace,
          Timer_Hash_T,
          "$Id$")

template <class TYPE>
struct Hash_Token
{
  Hash_Token (const void *act,
              size_t pos,
              long orig_id,
              const TYPE &type)
    : act_ (act),
      pos_ (pos),
      orig_id_ (orig_id),
      type_ (type)
  {}

  const void *act_;
  size_t pos_;
  long orig_id_;
  TYPE type_;
};

// Default constructor

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Hash_Upcall (void)
  : timer_hash_ (0)
{
  // Nothing
}

// Constructor that specifies a Timer_Hash to call up to

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Hash_Upcall (ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> *timer_hash)
  : timer_hash_ (timer_hash)
{
  // Nothing
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::registration (TIMER_QUEUE &,
                                                              ACE_Event_Handler *,
                                                              const void *)
{
  // Registration will be handled by the upcall functor of the timer
  // hash.
  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::preinvoke (TIMER_QUEUE &,
                                                           ACE_Event_Handler *,
                                                           const void *,
                                                           int,
                                                           const ACE_Time_Value &,
                                                           const void *&)
{
  // This method should never be invoked since we don't invoke
  // expire() on the buckets.
  ACE_ASSERT (0);
  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::postinvoke (TIMER_QUEUE &,
                                                            ACE_Event_Handler *,
                                                            const void *,
                                                            int,
                                                            const ACE_Time_Value &,
                                                            const void *)
{
  // This method should never be invoked since we don't invoke
  // expire() on the buckets.
  ACE_ASSERT (0);
  return 0;
}

// Calls up to timer_hash's upcall functor
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::timeout (TIMER_QUEUE &,
                                                         ACE_Event_Handler *,
                                                         const void *,
                                                         int,
                                                         const ACE_Time_Value &)
{
  // This method should never be invoked since we don't invoke
  // expire() on the buckets.
  ACE_ASSERT (0);
  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::cancel_type (TIMER_QUEUE &,
                                                             ACE_Event_Handler *,
                                                             int,
                                                             int &)
{
  // Cancellation will be handled by the upcall functor of the timer
  // hash.
  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::cancel_timer (TIMER_QUEUE &,
                                                              ACE_Event_Handler *,
                                                              int,
                                                              int)
{
  // Cancellation will be handled by the upcall functor of the timer
  // hash.
  return 0;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>::deletion (TIMER_QUEUE &,
                                                          ACE_Event_Handler *event_handler,
                                                          const void *arg)
{
  // Call up to the upcall functor of the timer hash since the timer
  // hash does not invoke deletion() on its upcall functor directly.
  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          ACE_const_cast (void *,
                                          arg));

  int result =
    this->timer_hash_->upcall_functor ().
    deletion (*this->timer_hash_,
              event_handler,
              h->act_);

  delete h;

  return result;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::ACE_Timer_Hash_Iterator_T (ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> &hash)
  : timer_hash_ (hash)
{
  this->first ();
  // Nothing
}

// Positions the iterator at the first node in the timing hash table

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> void
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::first (void)
{
  for (this->position_ = 0;
       this->position_ < this->timer_hash_.table_size_;
       this->position_++)
    {
      // Check for an empty entry
      if (!this->timer_hash_.table_[this->position_]->is_empty ())
        {
          this->iter_ = &this->timer_hash_.table_[this->position_]->iter ();
          this->iter_->first ();
          return;
        }
    }

  // Didn't find any
  this->iter_ = 0;
}

// Positions the iterator at the next node in the bucket or goes to the next
// bucket

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> void
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::next (void)
{
  if (this->isdone ())
    return;

  // If there is no more in the current bucket, go to the next
  if (this->iter_->isdone ())
    {
      for (this->position_++;
           this->position_ < this->timer_hash_.table_size_;
           this->position_++)
        {
          // Check for an empty entry
          if (!this->timer_hash_.table_[this->position_]->is_empty ())
            {
              this->iter_ = &this->timer_hash_.table_[this->position_]->iter ();
              this->iter_->first ();
              return;
            }
        }

      // Didn't find any.
      this->iter_ = 0;
    }
  else
    this->iter_->next ();
}

// Returns true when we are at the end (when bucket_item_ == 0)

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::isdone (void) const
{
  return this->iter_ == 0;
}

// Returns the node at the current position in the sequence

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::item (void)
{
  if (this->isdone ())
    return 0;

  return this->iter_->item ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::iter (void)
{
  this->iterator_->first ();
  return *this->iterator_;
}

// Create an empty queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::ACE_Timer_Hash_T (size_t table_size,
                                                                     FUNCTOR *upcall_functor,
                                                                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> (upcall_functor, freelist),
    size_ (0),
    table_size_ (table_size),
    table_functor_ (this),
    earliest_position_ (0)
#if defined (ACE_WIN64)
  , pointer_base_ (0)
#endif /* ACE_WIN64 */
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  ACE_NEW (table_,
           BUCKET *[table_size]);

  this->gettimeofday (ACE_OS::gettimeofday);

  for (size_t i = 0;
       i < table_size;
       i++)
    {
      ACE_NEW (this->table_[i],
               BUCKET (&this->table_functor_,
                       this->free_list_));
      this->table_[i]->gettimeofday (ACE_OS::gettimeofday);
    }

  ACE_NEW (iterator_,
           HASH_ITERATOR (*this));
}


template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::ACE_Timer_Hash_T (FUNCTOR *upcall_functor,
                                                                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> (upcall_functor, freelist),
    size_ (0),
    table_size_ (ACE_DEFAULT_TIMER_HASH_TABLE_SIZE),
    table_functor_ (this),
    earliest_position_ (0)
#if defined (ACE_WIN64)
  , pointer_base_ (0)
#endif /* ACE_WIN64 */
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  ACE_NEW (table_,
           BUCKET *[ACE_DEFAULT_TIMER_HASH_TABLE_SIZE]);


  this->gettimeofday (ACE_OS::gettimeofday);

  for (size_t i = 0;
       i < this->table_size_;
       i++)
    {
      ACE_NEW (this->table_[i],
               BUCKET (&this->table_functor_,
                       this->free_list_));
      this->table_[i]->gettimeofday (ACE_OS::gettimeofday);
    }

  ACE_NEW (iterator_,
           HASH_ITERATOR (*this));
}

// Remove all remaining items in the Queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::~ACE_Timer_Hash_T (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::~ACE_Timer_Hash_T");
  ACE_MT (ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_));

  delete iterator_;

  for (size_t i = 0;
       i < this->table_size_;
       i++)
    delete this->table_[i];

  delete [] this->table_;
}

// Checks if queue is empty.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::is_empty");
  return this->table_[this->earliest_position_]->is_empty ();
}

// Returns earliest time in a non-empty bucket

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> const ACE_Time_Value &
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::earliest_time");
  return this->table_[this->earliest_position_]->earliest_time ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Timer_Hash_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\ntable_size_ = %d"), this->table_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nearliest_position_ = %d"), this->earliest_position_));

  for (size_t i = 0; i < this->table_size_; i++)
    if (!this->table_[i]->is_empty ())
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nBucket %d contains nodes"), i));

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::reschedule (ACE_Timer_Node_T<TYPE> *expired)
{
  ACE_TRACE ("ACE_Timer_Hash_T::reschedule");

  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          ACE_const_cast (void *,
                                          expired->get_act ()));

  h->pos_ =
    expired->get_timer_value ().sec () % this->table_size_;

  h->orig_id_ =
    this->table_[h->pos_]->schedule (expired->get_type (),
                                     h,
                                     expired->get_timer_value (),
                                     expired->get_interval ());
  ACE_ASSERT (h->orig_id_ != -1);

#if 0
  ACE_DEBUG ((LM_DEBUG, "Hash::reschedule() resets %d in slot %d where it's id is %d and token is %x\n",
              expired->get_timer_value ().msec (),
              h->pos_,
              h->orig_id_,
              h));
#endif

  if (this->table_[this->earliest_position_]->is_empty ()
      || this->table_[h->pos_]->earliest_time ()
      < this->table_[this->earliest_position_]->earliest_time ())
    this->earliest_position_ = h->pos_;
}

// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> long
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::schedule_i (const TYPE &type,
                                                               const void *act,
                                                               const ACE_Time_Value &future_time,
                                                               const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Hash_T::schedule_i");

  size_t position =
    future_time.sec () % this->table_size_;

  Hash_Token<TYPE> *h;

  ACE_NEW_RETURN (h,
                  Hash_Token<TYPE> (act,
                                    position,
                                    0,
                                    type),
                  -1);

  h->orig_id_ =
    this->table_[position]->schedule (type,
                                      h,
                                      future_time,
                                      interval);
  ACE_ASSERT (h->orig_id_ != -1);

#if 0
  ACE_DEBUG ((LM_DEBUG, "Hash::schedule() placing %d in slot %d where it's id is %d and token is %x\n",
              future_time.msec (),
              position,
              h->orig_id_,
              h));
#endif

  if (this->table_[this->earliest_position_]->is_empty ()
      || this->table_[position]->earliest_time ()
      < this->table_[this->earliest_position_]->earliest_time ())
    this->earliest_position_ = position;

  ++this->size_;

#if defined (ACE_WIN64)
  // This is a Win64 hack, necessary because of the original (bad) decision
  // to use a pointer as the timer ID. This class doesn't follow the usual
  // timer expiration rules (see comments in header file) and is probably
  // not used much. The dynamic allocation of Hash_Tokens without
  // recording them anywhere is a large problem for Win64 since the
  // size of a pointer is 64 bits, but a long is 32. Since this class
  // is not much used, I'm hacking this, at least for now. If it becomes
  // an issue, I'll look at it again then.
  ptrdiff_t hi = ACE_reinterpret_cast (ptrdiff_t, h);
  if (this->pointer_base_ == 0)
    this->pointer_base_ = hi & 0xffffffff00000000;
  return ACE_reinterpret_cast (long, hi & 0xffffffff);
#else
  return ACE_reinterpret_cast (long, h);
#endif
}

// Locate and update the inteval on the timer_id

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::reset_interval (long timer_id,
                                                                   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Hash_T::reset_interval");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return -1;


#if defined (ACE_WIN64)
  unsigned long timer_offset = ACE_static_cast (unsigned long, timer_id);
  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          (this->pointer_base_ + timer_offset));
#else
  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          timer_id);
#endif /* ACE_WIN64 */

  return this->table_[h->pos_]->reset_interval (h->orig_id_,
                                                interval);
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the correct table timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::cancel (long timer_id,
                                                           const void **act,
                                                           int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return 0;

#if defined (ACE_WIN64)
  unsigned long timer_offset = ACE_static_cast (unsigned long, timer_id);
  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          (this->pointer_base_ + timer_offset));
#else
  Hash_Token<TYPE> *h =
    ACE_reinterpret_cast (Hash_Token<TYPE> *,
                          timer_id);
#endif /* ACE_WIN64 */

  int result = this->table_[h->pos_]->cancel (h->orig_id_,
                                              0,
                                              dont_call);

  if (result == 1)
    {
      // Call the close hooks.
      int cookie = 0;

      // cancel_type() called once per <type>.
      this->upcall_functor ().cancel_type (*this,
                                           h->type_,
                                           dont_call,
                                           cookie);

      // cancel_timer() called once per <timer>.
      this->upcall_functor ().cancel_timer (*this,
                                            h->type_,
                                            dont_call,
                                            cookie);

      if (h->pos_ == this->earliest_position_)
        this->find_new_earliest ();

      if (act != 0)
        *act = h->act_;

      delete h;

      --this->size_;
    }

  return result;
}

// Locate and remove all values of <type> from the timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::cancel (const TYPE &type,
                                                           int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");

  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  size_t i; // loop variable.

  Hash_Token<TYPE> **timer_ids;

  ACE_NEW_RETURN (timer_ids,
                  Hash_Token<TYPE> *[this->size_],
                  -1);
  size_t pos = 0;

  for (i = 0;
       i < this->table_size_;
       i++)
    {
      ACE_Timer_Queue_Iterator_T<TYPE,
                                 ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>,
                                 ACE_Null_Mutex> &iter =
        this->table_[i]->iter ();

      for (iter.first ();
           !iter.isdone ();
           iter.next ())
        if (iter.item ()->get_type () == type)
          timer_ids[pos++] =
            ACE_reinterpret_cast (Hash_Token<TYPE> *,
                                  ACE_const_cast (void *,
                                                  iter.item ()->get_act ()));
    }

  if (pos > this->size_)
    return -1;

  for (i = 0; i < pos; i++)
    {
      int result =
        this->table_[timer_ids[i]->pos_]->cancel (timer_ids[i]->orig_id_,
                                                  0,
                                                  dont_call);
      ACE_ASSERT (result == 1);
      ACE_UNUSED_ARG (result);

      delete timer_ids[i];

      --this->size_;
    }

  delete [] timer_ids;

  this->find_new_earliest ();

  // Call the close hooks.
  int cookie = 0;

  // cancel_type() called once per <type>.
  this->upcall_functor ().cancel_type (*this,
                                       type,
                                       dont_call,
                                       cookie);

  for (i = 0;
       i < pos;
       ++i)
    {
      // cancel_timer() called once per <timer>.
      this->upcall_functor ().cancel_timer (*this,
                                            type,
                                            dont_call,
                                            cookie);
    }

  return ACE_static_cast (int, pos);
}

// Removes the earliest node and finds the new earliest position

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::remove_first (void)
{
  if (this->is_empty ())
    return 0;

  ACE_Timer_Node_T<TYPE> *temp =
    this->table_[this->earliest_position_]->remove_first ();

  this->find_new_earliest ();

  --this->size_;

  return temp;
}

// Finds a new earliest position

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::find_new_earliest (void)
{
  for (size_t i = 0; i < this->table_size_; i++)
    if (!this->table_[i]->is_empty ())
      if (this->table_[this->earliest_position_]->is_empty ()
          || this->earliest_time () == ACE_Time_Value::zero
          || this->table_[i]->earliest_time () <= this->earliest_time ())
          this->earliest_position_ = i;
}

// Returns the earliest node without removing it

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::get_first (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::get_first");

  if (this->is_empty ())
    return 0;

  return this->table_[this->earliest_position_]->get_first ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::dispatch_info_i (const ACE_Time_Value &cur_time,
                                                                    ACE_Timer_Node_Dispatch_Info_T<TYPE> &info)
{
  int result =
    ACE_Timer_Queue_T<TYPE,FUNCTOR,ACE_LOCK>::dispatch_info_i (cur_time,
                                                               info);

  if (result == 1)
    {
      Hash_Token<TYPE> *h =
        ACE_reinterpret_cast (Hash_Token<TYPE> *,
                              ACE_const_cast (void *,
                                              info.act_));

      info.act_ = h->act_;
    }

  return result;
}

// Dummy version of expire to get rid of warnings in Sun CC 4.2

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::expire ()
{
  return ACE_Timer_Queue_T<TYPE,FUNCTOR,ACE_LOCK>::expire();
}

// Specialized expire for Timer Hash

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Hash_T::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;

  ACE_Timer_Node_T<TYPE> *expired;

  // Go through the table and expire anything that can be expired

  for (size_t i = 0;
       i < this->table_size_;
       i++)
    {
      while (!this->table_[i]->is_empty ()
             && this->table_[i]->earliest_time () <= cur_time)
        {
          expired = this->table_[i]->remove_first ();
          const void *act = expired->get_act ();
          int reclaim = 1;

          Hash_Token<TYPE> *h =
            ACE_reinterpret_cast (Hash_Token<TYPE> *,
                                  ACE_const_cast (void *,
                                                  act));

          ACE_ASSERT (h->pos_ == i);

#if 0
          ACE_DEBUG ((LM_DEBUG, "Hash::expire() expiring %d in slot %d where it's id is %d and token is %x\n",
                      expired->get_timer_value ().msec (),
                      h->pos_,
                      h->orig_id_,
                      h));
#endif

          // Check if this is an interval timer.
          if (expired->get_interval () > ACE_Time_Value::zero)
            {
              // Make sure that we skip past values that have already
              // "expired".
              do
                expired->set_timer_value (expired->get_timer_value ()
                                          + expired->get_interval ());
              while (expired->get_timer_value () <= cur_time);

              // Since this is an interval timer, we need to
              // reschedule it.
              this->reschedule (expired);
              reclaim = 0;
            }

          ACE_Timer_Node_Dispatch_Info_T<TYPE> info;

          // Get the dispatch info
          expired->get_dispatch_info (info);

          info.act_ = h->act_;

          const void *upcall_act = 0;

          this->preinvoke (info, cur_time, upcall_act);

          this->upcall (info, cur_time);

          this->postinvoke (info, cur_time, upcall_act);

          if (reclaim)
            {
              --this->size_;
              delete h;
            }

          number_of_timers_expired++;
         }
    }

  if (number_of_timers_expired > 0)
    this->find_new_earliest ();

  return number_of_timers_expired;
}

#endif /* ACE_TIMER_HASH_T_C */
