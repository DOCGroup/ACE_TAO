/* -*- C++ -*- */
// $Id$

// Containers.i

template <class T> ACE_INLINE void 
ACE_Bounded_Stack<T>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::push");
   this->stack_[this->top_++] = new_item;
}

template <class T> ACE_INLINE void
ACE_Bounded_Stack<T>::pop (T &item)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::pop");
  item = this->stack_[--this->top_];
}

template <class T> ACE_INLINE void
ACE_Bounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::top");
  item = this->stack_[this->top_ - 1];
}

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

//----------------------------------------

template <class T, size_t SIZE> ACE_INLINE void
ACE_Fixed_Stack<T, SIZE>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::push");
   this->stack_[this->top_++] = new_item;
}

template <class T, size_t SIZE> ACE_INLINE void
ACE_Fixed_Stack<T, SIZE>::pop (T &item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::pop");
  item = this->stack_[--this->top_];
}

template <class T, size_t SIZE> ACE_INLINE void
ACE_Fixed_Stack<T, SIZE>::top (T &item) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::top");
  item = this->stack_[this->top_ - 1];
}

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

//----------------------------------------

template <class T> ACE_INLINE void
ACE_Unbounded_Stack<T>::top (T &item) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::top");
  item = this->head_->item_;
}

template <class T> ACE_INLINE int 
ACE_Unbounded_Stack<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_empty");
  return this->head_ == 0;
}

template <class T> ACE_INLINE int 
ACE_Unbounded_Stack<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::is_full");
  return this->last_resort_ == 0;
}

template <class TYPE> ACE_INLINE size_t
ACE_Unbounded_Queue<TYPE>::size (void) const
{
  return this->cur_size_;
}
