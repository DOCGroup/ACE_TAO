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
      // Remove the first item and restore the heap property.

      node = this->timer_heap_.remove (0);
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

ACE_Timer_Node *
ACE_Timer_Heap::remove (int index)
{
  ACE_Timer_Node *result = this->heap_[index];

  this->cur_size_--;

  // Only try to reheapify if we're not deleting the last entry.
  
  if (index < this->cur_size_)
    {
      // Move the end node to the location being removed.
      this->heap_[index] = this->heap_[this->cur_size_];

      ACE_Timer_Node *moved_node = this->heap_[index];

      // If we're at the top there's no place to go but down!
      if (index == 0)
	this->reheap_down (moved_node, 1);

      // If the moved_node->time_value_ is smaller than its parent it
      // needs be moved up the heap.
      else if (moved_node->timer_value_ < this->heap_[(index - 1) / 2]->timer_value_)
	this->reheap_up (moved_node);

      // Call <reheap_down>, which will reheapify if the
      // moved_node->time_value_ is larger than either of its children
      // (who start at location <index + index>).
      else
	this->reheap_down (moved_node, index + index);
    }

  return result;
}

void
ACE_Timer_Heap::reheap_down (ACE_Timer_Node *moved_node,
			     int child_index)
{
  int parent = 0;

  // Restore the heap property after a deletion.

  for (int child = child_index; 
       child < this->cur_size_;
       child += child + 1) // Multiple child by 2 and add 1.
    {
      // Choose the smaller of the two children.
      if (child + 1 < this->cur_size_
	  && this->heap_[child + 1]->timer_value_ < this->heap_[child]->timer_value_)
	child++;

      // Perform a swap if the child has a larger timeout value than
      // the front node.
      if (this->heap_[child]->timer_value_ < moved_node->timer_value_)
	{
	  this->heap_[parent] = this->heap_[child];
	  parent = child;
	}
      else 
	break; 
    }

  // Insert moved_node into its final resting place.
  this->heap_[parent] = moved_node;   
}

void
ACE_Timer_Heap::insert (ACE_Timer_Node *new_node)
{
  this->reheap_up (new_node);
  this->cur_size_++;
}

void
ACE_Timer_Heap::reheap_up (ACE_Timer_Node *new_node)
{
  int parent;
  int child = this->cur_size_;

  // Restore the heap property after an insertion.

  while (child > 0) 
    {
      parent = (child - 1) / 2;

      if (new_node->timer_value_ < this->heap_[parent]->timer_value_)
	{
	  this->heap_[child] = this->heap_[parent];
	  child = parent;
	}
      else 
	break;
    }

  this->heap_[child] = new_node;
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

void 
ACE_Timer_Heap::reschedule (ACE_Timer_Node *expired)
{
  ACE_TRACE ("ACE_Timer_Heap::reschedule");

  // Restore the heap property.
  this->insert (expired);
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
      
      this->insert (temp);
    }

  return timer_id;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.

int
ACE_Timer_Heap::cancel (int timer_id, 
			const void **arg)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int i;

  // Try to locate the ACE_Timer_Node that matches the timer_id.

  for (i = 0; 
       i < this->cur_size_ && this->heap_[i]->timer_id_ != timer_id;
       i++)
    continue;

  if (i == this->cur_size_)
    return 0;
  else
    {
      ACE_Timer_Node *temp = this->remove (i);

      if (arg != 0)
	*arg = temp->arg_;

      delete temp;
      return 1;
    }
}

// Locate and remove all values of <handler> from the timer queue.

int
ACE_Timer_Heap::cancel (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_cancellations = 0;

  // Try to locate the ACE_Timer_Node that matches the timer_id.

  for (int i = 0; 
       i < this->cur_size_;
       )
    {
      if (this->heap_[i]->handler_ == handler)
	{
	  ACE_Timer_Node *temp = this->remove (i);
	  delete temp;
	  number_of_cancellations++;
	}
      else
       i++;
    }

  return number_of_cancellations;
}
