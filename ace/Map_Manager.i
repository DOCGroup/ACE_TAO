/* -*- C++ -*- */
// $Id$

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class EXT_ID, class INT_ID> ACE_INLINE
ACE_Map_Entry<EXT_ID, INT_ID>::~ACE_Map_Entry (void)
{
  // No-op just to keep some compilers happy...
}
#endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

template <class EXT_ID, class INT_ID> ACE_INLINE size_t
ACE_Map_Entry<EXT_ID, INT_ID>::next (void) const
{
  return this->next_;
}

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_Map_Entry<EXT_ID, INT_ID>::next (size_t n)
{
  this->next_ = n;
}

template <class EXT_ID, class INT_ID> ACE_INLINE size_t
ACE_Map_Entry<EXT_ID, INT_ID>::prev (void) const
{
  return this->prev_;
}

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_Map_Entry<EXT_ID, INT_ID>::prev (size_t p)
{
  this->prev_ = p;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::ACE_Map_Manager (size_t size,
                                                            ACE_Allocator *alloc)
  : allocator_ (0),
    search_structure_ (0),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (size, alloc) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("ACE_Map_Manager\n")));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::ACE_Map_Manager (ACE_Allocator *alloc)
  : allocator_ (0),
    search_structure_ (0),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (ACE_DEFAULT_MAP_SIZE, alloc) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("ACE_Map_Manager\n")));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::close (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->close_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::~ACE_Map_Manager (void)
{
  this->close ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind (const EXT_ID &ext_id,
                                                 const INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->bind_i (ext_id,
                       int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind (const EXT_ID &ext_id,
                                                   const INT_ID &int_id,
                                                   EXT_ID &old_ext_id,
                                                   INT_ID &old_int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id,
                         int_id,
                         old_ext_id,
                         old_int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind (const EXT_ID &ext_id,
                                                   const INT_ID &int_id,
                                                   INT_ID &old_int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id,
                         int_id,
                         old_int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind (const EXT_ID &ext_id,
                                                   const INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id,
                         int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind (const EXT_ID &ext_id,
                                                    INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->trybind_i (ext_id,
                          int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find (const EXT_ID &ext_id)
{
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  size_t index = 0;
  return this->find_and_return_index (ext_id,
                                      index);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find (const EXT_ID &ext_id,
                                                 INT_ID &int_id)
{
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id,
                       int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_i (const EXT_ID &ext_id)
{
  // Unbind the entry.
  size_t index = 0;
  return this->unbind_and_return_index (ext_id,
                                        index);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind (const EXT_ID &ext_id,
                                                   INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (ext_id,
                         int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind (const EXT_ID &ext_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);
  return this->unbind_i (ext_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE size_t
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::current_size (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_,
                          ACE_static_cast(size_t, -1));
  return this->cur_size_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE size_t
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::total_size (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_,
                          ACE_static_cast (size_t, -1));
  return this->total_size_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE ACE_LOCK &
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::mutex (void)
{
  return this->lock_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE void
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::move_from_free_list_to_occupied_list (size_t index)
{
  this->shared_move (index,
                     this->free_list_,
                     this->free_list_id (),
                     this->occupied_list_,
                     this->occupied_list_id ());
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE void
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::move_from_occupied_list_to_free_list (size_t index)
{
  this->shared_move (index,
                     this->occupied_list_,
                     this->occupied_list_id (),
                     this->free_list_,
                     this->free_list_id ());
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::equal (const EXT_ID &id1,
                                                  const EXT_ID &id2)
{
  return id1 == id2;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE size_t
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::free_list_id (void) const
{
  // If you change ~0, please change
  // ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key()
  // accordingly.
  return ~0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE size_t
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::occupied_list_id (void) const
{
  return ~1;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::begin (void)
{
  return ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::end (void)
{
  return ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this, 1);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rbegin (void)
{
  return ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rend (void)
{
  return ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this, 1);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::ACE_Map_Iterator_Base (ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm)
  : map_man_ (mm),
    next_ (this->map_man_.occupied_list_id ())
{
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::next (ACE_Map_Entry<EXT_ID, INT_ID> *&mm) const
{
  if (this->next_ != this->map_man_.occupied_list_id ())
    {
      mm = &this->map_man_.search_structure_[this->next_];
      return 1;
    }
  else
    {
      return 0;
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::done (void) const
{
  return this->next_ == this->map_man_.occupied_list_id ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::forward_i (void)
{
  if (this->next_ == this->map_man_.occupied_list_id ())
    {
      this->next_ = this->map_man_.occupied_list_.next ();
    }
  else
    {
      this->next_ = this->map_man_.search_structure_[this->next_].next ();
    }

  return this->next_ != this->map_man_.occupied_list_id ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::reverse_i (void)
{
  if (this->next_ == this->map_man_.occupied_list_id ())
    {
      this->next_ = this->map_man_.occupied_list_.prev ();
    }
  else
    {
      this->next_ = this->map_man_.search_structure_[this->next_].prev ();
    }

  return this->next_ != this->map_man_.occupied_list_id ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>&
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::map (void)
{
  return this->map_man_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator== (const ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &rhs) const
{
  return (&this->map_man_ == &rhs.map_man_ &&
          this->next_ == rhs.next_);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator!= (const ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &rhs) const
{
  return !this->operator== (rhs);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Map_Iterator (ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                                                              int pass_end)
  : ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> (mm)
{
  if (!pass_end)
    this->forward_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance (void)
{
  return this->forward_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)
{
  this->forward_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)
{
  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->forward_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)
{
  this->reverse_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)
{
  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->reverse_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Map_Reverse_Iterator (ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                                                                              int pass_end)
  : ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> (mm)
{
  if (!pass_end)
    this->reverse_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE int
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance (void)
{
  return this->reverse_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)
{
  this->reverse_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)
{
  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->reverse_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)
{
  this->forward_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> ACE_INLINE
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)
{
  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->forward_i ();
  return retv;
}
