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
      return 0;
    }
  else
    return -1;
}

template <class T, size_t SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, SIZE>::pop (T &item)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::pop");
  if (this->is_empty () == 0)
    {
      item = this->stack_[--this->top_];
      return 0;
    }
  else
    return -1;
}

template <class T, size_t SIZE> ACE_INLINE int
ACE_Fixed_Stack<T, SIZE>::top (T &item) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::top");
  if (this->is_empty () == 0)
    {
      item = this->stack_[this->top_ - 1];
      return 0;
    }
  else
    return -1;
}

template <class T, size_t SIZE> ACE_INLINE size_t
ACE_Fixed_Stack<T, SIZE>::size (void) const
{
  return this->size_;
}

//----------------------------------------
#if defined (NANBOR_EXP_CODES)
ACE_INLINE
ACE_DNode_Base::ACE_DNode_Base (void)
{
}

// ACE_INLINE
// ACE_DNode_Base::ACE_DNode_Base (ACE_DNode_Base *n, ACE_DNode_Base *p)
//   : next_ (n), prev_ (p)
// {
// }

ACE_INLINE
ACE_Double_Linked_List_Iterator_Base::ACE_Double_Linked_List_Iterator_Base
(ACE_Double_Linked_List_Base &dll)
  : dllist_ (dll)
{
  this->current_ = dll.head_->next_;  // Initialize head ptr.
}

ACE_INLINE ACE_DNode_Base *
ACE_Double_Linked_List_Iterator_Base::not_done (void)
{
  if (this->current_ != this->dllist_.head_)
    return this->current_;
  else
    return 0;
}

ACE_INLINE ACE_DNode_Base *
ACE_Double_Linked_List_Iterator_Base::do_advance (void)
{
  if (this->not_done ())
    {
      this->current_ = this->current_->next_;
      return this->not_done ();
    }
  else
    return 0;  
}

ACE_INLINE
ACE_Double_Linked_List_Base::ACE_Double_Linked_List_Base (void)
  : head_ (0), size_ (0)
{
}

ACE_INLINE void
ACE_Double_Linked_List_Base::init_head (void)
{
  this->head_->next_ = this->head_->prev_ = this->head_;
}
  
ACE_INLINE size_t
ACE_Double_Linked_List_Base::size (void)
{
  return this->size_;
}

ACE_INLINE int
ACE_Double_Linked_List_Base::insert_element (ACE_DNode_Base *new_item,
                                             int before,
                                             ACE_DNode_Base *old_item)
{
  if (old_item == 0)
    old_item = this->head_;
  if (before)
    old_item = old_item->prev_;

  (new_item->next_ = old_item->next_)->prev_ = new_item;
  (new_item->prev_ = old_item)->next_ = new_item;
  this->size_++;
  return 0;                     // Well, what will cause errors here?
}

ACE_INLINE int
ACE_Double_Linked_List_Base::remove_element (ACE_DNode_Base *item)
{
  // Notice that you have to ensure that item is an element of this
  // list.  We can't do much checking here.
  if (item == this->head_ || this->size () == 0)      // Can't remove head
    return -1;
  (item->prev_->next_ = item->next_)->prev_ = item->prev_;
  this->size_--;
  return 0;
}
  
ACE_INLINE ACE_Double_Linked_List_Iterator_Base *
ACE_Double_Linked_List_Base::iter (void)
{
  ACE_Double_Linked_List_Iterator_Base *itr;
  ACE_NEW_RETURN (itr, ACE_Double_Linked_List_Iterator_Base (*this), 0);
  return itr;
}

#endif /* NANBOR_EXP_CODES */
//----------------------------------------

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
