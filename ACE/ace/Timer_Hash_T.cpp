#ifndef ACE_TIMER_HASH_T_CPP
#define ACE_TIMER_HASH_T_CPP

#include "ace/Timer_Hash_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Guard_T.h"
#include "ace/Log_Category.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Synch.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE>
class Hash_Token
{
public:
  // This constructor is required by ACE_Locked_Free_List::alloc.
  Hash_Token (void) :
    act_ (0),
    pos_ (0),
    orig_id_ (0),
    next_ (0)
  {
  }

  Hash_Token<TYPE> *get_next (void)
  {
    return this->next_;
  }

  void set_next (Hash_Token<TYPE> *next)
  {
    this->next_ = next;
  }

  void set (const void *act,
            size_t pos,
            long orig_id,
            const TYPE *type)
  {
    this->act_ = act;
    this->pos_ = pos;
    this->orig_id_ = orig_id;
    this->type_ = type;
    this->next_ = 0;
  }

  const void *act_;
  size_t pos_;
  long orig_id_;
  const TYPE* type_;
  /// Pointer to next token.
  Hash_Token<TYPE> *next_;
};

// Default constructor

template <class TQ_TYPE, class TYPE, class FUNCTOR>
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::ACE_Timer_Hash_Upcall (void)
  : timer_hash_ (0)
  , upcall_functor_ (0)
  , delete_upcall_functor_ (false)
{
  // Nothing
}

// Constructor that specifies a Timer_Hash to call up to

template <class TQ_TYPE, class TYPE, class FUNCTOR>
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::ACE_Timer_Hash_Upcall (
  TQ_TYPE *timer_hash,
  FUNCTOR *upcall_functor)
  : timer_hash_ (timer_hash)
  , upcall_functor_ (upcall_functor)
  , delete_upcall_functor_ (false)
{
  // Nothing
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> void
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::upcall_functor (FUNCTOR *upcall_functor,
                                                               bool delete_upcall_functor)
{
  if (delete_upcall_functor_)
    delete upcall_functor_;

  upcall_functor_ = upcall_functor;
  delete_upcall_functor_ = (upcall_functor && delete_upcall_functor);
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::registration (
  TQ_TYPE &,
  TYPE *handler,
  const void *act)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->registration (*this->timer_hash_,
                                                  handler,
                                                  act);

  return result;
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::preinvoke (
  TQ_TYPE &,
  TYPE *handler,
  const void *arg,
  int recurring_timer,
  const ACE_Time_Value &cur_time,
  const void *act)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->preinvoke (*this->timer_hash_,
                                               handler,
                                               arg,
                                               recurring_timer,
                                               cur_time,
                                               act);

  return result;
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::postinvoke (
  TQ_TYPE &,
  TYPE *handler,
  const void *arg,
  int recurring_timer,
  const ACE_Time_Value &cur_time,
  const void *act)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->postinvoke (*this->timer_hash_,
                                                handler,
                                                arg,
                                                recurring_timer,
                                                cur_time,
                                                act);

  return result;
}

// Calls up to timer_hash's upcall functor
template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::timeout (
  TQ_TYPE &,
  TYPE *handler,
  const void *arg,
  int dont_call,
  const ACE_Time_Value &cur_time)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->timeout (*this->timer_hash_,
                                             handler,
                                             arg,
                                             dont_call,
                                             cur_time);

  return result;
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::cancel_type (
  TQ_TYPE &,
  TYPE *handler,
  int dont_call,
  int &requires_reference_counting)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->cancel_type (*this->timer_hash_,
                                                 handler,
                                                 dont_call,
                                                 requires_reference_counting);

  return result;
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::cancel_timer (
  TQ_TYPE &,
  TYPE *handler,
  int dont_call,
  int requires_reference_counting)
{
  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->cancel_timer (*this->timer_hash_,
                                                  handler,
                                                  dont_call,
                                                  requires_reference_counting);

  return result;
}

template <class TQ_TYPE, class TYPE, class FUNCTOR> int
ACE_Timer_Hash_Upcall<TQ_TYPE, TYPE, FUNCTOR>::deletion (
  TQ_TYPE &,
  TYPE *handler,
  const void *arg)
{
  Hash_Token<TYPE> *h =
    reinterpret_cast<Hash_Token<TYPE> *> (const_cast<void *> (arg));

  int result = -1;

  if (upcall_functor_)
    result = this->upcall_functor_->deletion (*this->timer_hash_,
                                              handler,
                                              h->act_);

  return result;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::ACE_Timer_Hash_Iterator_T (Hash & hash)
  : timer_hash_ (hash)
{
  this->first ();
  // Nothing
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::~ACE_Timer_Hash_Iterator_T ()
{
  // Nothing
}

// Positions the iterator at the first node in the timing hash table

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::first (void)
{
  for (this->position_ = 0;
       this->position_ < this->timer_hash_.table_size_;
       ++this->position_)
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

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::next (void)
{
  if (this->isdone ())
    return;

  // If there is no more in the current bucket, go to the next
  if (this->iter_->isdone ())
    {
      for (++this->position_;
           this->position_ < this->timer_hash_.table_size_;
           ++this->position_)
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

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> bool
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::isdone (void) const
{
  return this->iter_ == 0;
}

// Returns the node at the current position in the sequence

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::item (void)
{
  if (this->isdone ())
    return 0;

  return this->iter_->item ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> ACE_Timer_Queue_Iterator_T<TYPE> &
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::iter (void)
{
  this->iterator_->first ();
  return *this->iterator_;
}

// Create an empty queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::ACE_Timer_Hash_T (
  size_t table_size,
  FUNCTOR *upcall_functor,
  ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist,
  TIME_POLICY const &time_policy)
  : inherited (upcall_functor, freelist, time_policy),
    size_ (0),
    table_size_ (table_size),
    table_functor_ (this, upcall_functor),
    earliest_position_ (0),
    iterator_ (0)
#if defined (ACE_WIN64)
  , pointer_base_ (0)
#endif /* ACE_WIN64 */
  , token_list_ ()
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  ACE_NEW (table_,
           BUCKET *[table_size]);

  for (size_t i = 0;
       i < table_size;
       ++i)
    {
      ACE_NEW (this->table_[i],
               BUCKET (&this->table_functor_,
                       this->free_list_,
                       time_policy));
    }

  if (!upcall_functor)
    {
      FUNCTOR* functor = 0;
      ACE_NEW (functor,
               FUNCTOR ());

      this->table_functor_.upcall_functor (functor, true);
    }

  ACE_NEW (iterator_,
           ITERATOR_T (*this));
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::ACE_Timer_Hash_T (
  FUNCTOR *upcall_functor,
  ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist,
  TIME_POLICY const &time_policy)
  : inherited (upcall_functor, freelist, time_policy),
    size_ (0),
    table_size_ (ACE_DEFAULT_TIMER_HASH_TABLE_SIZE),
    table_functor_ (this, upcall_functor),
    earliest_position_ (0)
#if defined (ACE_WIN64)
  , pointer_base_ (0)
#endif /* ACE_WIN64 */
  , token_list_ ()
{
  ACE_TRACE ("ACE_Timer_Hash_T::ACE_Timer_Hash_T");

  ACE_NEW (table_,
           BUCKET *[ACE_DEFAULT_TIMER_HASH_TABLE_SIZE]);

  for (size_t i = 0;
       i < this->table_size_;
       ++i)
    {
      ACE_NEW (this->table_[i],
               BUCKET (&this->table_functor_,
                       this->free_list_,
                       time_policy));
    }

    if (!upcall_functor)
    {
      FUNCTOR* functor = 0;
      ACE_NEW (functor,
               FUNCTOR ());

      this->table_functor_.upcall_functor (functor, true);
    }

  ACE_NEW (iterator_,
           ITERATOR_T (*this));
}

// Remove all remaining items in the Queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY>
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::~ACE_Timer_Hash_T (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::~ACE_Timer_Hash_T");
  //ACE_MT (ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_));

  delete iterator_;

  this->close ();

  for (size_t i = 0;
       i < this->table_size_;
       ++i)
    delete this->table_[i];


  delete [] this->table_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::close (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::close");
  //ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Remove all remaining items from the queue.
  while (!this->is_empty ())
    {
      ACE_Timer_Node_T<TYPE>* n = this->remove_first ();
      this->upcall_functor ().deletion (*this,
                                        n->get_type (),
                                        n->get_act ());
      this->free_node (n);
    }

  // leave the rest to destructor
  return 0;
}

// Checks if queue is empty.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> bool
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::is_empty");

  BUCKET* bucket = this->table_[this->earliest_position_];
  ACE_ASSERT (bucket);
  return bucket->is_empty ();
}

// Returns earliest time in a non-empty bucket

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> const ACE_Time_Value &
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Hash_T::earliest_time");

  BUCKET* bucket = this->table_[this->earliest_position_];
  ACE_ASSERT (bucket);
  return bucket->earliest_time ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Timer_Hash_T::dump");
  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\ntable_size_ = %d"), this->table_size_));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\nearliest_position_ = %d"), this->earliest_position_));

  for (size_t i = 0; i < this->table_size_; ++i)
    if (!this->table_[i]->is_empty ())
      ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\nBucket %d contains nodes"), i));

  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::reschedule (
  ACE_Timer_Node_T<TYPE> *expired)
{
  ACE_TRACE ("ACE_Timer_Hash_T::reschedule");

  Hash_Token<TYPE> *h =
    reinterpret_cast<Hash_Token<TYPE> *> (
      const_cast<void *> (expired->get_act ()));
  ACE_ASSERT (h);

  // Don't use ACE_Utils::truncate_cast<> here.  A straight
  // static_cast<>  will provide more unique results when the number
  // of seconds is greater than std::numeric_limits<size_t>::max().
  size_t const secs_hash_input =
    static_cast<size_t> (expired->get_timer_value ().sec ());
  h->pos_ = secs_hash_input % this->table_size_;

  h->orig_id_ =
    this->table_[h->pos_]->schedule (expired->get_type (),
                                     h,
                                     expired->get_timer_value (),
                                     expired->get_interval ());
  ACE_ASSERT (h->orig_id_ != -1);

  // Since schedule() above will allocate a new node
  // then here schedule <expired> for deletion. Don't call
  // this->free_node() because that will invalidate <h>
  // and that's what user have as timer_id.
  inherited::free_node (expired);

  if (this->table_[this->earliest_position_]->is_empty ()
      || this->table_[h->pos_]->earliest_time ()
      < this->table_[this->earliest_position_]->earliest_time ())
    this->earliest_position_ = h->pos_;
}

// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> long
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::schedule_i (
  TYPE *handler,
  const void *act,
  const ACE_Time_Value &future_time,
  const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Hash_T::schedule_i");

  // Don't use ACE_Utils::truncate_cast<> here.  A straight
  // static_cast<>  will provide more unique results when the number
  // of seconds is greater than std::numeric_limits<size_t>::max().
  size_t const secs_hash_input = static_cast<size_t> (future_time.sec ());
  size_t const position = secs_hash_input % this->table_size_;

  // Don't create Hash_Token directly. Instead we get one from Free_List
  // and then set it properly.
  Hash_Token<TYPE> *h = this->token_list_.remove ();
  ACE_ASSERT (h);
  h->set (act, position, 0, handler);

  h->orig_id_ =
    this->table_[position]->schedule (handler,
                                      h,
                                      future_time,
                                      interval);
  ACE_ASSERT (h->orig_id_ != -1);

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
  intptr_t hi = reinterpret_cast<intptr_t> (h);
  if (this->pointer_base_ == 0)
    this->pointer_base_ = hi & 0xffffffff00000000;
  return static_cast<long> (hi & 0xffffffff);
#else
  return reinterpret_cast<long> (h);
#endif
}

// Locate and update the inteval on the timer_id

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::reset_interval (
  long timer_id,
  const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Hash_T::reset_interval");

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return -1;

  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));
#if defined (ACE_WIN64)
  unsigned long const timer_offset =
    static_cast<unsigned long> (timer_id);

  Hash_Token<TYPE> * const h =
    reinterpret_cast<Hash_Token<TYPE> *> (this->pointer_base_ + timer_offset);
#else
  Hash_Token<TYPE> * const h =
    reinterpret_cast<Hash_Token<TYPE> *> (timer_id);
#endif /* ACE_WIN64 */
  ACE_ASSERT (h);

  return this->table_[h->pos_]->reset_interval (h->orig_id_,
                                                interval);
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// @a timer_id from the correct table timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::cancel (
  long timer_id,
  const void **act,
  int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return 0;

  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));
#if defined (ACE_WIN64)
  unsigned long const timer_offset =
    static_cast<unsigned long> (timer_id);

  Hash_Token<TYPE> * const h =
    reinterpret_cast<Hash_Token<TYPE> *> (this->pointer_base_ + timer_offset);
#else
  Hash_Token<TYPE> * const h =
    reinterpret_cast<Hash_Token<TYPE> *> (timer_id);
#endif /* ACE_WIN64 */
  ACE_ASSERT (h);

  // Note: close hooks to be called by the bucket
  int const result = this->table_[h->pos_]->cancel (h->orig_id_,
                                                    0,
                                                    dont_call);

  if (result == 1)
    {
      if (h->pos_ == this->earliest_position_)
        this->find_new_earliest ();

      if (act != 0)
        *act = h->act_;

      // We could destruct Hash_Token explicitly but we better
      // schedule it for destruction. In this case next
      // token_list_.remove () will use it.
      this->token_list_.add (h);

      --this->size_;
    }

  return result;
}

// Locate and remove all values of <type> from the timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::cancel (
  TYPE *handler,
  int dont_call)
{
  ACE_TRACE ("ACE_Timer_Hash_T::cancel");

  size_t i; // loop variable.
  int result = 0;

  Hash_Token<TYPE> **timer_ids = 0;
  size_t pos = 0;

  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  ACE_NEW_RETURN (timer_ids,
                  Hash_Token<TYPE> *[this->size_],
                  -1);

  int dont_call_handle_close = dont_call;
  bool has_type = false;
  int result_temp = 0;
  for (i = 0;
       i < this->table_size_;
       ++i)
    {
      has_type = false;

      ACE_Timer_Queue_Iterator_T<TYPE> & iter =
        this->table_[i]->iter ();

      for (iter.first ();
           !iter.isdone ();
           iter.next ())
        if (iter.item ()->get_type () == handler)
          {
            has_type = true;

            timer_ids[pos] =
              reinterpret_cast<Hash_Token<TYPE> *> (
                const_cast<void *> (iter.item ()->get_act ()));
            ACE_ASSERT (timer_ids[pos]);

            ++pos;
          }
    }
    if (has_type)
      {
        result_temp = this->table_[i]->cancel (handler,
                                               dont_call_handle_close);
        if (result_temp == -1)
          result = -1;
        else
          result += result_temp;
        if (dont_call_handle_close == 0)
          dont_call_handle_close = 1; // call once per type only (iff at all)
      }

  if (pos > this->size_)
    return -1;

  for (i = 0; i < pos; ++i)
    {
      // We could destruct Hash_Token explicitly but we better
      // schedule it for destruction.
      this->token_list_.add (timer_ids[i]);

      --this->size_;
    }

  delete [] timer_ids;

  this->find_new_earliest ();

  // Note: close hooks to be called by the bucket(s)
  // iff there were no timers to cancel, invoke cancel_type() once anyway
  // (otherwise it would have been called from the bucket(s), once per bucket
  // [note that this behaviour is inconsistent, but unavoidable at this stage])

  // Call the close hooks.
  int cookie = 0;

  // cancel_type() called once per <type>.
  if (result <= 0)
    this->upcall_functor ().cancel_type (*this,
                                         handler,
                                         dont_call,
                                         cookie);

  return static_cast<int> (pos);
}

// Removes the earliest node and finds the new earliest position

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::remove_first (void)
{
  if (this->is_empty ())
    return 0;

  BUCKET* bucket = this->table_[this->earliest_position_];
  ACE_ASSERT (bucket);
  ACE_Timer_Node_T<TYPE> *temp = bucket->remove_first ();

  this->find_new_earliest ();

  --this->size_;

  return temp;
}

// Finds a new earliest position

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::find_new_earliest (void)
{
  for (size_t i = 0; i < this->table_size_; ++i)
    if (!this->table_[i]->is_empty ())
      if (this->table_[this->earliest_position_]->is_empty ()
          || this->earliest_time () == ACE_Time_Value::zero
          || this->table_[i]->earliest_time () <= this->earliest_time ())
          this->earliest_position_ = i;
}

// Returns the earliest node without removing it

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::get_first (void)
{
  ACE_TRACE ("ACE_Timer_Hash_T::get_first");

  if (this->is_empty ())
    return 0;

  BUCKET* bucket = this->table_[this->earliest_position_];
  ACE_ASSERT (bucket);
  return bucket->get_first ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> void
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::free_node (ACE_Timer_Node_T<TYPE> *node)
{
  Hash_Token<TYPE> *h =
    reinterpret_cast<Hash_Token<TYPE> *> (const_cast<void *> (node->get_act ()));
  ACE_ASSERT (h);
  this->token_list_.add (h);

  inherited::free_node (node);
}

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::dispatch_info_i (const ACE_Time_Value &cur_time,
                                                                    ACE_Timer_Node_Dispatch_Info_T<TYPE> &info)
{
  int const result =
    inherited::dispatch_info_i (cur_time, info);

  if (result == 1)
    {
      Hash_Token<TYPE> *h =
        reinterpret_cast<Hash_Token<TYPE> *> (const_cast<void *> (info.act_));
      ACE_ASSERT (h);

      info.act_ = h->act_;
    }

  return result;
}

// Dummy version of expire to get rid of warnings in Sun CC 4.2

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::expire ()
{
  return inherited::expire ();
}

// Specialized expire for Timer Hash

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET, typename TIME_POLICY> int
ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET, TIME_POLICY>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Hash_T::expire");

  int number_of_timers_expired = 0;

  ACE_Timer_Node_T<TYPE> *expired = 0;
  Hash_Token<TYPE> *h = 0;
  ACE_Timer_Node_Dispatch_Info_T<TYPE> info;
  const void *upcall_act = 0;

  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Go through the table and expire anything that can be expired

  bool reclaim = true;
  for (size_t i = 0;
       i < this->table_size_;
       ++i)
    {
      while (!this->table_[i]->is_empty ()
             && this->table_[i]->earliest_time () <= cur_time)
        {
          expired = this->table_[i]->remove_first ();
          ACE_ASSERT (expired);
          h = 0;
          upcall_act = 0;
          reclaim = true;

          // Get the dispatch info
          expired->get_dispatch_info (info);

          h =
            reinterpret_cast<Hash_Token<TYPE> *> (const_cast<void *> (expired->get_act ()));
          ACE_ASSERT (h && (h->pos_ == i));

          info.act_ = h->act_;

          // Check if this is an interval timer.
          if (expired->get_interval () > ACE_Time_Value::zero)
            {
              // Make sure that we skip past values that have already
              // "expired".
              this->recompute_next_abs_interval_time (expired, cur_time);

              // Since this is an interval timer, we need to
              // reschedule it.
              this->reschedule (expired);
              reclaim = false;
            }
          else
            {
              this->free_node (expired);
            }

          this->preinvoke (info, cur_time, upcall_act);

          this->upcall (info, cur_time);

          this->postinvoke (info, cur_time, upcall_act);

          if (reclaim)
            --this->size_;

          ++number_of_timers_expired;
        }
    }

  if (number_of_timers_expired > 0)
    this->find_new_earliest ();

  return number_of_timers_expired;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_TIMER_HASH_T_CPP */
