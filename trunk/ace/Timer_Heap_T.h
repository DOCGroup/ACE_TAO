/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Timer_Heap_T.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TIMER_HEAP_T_H)
#define ACE_TIMER_HEAP_T_H

#include "ace/Timer_Queue.h"
#include "ace/Set.h"

// Forward declaration
template <class TYPE, class FUNCTOR>
class ACE_Timer_Heap_T;

template <class TYPE, class FUNCTOR>
class ACE_Timer_Heap_Iterator_T : public ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> 
  // = TITLE
  //     Iterates over an <ACE_Timer_Queue>.
  //
  // = DESCRIPTION
  //     This is a special type of iterator that "advances" by moving
  //     the head of the timer queue up by one every time.
{
public:
  ACE_Timer_Heap_Iterator_T (ACE_Timer_Heap_T<TYPE, FUNCTOR> &);
  // Constructor.

  virtual int next (ACE_Timer_Node_T<TYPE, FUNCTOR> *&timer_node, 
		    const ACE_Time_Value &cur_time);
  // Pass back the next <timer_node> that hasn't been seen yet, if its
  // <time_value_> <= <cur_time>.  In addition, moves the timer queue
  // forward by one node.  Returns 0 when all <timer_nodes> have been
  // seen, else 1.

protected:
  ACE_Timer_Heap_T<TYPE, FUNCTOR> &timer_heap_;
  // Pointer to the <ACE_Timer_Heap> that we are iterating over.
};

template <class TYPE, class FUNCTOR>
class ACE_Timer_Heap_T : public ACE_Timer_Queue_T<TYPE, FUNCTOR>
  // = TITLE 
  //      Provides a very fast and predictable timer implementation.
  //
  // = DESCRIPTION
  //
  //      This implementation uses a heap-based callout queue of
  //      absolute times.  Therefore, in the average and worst case,
  //      scheduling, canceling, and expiring timers is O(log N) (where
  //      N is the total number of timers).  In addition, we can also
  //      preallocate as many <ACE_Timer_Nodes> as there are slots in
  //      the heap.  This allows us to completely remove the need for
  //      dynamic memory allocation, which is important for real-time
  //      systems.
{
public:
  typedef ACE_Timer_Heap_Iterator_T<TYPE, FUNCTOR> HEAP_ITERATOR;
  friend HEAP_ITERATOR;

  typedef ACE_Timer_Queue_T<TYPE, FUNCTOR> INHERITED;

  // = Initialization and termination methods.
  ACE_Timer_Heap_T (size_t size = ACE_DEFAULT_TIMERS,
		    int preallocated = 0,
		    FUNCTOR *upcall_functor = 0);
  // The Constructor creates a heap with <size> elements.  If
  // <preallocated> is non-0 then we'll pre-allocate all the memory
  // for the <ACE_Timer_Nodes>.  This saves time and is more
  // predictable (though it requires more space).  Otherwise, we'll
  // just allocate the nodes as we need them.

  virtual ~ACE_Timer_Heap_T (void);
  // Destructor.

  virtual int is_empty (void) const;
  // True if heap is empty, else false.

  virtual const ACE_Time_Value &earliest_time (void) const;
  // Returns the time of the earlier node in the Timer_Queue.

  virtual int schedule (const TYPE &type, 
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

  virtual int cancel (int timer_id, 
		      const void **act = 0,
		      int dont_call_handle_close = 1);
  // Cancel the single timer that matches the <timer_id> value (which
  // was returned from the <schedule> method).  If act is non-NULL
  // then it will be set to point to the ``magic cookie'' argument
  // passed in when the timer was registered.  This makes it possible
  // to free up the memory and avoid memory leaks.  If <dont_call> is
  // 0 then the <functor> will be invoked.  Returns 1 if cancellation
  // succeeded and 0 if the <timer_id> wasn't found.

  virtual void dump (void) const;
  // Dump the state of an object.

protected:
  virtual void reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR> *);
  // Reschedule an "interval" <ACE_Timer_Node>.

  virtual ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> &iter (void);
  // Returns a pointer to this <ACE_Timer_Queue>'s iterator.

  virtual ACE_Timer_Node_T<TYPE, FUNCTOR> *alloc_node (void);
  // Factory method that allocates a new node (uses operator new if
  // we're *not* preallocating, otherwise uses an internal freelist).

  virtual void free_node (ACE_Timer_Node_T<TYPE, FUNCTOR> *);
  // Factory method that frees a previously allocated node (uses
  // operatord delete if we're *not* preallocating, otherwise uses an
  // internal freelist).

private:
  ACE_Timer_Node_T<TYPE, FUNCTOR> *remove (size_t index);
  // Remove and return the <index>th <ACE_Timer_Node> and restore the
  // heap property.

  void insert (ACE_Timer_Node_T<TYPE, FUNCTOR> *new_node);
  // Insert <new_node> into the heap and restore the heap property.

  void grow_heap (void);
  // Doubles the size of the heap and the corresponding timer_ids array.
  // If preallocation is used, will also double the size of the 
  // preallocated array of ACE_Timer_Nodes.

  void reheap_up (ACE_Timer_Node_T<TYPE, FUNCTOR> *new_node, 
		  size_t index,
		  size_t parent);
  // Restore the heap property, starting at <index>.

  void reheap_down (ACE_Timer_Node_T<TYPE, FUNCTOR> *moved_node, 
		    size_t index, 
		    size_t child);
  // Restore the heap property, starting at <index>.

  void copy (int index, ACE_Timer_Node_T<TYPE, FUNCTOR> *moved_node);
  // Copy <moved_node> into the <index> slot of <heap_> and move
  // <index> into the corresponding slot in the <timer_id_> array.

  int timer_id (void);
  // Returns a timer id that uniquely identifies this timer.  This id
  // can be used to cancel a timer via the <cancel (int)> method.  The
  // timer id returned from this method will never == -1 to avoid
  // conflicts with other failure return values.

  int pop_freelist (void);
  // Pops and returns a new timer id from the freelist.

  void push_freelist (int old_id);
  // Pushes <old_id> onto the freelist.

  size_t max_size_;
  // Maximum size of the heap.

  size_t cur_size_;
  // Current size of the heap.
  
  HEAP_ITERATOR iterator_;
  // Iterator used to expire timers.

  ACE_Timer_Node_T<TYPE, FUNCTOR> **heap_;
  // Current contents of the Heap, which is organized as a "heap" of
  // <ACE_Timer_Node> *'s.  In this context, a heap is a "partially
  // ordered, almost complete" binary tree, which is stored in an
  // array.

  int *timer_ids_;
  // An array of "pointers" that allows each <ACE_Timer_Node> in the
  // <heap_> to be located in O(1) time.  Basically, <timer_id_[i]>
  // contains the index in the <heap_> array where an <ACE_Timer_Node>
  // * with timer id <i> resides.  Thus, the timer id passed back from
  // <schedule> is really an index into the <timer_ids> array.  The
  // <timer_ids_> array serves two purposes: negative values are
  // treated as "pointers" for the <freelist_>, whereas positive
  // values are treated as "pointers" into the <heap_> array.

  int timer_ids_freelist_;
  // "Pointer" to the first element in the freelist contained within
  // the <timer_ids_> array, which is organized as a stack.

  ACE_Timer_Node_T<TYPE, FUNCTOR> *preallocated_nodes_;
  // If this is non-0, then we preallocate <max_size_> number of
  // <ACE_Timer_Node> objects in order to reduce dynamic allocation
  // costs.  In auto-growing implementation, this points to the 
  // last array of nodes allocated.

  ACE_Timer_Node_T<TYPE, FUNCTOR> *preallocated_nodes_freelist_;
  // This points to the head of the <preallocated_nodes_> freelist,
  // which is organized as a stack.

  ACE_Unbounded_Set<ACE_Timer_Node_T<TYPE, FUNCTOR> *> preallocated_node_set_;
  // Set of pointers to the arrays of preallocated timer nodes.
  // Used to delete the allocated memory when required.

  // = Don't allow these operations for now.
  ACE_Timer_Heap_T (const ACE_Timer_Heap_T<TYPE, FUNCTOR> &);
  void operator= (const ACE_Timer_Heap_T<TYPE, FUNCTOR> &);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Timer_Heap_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Timer_Heap_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TIMER_HEAP_T_H */
