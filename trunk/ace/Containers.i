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
      return 1;
    }
  else
    return 0;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::pop (T &item)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::pop");
  if (this->is_empty () == 0)
    {
      item = this->stack_[--this->top_];
      return 1;
    }
  else
    return 0;
}

template <class T> ACE_INLINE int
ACE_Bounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::top");
  if (this->is_empty () == 0)
    {
      item = this->stack_[this->top_ - 1];
      return 1;
    }
  else
    return 0;
}

//----------------------------------------

template <class T, size_t SIZE> ACE_INLINE int 
ACE_Fixed_Stack<T, SIZE>::is_empty (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::is_empty");
  return this->top_ == 0;
}

template <class T, size_t SIZE> ACE_INLINE int 
ACE_Fixed_Stack<T, SIZE>::is_full (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::is_full");
  return this->top_ >= this->size_;
}

template <class T, size_t SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, SIZE>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::push");
  if (this->is_full () == 0)
    {
      this->stack_[this->top_++] = new_item;
      return 1;
    }
  else
    return 0;
}

template <class T, size_t SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, SIZE>::pop (T &item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::pop");
  if (this->is_empty () == 0)
    {
      item = this->stack_[--this->top_];
      return 1;
    }
  else
    return 0;
}

template <class T, size_t SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, SIZE>::top (T &item) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::top");
  if (this->is_empty () == 0)
    {
      item = this->stack_[this->top_ - 1];
      return 1;
    }
  else
    return 0;
}

//----------------------------------------

template <class T> ACE_INLINE int 
ACE_Unbounded_Stack<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::top");
  if (this->is_empty () == 0)
    {
      item = this->head_->next_->item_;
      return 1;
    }
  else
    return 0;
}

template <class T> ACE_INLINE int 
ACE_Unbounded_Stack<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_full");
  return 0; // ???
}

template <class TYPE> ACE_INLINE size_t
ACE_Unbounded_Queue<TYPE>::size (void) const
{
  return this->cur_size_;
}

// --- 

template <class T> ACE_INLINE int 
ACE_Unbounded_Queue<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int 
ACE_Unbounded_Queue<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Queue<T>::is_full");
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

template <class T, size_t SIZE> ACE_INLINE int 
ACE_Fixed_Set<T, SIZE>::is_empty (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T>::is_empty");
  return this->cur_size_ == 0;
}

template <class T, size_t SIZE> ACE_INLINE int 
ACE_Fixed_Set<T, SIZE>::is_full (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, SIZE>::is_full");
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
