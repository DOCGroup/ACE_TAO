// $Id$

/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Hash_T.h
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu> 
// 
// ============================================================================

#if !defined (ACE_TIMER_HASH_T_H)
#define ACE_TIMER_HASH_T_H

#include "ace/Timer_Queue_T.h"
#include "ace/Free_List.h"

// Forward declaration.
template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
class ACE_Timer_Hash_T;

template <class TYPE, class FUNCTOR, class ACE_LOCK>
class ACE_Timer_Hash_Upcall
  // = TITLE 
  //      Functor for Timer_Hash
  //
  // = DESCRIPTION
  //      This class calls up to the Timer Hash's functor from the 
  //      timer queues in the hash table
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *, 
                            ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>, 
                            ACE_Null_Mutex>
          TIMER_QUEUE;
  
  ACE_Timer_Hash_Upcall (void);
  // Default constructor (creates an invalid object, but needs to be here
  // so timer queues using this functor can be constructed)

  ACE_Timer_Hash_Upcall (ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> *timer_hash);
  // Constructor that specifies a Timer_Hash to call up to

  int timeout (TIMER_QUEUE &timer_queue,
	       ACE_Event_Handler *handler,
	       const void *arg,
	       const ACE_Time_Value &cur_time);
  // This method is called when the timer expires
  
  int cancellation (TIMER_QUEUE &timer_queue,
		    ACE_Event_Handler *handler);
  // This method is called when the timer is canceled

  int deletion (TIMER_QUEUE &timer_queue,
                ACE_Event_Handler *handler,
                const void *arg);
  // This method is called when the timer queue is destroyed and
  // the timer is still contained in it

private:
  ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> *timer_hash_;
  // Timer Queue to do the calling up to

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Timer_Hash_Upcall (const ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK> &))
};

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
class ACE_Timer_Hash_Iterator_T : public ACE_Timer_Queue_Iterator_T <TYPE, FUNCTOR, ACE_LOCK>
  // = TITLE
  //     Iterates over an <ACE_Timer_Hash>.
  //
  // = DESCRIPTION
  //     This is a generic iterator that can be used to visit every
  //     node of a timer queue.  Be aware that it doesn't transverse
  //     in the order of timeout values.  
{
public:
  ACE_Timer_Hash_Iterator_T (ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> &);
  // Constructor.

  virtual void first (void);
  // Positions the iterator at the earliest node in the Timer Queue

  virtual void next (void);
  // Positions the iterator at the next node in the Timer Queue

  virtual int isdone (void);
  // Returns true when there are no more nodes in the sequence

  virtual ACE_Timer_Node_T<TYPE> *item (void);
  // Returns the node at the current position in the sequence

protected:
  ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> &timer_hash_;
  // Pointer to the <ACE_Timer_Hash> that we are iterating over.

  size_t position_;
  // Current position in <timer_hash_>'s table

  ACE_Timer_Queue_Iterator_T<TYPE, ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK>, ACE_Null_Mutex> *iter_;
  // Current iterator used on <position>'s bucket
};

template <class TYPE, class FUNCTOR, class ACE_LOCK, class BUCKET>
class ACE_Timer_Hash_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>
  // = TITLE 
  //      Provides a hash table of <BUCKET>s as an implementation for
  //      a timer queue.
  //
  // = DESCRIPTION
  //      This implementation uses a hash table of BUCKETs.  The hash 
  //      is based on the time_value of the event.  Unlike other Timer 
  //      Queues, ACE_Timer_Hash does not expire events in order.  
{
public: 
  typedef ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> HASH_ITERATOR;
  // Type of iterator

  friend class ACE_Timer_Hash_Iterator_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET>;
  // Iterator is a friend

  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> INHERITED;
  // Type inherited from 

  // = Initialization and termination methods.
  ACE_Timer_Hash_T (size_t table_size,
                    FUNCTOR *upcall_functor = 0,
                    ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Default constructor. <table_size> determines the size of the
  // hash table.  <upcall_functor> is the instance of the FUNCTOR 
  // to be used by the buckets. If <upcall_functor> is 0, a default 
  // FUNCTOR will be created.

  ACE_Timer_Hash_T (FUNCTOR *upcall_functor = 0, ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Default constructor. <upcall_functor> is the instance of the
  // FUNCTOR to be used by the queue. If <upcall_functor> is 0, Timer
  // Hash will create a default FUNCTOR.  <freelist> the freelist of
  // timer nodes.  If 0, then a default freelist will be created.  The default
  // size will be ACE_DEFAULT_TIMERS and there will be no preallocation.

  virtual ~ACE_Timer_Hash_T (void);
  // Destructor

  virtual int is_empty (void) const;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the <ACE_Timer_Hash>.

  virtual long schedule (const TYPE &type, 
			 const void *act, 
			 const ACE_Time_Value &delay,
			 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule <type> that will expire after <delay> amount of time.
  // If it expires then <act> is passed in as the value to the
  // <functor>.  If <interval> is != to <ACE_Time_Value::zero> then it
  // is used to reschedule the <type> automatically.  This method
  // returns a <timer_id> that is a pointer to a token which stores 
  // information about the event. This <timer_id> can be used to cancel
  // the timer before it expires.  Returns -1 on failure.

  virtual int cancel (const TYPE &type,
		      int dont_call_handle_close = 1);
  // Cancel all timer associated with <type>.  If <dont_call> is 0
  // then the <functor> will be invoked.  Returns number of timers
  // cancelled.

  virtual int cancel (long timer_id,
		      const void **act = 0,
		      int dont_call_handle_close = 1);
  // Cancel the single timer that matches the <timer_id> value (which
  // was returned from the <schedule> method).  If act is non-NULL
  // then it will be set to point to the ``magic cookie'' argument
  // passed in when the timer was registered.  This makes it possible
  // to free up the memory and avoid memory leaks.  If <dont_call> is
  // 0 then the <functor> will be invoked.  Returns 1 if cancellation
  // succeeded and 0 if the <timer_id> wasn't found.

  virtual int expire (void);
  // Run the <functor> for all timers whose values are <=
  // <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.  Returns
  // the number of timers canceled.

  virtual int expire (const ACE_Time_Value &current_time);
  // Run the <functor> for all timers whose values are <= <cur_time>.
  // This does not account for <timer_skew>.  Returns the number of
  // timers canceled.

  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &iter (void);
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  virtual ACE_Timer_Node_T<TYPE> *remove_first (void);
  // Removes the earliest node from the queue and returns it

  virtual void dump (void) const;
  // Dump the state of an object.

private:
  virtual void reschedule (ACE_Timer_Node_T<TYPE> *);
  // Reschedule an "interval" <ACE_Timer_Node>.

  size_t size_;
  // Keeps track of the size of the queue
  
  BUCKET **table_;
  // Table of BUCKETS

  size_t table_size_;
  // Keeps track of the size of <table>

  ACE_Timer_Hash_Upcall<TYPE, FUNCTOR, ACE_LOCK> table_functor_;
  // Functor used for the table's timer queues
  
  size_t earliest_position_;
  // Index to the position with the earliest entry

  HASH_ITERATOR *iterator_;
  // Iterator used to expire timers.

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Timer_Hash_T (const ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Timer_Hash_T<TYPE, FUNCTOR, ACE_LOCK, BUCKET> &))
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE) && !defined(ACE_HPUX_BROKEN_TEMPLATES)
#include "ace/Timer_Hash_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE && !ACE_HPUX_BROKEN_TEMPLATES */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Hash_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_HASH_T_H */
