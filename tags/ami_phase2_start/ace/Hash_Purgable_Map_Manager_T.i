// $Id$

template <class EXT_ID, class INT_ID> ACE_INLINE void 
ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>::set_purge_tag (u_long value)
{
  this->purge_tag_ = value;
}

template <class EXT_ID, class INT_ID> ACE_INLINE u_long
ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>::get_purge_tag (void)
{
  return this->purge_tag_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Purgable_Map_Manager_Ex (size_t size,
                                                                                                                      ACE_Allocator *alloc)
  : ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (size,
                                                                               alloc),
    timer_ (0)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::ACE_Hash_Purgable_Map_Manager_Ex (ACE_Allocator *alloc)
  : ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> (alloc),
    timer_ (0)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::~ACE_Hash_Purgable_Map_Manager_Ex (void)
{
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE int 
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>:: purge_i (int num)
{
  for (int i = 0; i < num; ++i)
    {
      if (this->purge () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failure in removing minimum entry from cache\n"),
                          -1);
    }
  
  return 0; 
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_INLINE int 
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>:: purge (int num)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->purge_i (num);
}
