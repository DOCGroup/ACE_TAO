#include "ace/Timer_Heap.h"

ACE_Timer_Heap_Iterator::ACE_Timer_Heap_Iterator (ACE_Timer_Heap &heap)
  : timer_heap_ (heap)
{
}

int
ACE_Timer_Heap_Iterator::next (ACE_Timer_Node *&node,
			       const ACE_Time_Value &cur_time)
{
  if (this->timer_heap_.cur_size_ == 0 
      || this->timer_heap_.heap_[0]->timer_value_ > cur_time)
    return 0;
  else
    {
      node = this->timer_heap_.heap_[0];

      // Transfer the last element in the heap to the front and
      // restore the heap property.

      this->timer_heap_.heap_[0] = 
	this->timer_heap_.heap_[--this->timer_heap_.cur_size_];

      this->timer_heap_.reheap_down ();
      return 1;
    }
}

ACE_Timer_Heap::ACE_Timer_Heap (size_t size)
  : max_size_ (size),
    cur_size_ (0),
    iterator_ (*this)
{
  ACE_NEW (this->heap_,
	   ACE_Timer_Node *[size]);
}

ACE_Timer_Heap::~ACE_Timer_Heap (void)
{
  delete [] this->heap_;
}

// Checks if queue is empty. 

int 
ACE_Timer_Heap::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Heap::is_empty");
  return this->cur_size_ == 0;
}

ACE_Timer_Queue_Iterator &
ACE_Timer_Heap::iterator (void)
{
  return this->iterator_;
}

// Returns earliest time in a non-empty queue.

const ACE_Time_Value &
ACE_Timer_Heap::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Heap::earliest_time");
  return this->heap_[0]->timer_value_;
}

void
ACE_Timer_Heap::dump (void) const
{
  for (size_t index = 0; index < this->cur_size_; index++)
    this->heap_[index]->dump ();
}

void 
ACE_Timer_Heap::reheap_down (void)
{
  int parent = 0;
  int child = 1;
  ACE_Timer_Node *temp = this->heap_[parent];

  // Restore the heap property.

  while (child < this->cur_size_) 
    {
      if (child + 1 < this->cur_size_
	  && this->heap_[child + 1]->timer_value_ > this->heap_[child]->timer_value_)
	child++;

      if (this->heap_[child]->timer_value_ > temp->timer_value_)
	{
	  this->heap_[parent] = this->heap_[child];
	  parent = child;
	  // Multiple child by 2 and add 1.
	  child += child + 1;
	}
      else 
	break; 
    }

  this->heap_[parent] = temp;   
}

void
ACE_Timer_Heap::reheap_up (void)
{
  int parent;
  int child = this->cur_size_ - 1;
  ACE_Timer_Node *temp = this->heap_[child];
   
  // Restore the heap property.

  while (child > 0) 
    {
      parent = (child - 1) / 2;

      if (temp->timer_value_ < this->heap_[parent]->timer_value_)
	{
	  this->heap_[child] = this->heap_[parent];
	  child = parent;
	}
      else 
	break;
    }

  this->heap_[child] = temp;
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

void 
ACE_Timer_Heap::reschedule (ACE_Timer_Node *expired)
{
  ACE_TRACE ("ACE_Timer_Heap::reschedule");

  // Insert the <expired> node into the end of the heap and restore
  // the heap property.
  this->heap_[this->cur_size_++] = expired;
  this->reheap_up ();
}

// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

int
ACE_Timer_Heap::schedule (ACE_Event_Handler *handler,
			   const void *arg,
			   const ACE_Time_Value	&future_time, 
			   const ACE_Time_Value	&interval)
{
  ACE_TRACE ("ACE_Timer_Heap::schedule");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  // Increment the sequence number (it will wrap around).
  int timer_id = this->timer_id ();

  if (this->cur_size_ >= this->max_size_)
    {
      errno = ENOMEM;
      return -1;
    }
  else
    {
      ACE_Timer_Node *temp;

      ACE_NEW_RETURN (temp,
		      ACE_Timer_Node (handler, 
				      arg, 
				      future_time,
				      interval, 
				      0,
				      timer_id),
		      -1);
      
      this->reheap_up (temp);
    }

  return timer_id;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.

int
ACE_Timer_Heap::cancel (int timer_id, const void **arg)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  return 0;
}

// Locate and remove all values of <handler> from the timer queue.

int
ACE_Timer_Heap::cancel (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  return 0;
}
