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

// Clean up the array (e.g., delete dynamically allocated memory).

template <class T> ACE_INLINE
ACE_Array<T>::~ACE_Array (void)
{
   delete [] this->array_;
}

template <class T> ACE_INLINE size_t
ACE_Array<T>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE int
ACE_Array<T>::in_range (size_t index) const
{
  return index < this->cur_size_;
}

template <class T> ACE_INLINE T &
ACE_Array<T>::operator[] (size_t index)
{
  return this->array_[index];
}

template <class T> ACE_INLINE const T &
ACE_Array<T>::operator[] (size_t index) const
{
  return this->array_[index];
}

// Compare this array with <s> for inequality.

template <class T> ACE_INLINE int
ACE_Array<T>::operator!= (const ACE_Array<T> &s) const
{
  return !(*this == s);
}

template <class T> ACE_INLINE void
ACE_Array_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::dump");
}

template <class T> ACE_INLINE
ACE_Array_Iterator<T>::ACE_Array_Iterator (ACE_Array<T> &a)
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

#if !defined (ACE_LACKS_TEMPLATE_AS_TEMPLATE_PARAMETER)

template <class T> ACE_INLINE
ACE_DLList_Node<T>::ACE_DLList_Node (void)
  : item_ (0),
    next_ (0),
    prev_ (0)
{}

template <class T> ACE_INLINE
ACE_DLList_Node<T>::ACE_DLList_Node (T *&i,
				     ACE_DLList_Node<T> *n,
				     ACE_DLList_Node<T> *p)
  : item_ (i),
    next_ (n),
    prev_ (p)
{
}

template <class T> ACE_INLINE
ACE_DLList_Node<T>::~ACE_DLList_Node (void) 
{
}

template <class T> ACE_INLINE void
ACE_DLList<T>::operator= (ACE_DLList<T> &l) 
{
  *(DLList *) this = l; 
}

template <class T> ACE_INLINE int 
ACE_DLList<T>::is_empty (void) const
{
  return ACE_Double_Linked_List< ACE_DLList_Node<T> >::is_empty (); 
}

template <class T> ACE_INLINE int 
ACE_DLList<T>::is_full (void) const
{
  return ACE_Double_Linked_List< ACE_DLList_Node<T> >::is_full (); 
}

template <class T> ACE_INLINE void 
ACE_DLList<T>::reset (void)
{
  ACE_Double_Linked_List< ACE_DLList_Node<T> >::reset (); 
}

template <class T> ACE_INLINE int 
ACE_DLList<T>::get (T *&item, size_t index)
{
  DLList_NODE *node;
  int result
    = ACE_Double_Linked_List< ACE_DLList_Node<T> >::get (node,
                                                         index);
  item = node->item_;
  return result;
}

template <class T> ACE_INLINE size_t 
ACE_DLList<T>::size (void) const
{
  return ACE_Double_Linked_List< ACE_DLList_Node<T> >::size (); 
}

template <class T> ACE_INLINE void 
ACE_DLList<T>::dump (void) const
{ 
  ACE_Double_Linked_List< ACE_DLList_Node<T> >::dump (); 
}

template <class T> ACE_INLINE
ACE_DLList<T>::ACE_DLList (ACE_Allocator *alloc )
  : ACE_Double_Linked_List< ACE_DLList_Node<T> > (alloc) 
{
}

template <class T> ACE_INLINE
ACE_DLList<T>::ACE_DLList (ACE_DLList<T> &l)
  : ACE_Double_Linked_List< ACE_DLList_Node<T> > ((DLList &) l)
{
}

template <class T> ACE_INLINE
ACE_DLList<T>::~ACE_DLList (void) 
{ 
  while (this->delete_head ()) ; 
}

template <class T> ACE_INLINE
ACE_DLList_Iterator<T>::ACE_DLList_Iterator (ACE_DLList<T> &l)
  : ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> > (l) 
{
}

template <class T> ACE_INLINE T *
ACE_DLList_Iterator<T>::next (void) const
{
  ACE_DLList_Node<T> *temp
    = ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> >::next ();
  return temp ? temp->item_ : 0;
}

template <class T> ACE_INLINE int 
ACE_DLList_Iterator<T>::advance (void)
{
  return ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> >::advance (); 
}

template <class T> ACE_INLINE int 
ACE_DLList_Iterator<T>::first (void)
{
  return ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> >::first (); 
}

template <class T> ACE_INLINE int 
ACE_DLList_Iterator<T>::done (void) const
{
  return ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> >::done (); 
}

template <class T> ACE_INLINE void 
ACE_DLList_Iterator<T>::dump (void) const
{ 
  ACE_Double_Linked_List_Iterator< ACE_DLList_Node<T> >::dump (); 
}

#endif /* ! defined (ACE_LACKS_TEMPLATE_AS_TEMPLATE_PARAMETER) */
