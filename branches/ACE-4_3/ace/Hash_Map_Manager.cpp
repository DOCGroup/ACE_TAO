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
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_Entry (void)
{
}

template <class EXT_ID, class INT_ID> void
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "next_ = %d", this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Map_Entry (const EXT_ID &ext_id,
							const INT_ID &int_id,
							ACE_Hash_Map_Entry<EXT_ID, INT_ID> *ptr)
  : ext_id_ (ext_id),
    int_id_ (int_id),
    next_ (ptr)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_Entry (void) 
{
}

template <class EXT_ID, class INT_ID, class LOCK> void
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "total_size_ = %d", this->total_size_));
  ACE_DEBUG ((LM_DEBUG, "\ncur_size_ = %d", this->cur_size_));
  this->allocator_->dump ();
  this->lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::resize_i (size_t size)
{
  size_t bytes = size * sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *);
  void *ptr;

  ACE_ALLOCATOR_RETURN (ptr,
			this->allocator_->malloc (bytes),
			-1);

  this->table_ = (ACE_Hash_Map_Entry<EXT_ID, INT_ID> **) ptr;

  ptr = this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>));
  if (ptr == 0)
    {
      this->allocator_->free (this->table_);
      this->table_ = 0;
      errno = ENOMEM;
      return -1;
    }

  this->sentinel_ = (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *) ptr;
  new (this->sentinel_) ACE_Hash_Map_Entry<EXT_ID, INT_ID>;

  // This isn't strictly necessary, but we'll do it to make life
  // easier.
  this->sentinel_->next_ = this->sentinel_;


  this->total_size_ = size;

  // Initialize the hash table to point to the sentinel node.
  for (size_t i = 0; i < this->total_size_; i++)
    this->table_[i] = this->sentinel_;

  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::open (size_t size,
						  ACE_Allocator *alloc)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  if (alloc == 0)
    alloc = ACE_Allocator::instance ();

  this->allocator_ = alloc;

  // This assertion is here to help track a situation that shouldn't happen,
  // but did with Sun C++ 4.1 (before a change to this class was made:
  // it used to have an enum that was supposed to be defined to be
  // ACE_DEFAULT_MAP_SIZE, but instead was defined to be 0.
  ACE_ASSERT (size != 0);

  // If we need to grow buffer, then remove the existing buffer.
  if (this->total_size_ < size)
    return this->resize_i (size);
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Hash_Map_Manager (size_t size,
								  ACE_Allocator *alloc)
  : allocator_ (alloc),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (size, alloc) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Hash_Map_Manager\n"));
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Hash_Map_Manager (ACE_Allocator *alloc)
  : allocator_ (alloc),
    total_size_ (0),
    cur_size_ (0)
{
  if (this->open (ACE_DEFAULT_MAP_SIZE, alloc) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Hash_Map_Manager\n"));
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::close_i (void)
{
  if (this->table_ != 0)
    {
      // Iterate through the entire map calling the destuctor of each
      // <ACE_Hash_Map_Entry>.  
      for (size_t i = 0; i < this->total_size_; i++)
	{
	  for (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp_ptr = this->table_[i]; 
	       temp_ptr != sentinel_;
	       )
	    {
	      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *hold_ptr = temp_ptr;
	      temp_ptr = temp_ptr->next_;
	      // Explicitly call the destructor.
	      hold_ptr->ACE_Hash_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_Entry ();
	      this->allocator_->free (hold_ptr);
	    }
	}   

      this->allocator_->free (this->table_);
      this->table_ = 0;

      this->sentinel_->ext_id_ = sentinel_ext_id_fakenull_;
      this->allocator_->free (this->sentinel_);
      this->sentinel_ = 0;
    }
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::close (void)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->close_i ();
}

template <class EXT_ID, class INT_ID, class LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::~ACE_Hash_Map_Manager (void) 
{
  this->close ();
}

template <class EXT_ID, class INT_ID, class LOCK> size_t
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::current_size (void)
{
  return this->cur_size_;
}

template <class EXT_ID, class INT_ID, class LOCK> size_t
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::total_size (void)
{
  return this->total_size_;
}

template <class EXT_ID, class INT_ID, class LOCK> u_long
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::hash (const EXT_ID &ext_id)
{
  return ext_id.hash ();
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::equal (const EXT_ID &id1,
						   const EXT_ID &id2)
{
  return id1 == id2;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::bind_i (const EXT_ID &ext_id,
						    const INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;

  u_long loc;
  int result = this->shared_find (ext_id, temp, loc);

  if (result == -1)
    {
      void *ptr;
      // Not found.
      ACE_ALLOCATOR_RETURN (ptr,
			    this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)),
			    -1);

      this->table_[loc] = 
	new (ptr) ACE_Hash_Map_Entry<EXT_ID, INT_ID> (ext_id, int_id, this->table_[loc]);

      this->cur_size_++;
      return 0;
    }
  else
    return 1;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::bind (const EXT_ID &ext_id,
						  const INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->bind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind_i (const EXT_ID &ext_id,
						       INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;

  u_long loc;
  int result = this->shared_find (ext_id, temp, loc);

  if (result == -1)
    {
      // Not found.
      void *ptr;
      ACE_ALLOCATOR_RETURN (ptr,
			    this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)),
			    -1);

      this->table_[loc] = 
	new (ptr) ACE_Hash_Map_Entry<EXT_ID, INT_ID> (ext_id, int_id, this->table_[loc]);

      this->cur_size_++;
      return 0;
    }
  else
    {
      temp->int_id_ = int_id;
      return 1;
    }
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::trybind (const EXT_ID &ext_id,
						     INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->trybind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i (const EXT_ID &ext_id,
						      INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp;
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0;

  u_long loc;
  int result = this->shared_find (ext_id, temp, prev, loc);

  if (result == -1) 
    {
      errno = ENOENT;
      return -1;
    }
  else if (prev == 0)
    this->table_[loc] = this->table_[loc]->next_;
  else
    prev->next_ = temp->next_;

  int_id = temp->int_id_;
  // Explicitly call the destructor.
  temp->ACE_Hash_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Map_Entry ();
  this->allocator_->free (temp);
  this->cur_size_--;
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind_i (const EXT_ID &ext_id)
{
  INT_ID int_id;
  
  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind (const EXT_ID &ext_id,
						    INT_ID &int_id)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::unbind (const EXT_ID &ext_id)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->unbind_i (ext_id) == -1 ? -1 : 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_find (const EXT_ID &ext_id,
							 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
							 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&prev,
                                                         u_long &loc)
{
  loc = this->hash (ext_id) % this->total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc];

  for (this->sentinel_->ext_id_ = ext_id;
       this->equal (temp->ext_id_, ext_id) == 0;
       temp = temp->next_)
    prev = temp;

  this->sentinel_->ext_id_ = sentinel_ext_id_fakenull_;

  if (temp != this->sentinel_)
    {
      entry = temp;
      return 0;
    }
  else
    {
      errno = ENOENT;
      return -1;
    }
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::shared_find (const EXT_ID &ext_id,
							 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                                                         u_long &loc)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev;
  return this->shared_find (ext_id, entry, prev, loc);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i (const EXT_ID &ext_id,
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

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i (const EXT_ID &ext_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry;

  u_long dummy;
  return this->shared_find (ext_id, entry, dummy);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::find (const EXT_ID &ext_id,
						  INT_ID &int_id)
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::find (const EXT_ID &ext_id)
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->find_i (ext_id);
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind_i (const EXT_ID &ext_id, 
						      const INT_ID &int_id,
						      EXT_ID &old_ext_id, 
						      INT_ID &old_int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *node;

  u_long dummy;
  if (this->shared_find (ext_id, node, dummy) == -1)
    return this->bind_i (ext_id, int_id);
  else
    {
      old_ext_id = node->ext_id_;
      old_int_id = node->int_id_;
      node->ext_id_ = ext_id;
      node->int_id_ = int_id;
      return 1;
    }
}

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::rebind (const EXT_ID &ext_id, 
						    const INT_ID &int_id,
						    EXT_ID &old_ext_id, 
						    INT_ID &old_int_id)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id);
}

template <class EXT_ID, class INT_ID, class LOCK> void
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::dump (void) const
{

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "next_ = %d", this->next_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class EXT_ID, class INT_ID, class LOCK> 
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::ACE_Hash_Map_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK> &mm)
  : map_man_ (mm), 
    index_ (0),
    next_ (this->map_man_.sentinel_)
{
  if (this->map_man_.table_ != 0)
    {
      this->next_ = this->map_man_.table_[0];
      this->advance ();
    }
}

template <class EXT_ID, class INT_ID, class LOCK>  int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->map_man_.lock_, -1);

  if (this->map_man_.table_ != 0 
      && this->index_ < this->map_man_.total_size_
      && this->next_ != this->map_man_.sentinel_)
    {
      entry = this->next_;
      return 1;
    }
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK>  int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::done (void) const
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->map_man_.lock_, -1);

  if (this->map_man_.table_ != 0 
      && this->index_ < this->map_man_.total_size_
      && this->next_ != this->map_man_.sentinel_)
    return 0;
  else
    return 1;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::advance (void) 
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->map_man_.lock_, -1);

  if (this->map_man_.table_ == 0)
    return -1;

  if (this->next_->next_ != this->map_man_.sentinel_)
    this->next_ = this->next_->next_;
  else
    while (++this->index_ < this->map_man_.total_size_)
      if (this->map_man_.table_[this->index_]
          != this->map_man_.sentinel_)
        {
          this->next_ = this->map_man_.table_[this->index_];
          break;
        }

  return this->index_ < this->map_man_.total_size_
    && this->next_ != this->map_man_.sentinel_;
}

#endif /* ACE_HASH_MAP_MANAGER_C */
