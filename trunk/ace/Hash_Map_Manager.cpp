#include "ace/Service_Config.h"
#include "ace/Hash_Map_Manager.h"

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

  this->sentinel_ = (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *) this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>));

  if (this->sentinel_ == 0)
    {
      this->allocator_->free (this->table_);
      errno = ENOMEM;
      return -1;
    }
  else
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
						  ACE_Allocator *allocator)
{
  ACE_WRITE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1);

  if (allocator == 0)
    allocator = ACE_Service_Config::alloc ();

  this->allocator_ = allocator;

  // If we need to grow buffer, then remove the existing buffer.
  if (this->total_size_ < size)
    return this->resize_i (size);
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK>
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::ACE_Hash_Map_Manager (size_t size,
								  ACE_Allocator *allocator)
  : total_size_ (0),
    cur_size_ (0),
    allocator_ (allocator)
{
  if (this->open (size, allocator) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Hash_Map_Manager\n"));
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::close_i (void)
{
  if (this->table_ != 0)
    {
      for (size_t i = 0; i < this->total_size_; i++)
	{
	  for (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp_ptr = this->table_[i]; 
	       temp_ptr != sentinel_;)
	    {
	      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *hold_ptr = temp_ptr;
	      temp_ptr = temp_ptr->next_;
	      this->allocator_->free (hold_ptr);
	    }
	}   

      this->allocator_->free (this->table_);
      this->table_ = 0;
      this->allocator_->free (this->sentinel_);
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

template <class EXT_ID, class INT_ID, class LOCK> int 
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::bind_i (const EXT_ID &ext_id,
						    const INT_ID &int_id)
{
  size_t loc = ext_id.hash () % this->total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc];

  assert (temp != 0);

  for (this->sentinel_->ext_id_ = ext_id;
       temp->ext_id_ != ext_id;
       temp = temp->next_)
    continue;

  if (temp == this->sentinel_) 
    {
      void *ptr;
      // Not found.
      ACE_ALLOCATOR_RETURN (ptr,
			    this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)			    ),
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
  size_t loc = ext_id.hash () % this->total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc];

  assert (temp != 0);

  for (this->sentinel_->ext_id_ = ext_id;
       temp->ext_id_ != ext_id;
       temp = temp->next_)
    continue;

  if (temp == this->sentinel_) 
    {
      // Not found.
      void *ptr;
      // Not found.
      ACE_ALLOCATOR_RETURN (ptr,
			    this->allocator_->malloc (sizeof (ACE_Hash_Map_Entry<EXT_ID, INT_ID>)			    ),
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
  size_t loc = ext_id.hash () % this->total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc];
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0;

  for (this->sentinel_->ext_id_ = ext_id;
       temp->ext_id_ != ext_id;
       temp = temp->next_)
    prev = temp;

  if (temp == this->sentinel_) 
    {
      errno = ENOENT;
      return -1;
    }
  else if (prev == 0)
    this->table_[loc] = this->table_[loc]->next_;
  else
    prev->next_ = temp->next_;

  int_id = temp->int_id_;
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
							 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry)
{
  size_t loc = ext_id.hash () % total_size_;

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *temp = this->table_[loc];

  assert (temp != 0);

  for (this->sentinel_->ext_id_ = ext_id;
       temp->ext_id_ != ext_id;
       temp = temp->next_)
    continue;

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
ACE_Hash_Map_Manager<EXT_ID, INT_ID, LOCK>::find_i (const EXT_ID &ext_id,
						    INT_ID &int_id)
{
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry;
  
  if (this->shared_find (ext_id, entry) == -1)
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
  
  return this->shared_find (ext_id, entry);
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

  if (this->shared_find (ext_id, node) == -1)
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
    this->advance ();
}

template <class EXT_ID, class INT_ID, class LOCK>  int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&mm)
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->map_man_.lock_, -1);

  if (this->map_man_.table_ != 0 
      && this->index_ < this->map_man_.total_size_
      && this->next_ != this->map_man_.sentinel_)
    {
      mm = this->next_;
      return 1;
    }
  else
    return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
ACE_Hash_Map_Iterator<EXT_ID, INT_ID, LOCK>::advance (void) 
{
  ACE_READ_GUARD_RETURN (LOCK, ace_mon, this->map_man_.lock_, -1);

  if (this->next_->next_ != this->map_man_.sentinel_)
    this->next_ = this->next_->next_;
  else
    while (this->index_++ < this->map_man_.total_size_)
      if (this->map_man_.table_[this->index_ - 1] != this->map_man_.sentinel_)
	{
	  this->next_ = this->map_man_.table_[this->index_ - 1];
	  break;
	}

  return 0;
}

