// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Map_Manager.cpp
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#if !defined (ACE_HASH_MAP_MANAGER_C)
#define ACE_HASH_MAP_MANAGER_C

#include "ace/Hash_Map_Manager.h"
#include "ace/Service_Config.h"
#include "ace/Malloc.h"

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

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_Entry (void)
{
}

template <class EXT_ID, class INT_ID> void
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("next_ = %d"), this->next_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("prev_ = %d"), this->prev_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("total_size_ = %d"), this->total_size_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncur_size_ = %d"), this->cur_size_));
  this->allocator_->dump ();
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::create_buckets (size_t size)
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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::open (size_t size,
                                                      ACE_Allocator *alloc)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  if (alloc == 0)
    alloc = ACE_Allocator::instance ();

  this->allocator_ = alloc;

  // This assertion is here to help track a situation that shouldn't
  // happen, but did with Sun C++ 4.1 (before a change to this class
  // was made: it used to have an enum that was supposed to be defined
  // to be ACE_DEFAULT_MAP_SIZE, but instead was defined to be 0).
  ACE_ASSERT (size != 0);

  // Calling this->close_i () to ensure we release previous allocated
  // memory before allocating new one.
  this->close_i ();

  return this->create_buckets (size);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Manager (size_t size,
                                                                      ACE_Allocator *alloc)
  : allocator_ (alloc),
    table_ (0),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (size, alloc) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("ACE_Hash_Map_Manager\n")));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Manager (ACE_Allocator *alloc)
  : allocator_ (alloc),
    table_ (0),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (ACE_DEFAULT_MAP_SIZE, alloc) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("ACE_Hash_Map_Manager\n")));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::close_i (void)
{
  // Protect against double deletion in case the destructor also gets
  // called.
  if (this->table_ != 0)
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

          // Destroy the dummy entry.
          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry = &table_[i];
          // The "if" second argument results in a no-op instead of
          // deallocation.
          ACE_DES_FREE_TEMPLATE2 (entry, if,
                                  ACE_Hash_Map_Entry, EXT_ID, INT_ID);
        }

      // Free table memory.
      this->allocator_->free (this->table_);
      this->cur_size_ = 0;
      this->total_size_ = 0;
      // Should be done last...
      this->table_ = 0;
    }
  return 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::close (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->close_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::~ACE_Hash_Map_Manager (void)
{
  this->close ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> size_t
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::current_size (void)
{
  return this->cur_size_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> size_t
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::total_size (void)
{
  return this->total_size_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> u_long
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::hash (const EXT_ID &ext_id)
{
  return ext_id.hash ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::equal (const EXT_ID &id1,
                                                       const EXT_ID &id2)
{
  return id1 == id2;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind_i (const EXT_ID &ext_id,
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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind_i (const EXT_ID &ext_id,
                                                        const INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;

  return this->bind_i (ext_id, int_id, temp);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind (const EXT_ID &ext_id,
                                                      const INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->bind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind (const EXT_ID &ext_id,
                                                      const INT_ID &int_id,
                                                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->bind_i (ext_id, int_id, entry);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind_i (const EXT_ID &ext_id,
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
    {
      entry->int_id_ = int_id;
      return 1;
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind_i (const EXT_ID &ext_id,
                                                           INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;

  return this->trybind_i (ext_id, int_id, temp);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind (const EXT_ID &ext_id,
                                                         INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->trybind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind (const EXT_ID &ext_id,
                                                         INT_ID &int_id,
                                                         ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->trybind_i (ext_id, int_id, entry);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_i (const EXT_ID &ext_id,
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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_i (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry)
{
  entry->next_->prev_ = entry->prev_;
  entry->prev_->next_ = entry->next_;

  // Explicitly call the destructor.
  ACE_DES_FREE_TEMPLATE2 (entry, this->allocator_->free,
                          ACE_Hash_Map_Entry, EXT_ID, INT_ID);
  this->cur_size_--;
  return 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_i (const EXT_ID &ext_id)
{
  INT_ID int_id;

  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind (const EXT_ID &ext_id,
                                                        INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind (const EXT_ID &ext_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (ext_id) == -1 ? -1 : 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (entry) == -1 ? -1 : 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::shared_find (const EXT_ID &ext_id,
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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find_i (const EXT_ID &ext_id,
                                                        INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry;

  u_long dummy;
  if (this->shared_find (ext_id, entry, dummy) == -1)
    return -1;
  else
    {
      int_id = entry->int_id_;
      return 0;
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find_i (const EXT_ID &ext_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry;

  u_long dummy;
  return this->shared_find (ext_id, entry, dummy);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find (const EXT_ID &ext_id,
                                                      INT_ID &int_id)
{
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find (const EXT_ID &ext_id)
{
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find_i (const EXT_ID &ext_id,
                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  u_long dummy;
  return this->shared_find (ext_id, entry, dummy);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find (const EXT_ID &ext_id,
                                                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id, entry);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                          const INT_ID &int_id,
                                                          EXT_ID &old_ext_id,
                                                          INT_ID &old_int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *node;

  return this->rebind_i (ext_id,
                         int_id,
                         old_ext_id,
                         old_int_id,
                         node);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind (const EXT_ID &ext_id,
                                                        const INT_ID &int_id,
                                                        EXT_ID &old_ext_id,
                                                        INT_ID &old_int_id)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind (const EXT_ID &ext_id,
                                                        const INT_ID &int_id,
                                                        EXT_ID &old_ext_id,
                                                        INT_ID &old_int_id,
                                                        ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id, entry);
}

// ------------------------------------------------------------
template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::begin (void)
{
  return ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::end (void)
{
  return ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this, 1);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rbegin (void)
{
  return ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rend (void)
{
  return ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> (*this, 1);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Map_Iterator_Base)

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::dump_i (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("index_ = %d "), this->index_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("next_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Iterator_Base (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                                                                                  int head)
  : map_man_ (&mm),
    index_ (head != 0 ? -1 : ACE_static_cast (ssize_t, mm.total_size_)),
    next_ (0)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Iterator_Base");
  if (mm.table_ != 0)
    this->next_ = &mm.table_[head != 0 ? 0 : mm.total_size_ - 1];
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::next");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, -1);

  if (this->map_man_->table_ != 0
      && this->index_ < ACE_static_cast (ssize_t, this->map_man_->total_size_)
      && this->index_ >= 0
      && this->next_ != &this->map_man_->table_[this->index_])
    {
      entry = this->next_;
      return 1;
    }
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::done (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::done");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, -1);

  return this->map_man_->table_ == 0
    || this->index_ >= ACE_static_cast (ssize_t, this->map_man_->total_size_)
    || this->index_ < 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::forward_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::forward_i");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, -1);

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

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::reverse_i (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::reverse_i");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, -1);

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

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Entry<EXT_ID, INT_ID> &
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator* (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator*");
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *retv = 0;

  int result = this->next (retv);

  ACE_ASSERT (result != 0);
  return *retv;
}

// Returns the reference to the hash_map_manager that is being
// iterated over.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>&
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::map (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::map");
  return *this->map_man_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator== (const ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &rhs) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator==");
  return this->map_man_ == rhs.map_man_
    && this->index_ == rhs.index_
    && this->next_ == rhs.next_;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator!= (const ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &rhs) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator!=");
  return this->next_ != rhs.next_
    || this->index_ != rhs.index_
    || this->map_man_ != rhs.map_man_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Map_Iterator)

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump");

  this->dump_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                                                                        int tail)
  : ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> (mm,
                                                          tail == 0 ? 1 : 0)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Iterator");
  if (tail == 0)
    this->forward_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance");
  return this->forward_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->forward_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>&
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  this->forward_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->reverse_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>&
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)
{
  ACE_TRACE ("ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  this->reverse_i ();
  return *this;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Map_Reverse_Iterator)

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump");

  this->dump_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Reverse_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm, int head)
  : ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> (mm, head)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::ACE_Hash_Map_Reverse_Iterator");
  if (head == 0)
    this->reverse_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance (void)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::advance");
  return this->reverse_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (void)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->reverse_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator++ (int)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  this->reverse_i ();
  return *this;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (void)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> retv (*this);
  this->forward_i ();
  return retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)
{
  ACE_TRACE ("ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::operator-- (int)");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->map_man_->lock_, *this);

  this->forward_i ();
  return *this;
}

#endif /* ACE_HASH_MAP_MANAGER_C */
