// Map_Manager.cpp
// $Id$

#if !defined (ACE_MAP_MANAGER_C)
#define ACE_MAP_MANAGER_C

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Malloc.h"
#include "ace/Service_Config.h"
#include "ace/Map_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Map_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Entry)

template <class EXT_ID, class INT_ID> void
ACE_Map_Entry<EXT_ID, INT_ID>::dump (void) const
{
  ACE_TRACE ("ACE_Map_Entry<EXT_ID, INT_ID>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "is_free_ = %d", this->is_free_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Manager)

template <class EXT_ID, class INT_ID, class LOCK> void
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "max_size_ = %d", this->max_size_));
  ACE_DEBUG ((LM_DEBUG, "\ncur_size_ = %d", this->cur_size_));
  this->allocator_->dump ();
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class LOCK>
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Map_Manager (size_t size,
                                                        ACE_Allocator *allocator)
  : search_structure_ (0),
    allocator_ (0),
    max_size_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Map_Manager");

  if (this->open (size, allocator) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Map_Manager\n"));
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Map_Manager (ACE_Allocator *allocator)
  : search_structure_ (0),
    allocator_ (0),
    max_size_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Map_Manager");
  if (this->open (DEFAULT_SIZE, allocator) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Map_Manager\n"));
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::close_i (void)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::close_i");

  if (this->search_structure_ != 0)
    {
      this->allocator_->free (this->search_structure_);
      this->search_structure_ = 0;
    }
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::close (void)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::close");
  ACE_Write_Guard<LOCK> m (this->lock_);

  return this->close_i ();
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::~ACE_Map_Manager (void)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::~ACE_Map_Manager");
  this->close ();
}

// Create a new search_structure of size SIZE.

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::open (size_t size,
					     ACE_Allocator *allocator)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::open");
  ACE_Write_Guard<LOCK> m (this->lock_);

  if (allocator == 0)
    allocator = ACE_Service_Config::allocator ();

  this->allocator_ = allocator;

  // If we need to grow buffer, then remove the existing buffer.
  if (this->max_size_ < size)
    return this->resize_i (size);
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::resize_i (size_t size)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::resize_i");

  // If we need to grow buffer, then remove the existing buffer.
  void *ptr = this->allocator_->malloc (size * sizeof (ACE_Map_Entry<EXT_ID, INT_ID>));

  if (ptr == 0)
    {
      errno = ENOMEM;
      return -1;
    }

  size_t i;
  
  ACE_Map_Entry<EXT_ID, INT_ID> *temp = (ACE_Map_Entry<EXT_ID, INT_ID> *) ptr;
  ACE_Map_Entry<EXT_ID, INT_ID> *foo;

  // Copy over the currently active elements.
  for (i = 0; i < this->cur_size_; i++)
    {
      temp[i] = this->search_structure_[i]; // Structure assignment.
    }
  
  this->max_size_ = size;
  
  // Mark the newly allocated elements as being "free".
  
  for (i = this->cur_size_; i < this->max_size_; i++)
    {
      // call the constructor for each element in the array
      foo = new (&(temp[i])) ACE_Map_Entry<EXT_ID, INT_ID>;
      temp[i].is_free_ = 1;
    }
  
  this->allocator_->free (this->search_structure_);
  
  this->search_structure_ = temp;
  return 0;  
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_find (const EXT_ID &ext_id,
                                                    int &first_free)
{
  // See if the entry is already there, keeping track of the first
  // free slot.

  for (size_t i = 0; i < this->cur_size_; i++)
    {
      ACE_Map_Entry<EXT_ID, INT_ID> &ss = this->search_structure_[i];

      if (ss.is_free_ == 0)
        {
          if (ss.ext_id_ == ext_id)
            return i;
        }
      else if (first_free == -1)
        first_free = int (i);
    }

  errno = ENOENT;
  return -1;
}

// Find the <int_id> corresponding to the <ext_id>.

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_find (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_find");

  for (size_t i = 0; i < this->cur_size_; i++)
    {
      const ACE_Map_Entry<EXT_ID, INT_ID> &ss 
        = this->search_structure_[i];

      if (ss.is_free_ == 0 && ss.ext_id_ == ext_id)
        // We found it!
        return i;
    }

  // not found
  errno = ENOENT;
  return -1;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_bind (const EXT_ID &ext_id,
                                                    const INT_ID &int_id,
                                                    int first_free)
{
  if (first_free > -1)   
    {
      // We found a free spot, let's reuse it.

      ACE_Map_Entry<EXT_ID, INT_ID> &ss = this->search_structure_[first_free];

      ss.ext_id_  = ext_id;
      ss.int_id_  = int_id;
      ss.is_free_ = 0;
      this->allocator_->sync ((void *) &this->search_structure_[first_free], sizeof ss);
      return 0;
    }

  // Check if we have reached max_size_
  else if (this->cur_size_ == this->max_size_)
    // We are out of room so grow the map
    if (this->resize_i (this->max_size_ + DEFAULT_SIZE) == -1)
      {
        // Out of memory
        errno = ENOMEM;
        return -1;
      }    

  // Insert at the end of the active portion. 
  
  ACE_Map_Entry<EXT_ID, INT_ID> &ss = this->search_structure_[this->cur_size_];
  
  ss.int_id_  = int_id;
  ss.ext_id_  = ext_id;
  ss.is_free_ = 0;
  this->allocator_->sync ((void *) &this->search_structure_[this->cur_size_], sizeof ss);
  this->cur_size_++;
  this->allocator_->sync ((void *) &this->cur_size_, sizeof this->cur_size_);
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind_i (const EXT_ID &ext_id,
						  INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind_i");
  int first_free = -1;
  int index = this->shared_find (ext_id, first_free);

  if (index >= 0)
    {
      // There was already something there, so make a copy, but
      // *don't* update anything in the map!

      int_id = this->search_structure_[index].int_id_; 
      return 1;
    }
  else
    // We didn't find it, so let's bind it!
    return this->shared_bind (ext_id, int_id, first_free);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind (const EXT_ID &ext_id,
                                                INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind");
  ACE_Write_Guard<LOCK> m (this->lock_);

  return this->trybind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i");
  return this->shared_find (ext_id);
}

// Find the INT_ID corresponding to the EXT_ID. 

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find");
  ACE_Read_Guard<LOCK> m (this->lock_);

  return this->find_i (ext_id);
}

// Unbind (remove) the EXT_ID from the map and return it via an out
// parameter.  Note that this method does *not* free up the INT_ID
// structure.  Thus, if there is dynamic memory associated with this,
// the caller is responsible for freeing this memory.

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i (const EXT_ID &ext_id,
						 INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i");

  ssize_t index = this->shared_unbind (ext_id);

  if (index == -1)
    return -1;
  else
    {
      int_id = this->search_structure_[index].int_id_;
      return 0;
    }
}

// Associate <ext_id> with <int_id>.  If <ext_id> is already in the 
// map then the <Map_Entry> is not changed.  Returns 0 if a new 
// entry is bound successfully, returns 1 if an attempt is made to 
// bind an existing entry, and returns -1 if failures occur.

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::bind_i (const EXT_ID &ext_id,
					       const INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::bind_i");

  int first_free = -1;
  int index = this->shared_find (ext_id, first_free);

  if (index >= 0)
    // It was already bound, so return 1.
    return 1;

  else
    // We didn't find it, so let's bind it!
    return this->shared_bind (ext_id, int_id, first_free);
}

// Associate <ext_id> with <int_id>.  If <ext_id> is not in the
// map then behaves just like <bind>.  Otherwise, store the old
// values of <ext_id> and <int_id> into the "out" parameters and
// rebind the new parameters.  This is very useful if you need to
// have an atomic way of updating <Map_Entries> and you also need
// full control over memory allocation.  Returns 0 if a new entry is
// bound successfully, returns 1 if an existing entry was rebound,
// and returns -1 if failures occur. 

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind_i (const EXT_ID &ext_id,
						 const INT_ID &int_id,
						 EXT_ID &old_ext_id, 
						 INT_ID &old_int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind_i");

  int first_free = -1;
  int index = this->shared_find (ext_id, first_free);

  if (index >= 0)
    {
      // We found it, so make copies of the old entries and rebind 
      // current entries.

      ACE_Map_Entry<EXT_ID, INT_ID> &ss = this->search_structure_[index];

      old_ext_id = ss.ext_id_;
      old_int_id = ss.int_id_;
      ss.ext_id_ = ext_id;
      ss.int_id_ = int_id;
      this->allocator_->sync ((void *) &this->search_structure_[index], sizeof ss);
      return 1;
    }
  else
    // We didn't find it, so let's bind it!
    return this->shared_bind (ext_id, int_id, first_free);
}

// Find the INT_ID corresponding to the EXT_ID. 

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i (const EXT_ID &ext_id,
					       INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i");

  int index = this->shared_find (ext_id);

  if (index == -1)
    // Didn't find it.
    return -1;
  else
    {
      // Found it, so assign a copy.
      int_id = this->search_structure_[index].int_id_;
      return 0;
    }
}

// Unbind (remove) the EXT_ID from the map.  Keeps track of where
// the EXT_ID was found so that this->unbind (EXT_ID, INT_ID)
// can return it to the caller.

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_unbind (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_unbind");
  for (size_t i = 0; i < this->cur_size_; i++)
    {
      ACE_Map_Entry<EXT_ID, INT_ID> &ss = this->search_structure_[i];

      if (ss.is_free_ == 0 && ss.ext_id_ == ext_id)
        {
          size_t index = i;

          // Mark this entry as being free.
          ss.is_free_ = 1;
          
          this->allocator_->sync ((void *) &ss.is_free_, 
                                  sizeof ss.is_free_);

          // If we just unbound the highest active entry, then we need
          // to figure out where the next highest active entry is.

          if (i + 1 == this->cur_size_)
            {
              while (i > 0 && this->search_structure_[--i].is_free_)
                continue;

              if (i == 0 && this->search_structure_[i].is_free_)
                this->cur_size_ = 0;
              else
                this->cur_size_ = i + 1;
              this->allocator_->sync ((void *) &this->cur_size_, 
                                      sizeof this->cur_size_);
            }
          return index;
        }
    }
  errno = ENOENT;
  return -1;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind (const EXT_ID &ext_id,
                                               INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind");
  ACE_Write_Guard<LOCK> m (this->lock_);

  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::bind (const EXT_ID &ext_id,
                                             const INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::bind");
  ACE_Write_Guard<LOCK> m (this->lock_);

  return this->bind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind (const EXT_ID &ext_id,
                                               const INT_ID &int_id,
                                               EXT_ID &old_ext_id, 
                                               INT_ID &old_int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind");
  ACE_Write_Guard<LOCK> m (this->lock_);

  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find (const EXT_ID &ext_id,
                                             INT_ID &int_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::find");
  ACE_Read_Guard<LOCK> m (this->lock_);

  return this->find_i (ext_id, int_id);
}

// Unbind (remove) the EXT_ID from the map.  Don't return the INT_ID
// to the caller (this is useful for collections where the INT_IDs are
// *not* dynamically allocated...)

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i");
  return this->shared_unbind (ext_id) == -1 ? -1 : 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind (const EXT_ID &ext_id)
{
  ACE_TRACE ("ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind");
  ACE_Write_Guard<LOCK> m (this->lock_);
  return this->unbind_i (ext_id) == -1 ? -1 : 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::current_size (void)
{
  ACE_TRACE ("ACE_Map_Manager::current_size");
  ACE_Write_Guard<LOCK> m (this->lock_);
  return this->cur_size_;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Manager<EXT_ID, INT_ID, LOCK>::total_size (void)
{
  ACE_TRACE ("ACE_Map_Manager::total_size");
  ACE_Write_Guard<LOCK> m (this->lock_);
  return this->max_size_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Iterator)

template <class EXT_ID, class INT_ID, class LOCK> void
ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "next_ = %d", this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::ACE_Map_Iterator (ACE_Map_Manager<EXT_ID, INT_ID, LOCK> &mm)
  : map_man_ (mm), 
    next_ (-1)
{
  ACE_TRACE ("ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::ACE_Map_Iterator");
  this->advance ();
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::next (ACE_Map_Entry<EXT_ID, INT_ID> *&mm) 
{
  ACE_TRACE ("ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::next");
  ACE_Read_Guard<LOCK> m (this->map_man_.lock_);

  // Note that this->next_ is never negative at this point...
  if (size_t (this->next_) < this->map_man_.cur_size_)
    {
      mm = &this->map_man_.search_structure_[this->next_];
      return 1;
    }
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::advance (void) 
{
  ACE_TRACE ("ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>::advance");
  ACE_Read_Guard<LOCK> m (this->map_man_.lock_);

  for (++this->next_;
       size_t (this->next_) < this->map_man_.cur_size_
       && this->map_man_.search_structure_[this->next_].is_free_;
       this->next_++)
    continue;
  return this->next_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Map_Reverse_Iterator)

template <class EXT_ID, class INT_ID, class LOCK> void
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "next_ = %d", this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::ACE_Map_Reverse_Iterator (ACE_Map_Manager<EXT_ID, INT_ID, LOCK> &mm)
  : map_man_ (mm), 
    next_ (this->map_man_.cur_size_)
{
  ACE_TRACE ("ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::ACE_Map_Reverse_Iterator");
  this->advance ();
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::next (ACE_Map_Entry<EXT_ID, INT_ID> *&mm) 
{
  ACE_TRACE ("ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::next");
  ACE_Read_Guard<LOCK> m (this->map_man_.lock_);

  if (this->next_ >= 0)
    {
      mm = &this->map_man_.search_structure_[this->next_];
      return 1;
    }
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::advance (void) 
{
  ACE_TRACE ("ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>::advance");
  ACE_Read_Guard<LOCK> m (this->map_man_.lock_);

  for (--this->next_;
       this->next_ >= 0
       && this->map_man_.search_structure_[this->next_].is_free_;
       this->next_--)
    continue;
  return this->next_;
}

#endif /* ACE_MAP_MANAGER_C */
