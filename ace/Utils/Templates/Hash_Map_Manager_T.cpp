// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Map_Manager_T.cpp
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_HASH_MAP_MANAGER_T_CPP
#define ACE_HASH_MAP_MANAGER_T_CPP

#include "ace/Utils/Templates/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
# include "ace/Utils/Templates/Hash_Map_Manager_T.i"
#elif defined (__SUNPRO_CC) && (__SUNPRO_CC == 0x420)
// If ACE_INLINE is on and we are on SunCC, undefine ACE_INLINE,
// include ace/Hash_Map_Manager_T.i, and then redefine ACE_INLINE.
// This nonsense is necessary since SunCC (version 4.2) cannot inline
// the code in ace/Hash_Map_Manager_T.i (with the fast option).
# undef ACE_INLINE
# define ACE_INLINE
# include "ace/Utils/Templates/Hash_Map_Manager_T.i"
# undef ACE_INLINE
# define ACE_INLINE inline
#endif /* __ACE_INLINE__ */

#include "ace/Threads/Synch.h"
#include "ace/Svcconf/Service_Config.h"
#include "ace/Memory/Malloc.h"

ACE_RCSID(ace, Hash_Map_Manager_T, "$Id$")

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_Entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev)
  : next_ (next),
    prev_ (prev)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_Entry (const EXT_ID &ext_id,
                                                        const INT_ID &int_id,
                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev)
  : ext_id_ (ext_id),
    int_id_ (int_id),
    next_ (next),
    prev_ (prev)
{
}

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_Entry (void)
{
}
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

#ifdef ACE_SUBSET_0
template <class EXT_ID, class INT_ID> void
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("next_ = %d"), this->next_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("prev_ = %d"), this->prev_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> void
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("total_size_ = %d"), this->total_size_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\ncur_size_ = %d"), this->cur_size_));
  this->allocator_->dump ();
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_SUBSET_0 */

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::create_buckets (size_t size)
{
  size_t bytes = size * sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>);
  void *ptr;

  ACE_ALLOCATOR_RETURN (ptr,
                        this->allocator_->malloc (bytes),
                        -1);

  this->table_ = (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *) ptr;

  this->total_size_ = size;

  // Initialize each entry in the hash table to be a circular linked
  // list with the dummy node in the front serving as the anchor of
  // the list.
  for (size_t i = 0; i < size; i++)
    new (&this->table_[i]) ACE_Hash_Map_Entry<EXT_ID, INT_ID> (&this->table_[i],
                                                               &this->table_[i]);
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::open (size_t size,
                                                                                 ACE_Allocator *alloc)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  // Calling this->close_i () to ensure we release previous allocated
  // memory before allocating new one.
  this->close_i ();

  if (alloc == 0)
    alloc = ACE_Allocator::instance ();

  this->allocator_ = alloc;

  // This assertion is here to help track a situation that shouldn't
  // happen, but did with Sun C++ 4.1 (before a change to this class
  // was made: it used to have an enum that was supposed to be defined
  // to be ACE_DEFAULT_MAP_SIZE, but instead was defined to be 0).
  ACE_ASSERT (size != 0);

  return this->create_buckets (size);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::close_i (void)
{
  // Protect against "double-deletion" in case the destructor also
  // gets called.
  if (this->table_ != 0)
    {
      // Remove all the entries.
      this->unbind_all_i ();

      // Iterate through the buckets cleaning up the sentinels.
      for (size_t i = 0; i < this->total_size_; i++)
        {
          // Destroy the dummy entry.
          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry = &this->table_[i];
          // The "if" second argument results in a no-op instead of
          // deallocation.
          ACE_DES_FREE_TEMPLATE2 (entry, ACE_NOOP,
                                  ACE_Hash_Map_Entry, EXT_ID, INT_ID);
        }

      // Reset size.
      this->total_size_ = 0;

      // Free table memory.
      this->allocator_->free (this->table_);

      // Should be done last...
      this->table_ = 0;
    }

  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::unbind_all_i (void)
{
  // Iterate through the entire map calling the destuctor of each
  // <ACE_Hash_Map_Entry>.
  for (size_t i = 0; i < this->total_size_; i++)
    {
      for (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp_ptr = this->table_[i].next_;
           temp_ptr != &this->table_[i];
           )
        {
          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *hold_ptr = temp_ptr;
          temp_ptr = temp_ptr->next_;

          // Explicitly call the destructor.
          ACE_DES_FREE_TEMPLATE2 (hold_ptr, this->allocator_->free,
                                  ACE_Hash_Map_Entry, EXT_ID, INT_ID);
        }

      // Restore the sentinel.
      this->table_[i].next_ = &this->table_[i];
      this->table_[i].prev_ = &this->table_[i];
    }

  this->cur_size_ = 0;

  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::bind_i (const EXT_ID &ext_id,
                                                                                   const INT_ID &int_id,
                                                                                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long loc;
  int result = this->shared_find (ext_id, entry, loc);

  if (result == -1)
    {
      void *ptr;
      // Not found.
      ACE_ALLOCATOR_RETURN (ptr,
                            this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)),
                            -1);

      entry = new (ptr) ACE_Hash_Map_Entry<EXT_ID, INT_ID> (ext_id,
                                                            int_id,
                                                            this->table_[loc].next_,
                                                            &this->table_[loc]);
      this->table_[loc].next_ = entry;
      entry->next_->prev_ = entry;
      this->cur_size_++;
      return 0;
    }
  else
    return 1;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::trybind_i (const EXT_ID &ext_id,
                                                                                      INT_ID &int_id,
                                                                                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long loc;
  int result = this->shared_find (ext_id, entry, loc);

  if (result == -1)
    {
      // Not found.
      void *ptr;
      ACE_ALLOCATOR_RETURN (ptr,
                            this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)),
                            -1);

      entry = new (ptr) ACE_Hash_Map_Entry<EXT_ID, INT_ID> (ext_id,
                                                            int_id,
                                                            this->table_[loc].next_,
                                                            &this->table_[loc]);
      this->table_[loc].next_ = entry;
      entry->next_->prev_ = entry;
      this->cur_size_++;
      return 0;
    }
  else
    return 1;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::unbind_i (const EXT_ID &ext_id,
                                                                                     INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;

  u_long loc;
  int result = this->shared_find (ext_id, temp, loc);

  if (result == -1)
    {
      errno = ENOENT;
      return -1;
    }

  int_id = temp->int_id_;

  return this->unbind_i (temp);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::unbind_i (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry)
{
  entry->next_->prev_ = entry->prev_;
  entry->prev_->next_ = entry->next_;

  // Explicitly call the destructor.
  ACE_DES_FREE_TEMPLATE2 (entry, this->allocator_->free,
                          ACE_Hash_Map_Entry, EXT_ID, INT_ID);
  this->cur_size_--;
  return 0;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::shared_find (const EXT_ID &ext_id,
                                                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                                                                                        u_long &loc)
{
  loc = this->hash (ext_id) % this->total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc].next_;

  while (temp != &this->table_[loc] && this->equal (temp->ext_id_, ext_id) == 0)
    temp = temp->next_;

  if (temp == &this->table_[loc])
    {
      errno = ENOENT;
      return -1;
    }
  else
    {
      entry = temp;
      return 0;
    }
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                                                     const INT_ID &int_id,
                                                                                     ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long dummy;
  if (this->shared_find (ext_id, entry, dummy) == -1)
    return this->bind_i (ext_id, int_id);
  else
    {
      entry->ext_id_ = ext_id;
      entry->int_id_ = int_id;
      return 1;
    }
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                                                     const INT_ID &int_id,
                                                                                     INT_ID &old_int_id,
                                                                                     ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long dummy;
  if (this->shared_find (ext_id, entry, dummy) == -1)
    return this->bind_i (ext_id, int_id);
  else
    {
      old_int_id = entry->int_id_;
      entry->ext_id_ = ext_id;
      entry->int_id_ = int_id;
      return 1;
    }
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                                                     const INT_ID &int_id,
                                                                                     EXT_ID &old_ext_id,
                                                                                     INT_ID &old_int_id,
                                                                                     ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long dummy;
  if (this->shared_find (ext_id, entry, dummy) == -1)
    return this->bind_i (ext_id, int_id);
  else
    {
      old_ext_id = entry->ext_id_;
      old_int_id = entry->int_id_;
      entry->ext_id_ = ext_id;
      entry->int_id_ = int_id;
      return 1;
    }
}

// ------------------------------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Map_Iterator_Base_Ex)

#ifdef ACE_SUBSET_0
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> void
ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::dump_i (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::dump_i");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("index_ = %d "), this->index_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("next_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif /* ACE_SUBSET_0 */

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::forward_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::forward_i");

  if (this->map_man_->table_ == 0)
    return -1;
  // Handle initial case specially.
  else if (this->index_ == -1)
    {
      this->index_++;
      return this->forward_i ();
    }
  else if (this->index_ >= ACE_static_cast (ssize_t, this->map_man_->total_size_))
    return 0;

  this->next_ = this->next_->next_;
  if (this->next_ == &this->map_man_->table_[this->index_])
    {
      while (++this->index_ < ACE_static_cast (ssize_t,
                                               this->map_man_->total_size_))
        {
          this->next_ = this->map_man_->table_[this->index_].next_;
          if (this->next_ != &this->map_man_->table_[this->index_])
            break;
        }
    }

  return this->index_ < ACE_static_cast (ssize_t, this->map_man_->total_size_);
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::reverse_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::reverse_i");

  if (this->map_man_->table_ == 0)
    return -1;
  else if (this->index_ == ACE_static_cast (ssize_t, this->map_man_->total_size_))
    {
      this->index_--;
      return this->reverse_i ();
    }
  else if (this->index_ < 0)
    return 0;

  this->next_ = this->next_->prev_;
  if (this->next_ == &this->map_man_->table_[this->index_])
    {
      while (--this->index_ >= 0)
        {
          this->next_ = this->map_man_->table_[this->index_].prev_;
          if (this->next_ != &this->map_man_->table_[this->index_])
            break;
        }
    }

  return this->index_ >= 0;
}

// ------------------------------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Map_Const_Iterator_Base_Ex)

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> void
ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::dump_i (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::dump_i");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("index_ = %d "), this->index_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("next_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::forward_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::forward_i");

  if (this->map_man_->table_ == 0)
    return -1;
  // Handle initial case specially.
  else if (this->index_ == -1)
    {
      this->index_++;
      return this->forward_i ();
    }
  else if (this->index_ >= (ssize_t) this->map_man_->total_size_)
    return 0;

  this->next_ = this->next_->next_;
  if (this->next_ == &this->map_man_->table_[this->index_])
    {
      while (++this->index_ < (ssize_t) this->map_man_->total_size_)
        {
          this->next_ = this->map_man_->table_[this->index_].next_;
          if (this->next_ != &this->map_man_->table_[this->index_])
            break;
        }
    }

  return this->index_ < (ssize_t) this->map_man_->total_size_;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::reverse_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::reverse_i");

  if (this->map_man_->table_ == 0)
    return -1;
  else if (this->index_ == (ssize_t) this->map_man_->total_size_)
    {
      this->index_--;
      return this->reverse_i ();
    }
  else if (this->index_ < 0)
    return 0;

  this->next_ = this->next_->prev_;
  if (this->next_ == &this->map_man_->table_[this->index_])
    {
      while (--this->index_ >= 0)
        {
          this->next_ = this->map_man_->table_[this->index_].prev_;
          if (this->next_ != &this->map_man_->table_[this->index_])
            break;
        }
    }

  return this->index_ >= 0;
}

#endif /* ACE_HASH_MAP_MANAGER_T_CPP */
