/* -*- C++ -*- */
// $Id$

// Memory_Pool.i

ACE_INLINE int
ACE_Local_Memory_Pool::sync (ssize_t, int)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int
ACE_Local_Memory_Pool::sync (void *, size_t, int)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int 
ACE_Local_Memory_Pool::protect (ssize_t, int)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::protect");
  return 0;
}

ACE_INLINE int 
ACE_Local_Memory_Pool::protect (void *, size_t, int)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::protect");
  return 0;
}

ACE_INLINE size_t
ACE_MMAP_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::round_up");
  return ACE::round_to_pagesize (nbytes);
}

// Ask system for initial chunk of local memory.

ACE_INLINE void *
ACE_Local_Memory_Pool::init_acquire (size_t nbytes, 
				     size_t &rounded_bytes, 
				     int &first_time)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::init_acquire");
  // Note that we assume that when ACE_Local_Memory_Pool is used,
  // ACE_Malloc's constructor will only get called once.  If this
  // assumption doesn't hold, we are in deep trouble!

  first_time = 1;
  return this->acquire (nbytes, rounded_bytes);
}

// Let the underlying new operator figure out the alignment...

ACE_INLINE size_t
ACE_Local_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::round_up");
  return ACE::round_to_pagesize (nbytes);
}

// No-op for now...

ACE_INLINE int
ACE_Local_Memory_Pool::release (void)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::release");
  return 0;
}

#if !defined (ACE_LACKS_SYSV_SHMEM)
// Implement the algorithm for rounding up the request to an
// appropriate chunksize.

ACE_INLINE size_t
ACE_Shared_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::round_up");
  if (nbytes < ACE_DEFAULT_SEGMENT_SIZE)
    nbytes = ACE_DEFAULT_SEGMENT_SIZE;

  return ACE::round_to_pagesize (nbytes);
}

ACE_INLINE int
ACE_Shared_Memory_Pool::sync (ssize_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int
ACE_Shared_Memory_Pool::sync (void *, size_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int 
ACE_Shared_Memory_Pool::protect (ssize_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::protect");
  return 0;
}

ACE_INLINE int 
ACE_Shared_Memory_Pool::protect (void *, size_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::protect");
  return 0;
}
#endif /* !ACE_LACKS_SYSV_SHMEM */

#if !defined (ACE_LACKS_SBRK)

// Ask system for initial chunk of local memory.

ACE_INLINE void *
ACE_Sbrk_Memory_Pool::init_acquire (size_t nbytes, 
				    size_t &rounded_bytes, 
				    int &first_time)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::init_acquire");
  // Note that we assume that when ACE_Sbrk_Memory_Pool is used,
  // ACE_Malloc's constructor will only get called once.  If this
  // assumption doesn't hold, we are in deep trouble!

  first_time = 1;
  return this->acquire (nbytes, rounded_bytes);
}

// Round up the request to a multiple of the page size.

ACE_INLINE size_t
ACE_Sbrk_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::round_up");
  return ACE::round_to_pagesize (nbytes);
}

/* No-op for now... */

ACE_INLINE int
ACE_Sbrk_Memory_Pool::release (void)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::release");
  return 0;
}

ACE_INLINE int
ACE_Sbrk_Memory_Pool::sync (ssize_t, int)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int
ACE_Sbrk_Memory_Pool::sync (void *, size_t, int)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::sync");
  return 0;
}

ACE_INLINE int 
ACE_Sbrk_Memory_Pool::protect (ssize_t, int)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::protect");
  return 0;
}

ACE_INLINE int 
ACE_Sbrk_Memory_Pool::protect (void *, size_t, int)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::protect");
  return 0;
}
#endif /* !ACE_LACKS_SBRK */
