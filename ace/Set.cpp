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
	  size_t index = i;

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
	  size_t index = i;

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
  ACE_Set_Node (ACE_Set_Node<T> *n = 0);
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
ACE_Set_Node<T>::ACE_Set_Node (ACE_Set_Node<T> *n)
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

template <class T>
ACE_Unbounded_Set<T>::~ACE_Unbounded_Set (void)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::~ACE_Unbounded_Set");
  while (this->head_ != 0)
    {
      ACE_Set_Node<T> *temp = this->head_;
      this->head_ = this->head_->next_;
      this->cur_size_--;
      delete temp;
    }
}

template <class T> int
ACE_Unbounded_Set<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::find");
  for (ACE_Set_Node<T> *temp = this->head_;
       temp != 0;
       temp = temp->next_)
    if (temp->item_ == item)
      return 1;

  return 0;    
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (void)
  : head_ (0),
    cur_size_ (0)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");
}

template <class T> int
ACE_Unbounded_Set<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::insert");
  if (this->find (item) == 0)
    {
      ACE_NEW_RETURN (this->head_, ACE_Set_Node<T> (item, this->head_), -1);
      this->cur_size_++;
      return 0;
    }
  else
    return 1;
}

template <class T> int
ACE_Unbounded_Set<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::remove");
  ACE_Set_Node<T> *prev = 0;
  ACE_Set_Node<T> *temp;

  for (temp = this->head_;
       temp != 0;
       temp = temp->next_)
    {
      if (temp->item_ == item)
	break;
      prev = temp;
    }

  if (temp == 0)
    return 0;
  else if (prev == 0) // Deleting the front of the list.
    this->head_ = this->head_->next_;
  else
    prev->next_ = temp->next_;

  this->cur_size_--;

  delete temp;
  return 1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Iterator)

template <class T> void
ACE_Unbounded_Set_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s)
    : current_ (s.head_)
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
  if (this->current_ == 0)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

#endif /* ACE_SET_C */
