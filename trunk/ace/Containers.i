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

template <class T> ACE_INLINE int
ACE_Bounded_Set<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::is_full");
  return this->cur_size_ == this->max_size_;
}

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
