// Malloc_T.cpp
// $Id$

#ifndef ACE_MALLOC_T_C
#define ACE_MALLOC_T_C

#define ACE_BUILD_DLL
#include "ace/Malloc_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Malloc_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Malloc_T, "$Id$")

template <class T, class ACE_LOCK>
ACE_Cached_Allocator<T, ACE_LOCK>::ACE_Cached_Allocator (size_t n_chunks)
  : pool_ (0),
    free_list_ (ACE_PURE_FREE_LIST)
{
  ACE_NEW (this->pool_,
           char[n_chunks * sizeof (T)]);

  for (size_t c = 0;
       c < n_chunks;
       c++)
    {
      void* placement = this->pool_ + c * sizeof(T);
      this->free_list_.add (new (placement) ACE_Cached_Mem_Pool_Node<T>);
    }
  // Put into free list using placement contructor, no real memory
  // allocation in the above <new>.
}

template <class T, class ACE_LOCK>
ACE_Cached_Allocator<T, ACE_LOCK>::~ACE_Cached_Allocator (void)
{
  delete [] this->pool_;
}

ACE_ALLOC_HOOK_DEFINE (ACE_Malloc)

template <class MALLOC>
ACE_Allocator_Adapter<MALLOC>::ACE_Allocator_Adapter (LPCTSTR pool_name)
  : allocator_ (pool_name)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::ACE_Allocator_Adapter");
}

template <class MALLOC>
ACE_Allocator_Adapter<MALLOC>::~ACE_Allocator_Adapter (void)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::~ACE_Allocator_Adapter");
}

#if defined (ACE_HAS_MALLOC_STATS)
template <class MALLOC> void
ACE_Allocator_Adapter<MALLOC>::print_stats (void) const
{
  ACE_TRACE ("ACE_Malloc<MALLOC>::print_stats");
  this->allocator_.print_stats ();
}
#endif /* ACE_HAS_MALLOC_STATS */

template <class MALLOC> void
ACE_Allocator_Adapter<MALLOC>::dump (void) const
{
  ACE_TRACE ("ACE_Malloc<MALLOC>::dump");
  this->allocator_.dump ();
}

template <ACE_MEM_POOL_1, class ACE_LOCK> void
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->memory_pool_.dump ();
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("cb_ptr_ = %x"), this->cb_ptr_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
#if defined (ACE_HAS_MALLOC_STATS)
  if (this->cb_ptr_ != 0)
    this->cb_ptr_->malloc_stats_.dump ();
#endif /* ACE_HAS_MALLOC_STATS */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_MALLOC_STATS)

template <ACE_MEM_POOL_1, class ACE_LOCK> void
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::print_stats (void) const
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::print_stats");
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  if (this->cb_ptr_ == 0)
    return;
  this->cb_ptr_->malloc_stats_.dump ();
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) contents of freelist:\n")));

  for (ACE_Malloc_Header *currp = this->cb_ptr_->freep_->s_.next_block_;
       ;
       currp = currp->s_.next_block_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) ptr = %u, ACE_Malloc_Header units = %d, byte units = %d\n"),
                  currp,
                  currp->s_.size_,
                  currp->s_.size_ * sizeof (ACE_Malloc_Header)));
      if (currp == this->cb_ptr_->freep_)
        break;
    }
}
#endif /* ACE_HAS_MALLOC_STATS */

// Put <ptr> in the free list (locked version).

template<ACE_MEM_POOL_1, class ACE_LOCK> void
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::free (void *ptr)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::free");
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->shared_free (ptr);
}

// This function is called by the ACE_Malloc constructor to initialize
// the memory pool.  The first time in it allocates room for the
// control block (as well as a chunk of memory, depending on
// rounding...).  Depending on the type of <MEM_POOL> (i.e., shared
// vs. local) subsequent calls from other processes will only
// initialize the control block pointer.

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::open (void)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::open");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  size_t rounded_bytes = 0;
  int first_time = 0;

  this->cb_ptr_ = (ACE_Control_Block *)
    this->memory_pool_.init_acquire (sizeof *this->cb_ptr_,
                                     rounded_bytes,
                                     first_time);
  if (this->cb_ptr_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("init_acquire failed")),
                      -1);
  else if (first_time)
    {
      // ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) first time in, control block = %u\n"), this->cb_ptr_));

#if defined (ACE_HAS_MALLOC_STATS)
      // Call the constructor on the ACE_LOCK, using the placement
      // operator!
      new ((void *) &this->cb_ptr_->malloc_stats_) ACE_Malloc_Stats;
#endif /* ACE_HAS_MALLOC_STATS */

#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
      // Initialize the freelist pointer to point to the dummy
      // <ACE_Malloc_Header>.
      new ((void *) &this->cb_ptr_->freep_) ACE_MALLOC_HEADER_PTR (&this->cb_ptr_->base_);
      new ((void *) &this->cb_ptr_->name_head_) ACE_NAME_NODE_PTR;
#else
      // Initialize the freelist pointer to point to the dummy
      // <ACE_Malloc_Header>.
      this->cb_ptr_->freep_ = &this->cb_ptr_->base_;
      // initialize the name list to 0
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */

      this->cb_ptr_->name_head_ = (ACE_Name_Node *) 0;

      // Initialize the dummy <ACE_Malloc_Header> to point to itself.
      this->cb_ptr_->freep_->s_.size_ = 0;
      this->cb_ptr_->freep_->s_.next_block_ = this->cb_ptr_->freep_;

      if (rounded_bytes > (sizeof *this->cb_ptr_ + sizeof (ACE_Malloc_Header)))
        {
          // If we've got any extra space at the end of the control
          // block, then skip past the dummy ACE_Malloc_Header to
          // point at the first free block.
          ACE_Malloc_Header *p = ((ACE_Malloc_Header *) (this->cb_ptr_->freep_)) + 1;

          // Why aC++ in 64-bit mode can't grok this, I have no
          // idea... but it ends up with an extra bit set which makes
          // size_ really big without this hack.
#if defined (__hpux) && defined (__LP64__)
          size_t hpux11_hack = (rounded_bytes - sizeof *this->cb_ptr_)
                               / sizeof (ACE_Malloc_Header);
          p->s_.size_ = hpux11_hack;
#else
          p->s_.size_ = (rounded_bytes - sizeof *this->cb_ptr_)
            / sizeof (ACE_Malloc_Header);
#endif /* (__hpux) && defined (__LP64__) */

          AMS (++this->cb_ptr_->malloc_stats_.nchunks_);
          AMS (++this->cb_ptr_->malloc_stats_.nblocks_);
          AMS (++this->cb_ptr_->malloc_stats_.ninuse_);

          // Insert the newly allocated chunk of memory into the free
          // list.  Skip over the ACE_Malloc_Header when returning
          // pointer.
          this->shared_free (p + 1);
        }
    }
  return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc (LPCTSTR pool_name)
  : memory_pool_ (pool_name),
    lock_ (pool_name == 0 ? 0 : ACE::basename (pool_name,
                                               ACE_DIRECTORY_SEPARATOR_CHAR))
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc");
  if (this->open () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc")));
}

template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc (LPCTSTR pool_name,
                                                  LPCTSTR lock_name,
                                                  const ACE_MEM_POOL_OPTIONS *options)
  : memory_pool_ (pool_name, options),
    lock_ (lock_name != 0 ? lock_name : ACE::basename (pool_name,
                                                       ACE_DIRECTORY_SEPARATOR_CHAR))
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc");
  if (this->open () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p"),
                ASYS_TEXT ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc")));
}

#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc (LPCTSTR pool_name,
                                                  LPCTSTR lock_name,
                                                  const void *options)
  : memory_pool_ (pool_name,
                  (const ACE_MEM_POOL_OPTIONS *) options),
    lock_ (lock_name)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc");
  if (this->open () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p"),
                ASYS_TEXT ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc")));
}
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */


template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::~ACE_Malloc (void)
{
  ACE_TRACE ("ACE_Malloc<MEM_POOL>::~ACE_Malloc<MEM_POOL>");
}

// Clean up the resources allocated by ACE_Malloc.

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::remove (void)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::remove");
  // ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) destroying ACE_Malloc\n")));
  int result = 0;

#if defined (ACE_HAS_MALLOC_STATS)
  this->print_stats ();
#endif /* ACE_HAS_MALLOC_STATS */

  // Remove the ACE_LOCK.
  this->lock_.remove ();

  // Give the memory pool a chance to release its resources.
  result = this->memory_pool_.release ();

  return result;
}

// General-purpose memory allocator.  Assumes caller holds the locks.

template <ACE_MEM_POOL_1, class ACE_LOCK> void *
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_malloc (size_t nbytes)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_malloc");

  if (this->cb_ptr_ == 0)
    return 0;

  // Round up request to a multiple of the ACE_Malloc_Header size.
  size_t nunits =
    (nbytes + sizeof (ACE_Malloc_Header) - 1) / sizeof (ACE_Malloc_Header)
    + 1; // Add one for the <ACE_Malloc_Header> itself.

  // Begin the search starting at the place in the freelist where the
  // last block was found.
  ACE_Malloc_Header *prevp = this->cb_ptr_->freep_;
  ACE_Malloc_Header *currp = prevp->s_.next_block_;

  // Search the freelist to locate a block of the appropriate size.

  for (int i = 0;
       ; i++, prevp = currp, currp = currp->s_.next_block_)
    {
      if (currp->s_.size_ >= nunits) // Big enough
        {
          AMS (++this->cb_ptr_->malloc_stats_.ninuse_);
          if (currp->s_.size_ == nunits)
            // Exact size, just update the pointers.
            prevp->s_.next_block_ = currp->s_.next_block_;
          else
            {
              // Remaining chunk is larger than requested block, so
              // allocate at tail end.
              AMS (++this->cb_ptr_->malloc_stats_.nblocks_);
              currp->s_.size_ -= nunits;
              currp += currp->s_.size_;
              currp->s_.size_ = nunits;
            }
          this->cb_ptr_->freep_ = prevp;

          // Skip over the ACE_Malloc_Header when returning pointer.
          return currp + 1;
        }
      else if (currp == this->cb_ptr_->freep_)
        {
          // We've wrapped around freelist without finding a block.
          // Therefore, we need to ask the memory pool for a new chunk
          // of bytes.

          size_t chunk_bytes = 0;

          currp = (ACE_Malloc_Header *)
            this->memory_pool_.acquire (nunits * sizeof (ACE_Malloc_Header),
                                        chunk_bytes);

          if (currp != 0)
            {
              AMS (++this->cb_ptr_->malloc_stats_.nblocks_);
              AMS (++this->cb_ptr_->malloc_stats_.nchunks_);
              AMS (++this->cb_ptr_->malloc_stats_.ninuse_);

              // Compute the chunk size in ACE_Malloc_Header units.
              currp->s_.size_ = chunk_bytes / sizeof (ACE_Malloc_Header);

              // Insert the newly allocated chunk of memory into the
              // free list.  Skip over the ACE_Malloc_Header when
              // returning pointer.
              this->shared_free (currp + 1);
              currp = this->cb_ptr_->freep_;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("(%P|%t) %p\n"),
                               ASYS_TEXT ("malloc")),
                              0);
        }
    }
}

// General-purpose memory allocator.

template <ACE_MEM_POOL_1, class ACE_LOCK> void *
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::malloc (size_t nbytes)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::malloc");
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

  return this->shared_malloc (nbytes);
}

// General-purpose memory allocator.

template <ACE_MEM_POOL_1, class ACE_LOCK> void *
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::calloc (size_t nbytes,
                                              char initial_value)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::calloc");
  void *ptr = this->malloc (nbytes);

  if (ptr != 0)
    ACE_OS::memset (ptr, initial_value, nbytes);

  return ptr;
}

// Put block AP in the free list (must be called with locks held!)

template <ACE_MEM_POOL_1, class ACE_LOCK> void
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_free (void *ap)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_free");

  if (ap == 0 || this->cb_ptr_ == 0)
    return;

  // Adjust AP to point to the block ACE_Malloc_Header
  ACE_Malloc_Header *blockp = ((ACE_Malloc_Header *) ap) - 1;
  ACE_Malloc_Header *currp = this->cb_ptr_->freep_;

  // Search until we find the location where the blocks belongs.  Note
  // that addresses are kept in sorted order.

  for (;
       blockp <= currp 
         || blockp >= (ACE_Malloc_Header *) currp->s_.next_block_;
       currp = currp->s_.next_block_)
    {
      if (currp >= (ACE_Malloc_Header *) currp->s_.next_block_
          && (blockp > currp 
              || blockp < (ACE_Malloc_Header *) currp->s_.next_block_))
        // Freed block at the start or the end of the memory pool.
        break;
    }

  // Join to upper neighbor.
  if ((blockp + blockp->s_.size_) == currp->s_.next_block_)
    {
      AMS (--this->cb_ptr_->malloc_stats_.nblocks_);
      blockp->s_.size_ += currp->s_.next_block_->s_.size_;
      blockp->s_.next_block_ = currp->s_.next_block_->s_.next_block_;
    }
  else
    blockp->s_.next_block_ = currp->s_.next_block_;

  // Join to lower neighbor.
  if ((currp + currp->s_.size_) == blockp)
    {
      AMS (--this->cb_ptr_->malloc_stats_.nblocks_);
      currp->s_.size_ += blockp->s_.size_;
      currp->s_.next_block_ = blockp->s_.next_block_;
    }
  else
    currp->s_.next_block_ = blockp;

  AMS (--this->cb_ptr_->malloc_stats_.ninuse_);
  this->cb_ptr_->freep_ = currp;
}

// No locks held here, caller must acquire/release lock.

template <ACE_MEM_POOL_1, class ACE_LOCK> ACE_Name_Node *
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_find (const char *name)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_find");

  if (this->cb_ptr_ == 0)
    return 0;

  for (ACE_Name_Node *node = this->cb_ptr_->name_head_;
       node != 0;
       node = node->next_)
    if (ACE_OS::strcmp (node->name (),
                        name) == 0)
      return node;

  return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::shared_bind (const char *name,
                                                   void *pointer)
{
  if (this->cb_ptr_ == 0)
    return -1;

  // Combine the two allocations into one to avoid overhead...
  ACE_Name_Node *new_node = 0;

  ACE_ALLOCATOR_RETURN (new_node,
                        (ACE_Name_Node *)
                        this->shared_malloc (sizeof (ACE_Name_Node) + ACE_OS::strlen (name) + 1),
                        -1);
  char *name_ptr = (char *) (new_node + 1);

  // Use operator placement new to insert <new_node> at the head of
  // the linked list of <ACE_Name_Node>s.
  ACE_Name_Node *result =
    new (new_node) ACE_Name_Node (name,
                                  name_ptr,
                                  ACE_reinterpret_cast (char *,
                                                        pointer),
                                  this->cb_ptr_->name_head_);
  this->cb_ptr_->name_head_ = result;
  return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::trybind (const char *name,
                                               void *&pointer)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::trybind");
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  ACE_Name_Node *node = this->shared_find (name);
  if (node == 0)
    // Didn't find it, so insert it.
    return this->shared_bind (name, pointer);
  else
    {
      // Found it, so return a copy of the current entry.
      pointer = (char *) node->pointer_;
      return 1;
    }
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::bind (const char *name,
                                            void *pointer,
                                            int duplicates)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::bind");
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  if (duplicates == 0 && this->shared_find (name) != 0)
    // If we're not allowing duplicates, then if the name is already
    // present, return 1.
    return 1;
  else
    // If we get this far, either we're allowing duplicates or we didn't
    // find the name yet.

    return this->shared_bind (name, pointer);
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::find (const char *name,
                                            void *&pointer)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::find");

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  ACE_Name_Node *node = this->shared_find (name);

  if (node == 0)
    return -1;
  else
    {
      pointer = (char *) node->pointer_;
      return 0;
    }
}

// Returns a count of the number of available chunks that can hold
// <size> byte allocations.  Function can be used to determine if you
// have reached a water mark. This implies a fixed amount of allocated
// memory.
//
// @param size - the chunk size of that you would like a count of
// @return function returns the number of chunks of the given size
//          that would fit in the currently allocated memory

template <ACE_MEM_POOL_1, class ACE_LOCK> ssize_t
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::avail_chunks (size_t size) const
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::avail_chunks");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, (ACE_LOCK &) this->lock_, -1);

  if (this->cb_ptr_ == 0)
    return -1;

  size_t count = 0;
  // Avoid dividing by 0...
  size = size == 0 ? 1 : size;

  for (ACE_Malloc_Header *currp = this->cb_ptr_->freep_->s_.next_block_;
       currp != this->cb_ptr_->freep_;
       currp = currp->s_.next_block_)
    // Calculate how many will fit in this block.
    if (currp->s_.size_ * sizeof (ACE_Malloc_Header) >= size)
      count += currp->s_.size_ * sizeof (ACE_Malloc_Header) / size;

  return count;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::find (const char *name)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::find");
  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  return this->shared_find (name) == 0 ? -1 : 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::unbind (const char *name, void *&pointer)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::unbind");
  ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

  if (this->cb_ptr_ == 0)
    return -1;

  ACE_Name_Node *prev = 0;

  for (ACE_Name_Node *curr = this->cb_ptr_->name_head_;
       curr != 0;
       curr = curr->next_)
    {
      if (ACE_OS::strcmp (curr->name (), name) == 0)
        {
          pointer = (char *) curr->pointer_;

          if (prev == 0)
            this->cb_ptr_->name_head_ = curr->next_;
          else
            prev->next_ = curr->next_;

          // This will free up both the node and the name due to our
          // clever trick in <bind>!
          this->shared_free (curr);
          return 0;
        }
      prev = curr;
    }

  // Didn't find it, so fail.
  return -1;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::unbind (const char *name)
{
  ACE_TRACE ("ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK>::unbind");
  void *temp = 0;
  return this->unbind (name, temp);
}


template <ACE_MEM_POOL_1, class ACE_LOCK> void
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->curr_->dump ();
  this->guard_.dump ();
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("name_ = %s"), this->name_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc_Iterator (ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK> &malloc,
                                                                    const char *name)
  : malloc_ (malloc),
    curr_ (0),
    guard_ (malloc_.lock_),
    name_ (name != 0 ? ACE_OS::strdup (name) : 0)
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::ACE_Malloc_Iterator");
  // Cheap trick to make code simple.
  // @@ Doug, this looks like trouble...
  ACE_Name_Node temp;
  this->curr_ = &temp;
  this->curr_->next_ = malloc_.cb_ptr_->name_head_;

  this->advance ();
}

template <ACE_MEM_POOL_1, class ACE_LOCK>
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::~ACE_Malloc_Iterator (void)
{
  ACE_OS::free ((void *) this->name_);
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::next (void *&next_entry,
                                                     const char *&name)
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::next");

  if (this->curr_ != 0)
    {
      next_entry = (char *) this->curr_->pointer_;
      name = this->curr_->name ();
      return 1;
    }
  else
    return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::next (void *&next_entry)
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::next");

  if (this->curr_ != 0)
    {
      next_entry = this->curr_->pointer_;
      return 1;
    }
  else
    return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::done (void) const
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::done");

  return this->curr_ == 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::advance (void)
{
  ACE_TRACE ("ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>::advance");

  this->curr_ = this->curr_->next_;

  if (this->name_ == 0)
    return this->curr_ != 0;

  while (this->curr_ != 0
         && ACE_OS::strcmp (this->name_,
                            this->curr_->name ()) != 0)
    this->curr_ = this->curr_->next_;

  return this->curr_ != 0;
}

#endif /* ACE_MALLOC_T_C */
