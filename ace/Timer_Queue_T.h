/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Queue_T.h
//
// = AUTHOR
//    Doug Schmidt, Irfan Pyarali, and Darrell Brunsch
// 
// ============================================================================

#if !defined (ACE_TIMER_QUEUE_T_H)
#define ACE_TIMER_QUEUE_T_H

#include "ace/Time_Value.h"
#include "ace/Free_List.h"

// This should be nested within the ACE_Timer_Queue class but some C++
// compilers still don't like this...

template <class TYPE>
class ACE_Timer_Node_T
  // = TITLE
  //     Maintains the state associated with a Timer entry.
{
public:
  ACE_Timer_Node_T ();
  // Default constructor

  void set (const TYPE &type, 
            const void *a, 
            const ACE_Time_Value &t, 
            const ACE_Time_Value &i, 
            ACE_Timer_Node_T<TYPE> *n, 
            long timer_id);
  // singly linked list 

  void set (const TYPE &type, 
            const void *a, 
            const ACE_Time_Value &t, 
            const ACE_Time_Value &i, 
            ACE_Timer_Node_T<TYPE> *p,
            ACE_Timer_Node_T<TYPE> *n, 
            long timer_id);
  // doubly linked list version

  // = Accessors 
  
  TYPE &get_type (void);
  // get the type
  
  void set_type (TYPE &type);
  // set the type

  const void *get_act (void);
  // get the act

  void set_act (void *act);
  // set the act

  ACE_Time_Value &get_timer_value (void);
  // get the timer value

  void set_timer_value (ACE_Time_Value timer_value);
  // set the timer value

  ACE_Time_Value &get_interval (void);
  // get the timer interval

  void set_interval (ACE_Time_Value interval);
  // set the timer interval

  ACE_Timer_Node_T<TYPE> *get_prev (void);
  // get the previous pointer

  void set_prev (ACE_Timer_Node_T<TYPE> *prev);
  // set the previous pointer

  ACE_Timer_Node_T<TYPE> *get_next (void);
  // get the next pointer

  void set_next (ACE_Timer_Node_T<TYPE> *next);
  // set the next pointer
  
  long get_timer_id (void);
  // get the timer_id

  void set_timer_id (long timer_id);
  // set the timer_id

  void dump (void) const;
  // Dump the state of an TYPE.

private:
  TYPE type_;
  // Type of object stored in the Queue
  
  const void *act_;
  // Act associated with the timer.
  
  ACE_Time_Value timer_value_;
  // Time until the timer expires.
  
  ACE_Time_Value interval_;
  // If this is a periodic timer this holds the time until the next
  // timeout.

  ACE_Timer_Node_T<TYPE> *prev_;
  // Pointer to previous timer.

  ACE_Timer_Node_T<TYPE> *next_;
  // Pointer to next timer.
  
  long timer_id_;
  // Id of this timer (used to cancel timers before they expire).
};

template <class TYPE, class FUNCTOR, class LOCK>
class ACE_Timer_Queue_Iterator_T
  // = TITLE
  //     Generic interface for iterating over a subclass of
  //     <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //     This is a generic iterator that can be used to visit every
  //     node of a timer queue.  Be aware that it isn't guaranteed
  //     that the transversal will be in order of timeout values.  
{
public:

  // = Initialization and termination methods.
  ACE_Timer_Queue_Iterator_T (void);
  // Constructor.

  virtual ~ACE_Timer_Queue_Iterator_T (void);
  // Destructor.

  virtual void first (void) = 0;
  // Positions the iterator at the earliest node in the Timer Queue

  virtual void next (void) = 0;
  // Positions the iterator at the next node in the Timer Queue

  virtual int isdone (void) = 0;
  // Returns true when there are no more nodes in the sequence

  virtual ACE_Timer_Node_T<TYPE> *item (void) = 0;
  // Returns the node at the current position in the sequence
};

template <class TYPE, class FUNCTOR, class LOCK>
class ACE_Timer_Queue_T
  // = TITLE 
  //      Provides an interface to timers.
  //
  // = DESCRIPTION
  //      This is an abstract base class that provides hook for
  //      implementing specialized policies such as <ACE_Timer_List>
  //      and <ACE_Timer_Heap>.
{
public: 

  typedef ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK> ITERATOR;
  // Type of Iterator

  // = Initialization and termination methods.
  ACE_Timer_Queue_T (FUNCTOR *upcall_functor = 0, 
                     ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist = 0);
  // Default constructor. <upcall_functor> is the instance of the
  // FUNCTOR to be used by the queue. If <upcall_functor> is 0, Timer
  // Queue will create a default FUNCTOR.  <freelist> the freelist of
  // timer nodes.  If 0, then a default freelist will be created.

  virtual ~ACE_Timer_Queue_T (void);
  // Destructor - make virtual for proper destruction of inherited
  // classes.

  virtual int is_empty (void) const = 0;
  // True if queue is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const = 0;
  // Returns the time of the earlier node in the Timer_Queue.

  virtual long schedule (const TYPE &type, 
			 const void *act, 
			 const ACE_Time_Value &delay,
			 const ACE_Time_Value &interval = ACE_Time_Value::zero) = 0;
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
		      int dont_call_handle_close = 1) = 0;
  // Cancel all timer associated with <type>.  If <dont_call> is 0
  // then the <functor> will be invoked.  Returns number of timers
  // cancelled.

  virtual int cancel (long timer_id, 
		      const void **act = 0,
		      int dont_call_handle_close = 1) = 0;
  // Cancel the single timer that matches the <timer_id> value (which
  // was returned from the <schedule> method).  If act is non-NULL
  // then it will be set to point to the ``magic cookie'' argument
  // passed in when the timer was registered.  This makes it possible
  // to free up the memory and avoid memory leaks.  If <dont_call> is
  // 0 then the <functor> will be invoked.  Returns 1 if cancellation
  // succeeded and 0 if the <timer_id> wasn't found.

  virtual int expire (const ACE_Time_Value &current_time);
  // Run the <functor> for all timers whose values are <= <cur_time>.
  // This does not account for <timer_skew>.  Returns the number of
  // timers canceled.

  virtual int expire (void);
  // Run the <functor> for all timers whose values are <=
  // <ACE_OS::gettimeofday>.  Also accounts for <timer_skew>.  Returns
  // the number of timers canceled.

  virtual ACE_Time_Value gettimeofday (void);
  // Returns the current time of day.  This allows different
  // implementations of the timer queue to use special high resolution
  // timers.

  void gettimeofday (ACE_Time_Value (*gettimeofday)(void));
  // Allows applications to control how the timer queue gets the time
  // of day.

  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max);
  // Determine the next event to timeout.  Returns <max> if there are
  // no pending timers or if all pending timers are longer than max.

  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max,
					     ACE_Time_Value *&the_timeout);
  // Determine the next event to timeout.  Returns <max> if there are
  // no pending timers or if all pending timers are longer than max.
  // <the_timeout> should be a pointer to storage for the timeout value,
  // and this value is also returned.

  // = Set/get the timer skew for the Timer_Queue.
  void timer_skew (const ACE_Time_Value &skew);
  const ACE_Time_Value &timer_skew (void) const;

  LOCK &mutex (void); 
  // Synchronization variable used by the queue

  FUNCTOR &upcall_functor (void);
  // Accessor to the upcall functor

  virtual ITERATOR &iter (void) = 0;
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  virtual ACE_Timer_Node_T<TYPE> *remove_first (void) = 0;
  // Removes the earliest node from the queue and returns it

  virtual void dump (void) const;
  // Dump the state of a object.

protected:
  virtual void upcall (TYPE &type,
		       const void *act,
		       const ACE_Time_Value &cur_time);
  // This method will call the <functor> with the <type>, <act> and
  // <time>

  virtual void reschedule (ACE_Timer_Node_T<TYPE> *) = 0;
  // Reschedule an "interval" <ACE_Timer_Node>.

  virtual ACE_Timer_Node_T<TYPE> *alloc_node (void);
  // Factory method that allocates a new node.

  virtual void free_node (ACE_Timer_Node_T<TYPE> *);
  // Factory method that frees a previously allocated node.

  LOCK mutex_; 
  // Synchronization variable for <ACE_Timer_Queue>.

  ACE_Free_List<ACE_Timer_Node_T<TYPE> > *free_list_;
  // Class that implements a free list

  ACE_Time_Value (*gettimeofday_)(void);
  // Pointer to function that returns the current time of day.

  FUNCTOR *upcall_functor_;
  // Upcall functor

  int delete_upcall_functor_;
  // To delete or not to delete is the question?

  int delete_free_list_;
  // Flag to delete only if the class created the <free_list_>

private:
  
  ACE_Time_Value timeout_;
  // Returned by <calculate_timeout>.

  ACE_Time_Value timer_skew_;
  // Adjusts for timer skew in various clocks.

  // = Don't allow these operations for now.
  ACE_Timer_Queue_T (const ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> &);
  void operator= (const ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> &);
};

template <class LOCK>
class ACE_Event_Handler_Handle_Timeout_Upcall
  // = TITLE 
  //      Functor for Timer_Queues.
  //
  // = DESCRIPTION
  //      This class implements the functor required by the Timer
  //      Queue to call <handle_timeout> on ACE_Event_Handlers.
{
public:
  typedef ACE_Timer_Queue_T<ACE_Event_Handler *, 
                            ACE_Event_Handler_Handle_Timeout_Upcall<LOCK>, 
                            LOCK>
          TIMER_QUEUE;
  
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
};

#if defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Queue_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Queue_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_QUEUE_T_H */
