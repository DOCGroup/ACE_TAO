// Set.cpp
// $Id$

#if !defined (ACE_SET_C)
#define ACE_SET_C

#define ACE_BUILD_DLL
#include "ace/Set.h"

#if !defined (__ACE_INLINE__)
#include "ace/Set.i"
#endif /* __ACE_INLINE__ */

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

template <class T, size_t SIZE> void
ACE_Fixed_Set<T, SIZE>::operator = (const ACE_Fixed_Set<T, SIZE> &fs)
{
  ACE_TRACE ("ACE_Fixed_Set<T>::operator =");

  if (this == &fs)
    return;

  this->cur_size_ = bs.cur_size_;

  for (size_t i = 0; i < this->cur_size_; i++)
    this->search_structure_[i] = fs.search_structure_[i];
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
    {
      if (this->search_structure_[i].item_ == item 
	  && this->search_structure_[i].is_free_ == 0)
	return 1;
    }

  return 0;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set<T, SIZE>::find (T &item, size_t index) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::find");

  if (index > this->cur_size_
      || this->search_structure_[index].is_free_ != 0)
    return 0;
  else
    {
      item = this->search_structure_[i].item_;
      return 1;
    }

  return 0;
}

template <class T, size_t SIZE> int
ACE_Fixed_Set<T, SIZE>::insert (const T &item)

{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    {
      // First, make sure we don't allow duplicates.

      if (this->search_structure_[i].item_ == item 
	  && this->search_structure_[i].is_free_ == 0)
	return 1;
      else if (this->search_structure_[i].is_free_ 
	       && first_free == -1)
	first_free = i;
    }

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
    {
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
	  return 1;
	}
    }
  return 0;
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
	 
  return this->next_;
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

template <class T> void
ACE_Bounded_Set<T>::operator = (const ACE_Bounded_Set<T> &bs)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::operator =");

  if (this == &bs)
    return;
  else if (this->max_size_ < bs.cur_size_)
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
    {
      if (this->search_structure_[i].item_ == item 
	  && this->search_structure_[i].is_free_ == 0)
	return 1;
    }

  return 0;
}

template <class T> int
ACE_Bounded_Set<T>::find (T &item, size_t index) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::find");

  if (index > this->cur_size_
      || this->search_structure_[index].is_free_ != 0)
    return 0;
  else
    {
      item = this->search_structure_[i].item_;
      return 1;
    }

  return 0;
}

template <class T> int
ACE_Bounded_Set<T>::insert (const T &item)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    {
      // First, make sure we don't allow duplicates.

      if (this->search_structure_[i].item_ == item 
	  && this->search_structure_[i].is_free_ == 0)
	return 1;
      else if (this->search_structure_[i].is_free_ && first_free == -1)
	first_free = i;
    }

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
    {
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
	  return 1;
	}
    }
  return 0;
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
	 
  return this->next_;
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

ACE_ALLOC_HOOK_DEFINE(ACE_Set_Node)

template <class T>
class ACE_Set_Node
{
friend class ACE_Unbounded_Set<T>;
friend class ACE_Unbounded_Set_Iterator<T>;
public:
  // = Initialization methods
  ACE_Set_Node (const T &i, ACE_Set_Node<T> *n);
  ACE_Set_Node (ACE_Set_Node<T> *n = 0, int MS_SUCKS = 0);
  ACE_Set_Node (const ACE_Set_Node<T> &n);

private:
  ACE_Set_Node<T> *next_;
  // Pointer to next element in the list of ACE_Set_Nodes.

  T item_;
  // Current value of the item in this node.
};

template <class T>
ACE_Set_Node<T>::ACE_Set_Node (const T &i, ACE_Set_Node<T> *n)
  : next_ (n), 
    item_ (i)
{
// ACE_TRACE ("ACE_Set_Node<T>::ACE_Set_Node");
}

template <class T>
ACE_Set_Node<T>::ACE_Set_Node (ACE_Set_Node<T> *n, int /* MS_SUCKS */)
  : next_ (n)
{
// ACE_TRACE ("ACE_Set_Node<T>::ACE_Set_Node");
}

template <class T>
ACE_Set_Node<T>::ACE_Set_Node (const ACE_Set_Node<T> &s)
  : next_ (s.next_), 
    item_ (s.item_)
{
// ACE_TRACE ("ACE_Set_Node<T>::ACE_Set_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set)

template <class T> void
ACE_Unbounded_Set<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::dump");
}

template <class T> void
ACE_Unbounded_Set<T>::copy_nodes (const ACE_Unbounded_Set<T> &us)
{
  for (ACE_Set_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    this->insert_tail (curr->item_);
}

template <class T> void
ACE_Unbounded_Set<T>::delete_nodes (void)
{
  ACE_Set_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Set_Node<T> *temp = curr;
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

template <class T> int
ACE_Unbounded_Set<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Set_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? 0 : 1;
}

template <class T> int
ACE_Unbounded_Set<T>::find (T &item, size_t index) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::find");

  ACE_Set_Node<T> *curr = this->head_->next_;

  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    {
      if (i == index)
	break;

      curr = curr->next_;
    }

  if (i < this->cur_size_)
    {
      ACE_ASSERT (curr != this->head_);
      item = curr->item_;
      return 1;
    }
  else
    return 0;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (void)
  : head_ (new ACE_Set_Node<T>),
    cur_size_ (0)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");
  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (const ACE_Unbounded_Set<T> &us)
  : head_ (new ACE_Set_Node<T>),
    cur_size_ (us.cur_size_)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");

  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_Set<T>::operator = (const ACE_Unbounded_Set<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::operator =");

  if (this == &us)
    return;

  this->delete_nodes ();
  this->copy_nodes (us);
}

// Insert the new node at the tail of the list.

template <class T> int
ACE_Unbounded_Set<T>::insert_tail (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::insert");
  ACE_Set_Node<T> *temp;
  ACE_NEW_RETURN (temp, ACE_Set_Node<T> (item, this->head_->next_), -1);
  this->head_->next_ = temp;
  this->cur_size_++;
  return 0;
}

template <class T> int
ACE_Unbounded_Set<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::insert");
  if (this->find (item) == 0)
    return this->insert_tail (item);
  else
    return 1;
}

template <class T> int
ACE_Unbounded_Set<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_Set_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return 0; // Item was not found.
  else
    {
      ACE_Set_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      delete temp;
      return 1;
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
  return 1;
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

#endif /* ACE_SET_C */
