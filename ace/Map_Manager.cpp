// $Id$

#ifndef ACE_MAP_MANAGER_C
#define ACE_MAP_MANAGER_C

#define ACE_BUILD_DLL
#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Map_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Map_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Map_Manager, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Entry)

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Manager)

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Iterator_Base)

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Iterator)

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Reverse_Iterator)

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::open (size_t size,
                                                 ACE_Allocator *alloc)
{
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  // Close old map (if any).
  this->close_i ();

  // Use the user specified allocator or the default singleton one.
  if (alloc == 0)
    alloc = ACE_Allocator::instance ();

  this->allocator_ = alloc;

  // This assertion is here to help track a situation that shouldn't
  // happen.
  ACE_ASSERT (size != 0);

  // Resize from 0 to <size>.  Note that this will also set up the
  // circular free list.
  return this->resize_i (size);
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::close_i (void)
{
  // Free entries.
  this->free_search_structure ();

  // Reset sizes.
  this->total_size_ = 0;
  this->cur_size_ = 0;

  // Reset circular free list.
  this->free_list_.next (this->free_list_id ());
  this->free_list_.prev (this->free_list_id ());

  // Reset circular occupied list.
  this->occupied_list_.next (this->occupied_list_id ());
  this->occupied_list_.prev (this->occupied_list_id ());

  return 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::bind_i (const EXT_ID &ext_id,
                                                   const INT_ID &int_id)
{
  // Try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // We found the key. Nothing to change.
      return 1;
    }
  else
    {
      // We didn't find the key.
      return this->shared_bind (ext_id,
                                int_id);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::next_free (size_t &free_slot)
{
  // Find an empty slot.
  free_slot = this->free_list_.next ();

  // Make sure we haven't run out of free slots.
  if (free_slot != this->free_list_id ())
    {
      return 0;
    }
  else
    {
      // Resize the map.
      int result = this->resize_i (this->new_size ());

      // Check for errors.
      if (result == 0)
        {
          // New free slot.
          free_slot = this->free_list_.next ();
        }

      return result;
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::shared_move (size_t index,
                                                        ACE_Map_Entry<EXT_ID, INT_ID> &current_list,
                                                        size_t current_list_id,
                                                        ACE_Map_Entry<EXT_ID, INT_ID> &new_list,
                                                        size_t new_list_id)
{
  // Grab the entry.
  ENTRY &entry = this->search_structure_[index];

  //
  // Remove from current list.
  //

  // Fix the entry before us.
  size_t current_list_prev = entry.prev ();
  if (current_list_prev == current_list_id)
    {
      current_list.next (entry.next ());
    }
  else
    {
      this->search_structure_[current_list_prev].next (entry.next ());
    }

  // Fix the entry after us.
  size_t current_list_next = entry.next ();
  if (current_list_next == current_list_id)
    {
      current_list.prev (entry.prev ());
    }
  else
    {
      this->search_structure_[current_list_next].prev (entry.prev ());
    }

  //
  // Add to new list.
  //

  // Fix us.
  size_t new_list_next = new_list.next ();
  entry.next (new_list_next);
  entry.prev (new_list_id);

  // Fix entry before us.
  new_list.next (index);

  // Fix entry after us.
  if (new_list_next == new_list_id)
    {
      new_list.prev (index);
    }
  else
    {
      this->search_structure_[new_list_next].prev (index);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::shared_bind (const EXT_ID &ext_id,
                                                        const INT_ID &int_id)
{
  // This function assumes that the find() has already been done, and
  // therefore, simply adds to the map.

  // Find an empty slot.
  size_t index = 0;
  int result = this->next_free (index);

  if (result == 0)
    {
      // Copy key and value.
      this->search_structure_[index].int_id_ = int_id;
      this->search_structure_[index].ext_id_ = ext_id;

      // Move from free list to occupied list
      this->move_from_free_list_to_occupied_list (index);

      // Update the current size.
      ++this->cur_size_;
    }

  return result;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                     const INT_ID &int_id,
                                                     EXT_ID &old_ext_id,
                                                     INT_ID &old_int_id)
{
  // First try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // We found it, so make copies of the old entries and rebind
      // current entries.
      ENTRY &ss = this->search_structure_[index];
      old_ext_id = ss.ext_id_;
      old_int_id = ss.int_id_;
      ss.ext_id_ = ext_id;
      ss.int_id_ = int_id;

      // Sync changed entry.
      this->allocator_->sync (&ss, sizeof ss);

      return 1;
    }
  else
    {
      // We didn't find it, so let's add it.
      return this->shared_bind (ext_id,
                                int_id);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                     const INT_ID &int_id,
                                                     INT_ID &old_int_id)
{
  // First try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // We found it, so make copies of the old entries and rebind
      // current entries.
      ENTRY &ss = this->search_structure_[index];
      old_int_id = ss.int_id_;
      ss.ext_id_ = ext_id;
      ss.int_id_ = int_id;

      // Sync changed entry.
      this->allocator_->sync (&ss, sizeof ss);

      return 1;
    }
  else
    {
      // We didn't find it, so let's add it.
      return this->shared_bind (ext_id,
                                int_id);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::rebind_i (const EXT_ID &ext_id,
                                                     const INT_ID &int_id)
{

  // First try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // We found it, so rebind current entries.
      ENTRY &ss = this->search_structure_[index];
      ss.ext_id_ = ext_id;
      ss.int_id_ = int_id;

      // Sync changed entry.
      this->allocator_->sync (&ss, sizeof ss);

      return 1;
    }
  else
    {
      // We didn't find it, so let's add it.
      return this->shared_bind (ext_id,
                                int_id);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::trybind_i (const EXT_ID &ext_id,
                                                      INT_ID &int_id)
{
  // Try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // Key was found.  Make a copy of value, but *don't* update
      // anything in the map!
      int_id = this->search_structure_[index].int_id_;
      return 1;
    }
  else
    {
      // We didn't find it, so let's bind it!
      return this->bind_i (ext_id,
                           int_id);
    }
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find_and_return_index (const EXT_ID &ext_id,
                                                                  size_t &index)
{
  // Go through the entire occupied list looking for the key.
  for (size_t i = this->occupied_list_.next ();
       i != this->occupied_list_id ();
       i = this->search_structure_[i].next ())
    {
      if (this->equal (this->search_structure_[i].ext_id_,
                       ext_id))
        {
          // If found, return index.
          index = i;
          return 0;
        }
    }

  // Key was not found.
  return -1;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::find_i (const EXT_ID &ext_id,
                                                   INT_ID &int_id)
{
  // Try to find the key.
  size_t index = 0;
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // Key was found.  Make a copy of value.
      int_id = this->search_structure_[index].int_id_;
    }

  return result;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_and_return_index (const EXT_ID &ext_id,
                                                                    size_t &index)
{
  // Try to find the key.
  int result = this->find_and_return_index (ext_id,
                                            index);

  if (result == 0)
    {
      // Move from occupied list to free list
      this->move_from_occupied_list_to_free_list (index);

      // Update the current size.
      --this->cur_size_;
    }

  return result;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::unbind_i (const EXT_ID &ext_id,
                                                     INT_ID &int_id)
{
  // Unbind the entry.
  size_t index = 0;
  int result = this->unbind_and_return_index (ext_id,
                                              index);

  if (result == 0)
    {
      // If found, copy the value.
      int_id = this->search_structure_[index].int_id_;
    }

  return result;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::resize_i (size_t new_size)
{
  size_t i;
  ENTRY *temp = 0;

  // Allocate new memory.
  ACE_ALLOCATOR_RETURN (temp,
                        (ENTRY *) this->allocator_->malloc (new_size * sizeof (ENTRY)),
                        -1);

  // Copy over the occupied entires.
  for (i = this->occupied_list_.next ();
       i != this->occupied_list_id ();
       i = this->search_structure_[i].next ())
    {
      // Copy constructor.
      new (&(temp[i])) ENTRY (this->search_structure_[i]);
    }

  // Copy over the free entires.
  for (i = this->free_list_.next ();
       i != this->free_list_id ();
       i = this->search_structure_[i].next ())
    {
      // Copy constructor.
      new (&(temp[i])) ENTRY (this->search_structure_[i]);
    }

  // Construct the new elements.
  for (i = this->total_size_; i < new_size; i++)
    {
      // Call the constructor for each element in the array.  Note
      // that this requires a default constructor for <EXT_ID> and
      // <INT_ID>.
      new (&(temp[i])) ENTRY;
      temp[i].next (i + 1);
      temp[i].prev (i - 1);
    }

  // Add new entries to the free list.
  this->free_list_.next (this->total_size_);
  this->free_list_.prev (new_size - 1);
  temp[new_size - 1].next (this->free_list_id ());
  temp[this->total_size_].prev (this->free_list_id ());

  // Remove/free old elements, update the new totoal size.
  this->free_search_structure ();
  this->total_size_ = new_size;

  // Start using new elements.
  this->search_structure_ = temp;

  return 0;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> size_t
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::new_size (void)
{
  // Calculate the new size.
  size_t current_size = this->total_size_;

  if (current_size < MAX_EXPONENTIAL)
    {
      // Exponentially increase if we haven't reached MAX_EXPONENTIAL.
      current_size *= 2;
    }
  else
    {
      // Linear increase if we have reached MAX_EXPONENTIAL.
      current_size += LINEAR_INCREASE;
    }

  // This should be the new size.
  return current_size;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::free_search_structure (void)
{
  // Free up the structure.
  if (this->search_structure_ != 0)
    {
      for (size_t i = 0; i < this->total_size_; i++)
        // Explicitly call the destructor.
        {
          ENTRY *ss = &this->search_structure_[i];
          // The "if" second argument results in a no-op instead of
          // deallocation.
          ACE_DES_FREE_TEMPLATE2 (ss, ACE_NOOP,
                                  ACE_Map_Entry, EXT_ID, INT_ID);
        }

      // Actually free the memory.
      this->allocator_->free (this->search_structure_);
      this->search_structure_ = 0;
    }
}

template <class EXT_ID, class INT_ID> void
ACE_Map_Entry<EXT_ID, INT_ID>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("next_ = %d"), this->next_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("prev_ = %d"), this->prev_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("total_size_ = %d"), this->total_size_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncur_size_ = %d"), this->cur_size_));
  this->allocator_->dump ();
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::dump_i (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("next_ = %d"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class ACE_LOCK>
ACE_Map_Entry<EXT_ID, INT_ID>&
ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>::operator* (void) const
{
  // @@ This function should be inlined.  We moved it here to avoid
  // a compiler bug in SunCC 4.2.  Once we know the correct patch to fix
  // the compiler problem, it should be moved back to .i file again.
  ACE_Map_Entry<EXT_ID, INT_ID> *retv = 0;

  int result = this->next (retv);
  ACE_ASSERT (result != 0);
  ACE_UNUSED_ARG (result);

  return *retv;
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  this->dump_i ();
}

template <class EXT_ID, class INT_ID, class ACE_LOCK> void
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>::dump (void) const
{
  this->dump_i ();
}

#endif /* ACE_MAP_MANAGER_C */
