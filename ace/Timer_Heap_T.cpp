// $Id$

#if !defined (ACE_TIMER_HEAP_T_C)
#define ACE_TIMER_HEAP_T_C

#define ACE_BUILD_DLL

#include "ace/Timer_Heap_T.h"

// Define some simple macros to clarify the code.
#define ACE_HEAP_PARENT(X) (X == 0 ? 0 : (((X) - 1) / 2))
#define ACE_HEAP_LCHILD(X) (((X)+(X))+1)

template <class TYPE, class FUNCTOR> 
ACE_Timer_Heap_Iterator_T<TYPE, FUNCTOR>::ACE_Timer_Heap_Iterator_T (ACE_Timer_Heap_T<TYPE, FUNCTOR> &heap)
  : timer_heap_ (heap)
{
  ACE_TRACE ("ACE_Timer_Heap_Iterator::ACE_Timer_Heap_Iterator");
}


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_Iterator_T<TYPE, FUNCTOR>::next (ACE_Timer_Node_T<TYPE, FUNCTOR> *&node,
						const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Heap_Iterator::next");

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

template <class TYPE, class FUNCTOR> 
ACE_Timer_Heap_T<TYPE, FUNCTOR>::ACE_Timer_Heap_T (size_t size,
						   int preallocate,
						   FUNCTOR *upcall_functor)
  : INHERITED (upcall_functor),
    max_size_ (size),
    cur_size_ (0),
    iterator_ (*this),
    timer_ids_freelist_ (0),
    preallocated_nodes_ (0),
    preallocated_nodes_freelist_ (0)
{
  ACE_TRACE ("ACE_Timer_Heap::ACE_Timer_Heap");

  // Create the heap array.
  ACE_NEW (this->heap_, NODE *[size]);

  // Create the parallel 
  ACE_NEW (this->timer_ids_, int[size]);

  // Initialize the "freelist," which uses negative values to
  // distinguish freelist elements from "pointers" into the <heap_>
  // array.
  for (size_t i = 0; i < size; i++)
    this->timer_ids_[i] = -((int) (i + 1));

  if (preallocate)
    {
      ACE_NEW (this->preallocated_nodes_,
	       NODE[size]);

      // Add allocated array to set of such arrays for deletion
      // on cleanup.
      this->preallocated_node_set_.insert (this->preallocated_nodes_);

      // Form the freelist by linking the next_ pointers together.
      for (size_t j = 1; j < size; j++)
	this->preallocated_nodes_[j - 1].next_ = 
	  &this->preallocated_nodes_[j];

      // NULL-terminate the freelist.
      this->preallocated_nodes_[size - 1].next_ = 0;

      // Assign the freelist pointer to the front of the list.
      this->preallocated_nodes_freelist_ = 
	&this->preallocated_nodes_[0];
    }
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Heap_T<TYPE, FUNCTOR>::~ACE_Timer_Heap_T (void)
{
  ACE_TRACE ("ACE_Timer_Heap::~ACE_Timer_Heap");
  delete [] this->heap_;
  delete [] this->timer_ids_;

  // clean up any preallocated timer nodes
  if (preallocated_nodes_ != 0)
    {
      ACE_Unbounded_Set_Iterator<NODE *> 
	set_iterator (this->preallocated_node_set_);

      for (NODE **entry = 0;
	   set_iterator.next (entry) !=0;
	   set_iterator.advance ())
	delete [] *entry;
    }
}


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_T<TYPE, FUNCTOR>::pop_freelist (void)
{
  ACE_TRACE ("ACE_Timer_Heap::pop_freelist");

  int new_id = this->timer_ids_freelist_;
  // The freelist values in the <timer_ids_> are negative, so we need
  // to negate them to get the next freelist "pointer."
  this->timer_ids_freelist_ = -this->timer_ids_[this->timer_ids_freelist_];
  return new_id;
}


template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::push_freelist (int old_id)
{
  ACE_TRACE ("ACE_Timer_Heap::push_freelist");

  // The freelist values in the <timer_ids_> are negative, so we need
  // to negate them to get the next freelist "pointer."
  this->timer_ids_[old_id] = -this->timer_ids_freelist_;
  this->timer_ids_freelist_ = old_id;
}


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_T<TYPE, FUNCTOR>::timer_id (void)
{
  ACE_TRACE ("ACE_Timer_Heap::timer_id");

  // Return the next item off the freelist and use it as the timer id.
  return this->pop_freelist ();
}

// Checks if queue is empty. 


template <class TYPE, class FUNCTOR> int 
ACE_Timer_Heap_T<TYPE, FUNCTOR>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Heap::is_empty");
  return this->cur_size_ == 0;
}


template <class TYPE, class FUNCTOR> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> &
ACE_Timer_Heap_T<TYPE, FUNCTOR>::iter (void)
{
  return this->iterator_;
}

// Returns earliest time in a non-empty queue.


template <class TYPE, class FUNCTOR> const ACE_Time_Value &
ACE_Timer_Heap_T<TYPE, FUNCTOR>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Heap::earliest_time");
  return this->heap_[0]->timer_value_;
}


template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Heap::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nmax_size_ = %d", this->max_size_));
  ACE_DEBUG ((LM_DEBUG, "\ncur_size_ = %d", this->cur_size_));
  ACE_DEBUG ((LM_DEBUG, "\nheap_ = \n"));

  for (size_t i = 0; i < this->cur_size_; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d\n", i));
      this->heap_[i]->dump ();
    }

  ACE_DEBUG ((LM_DEBUG, "\ntimer_ids_ = \n"));

  for (size_t j = 0; j < this->cur_size_; j++)
    ACE_DEBUG ((LM_DEBUG, "%d\t%d\n", j, this->timer_ids_[j]));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::copy (int index, ACE_Timer_Node_T<TYPE, FUNCTOR> *moved_node)
{
  // Insert <moved_node> into its new location in the heap.
  this->heap_[index] = moved_node;

  ACE_ASSERT (moved_node->timer_id_ >= 0 
	      && moved_node->timer_id_ < (int) this->max_size_);
  // Update the corresponding slot in the parallel <timer_ids_> array.
  this->timer_ids_[moved_node->timer_id_] = index;
}

template <class TYPE, class FUNCTOR> ACE_Timer_Node_T<TYPE, FUNCTOR> *
ACE_Timer_Heap_T<TYPE, FUNCTOR>::remove (size_t index)
{
  NODE *removed_node = this->heap_[index];

  // Return this timer id to the freelist.
  this->push_freelist (removed_node->timer_id_);

  // Decrement the size of the heap by one since we're removing the
  // "index"th node.
  this->cur_size_--;

  // Only try to reheapify if we're not deleting the last entry.
  
  if (index < this->cur_size_)
    {
      NODE *moved_node = this->heap_[this->cur_size_];
      
      // Move the end node to the location being removed and update
      // the corresponding slot in the parallel <timer_ids> array.
      this->copy (index, moved_node);
      
      // If the <moved_node->time_value_> is great than or equal its
      // parent it needs be moved down the heap.
      int parent = ACE_HEAP_PARENT (index);

      if (moved_node->timer_value_ >= this->heap_[parent]->timer_value_)
        this->reheap_down (moved_node, index, ACE_HEAP_LCHILD (index));
      else
        this->reheap_up (moved_node, index, parent);
    }

  return removed_node;
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::reheap_down (ACE_Timer_Node_T<TYPE, FUNCTOR> *moved_node,
					      size_t index, 
					      size_t child)
{
  // Restore the heap property after a deletion.

  while (child < this->cur_size_)
    {
      // Choose the smaller of the two children.
      if (child + 1 < this->cur_size_
          && this->heap_[child + 1]->timer_value_ < this->heap_[child]->timer_value_)
        child++;
      
      // Perform a <copy> if the child has a larger timeout value than
      // the <moved_node>.
      if (this->heap_[child]->timer_value_ < moved_node->timer_value_)
        {
	  this->copy (index, this->heap_[child]);
          index = child;
	  child = ACE_HEAP_LCHILD (child);
        }
      else 
	// We've found our location in the heap.
        break; 
    }

  this->copy (index, moved_node);
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::reheap_up (ACE_Timer_Node_T<TYPE, FUNCTOR> *moved_node,
					    size_t index,
					    size_t parent)
{
  // Restore the heap property after an insertion.

  while (index > 0) 
    {
      // If the parent node is greater than the <moved_node> we need
      // to copy it down.
      if (moved_node->timer_value_ < this->heap_[parent]->timer_value_)
        {
	  this->copy (index, this->heap_[parent]);
          index = parent;
	  parent = ACE_HEAP_PARENT (index);
        }
      else 
        break;
    }

  // Insert the new node into its proper resting place in the heap and
  // update the corresponding slot in the parallel <timer_ids> array.
  this->copy (index, moved_node);
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::insert (ACE_Timer_Node_T<TYPE, FUNCTOR> *new_node)
{
  if (this->cur_size_ + 1 >= max_size_)
    this->grow_heap ();

  this->reheap_up (new_node, 
		   this->cur_size_, 
		   ACE_HEAP_PARENT (this->cur_size_));
  this->cur_size_++;
}


template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::grow_heap (void)
{
  // All the containers will double in size from max_size_
  size_t new_size = max_size_ * 2;

   // First grow the heap itself.

  NODE **new_heap = 0;
  ACE_NEW (new_heap, NODE *[new_size]);
  ACE_OS::memcpy (new_heap, this->heap_, 
		  max_size_ * sizeof *new_heap);
  delete [] this->heap_;
  this->heap_ = new_heap;

  // Grow the array of timer ids.

  int *new_timer_ids = 0;

  ACE_NEW (new_timer_ids, int[new_size]);

  ACE_OS::memcpy (new_timer_ids, 
		  this->timer_ids_, 
		  max_size_ * sizeof (int));

  delete [] timer_ids_;
  this->timer_ids_ = new_timer_ids;

  // and add the new elements to the end of the "freelist"
  for (size_t i = this->max_size_; i < new_size; i++)
    this->timer_ids_[i] = -((int) (i + 1));

   // Grow the preallocation array (if using preallocation)
  if (this->preallocated_nodes_ != 0)
    {
      // Create a new array with max_size elements to link in
      // to existing list.
      ACE_NEW (this->preallocated_nodes_, 
	       NODE[this->max_size_]);

      // Add it to the set for later deletion
      this->preallocated_node_set_.insert (this->preallocated_nodes_);      
      
      // link new nodes together (as for original list).
      for (size_t k = 1; k < this->max_size_; k++)
	this->preallocated_nodes_[k - 1].next_ = 
	  &this->preallocated_nodes_[k];

      // NULL-terminate the new list.
      this->preallocated_nodes_[this->max_size_ - 1].next_ = 0;

      // link new array to the end of the existling list
      if (this->preallocated_nodes_freelist_ == 0)
	this->preallocated_nodes_freelist_ = &preallocated_nodes_[0];
      else
	{
	  NODE *previous = this->preallocated_nodes_freelist_;
	
	  for (NODE *current = this->preallocated_nodes_freelist_->next_;
	       current != 0;
	       current = current->next_)
	    previous = current;

	  previous->next_ = &this->preallocated_nodes_[0];
	}
    }

  this->max_size_ = new_size;
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

template <class TYPE, class FUNCTOR> void 
ACE_Timer_Heap_T<TYPE, FUNCTOR>::reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR> *expired)
{
  ACE_TRACE ("ACE_Timer_Heap::reschedule");

  // Restore the heap property.
  this->insert (expired);
}


template <class TYPE, class FUNCTOR> ACE_Timer_Node_T<TYPE, FUNCTOR> *
ACE_Timer_Heap_T<TYPE, FUNCTOR>::alloc_node (void)
{
  ACE_Timer_Node *temp;

  // Only allocate a node if we are *not* using the preallocated heap.
  if (this->preallocated_nodes_ == 0)
    ACE_NEW_RETURN (temp,
		    NODE,
		    0);
  else
    {
      // check to see if the heap needs to grow
      if (this->preallocated_nodes_freelist_ == 0)
	this->grow_heap ();

      temp = this->preallocated_nodes_freelist_;

      // Remove the first element from the freelist.
      this->preallocated_nodes_freelist_ = 
	this->preallocated_nodes_freelist_->next_;
    }
  return temp;
}


template <class TYPE, class FUNCTOR> void
ACE_Timer_Heap_T<TYPE, FUNCTOR>::free_node (ACE_Timer_Node_T<TYPE, FUNCTOR> *node)
{
  // Only free up a node if we are *not* using the preallocated heap.
  if (this->preallocated_nodes_ == 0)
    delete node;
  else
    {
      node->next_ = this->preallocated_nodes_freelist_;
      this->preallocated_nodes_freelist_ = node;
    }
}

// Insert a new timer that expires at time future_time; if interval is
// > 0, the handler will be reinvoked periodically.


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_T<TYPE, FUNCTOR>::schedule (const TYPE &type,
					   const void *act,
					   const ACE_Time_Value &future_time, 
					   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Heap::schedule");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->cur_size_ < this->max_size_)
    {
      // Obtain the next unique sequence number.
      int timer_id = this->timer_id ();
      
      // Obtain the memory to the new node.
      NODE *temp = this->alloc_node ();

      if (temp)
	{
	  // Use operator placement new.
	  new (temp) NODE (type, 
			   act, 
			   future_time,
			   interval, 
			   0,
			   timer_id);
	  this->insert (temp);
	  return timer_id;
	}
    }
  // Failure return.
  errno = ENOMEM;
  return -1;
}

// Locate and remove the single timer with a value of <timer_id> from
// the timer queue.


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_T<TYPE, FUNCTOR>::cancel (int timer_id, 
					 const void **act,
					 int dont_call)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  // Locate the ACE_Timer_Node that corresponds to the timer_id.

  int timer_node_slot = this->timer_ids_[timer_id];

  if (timer_id != this->heap_[timer_node_slot]->timer_id_)
    {
      ACE_ASSERT (timer_id == this->heap_[timer_node_slot]->timer_id_);
      return 0;
    }
  else
    {
      NODE *temp = this->remove (timer_node_slot);

      if (dont_call == 0)
	// Call the close hook.
	this->upcall_functor_.operator () (*this, temp->type_);
      if (act != 0)
        *act = temp->act_;
      
      this->free_node (temp);
      return 1;
    }
}

// Locate and remove all values of <type> from the timer queue.


template <class TYPE, class FUNCTOR> int
ACE_Timer_Heap_T<TYPE, FUNCTOR>::cancel (const TYPE &type,
					 int dont_call)
{
  ACE_TRACE ("ACE_Timer_Heap::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_cancellations = 0;

  // Try to locate the ACE_Timer_Node that matches the timer_id.

  for (size_t i = 0; i < this->cur_size_; )
    {
      if (this->heap_[i]->type_ == type)
        {
          NODE *temp = this->remove (i);

          number_of_cancellations++;

	  if (dont_call == 0
	      && number_of_cancellations == 1)
	    // Call the close hook.
	    this->upcall_functor_.operator () (*this, temp->type_);

          this->free_node (temp);
        }
      else
	i++;
    }

  return number_of_cancellations;
}

#endif /* ACE_TIMER_HEAP_T_C */
