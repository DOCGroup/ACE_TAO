/* -*- C++ -*- */
// $Id$

// Containers.i

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::is_empty");
  return this->top_ == 0;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::is_full");
  return this->top_ >= this->size_;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::push");
  if (this->is_full () == 0)
    {
      this->stack_[this->top_++] = new_item;
      return 0;
    }
  else
    return -1;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::pop (T &item)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::pop");
  if (this->is_empty () == 0)
    {
      item = this->stack_[--this->top_];
      return 0;
    }
  else
    return -1;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::top");
  if (this->is_empty () == 0)
    {
      item = this->stack_[this->top_ - 1];
      return 0;
    }
  else
    return -1;
}

template <class T> ACE_INLINE size_t
ACE_Bounded_Stack<T>::size (void) const
{
  return this->size_;
}

//----------------------------------------

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, ACE_SIZE>::is_empty (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::is_empty");
  return this->top_ == 0;
}

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, ACE_SIZE>::is_full (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::is_full");
  return this->top_ >= this->size_;
}

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, ACE_SIZE>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::push");
  if (this->is_full () == 0)
    {
      this->stack_[this->top_++] = new_item;
      return 0;
    }
  else
    return -1;
}

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, ACE_SIZE>::pop (T &item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::pop");
  if (this->is_empty () == 0)
    {
      item = this->stack_[--this->top_];
      return 0;
    }
  else
    return -1;
}

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, ACE_SIZE>::top (T &item) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::top");
  if (this->is_empty () == 0)
    {
      item = this->stack_[this->top_ - 1];
      return 0;
    }
  else
    return -1;
}

template <class T, size_t ACE_SIZE> ACE_INLINE size_t
ACE_Fixed_Stack<T, ACE_SIZE>::size (void) const
{
  return this->size_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Stack<T>::is_empty (void) const
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::top");
  if (this->is_empty () == 0)
    {
      item = this->head_->next_->item_;
      return 0;
    }
  else
    return -1;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Stack<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_full");
  return 0; // ???
}

template <class T> ACE_INLINE size_t
ACE_Unbounded_Stack<T>::size (void) const
{
  return this->cur_size_;
}

// ---

template <class T> ACE_INLINE size_t
ACE_Unbounded_Queue<T>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Queue<T>::is_empty (void) const
{
  //  ACE_TRACE ("ACE_Unbounded_Queue<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Queue<T>::is_full (void) const
{
  //  ACE_TRACE ("ACE_Unbounded_Queue<T>::is_full");
  return 0; // We should implement a "node of last resort for this..."
}

// ---

template <class T> ACE_INLINE int
ACE_Unbounded_Set<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Set<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::is_full");
  return 0; // We should implement a "node of last resort for this..."
}

// ---

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Set<T, ACE_SIZE>::is_empty (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T>::is_empty");
  return this->cur_size_ == 0;
}

template <class T, size_t ACE_SIZE> ACE_INLINE int
ACE_Fixed_Set<T, ACE_SIZE>::is_full (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::is_full");
  return this->cur_size_ == this->max_size_;
}

// ---

template <class T> ACE_INLINE int
ACE_Bounded_Set<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::is_empty");
  return this->cur_size_ == 0;
}

#if !defined (__Lynx__)
 // LynxOS 3.0.0 native g++ compiler raises internal error with this inline.
template <class T> ACE_INLINE int
ACE_Bounded_Set<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::is_full");
  return this->cur_size_ == this->max_size_;
}
#endif /* ! __Lynx__ */

// --

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet_Iterator<T>::first (void)
{
  ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::first");
  current_ = set_.head_;

  return (current_ ? 1 : 0);
}

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet_Iterator<T>::last (void)
{
  ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::last");
  current_ = set_.tail_;

  return (current_ ? 1 : 0);
}

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet_Iterator<T>::advance (void)
{
  ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::advance");

  current_ = current_ ? current_->next_ : 0;

  return (current_ ? 1 : 0);
}

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet_Iterator<T>::retreat (void)
{
  ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::retreat");

  current_ = current_ ? current_->prev_ : 0;

  return (current_ ? 1 : 0);
}

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::done");

  return (current_ ? 0 : 1);
}

template <class T> ACE_INLINE void
ACE_Ordered_MultiSet_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::dump");
}



// --

template <class T> ACE_INLINE int
ACE_Ordered_MultiSet<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Ordered_MultiSet<T>::is_empty");
  return this->cur_size_ > 0 ? 0 : 1;
}

template <class T> ACE_INLINE size_t
ACE_Ordered_MultiSet<T>::size (void) const
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::size");
  return this->cur_size_;
}

// ****************************************************************

// Clean up the array (e.g., delete dynamically allocated memory).

template <class T> ACE_INLINE
ACE_Array_Base<T>::~ACE_Array_Base (void)
{
  ACE_DES_FREE (this->array_,
                this->allocator_->free,
                T);
}

template <class T> ACE_INLINE size_t
ACE_Array_Base<T>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE size_t
ACE_Array_Base<T>::max_size (void) const
{
  return this->max_size_;
}

template <class T> ACE_INLINE int
ACE_Array_Base<T>::in_range (size_t index) const
{
  return index < this->cur_size_;
}

template <class T> ACE_INLINE T &
ACE_Array_Base<T>::operator[] (size_t index)
{
  return this->array_[index];
}

template <class T> ACE_INLINE const T &
ACE_Array_Base<T>::operator[] (size_t index) const
{
  return this->array_[index];
}

// ****************************************************************

template <class T> ACE_INLINE
ACE_Array<T>::ACE_Array (size_t size, 
                         ACE_Allocator *alloc)
  : ACE_Array_Base<T> (size, alloc)
{
}

template <class T> ACE_INLINE
ACE_Array<T>::ACE_Array (size_t size,
                         const T &default_value,
                         ACE_Allocator *alloc)
  : ACE_Array_Base<T> (size, default_value, alloc)
{
}

// The copy constructor (performs initialization).

template <class T> ACE_INLINE
ACE_Array<T>::ACE_Array (const ACE_Array<T> &s)
   : ACE_Array_Base<T> (s)
{
}

// Assignment operator (performs assignment).

template <class T> ACE_INLINE void
ACE_Array<T>::operator= (const ACE_Array<T> &s)
{
  // Check for "self-assignment".

  if (this != &s)
    this->ACE_Array_Base<T>::operator= (s);
}

// Compare this array with <s> for inequality.

template <class T> ACE_INLINE int
ACE_Array<T>::operator!= (const ACE_Array<T> &s) const
{
  return !(*this == s);
}

// ****************************************************************

template <class T> ACE_INLINE void
ACE_Array_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::dump");
}

template <class T> ACE_INLINE
ACE_Array_Iterator<T>::ACE_Array_Iterator (ACE_Array_Base<T> &a)
    : current_ (0),
      array_ (a)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::ACE_Array_Iterator");
}

template <class T> ACE_INLINE int
ACE_Array_Iterator<T>::advance (void)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::advance");

  if (this->current_ < array_.size ())
    {
      ++this->current_;
      return 1;
    }
  else
    {
      // Already finished iterating.
      return 0;
    }
}

template <class T> ACE_INLINE int
ACE_Array_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Array_Iterator<T>::done");

  return this->current_ >= array_.size ();
}

// ****************************************************************

template <class T> ACE_INLINE void
ACE_DLList<T>::operator= (ACE_DLList<T> &l)
{
  *(ACE_DLList_Base *) this = l;
}

template <class T> ACE_INLINE int
ACE_DLList<T>::get (T *&item, size_t index)
{
  ACE_DLList_Node *node;
  int result = ACE_DLList_Base::get (node, index);
  item = (T *) node->item_;
  return result;
}

template <class T> ACE_INLINE void
ACE_DLList<T>::dump (void) const
{
  ACE_DLList_Base::dump ();
}

template <class T> ACE_INLINE int
ACE_DLList<T>::remove (ACE_DLList_Node *n)
{
  int result = ACE_DLList_Base::remove (n);
  ACE_DES_FREE (n,
                this->allocator_->free,
                ACE_DLList_Node);
  return result;
}

template <class T> ACE_INLINE
ACE_DLList<T>::ACE_DLList (ACE_Allocator *alloc )
  : ACE_DLList_Base (alloc)
{
}

template <class T> ACE_INLINE
ACE_DLList<T>::ACE_DLList (ACE_DLList<T> &l)
  : ACE_DLList_Base ((ACE_DLList<T> &) l)
{
}

template <class T> ACE_INLINE
ACE_DLList<T>::~ACE_DLList (void)
{
  while (this->delete_head ()) ;
}

template <class T> ACE_INLINE
ACE_DLList_Iterator<T>::ACE_DLList_Iterator (ACE_DLList<T> &l)
  : ACE_DLList_Iterator_Base ((ACE_DLList_Base &)l),
    list_ (l)
{
}

template <class T> ACE_INLINE T *
ACE_DLList_Iterator<T>::next (void) const
{
  ACE_DLList_Node *temp = ACE_DLList_Iterator_Base::next ();
  return (T *) (temp ? temp->item_ : 0);
}

template <class T> ACE_INLINE int
ACE_DLList_Iterator<T>::remove (void)
{
  ACE_DLList_Node *temp = ACE_DLList_Iterator_Base::next ();
  ACE_DLList_Iterator_Base::advance ();
  return this->list_.remove (temp);
}

template <class T> ACE_INLINE void
ACE_DLList_Iterator<T>::dump (void) const
{
  ACE_DLList_Iterator_Base::dump ();
}
