#if !defined (ACE_TIMER_HASH_T_C)
#define ACE_TIMER_HASH_T_C

#define ACE_BUILD_DLL

#include "ace/Timer_Hash_T.h"

struct Hash_Token
{
  Hash_Token (const void *act, size_t pos, long orig_id)
    : act_ (act), pos_ (pos), orig_id_ (orig_id) 
  {}

  const void *act_;
  size_t pos_;
  long orig_id_;
};

// Default constructor

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>::ACE_Timer_Hash_Upcall (void)
  : timer_hash_ (NULL)
{
  // Nothing
}

// Constructor that specifies a Timer_Hash to call up to

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>::ACE_Timer_Hash_Upcall (ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> *timer_hash)
  : timer_hash_ (timer_hash)
{
  // Nothing
}

// Calls up to timer_hash's upcall functor

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>::timeout (ACE_Timer_Queue_T<ACE_Event_Handler *, 
						     ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>, 
						     ACE_Null_Mutex> &timer_queue,
                                                     ACE_Event_Handler *handler,
                                                     const void *arg,
                                                     const ACE_Time_Value &cur_time)
{
  ACE_UNUSED_ARG (timer_queue);
  
  Hash_Token *h = (Hash_Token *)arg;
  
  int ret = this->timer_hash_->upcall_functor ().timeout (*this->timer_hash_, 
                                                          handler, 
                                                          h->act_, 
                                                          cur_time);
  delete h;
  return ret;
}
  

// Calls up to timer_hash's upcall functor

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>::cancellation (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							  ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>, 
							  ACE_Null_Mutex> &timer_queue,
                                                          ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);
  return this->timer_hash_->upcall_functor ().cancellation (*this->timer_hash_, 
                                                            handler);
}


// Calls up to timer_hash's upcall functor

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>::deletion (ACE_Timer_Queue_T<ACE_Event_Handler *, 
						      ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>, 
						      ACE_Null_Mutex> &timer_queue,
                                                      ACE_Event_Handler *handler,
                                                      const void *arg)
{
  ACE_UNUSED_ARG (timer_queue);

  Hash_Token *h = (Hash_Token *)arg;
  
  int ret = this->timer_hash_->upcall_functor ().deletion (*this->timer_hash_, 
                                                           handler,
                                                           h->act_);
  delete h;
  return ret;
}



template <class TYPE, class FUNCTOR, class LOCK, class BUCKET>
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, LOCK, BUCKET>::ACE_Timer_Hash_Iterator_T (ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET> &hash)
  : timer_hash_ (hash)
{
  // Nothing
}


// Positions the iterator at the first node in the timing hash table

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> void 
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, LOCK, BUCKET>::first (void)
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
  this->iter_ = NULL;
}


// Positions the iterator at the next node in the bucket or goes to the next
// bucket

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> void 
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, LOCK, BUCKET>::next (void)
{
  if (this->isdone ())
    return;

  // If there is no more in the current bucket, go to the next
  if (this->iter_->isdone ())
    {
      for (this->position_++; this->position_ < this->timer_hash_.table_size_; this->position_++)
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
      this->iter_ = NULL;
    }
  else
    this->iter_->next ();
}


// Returns true when we are at the end (when bucket_item_ == 0)

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> int 
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, LOCK, BUCKET>::isdone (void)
{
  return this->iter_ == NULL;
}


// Returns the node at the current position in the sequence

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, LOCK, BUCKET>::item (void)
{
  if (this->isdone ())
    return NULL;

  return this->iter_->item ();
}

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK> &
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::iter (void)
{
  return this->iterator_;
}

// Create an empty queue.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::ACE_Timer_Hash_T (size_t table_size, 
                                                                 FUNCTOR *upcall_functor, 
                                                                 ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> (upcall_functor, freelist),
    size_ (0),
    table_ (new BUCKET* [table_size]),
    table_size_ (table_size),
    table_functor_ (this),
    earliest_position_ (0),
    iterator_ (*this)
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  this->gettimeofday (ACE_High_Res_Timer::gettimeofday);

  for (size_t i = 0; i < table_size; i++)
    {
      this->table_[i] = new BUCKET;
      this->table_[i]->gettimeofday (ACE_High_Res_Timer::gettimeofday);
      this->table_[i]->set_upcall_functor (&this->table_functor_);
      this->table_[i]->free_list (this->free_list_);
    }

  this->table_functor_;
}


template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::ACE_Timer_Hash_T (FUNCTOR *upcall_functor, 
                                                                 ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> (upcall_functor, freelist),
    size_ (0),
    table_ (new BUCKET* [ACE_DEFAULT_TIMER_HASH_TABLE_SIZE]),
    table_size_ (ACE_DEFAULT_TIMER_HASH_TABLE_SIZE),
    table_functor_ (this),
    earliest_position_ (0),
    iterator_ (*this)
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  this->gettimeofday (ACE_High_Res_Timer::gettimeofday);

  for (size_t i = 0; i < this->table_size_; i++)
    {
      this->table_[i] = new BUCKET (&this->table_functor_, this->free_list_);
      this->table_[i]->gettimeofday (ACE_High_Res_Timer::gettimeofday);
    }

  this->table_functor_;
}


// Remove all remaining items in the Queue.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::~ACE_Timer_Hash_T (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::~ACE_Timer_Hash_T");
  ACE_MT (ACE_GUARD (LOCK, ace_mon, this->mutex_));

  for (size_t i = 0; i < this->table_size_; i++)
    delete this->table_[i];

  delete [] this->table_;    
}

// Checks if queue is empty.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> int 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::is_empty");
  return this->table_[this->earliest_position_]->is_empty ();
}


// Returns earliest time in a non-empty bucket

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> const ACE_Time_Value &
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::earliest_time");
  return this->table_[this->earliest_position_]->earliest_time ();
}

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> void 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\ntable_size_ = %d", this->table_size_));
  ACE_DEBUG ((LM_DEBUG, "\nearliest_position_ = %d", this->earliest_position_));
  for (size_t i = 0; i < this->table_size_; i++)
      if (!this->table_[i]->is_empty ())
        ACE_DEBUG ((LM_DEBUG, "\nBucket %d contains nodes", i));  
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}


// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> void 
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::reschedule (ACE_Timer_Node_T<TYPE> *expired)
{
  ACE_TRACE ("ACE_Timer_Hash_T::reschedule");

  size_t position = expired->get_timer_value ().usec () % this->table_size_;

  Hash_Token *h = (Hash_Token *)expired->get_act ();

  h->orig_id_ = this->table_[position]->schedule (expired->get_type (),
                                                  h,
                                                  expired->get_timer_value (),
                                                  expired->get_interval ());

  if (this->table_[this->earliest_position_]->is_empty () 
      || this->table_[this->earliest_position_]->earliest_time () < this->table_[position]->earliest_time ())
    this->earliest_position_ = position;
}


// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> long
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::schedule (const TYPE &type,
						 const void *act,
						 const ACE_Time_Value &future_time, 
						 const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Hash_T::schedule");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  size_t position = future_time.usec () % this->table_size_;

  Hash_Token *h = new Hash_Token (act, position, 0);

  h->orig_id_ = this->table_[position]->schedule (type,
                                                  h,
                                                  future_time,
                                                  interval);

  if (this->table_[this->earliest_position_]->is_empty ()
      || this->table_[this->earliest_position_]->earliest_time () < this->table_[position]->earliest_time ())
    this->earliest_position_ = position;

  ++this->size_;

  return (long) h;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the correct table timer queue.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::cancel (long timer_id,
					       const void **act,
					       int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by schedule ()
  if (timer_id == -1)
    return 0;
 
  Hash_Token *h = (Hash_Token *)timer_id;

  int ret = this->table_[h->pos_]->cancel (h->orig_id_, act, dont_call);

  if (act != 0)
    *act = h->act_;

  delete h;

  --this->size_;

  return ret;
}


// Locate and remove all values of <type> from the timer queue.

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::cancel (const TYPE &type,
					       int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  size_t i; // loop  variable

  Hash_Token **timer_ids = new Hash_Token *[this->size_];
  size_t pos = 0;

  for (i = 0; i < this->table_size_; i++)
    {
      ACE_Timer_Queue_Iterator_T<TYPE, ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, LOCK>, ACE_Null_Mutex> &iter = this->table_[i]->iter ();
      for (iter.first (); !iter.isdone (); iter.next ())
        timer_ids[pos++] = (Hash_Token *)iter.item ()->get_act ();
    }

  ACE_ASSERT (pos <= this->size_);

  for (i = 0; i < pos; i++)
    {
      this->table_[timer_ids[i]->pos_]->cancel (timer_ids[i]->orig_id_, 0, dont_call);
    
      dont_call = 1; // Make sure the functor is only called on the first expiration

      delete timer_ids[i];

      --this->size_;
    }

  delete timer_ids;

  return pos;
}


// Removes the earliest node and finds the new earliest position

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::remove_first (void)
{
  if (this->is_empty ())
    return NULL;

  ACE_Timer_Node_T<TYPE> *temp = this->table_[this->earliest_position_]->remove_first ();

  for (size_t i = 0; i < this->table_size_; i++)
    if (!this->table_[i]->is_empty ())
      if (this->earliest_time () == ACE_Time_Value::zero
          || this->table_[i]->earliest_time () < this->earliest_time ())
          this->earliest_position_ = i;

  --this->size_;

  return temp;
}

template <class TYPE, class FUNCTOR, class LOCK, class BUCKET> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, LOCK, BUCKET>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Hash_T::expire");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;

  ACE_Timer_Node_T<TYPE> *expired;
  
  // Go through the table and expire anything that can be expired

  for (size_t i = 0; i < this->table_size_; i++)
    {
      while (!this->table_[i]->is_empty () &&
             this->table_[i]->earliest_time () <= cur_time)
        {
          expired = this->table_[i]->remove_first ();
          --this->size_;
          TYPE &type = expired->get_type ();
          const void *act = expired->get_act ();
          int reclaim = 1;
      
          // Check if this is an interval timer.
          if (expired->get_interval () > ACE_Time_Value::zero)
	    {
	      // Make sure that we skip past values that have already
	      // "expired".
	      do
	        expired->set_timer_value (expired->get_timer_value () + expired->get_interval ());
	      while (expired->get_timer_value () <= cur_time);

	      // Since this is an interval timer, we need to reschedule
	      // it.
	      this->reschedule (expired);
	      reclaim = 0;
	    }
     
          // call the functor
          Hash_Token *h = (Hash_Token *)act;
          this->upcall (type, h->act_, cur_time);
      
          if (reclaim)
            {  
              // Free up the node and the token
	      this->free_node (expired);
              delete h;
            }
      
          number_of_timers_expired++;
        }
    }

  return number_of_timers_expired;
}

#endif /* ACE_TIMER_HASH_T_C */
