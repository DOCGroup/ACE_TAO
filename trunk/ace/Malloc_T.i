/* -*- C++ -*- */
// $Id$

// Malloc_T.i

template <class T, class LOCK> void *
ACE_Cached_Allocator<T, LOCK>::malloc (size_t nbytes)
{
  // Check if size requested fits within pre-determined size.
  if (nbytes > sizeof (T))
    return NULL;

  // addr() call is really not absolutely necessary because of the way
  // ACE_Cached_Mem_Pool_Node's internal structure arranged.
  return this->free_list_.remove ()->addr ();
}

template <class T, class LOCK> void
ACE_Cached_Allocator<T, LOCK>::free (void * ptr)
{
  this->free_list_.add ((ACE_Cached_Mem_Pool_Node<T> *) ptr) ;
}

template <class MALLOC> ACE_INLINE void *
ACE_Allocator_Adapter<MALLOC>::malloc (size_t nbytes)
{ 
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::malloc");
  return this->allocator_.malloc (nbytes);
}

template <class MALLOC> ACE_INLINE void *
ACE_Allocator_Adapter<MALLOC>::calloc (size_t nbytes, 
				       char initial_value)
{ 
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::malloc");
  return this->allocator_.calloc (nbytes, initial_value);
}

template <class MALLOC> ACE_INLINE MALLOC &
ACE_Allocator_Adapter<MALLOC>::alloc (void)
{ 
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::allocator");
  return this->allocator_;
}

template <class MALLOC> ACE_INLINE void
ACE_Allocator_Adapter<MALLOC>::free (void *ptr)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::free");
  this->allocator_.free (ptr);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::remove (void)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::remove");
  return this->allocator_.remove ();
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::trybind (const char *name, 
					void *&pointer)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::trybind");
  return this->allocator_.trybind (name, pointer);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::bind (const char *name, 
				     void *pointer,
				     int duplicates)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::bind");
  return this->allocator_.bind (name, pointer, duplicates);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::find (const char *name, 
				     void *&pointer)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::find");
  return this->allocator_.find (name, pointer);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::find (const char *name)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::find");
  return this->allocator_.find (name);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::unbind (const char *name, void *&pointer)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::unbind");
  return this->allocator_.unbind (name, pointer);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::unbind (const char *name)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::unbind");
  return this->allocator_.unbind (name);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::sync (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::sync");
  return this->allocator_.sync (len, flags);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::sync (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::sync");
  return this->allocator_.sync (addr, len, flags);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::protect (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::protect");
  return this->allocator_.protect (len, flags);
}

template <class MALLOC> ACE_INLINE int
ACE_Allocator_Adapter<MALLOC>::protect (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::protect");
  return this->allocator_.protect (addr, len, flags);
}

template <ACE_MEM_POOL_1, class LOCK> ACE_MEM_POOL &
ACE_Malloc<ACE_MEM_POOL_2, LOCK>::memory_pool (void)
{
  ACE_TRACE ("ACE_Malloc<MEMORY_POOL, LOCK>::memory_pool");
  return this->memory_pool_;
}

template <ACE_MEM_POOL_1, class LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, LOCK>::sync (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Malloc<MEMORY_POOL, LOCK>::sync");
  return this->memory_pool_.sync (len, flags);
}

template <ACE_MEM_POOL_1, class LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, LOCK>::sync (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_Malloc<MEMORY_POOL, LOCK>::sync");
  return this->memory_pool_.sync (addr, len, flags);
}

template <ACE_MEM_POOL_1, class LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, LOCK>::protect (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Malloc<MEMORY_POOL, LOCK>::protect");
  return this->memory_pool_.protect (len, flags);
}

template <ACE_MEM_POOL_1, class LOCK> int
ACE_Malloc<ACE_MEM_POOL_2, LOCK>::protect (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_Malloc<MEMORY_POOL, LOCK>::protect");
  return this->memory_pool_.protect (addr, len, flags);
}

