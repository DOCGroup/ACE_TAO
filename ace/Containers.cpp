// Containers.cpp
// $Id$

#if !defined (ACE_CONTAINERS_C)
#define ACE_CONTAINERS_C

#define ACE_BUILD_DLL
#include "ace/Containers.h"

#if !defined (__ACE_INLINE__)
#include "ace/Containers.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Stack)

template <class T> void
ACE_Bounded_Stack<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::dump");
}

template<class T>
ACE_Bounded_Stack<T>::ACE_Bounded_Stack (size_t size)
  : top_ (0),
    size_ (size)
{
  ACE_NEW (this->stack_, T[size]);

  ACE_TRACE ("ACE_Bounded_Stack<T>::ACE_Bounded_Stack");
}

template<class T>
ACE_Bounded_Stack<T>::ACE_Bounded_Stack (const ACE_Bounded_Stack<T> &s)
  : top_ (s.top_),
    size_ (s.size_)
{
  ACE_NEW (this->stack_, T[s.size_]);

  ACE_TRACE ("ACE_Bounded_Stack<T>::ACE_Bounded_Stack");

  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T> const ACE_Bounded_Stack<T> &
ACE_Bounded_Stack<T>::operator= (const ACE_Bounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::operator=");
  if (&s != this)
    {
      if (this->size_ < s.size_)
	{
	  delete [] this->stack_;
	  ACE_NEW (this->stack_, T[s.size_]);
	}
      this->top_ = s.top_;
      
      for (size_t i = 0; i < this->top_; i++)
	this->stack_[i] = s.stack_[i];
    }
  return *this;
}

template<class T>
ACE_Bounded_Stack<T>::~ACE_Bounded_Stack (void) 
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::~ACE_Bounded_Stack");
  delete [] this->stack_;
}

// ----------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Stack)

template <class T, size_t SIZE> void
ACE_Fixed_Stack<T, SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::dump");
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack (void)
  : top_ (0),
    size_ (SIZE)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack");
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack (const ACE_Fixed_Stack<T, SIZE> &s)
  : top_ (s.top_),
    size_ (s.size_)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack");
  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T, size_t SIZE> const ACE_Fixed_Stack<T, SIZE> &
ACE_Fixed_Stack<T, SIZE>::operator= (const ACE_Fixed_Stack<T, SIZE> &s)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::operator=");
  if (&s != this)
    {
      this->top_ = s.top_;
      
      for (size_t i = 0; i < this->top_; i++)
	this->stack_[i] = s.stack_[i];      
    }
  return *this;
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::~ACE_Fixed_Stack (void) 
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::~ACE_Fixed_Stack");

  delete [] this->stack_;
}

//----------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Stack)

template <class T> void
ACE_Unbounded_Stack<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::dump");
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (void)
  : head_ (0),
    cur_size_ (0)
{
  ACE_NEW (this->head_, ACE_Node<T>);
  this->head_->next_ = this->head_;
  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
}

template<class T> void
ACE_Unbounded_Stack<T>::delete_all_nodes (void)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::delete_all_nodes");

  while (this->is_empty () == 0)
    {
      ACE_Node<T> *temp = this->head_->next_;
      this->head_->next_ = temp->next_;
      delete temp;
    }

  ACE_ASSERT (this->head_ == this->head_->next_
	      && this->is_empty ());
} 

template<class T> void
ACE_Unbounded_Stack<T>::copy_all_nodes (const ACE_Unbounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::copy_all_nodes");
  
  ACE_ASSERT (this->head_ == this->head_->next_);

  ACE_Node<T> *temp = this->head_;

  for (ACE_Node<T> *s_temp = s.head_->next_;
       s_temp != s.head_;
       s_temp = s_temp->next_)
    {
      ACE_NEW (temp->next_, 
	       ACE_Node<T> (s_temp->item_, temp->next_));
      temp = temp->next_;
    }
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (const ACE_Unbounded_Stack<T> &s)
  : head_ (0)
{
  ACE_NEW (this->head_, ACE_Node<T>);
  this->head_->next_ = this->head_;

  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
  this->copy_all_nodes (s);
}

template<class T> const ACE_Unbounded_Stack<T> &
ACE_Unbounded_Stack<T>::operator= (const ACE_Unbounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::operator=");
  if (this != &s)
    {
      this->delete_all_nodes ();
      this->copy_all_nodes (s);
    }
  return *this;
}

template<class T>
ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack (void) 
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack");

  this->delete_all_nodes ();
  delete this->head_;
}

template<class T> int
ACE_Unbounded_Stack<T>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::push");

  ACE_Node<T> *temp = 0;

  ACE_NEW_RETURN (temp, ACE_Node<T> (new_item, this->head_->next_), -1);

  this->head_->next_ = temp;
  return 0;
}

template<class T> int
ACE_Unbounded_Stack<T>::pop (T &item)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::pop");

  if (this->is_empty ())
    return -1;
  else
    {
      ACE_Node<T> *temp = this->head_->next_;
      item = temp->item_;
      this->head_->next_ = temp->next_;

      delete temp;
      return 0;
    }
}

template <class T> int
ACE_Unbounded_Stack<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? -1 : 0;
}

template <class T> int
ACE_Unbounded_Stack<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::insert");
  
  if (this->find (item) == 0)
    return 1;
  else
    return this->push (item);
}

template <class T> int
ACE_Unbounded_Stack<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      ACE_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      delete temp;
      return 0;
    }
}

template <class T>
ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (void)
  : head_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (void)");

  ACE_NEW (this->head_, ACE_Node<T>);

  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (const ACE_Unbounded_Queue<T> &us)
  : head_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue");

  ACE_NEW (this->head_, ACE_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_Queue<T>::operator = (const ACE_Unbounded_Queue<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::operator =");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
  return *this;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Queue)

template <class T> void
ACE_Unbounded_Queue<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhead_ = %u", this->head_));
  ACE_DEBUG ((LM_DEBUG, "\nhead_->next_ = %u", this->head_->next_));
  ACE_DEBUG ((LM_DEBUG, "\ncur_size_ = %d\n", this->cur_size_));

  T *item = 0;
  size_t count = 1;

  for (ACE_Unbounded_Queue_Iterator<T> iter (*(ACE_Unbounded_Queue<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, "count = %d\n", count++));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

template <class T> void
ACE_Unbounded_Queue<T>::copy_nodes (const ACE_Unbounded_Queue<T> &us)
{
  for (ACE_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    if (this->enqueue_tail (curr->item_) == -1)
      // @@ What's the right thing to do here?
      this->delete_nodes ();
}

template <class T> void
ACE_Unbounded_Queue<T>::delete_nodes (void)
{
  ACE_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Node<T> *temp = curr;
      curr = curr->next_;
      delete temp;
      this->cur_size_--;
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Queue<T>::~ACE_Unbounded_Queue (void)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::~ACE_Unbounded_Queue (void)");

  this->delete_nodes ();
  delete this->head_;
  this->head_ = 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::enqueue_head (const T &new_item)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::enqueue_tail");

  ACE_Node<T> *temp;

  // Create a new node that points to the original head.
  ACE_NEW_RETURN (temp, ACE_Node<T> (new_item, this->head_->next_), -1);

  // Link this pointer into the front of the list.
  this->head_->next_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::enqueue_tail (const T &new_item)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::enqueue_head");

  ACE_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = new_item;

  // Create a new dummy node.
  ACE_NEW_RETURN (temp, ACE_Node<T> (this->head_->next_), -1);

  // Link this dummy pointer into the list.
  this->head_->next_ = temp;

  // Point the head to the new dummy node.
  this->head_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::dequeue_head (T &item)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::dequeue_head");

  // Check for empty queue.
  if (this->is_empty ())
    return -1;

  ACE_Node<T> *temp = this->head_->next_;

  item = temp->item_;
  this->head_->next_ = temp->next_;
  delete temp;
  --this->cur_size_;
  return 0;
}

template <class T> void
ACE_Unbounded_Queue<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> int
ACE_Unbounded_Queue<T>::get (T *&item, size_t index) const
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::get");

  ACE_Node<T> *curr = this->head_->next_;

  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    {
      if (i == index)
	break;

      curr = curr->next_;
    }

  if (i < this->cur_size_)
    {
      item = &curr->item_;
      return 0;
    }
  else
    return -1;
}

template <class T> int
ACE_Unbounded_Queue<T>::set (const T &item, 
			     size_t index)
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::set");

  ACE_Node<T> *curr = this->head_->next_;

  size_t i;

  for (i = 0; 
       i < index && i < this->cur_size_; 
       i++)
    curr = curr->next_;

  if (i < this->cur_size_)
    {
      // We're in range, so everything's cool.
      curr->item_ = item;
      return 0;
    }
  else
    {
      // We need to expand the list.

      // A common case will be increasing the set size by 1.
      // Therefore, we'll optimize for this case.
      if (i == index)
	{
	  // Try to expand the size of the set by 1.
	  if (this->enqueue_tail (item) == -1)
	    return -1;
	  else
	    return 0;
	}
      else 
	{
	  T dummy;

	  // We need to expand the list by multiple (dummy) items.
	  for (; i < index; i++)
	    {
	      // This head points to the existing dummy node, which is
	      // about to be overwritten when we add the new dummy
	      // node.
	      curr = this->head_;

	      // Try to expand the size of the set by 1, but don't
	      // store anything in the dummy node (yet).
	      if (this->enqueue_tail (dummy) == -1)
		return -1;
	    }

	  curr->item_ = item;
	  return 0;
	}
    }
}

template <class T> void
ACE_Unbounded_Queue_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Queue_Iterator<T>::ACE_Unbounded_Queue_Iterator (ACE_Unbounded_Queue<T> &q)
    : current_ (q.head_->next_),
      queue_ (q)
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::ACE_Unbounded_Queue_Iterator");
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::advance (void) 
{ 
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->queue_.head_;
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::done (void) const
{ 
  ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::done");

  return this->current_ == this->queue_.head_;
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::next (T *&item)
{ 
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::next");
  if (this->current_ == this->queue_.head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Set)

template <class T, size_t SIZE> size_t
ACE_Fixed_Set<T, SIZE>::size (void) const
{
  return this->cur_size_;
}

template <class T> size_t
ACE_Bounded_Set<T>::size (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::size");
  return this->cur_size_;
}

template <class T> size_t
ACE_Unbounded_Set<T>::size (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::size");
  return this->cur_size_;
}

template <class T, size_t SIZE> void
ACE_Fixed_Set<T, SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::dump");
}

template <class T, size_t SIZE>
ACE_Fixed_Set<T, SIZE>::~ACE_Fixed_Set (void)
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::~ACE_Fixed_Set");
  this->cur_size_ = 0;
}

template <class T, size_t SIZE>
ACE_Fixed_Set<T, SIZE>::ACE_Fixed_Set (const ACE_Fixed_Set<T, SIZE> &fs)
  : cur_size_ (fs.cur_size_)
{
  ACE_TRACE ("ACE_Fixed_Set<T>::ACE_Fixed_Set");

  for (size_t i = 0; i < this->cur_size_; i++)
    this->search_structure_[i] = fs.search_structure_[i];
}

template <class T, size_t SIZE> const ACE_Fixed_Set<T, SIZE> &
ACE_Fixed_Set<T, SIZE>::operator = (const ACE_Fixed_Set<T, SIZE> &fs)
{
  ACE_TRACE ("ACE_Fixed_Set<T>::operator =");

  if (this != &fs)
    {
      this->cur_size_ = fs.cur_size_;
      
      for (size_t i = 0; i < this->cur_size_; i++)
	this->search_structure_[i] = fs.search_structure_[i];
    }
  return *this;
}

template <class T, size_t SIZE>
ACE_Fixed_Set<T, SIZE>::ACE_Fixed_Set (void)
  : cur_size_ (0), 
    max_size_ (SIZE)
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::ACE_Fixed_Set");
  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set<T, SIZE>::find (const T &item) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::find");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item 
	&& this->search_structure_[i].is_free_ == 0)
      return 0;

  return -1;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set<T, SIZE>::insert (const T &item)

{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    // First, make sure we don't allow duplicates.

    if (this->search_structure_[i].item_ == item 
	&& this->search_structure_[i].is_free_ == 0)
      return 1;
    else if (this->search_structure_[i].is_free_ 
	     && first_free == -1)
      first_free = i;

  // If we found a free spot let's reuse it.
  if (first_free > -1)   
    {
      this->search_structure_[first_free].item_ = item;
      this->search_structure_[first_free].is_free_ = 0;
      return 0;
    }
    // Insert at the end of the active portion.
  else if (i < this->max_size_) 
    {
      this->search_structure_[i].item_ = item;
      this->search_structure_[i].is_free_ = 0;
      this->cur_size_++;
      return 0;
    }
  else /* No more room! */
    {
      errno = ENOMEM;
      return -1;
    }
}

template <class T, size_t SIZE> int
ACE_Fixed_Set<T, SIZE>::remove (const T &item)
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::remove");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item)
      {
	// Mark this entry as being free.
	this->search_structure_[i].is_free_ = 1;
	  
	// If we just unbound the highest entry, then we need to
	// figure out where the next highest active entry is.
	if (i + 1 == this->cur_size_)
	  {
	    while (i > 0 
		   && this->search_structure_[--i].is_free_)
	      continue;

	    if (i == 0 
		&& this->search_structure_[i].is_free_)
	      this->cur_size_ = 0;
	    else
	      this->cur_size_ = i + 1;
	  }
	return 0;
      }

  return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Set_Iterator)

template <class T, size_t SIZE> void
ACE_Fixed_Set_Iterator<T, SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, SIZE>::dump");
}

template <class T, size_t SIZE>
ACE_Fixed_Set_Iterator<T, SIZE>::ACE_Fixed_Set_Iterator (ACE_Fixed_Set<T, SIZE> &s)
  : s_ (s), 
    next_ (-1)
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, SIZE>::ACE_Fixed_Set_Iterator");
  this->advance ();
}

template <class T, size_t SIZE> int
ACE_Fixed_Set_Iterator<T, SIZE>::advance (void) 
{ 
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, SIZE>::advance");

  for (++this->next_;
       size_t (this->next_) < this->s_.cur_size_
       && this->s_.search_structure_[this->next_].is_free_;
       ++this->next_)
    continue;
	 
  return size_t (this->next_) < this->s_.cur_size_;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set_Iterator<T, SIZE>::done (void) const
{ 
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, SIZE>::done");

  return size_t (this->next_) >= this->s_.cur_size_;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set_Iterator<T, SIZE>::next (T *&item)
{ 
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, SIZE>::next");
  if (size_t (this->next_) < this->s_.cur_size_)
    {
      item = &this->s_.search_structure_[this->next_].item_;
      return 1;
    }
  else
    return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Set)

template <class T> void
ACE_Bounded_Set<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::dump");
}

template <class T>
ACE_Bounded_Set<T>::~ACE_Bounded_Set (void)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::~ACE_Bounded_Set");
  delete [] this->search_structure_;
}

template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (void)
  : cur_size_ (0),
    max_size_ (size_t (ACE_Bounded_Set<T>::DEFAULT_SIZE))
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");

  ACE_NEW (this->search_structure_, ACE_Bounded_Set<T>::Search_Structure[this->max_size_]);

  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (const ACE_Bounded_Set<T> &bs)
  : cur_size_ (bs.cur_size_),
    max_size_ (bs.max_size_)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");

  ACE_NEW (this->search_structure_, ACE_Bounded_Set<T>::Search_Structure[this->max_size_]);

  for (size_t i = 0; i < this->cur_size_; i++)
    this->search_structure_[i] = bs.search_structure_[i];
}

template <class T> const ACE_Bounded_Set<T> &
ACE_Bounded_Set<T>::operator = (const ACE_Bounded_Set<T> &bs)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::operator =");

  if (this != &bs)
    {
      if (this->max_size_ < bs.cur_size_)
	{
	  delete [] this->search_structure_;
	  ACE_NEW (this->search_structure_, 
		   ACE_Bounded_Set<T>::Search_Structure[bs.cur_size_]);
	  this->max_size_ = bs.cur_size_;
	}
      
      this->cur_size_ = bs.cur_size_;
      
      for (size_t i = 0; i < this->cur_size_; i++)
	this->search_structure_[i] = bs.search_structure_[i];
    }
  return *this;
}
  
template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (size_t size)
  : cur_size_ (0), 
    max_size_ (size)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");
  ACE_NEW (this->search_structure_, ACE_Bounded_Set<T>::Search_Structure[size]);

  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T> int
ACE_Bounded_Set<T>::find (const T &item) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::find");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item 
	&& this->search_structure_[i].is_free_ == 0)
      return 0;

  return -1;
}

template <class T> int
ACE_Bounded_Set<T>::insert (const T &item)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    // First, make sure we don't allow duplicates.

    if (this->search_structure_[i].item_ == item 
	&& this->search_structure_[i].is_free_ == 0)
      return 1;
    else if (this->search_structure_[i].is_free_ && first_free == -1)
      first_free = i;

  if (first_free > -1)   // If we found a free spot let's reuse it.
    {
      this->search_structure_[first_free].item_ = item;
      this->search_structure_[first_free].is_free_ = 0;
      return 0;
    }
  else if (i < this->max_size_) // Insert at the end of the active portion.
    {
      this->search_structure_[i].item_ = item;
      this->search_structure_[i].is_free_ = 0;
      this->cur_size_++;
      return 0;
    }
  else /* No more room! */
    {
      errno = ENOMEM;
      return -1;
    }
}

template <class T> int
ACE_Bounded_Set<T>::remove (const T &item)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::remove");
  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item)
      {
	// Mark this entry as being free.
	this->search_structure_[i].is_free_ = 1;
	  
	// If we just unbound the highest entry, then we need to
	// figure out where the next highest active entry is.
	if (i + 1 == this->cur_size_)
	  {
	    while (i > 0 && this->search_structure_[--i].is_free_)
	      continue;

	    if (i == 0 && this->search_structure_[i].is_free_)
	      this->cur_size_ = 0;
	    else
	      this->cur_size_ = i + 1;
	  }
	return 0;
      }

  return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Set_Iterator)

template <class T> void
ACE_Bounded_Set_Iterator<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::dump");
}

template <class T>
ACE_Bounded_Set_Iterator<T>::ACE_Bounded_Set_Iterator (ACE_Bounded_Set<T> &s)
  : s_ (s), 
    next_ (-1)
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::ACE_Bounded_Set_Iterator");
  this->advance ();
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::advance (void) 
{ 
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::advance");

  for (++this->next_;
       size_t (this->next_) < this->s_.cur_size_
       && this->s_.search_structure_[this->next_].is_free_;
       ++this->next_)
    continue;
	 
  return size_t (this->next_) < this->s_.cur_size_;
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::done (void) const
{ 
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::done");

  return size_t (this->next_) >= this->s_.cur_size_;
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::next (T *&item)
{ 
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::next");
  if (size_t (this->next_) < this->s_.cur_size_)
    {
      item = &this->s_.search_structure_[this->next_].item_;
      return 1;
    }
  else
    return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Node)

template <class T>
ACE_Node<T>::~ACE_Node (void)
{
}

template <class T>
ACE_Node<T>::ACE_Node (const T &i, ACE_Node<T> *n)
  : next_ (n), 
    item_ (i)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

template <class T>
ACE_Node<T>::ACE_Node (ACE_Node<T> *n, int /* MS_SUCKS */)
  : next_ (n)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

template <class T>
ACE_Node<T>::ACE_Node (const ACE_Node<T> &s)
  : next_ (s.next_), 
    item_ (s.item_)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set)

template <class T> int
ACE_Unbounded_Set<T>::insert_tail (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Queue<T>::insert_tail");
  ACE_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = item;

  // Create a new dummy node.
  ACE_NEW_RETURN (temp, ACE_Node<T> (this->head_->next_), -1);

  // Link this pointer into the list.
  this->head_->next_ = temp;

  // Point the head to the new dummy node.
  this->head_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> void
ACE_Unbounded_Set<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhead_ = %u", this->head_));
  ACE_DEBUG ((LM_DEBUG, "\nhead_->next_ = %u", this->head_->next_));
  ACE_DEBUG ((LM_DEBUG, "\ncur_size_ = %d\n", this->cur_size_));

  T *item = 0;
  size_t count = 1;

  for (ACE_Unbounded_Set_Iterator<T> iter (*(ACE_Unbounded_Set<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG, "count = %d\n", count++));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

template <class T> void
ACE_Unbounded_Set<T>::copy_nodes (const ACE_Unbounded_Set<T> &us)
{
  for (ACE_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    this->insert_tail (curr->item_);
}

template <class T> void
ACE_Unbounded_Set<T>::delete_nodes (void)
{
  ACE_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Node<T> *temp = curr;
      curr = curr->next_;
      delete temp;
      this->cur_size_--;
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set<T>::~ACE_Unbounded_Set (void)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::~ACE_Unbounded_Set");

  this->delete_nodes ();

  // Delete the dummy node.
  delete this->head_;
  this->head_ = 0;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (void)
  : head_ (0),
    cur_size_ (0)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");

  ACE_NEW (this->head_, ACE_Node<T>);

  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (const ACE_Unbounded_Set<T> &us)
  : head_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");

  ACE_NEW (this->head_, ACE_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> const ACE_Unbounded_Set<T> &
ACE_Unbounded_Set<T>::operator = (const ACE_Unbounded_Set<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::operator =");

  if (this != &us)
    {  
      this->delete_nodes ();
      this->copy_nodes (us);
    }
  return *this;
}

template <class T> int
ACE_Unbounded_Set<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? -1 : 0;
}

template <class T> int
ACE_Unbounded_Set<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::insert");
  if (this->find (item) == 0)
    return 1;
  else
    return this->insert_tail (item);
}

template <class T> int
ACE_Unbounded_Set<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      ACE_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      delete temp;
      return 0;
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Iterator)

template <class T> void
ACE_Unbounded_Set_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s)
    : current_ (s.head_->next_),
      set_ (s)
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator");
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::advance (void) 
{ 
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->set_.head_;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::done (void) const
{ 
  ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::done");

  return this->current_ == this->set_.head_;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::next (T *&item)
{ 
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::next");
  if (this->current_ == this->set_.head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

template <class T> void
ACE_Unbounded_Stack_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Stack_Iterator<T>::ACE_Unbounded_Stack_Iterator (ACE_Unbounded_Stack<T> &q)
    : current_ (q.head_->next_),
      stack_ (q)
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::ACE_Unbounded_Stack_Iterator");
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::advance (void) 
{ 
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->stack_.head_;
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::done (void) const
{ 
  ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::done");

  return this->current_ == this->stack_.head_;
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::next (T *&item)
{ 
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::next");
  if (this->current_ == this->stack_.head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

#endif /* ACE_CONTAINERS_C */
