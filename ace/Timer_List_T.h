// $Id$

/* -*- C++ -*- */
// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_List_T.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_LIST_T_H)
#define ACE_TIMER_LIST_T_H

#include "ace/Timer_Queue_T.h"

// Forward declaration.
template <class TYPE, class FUNCTOR, class LOCK>
class ACE_Timer_List_T;

template <class TYPE, class FUNCTOR, class LOCK>
class ACE_Timer_List_Iterator_T : public ACE_Timer_Queue_Iterator_T <TYPE, FUNCTOR, LOCK>
  // = TITLE
  //     Iterates over an <ACE_Timer_List>.
  //
  // = DESCRIPTION
  //     This is a generic iterator that can be used to visit every
  //     node of a timer queue.  
{
public:
  ACE_Timer_List_Iterator_T (ACE_Timer_List_T<TYPE, FUNCTOR, LOCK> &);
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
  ACE_Timer_List_T<TYPE, FUNCTOR, LOCK> &timer_list_;
  // Pointer to the <ACE_Timer_List> that we are iterating over.

  ACE_Timer_Node_T<TYPE> *position_;
};

template <class TYPE, class FUNCTOR, class LOCK>
class ACE_Timer_List_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK>
  // = TITLE 
  //      Provides a simple implementation of timers.
  //
  // = DESCRIPTION
  //
  //      This implementation uses a linked list of absolute times.
  //      Therefore, in the average case, scheduling and canceling
  //      timers is O(N) (where N is the total number of timers) and
  //      expiring timers is O(K) (where K is the total number of timers
  //      that are < the current time of day).
  //
  //      More clever implementations could use a delta-list, a heap,
  //      or timing wheels, etc.  For instance, <ACE_Timer_Heap>
  //      is a subclass of <ACE_Timer_List> that implements a
  //      heap-based callout queue.  For most applications, the
  //      <ACE_Timer_Heap> will perform substantially faster than the
  //      <ACE_Timer_List>.
{
public: 
  typedef ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, LOCK> LIST_ITERATOR;
  // Type of iterator

  friend class ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, LOCK>;
  // Iterator is a friend

  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> INHERITED;
  // Type inherited from 

  // = Initialization and termination methods.
  ACE_Timer_List_T (FUNCTOR *upcall_functor = 0, 
                    ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Default constructor. <upcall_functor> is the instance of the
  // FUNCTOR to be used by the list. If <upcall_functor> is 0, a
  // default FUNCTOR will be created.  <freelist> the freelist of
  // timer nodes.  If 0, then a default freelist will be created.

  virtual ~ACE_Timer_List_T (void);
  // Destructor

  virtual int is_empty (void) const;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the <ACE_Timer_List>.

  virtual long schedule (const TYPE &type, 
			 const void *act, 
			 const ACE_Time_Value &delay,
			 const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule <type> that will expire after <delay> amount of time.
  // If it expires then <act> is passed in as the value to the
  // <functor>.  If <interval> is != to <ACE_Time_Value::zero> then it
  // is used to reschedule the <type> automatically.  This method
  // returns a <timer_id> that uniquely identifies the the <type>
  // entry in an internal list.  This <timer_id> can be used to cancel
  // the timer before it expires.  The cancellation ensures that
  // <timer_ids> are unique up to values of greater than 2 billion
  // timers.  As long as timers don't stay around longer than this
  // there should be no problems with accidentally deleting the wrong
  // timer.  Returns -1 on failure (which is guaranteed never to be a
  // valid <timer_id>).

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

  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK> &iter (void);
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  virtual ACE_Timer_Node_T<TYPE> *remove_first (void);
  // Removes the earliest node from the queue and returns it

  virtual void dump (void) const;
  // Dump the state of an object.

  virtual void reschedule (ACE_Timer_Node_T<TYPE> *);
  // Reschedule an "interval" <ACE_Timer_Node>.  This should be private 
  // but for now it needs to be public for <ACE_Timer_Hash_T>

protected:
/*  virtual ACE_Timer_Node_T<TYPE> *alloc_node (void);
  // Factory method that allocates a new node (uses operator new).

  virtual void free_node (ACE_Timer_Node_T<TYPE> *);
  // Factory method that frees a previously allocated node (uses
  // operator delete).
*/
private:
  ACE_Timer_Node_T<TYPE> *head_; 
  // Pointer to linked list of <ACE_Timer_Handles>.

  LIST_ITERATOR iterator_;
  // Iterator used to expire timers.

  long timer_id_;
  // Keeps track of the timer id that uniquely identifies each timer.
  // This id can be used to cancel a timer via the <cancel (int)>
  // method.

  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Timer_List_T (const ACE_Timer_List_T<TYPE, FUNCTOR, LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Timer_List_T<TYPE, FUNCTOR, LOCK> &))
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_List_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_List_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_LIST_T_H */
