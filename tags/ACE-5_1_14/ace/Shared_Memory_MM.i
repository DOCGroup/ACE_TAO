/* -*- C++ -*- */
// $Id$

// Shared_Memory_MM.i

// Return the name of file that is mapped (if any).

ACE_INLINE const ACE_TCHAR *
ACE_Shared_Memory_MM::filename (void) const
{
  return this->shared_memory_.filename ();
}

ACE_INLINE int
ACE_Shared_Memory_MM::open (ACE_HANDLE handle,
                            int length,
                            int prot,
                            int share,
                            char *addr,
                            off_t pos)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::open");
  return shared_memory_.map (handle, length, prot, share, addr, pos);
}

ACE_INLINE int
ACE_Shared_Memory_MM::open (const ACE_TCHAR *file_name,
                            int len,
                            int flags,
                            int mode,
                            int prot,
                            int share,
                            char *addr,
                            off_t pos)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::open");
  return shared_memory_.map (file_name, len, flags, mode,
                             prot, share, addr, pos);
}

// The overall size of the segment.

ACE_INLINE int
ACE_Shared_Memory_MM::get_segment_size (void) const
{
  ACE_TRACE ("ACE_Shared_Memory_MM::get_segment_size");
  return this->shared_memory_.size ();
}

// Unmaps the shared memory segment.

ACE_INLINE int
ACE_Shared_Memory_MM::remove (void)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::remove");
  return shared_memory_.remove ();
}

// Closes (unmaps) the shared memory segment.

ACE_INLINE int
ACE_Shared_Memory_MM::close (void)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::close");
  return shared_memory_.unmap ();
}

ACE_INLINE void *
ACE_Shared_Memory_MM::malloc (size_t)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::malloc");
  void *addr;

  return this->shared_memory_ (addr) == -1 ? 0 : addr;
}

ACE_INLINE ACE_HANDLE
ACE_Shared_Memory_MM::get_id (void) const
{
  ACE_TRACE ("ACE_Shared_Memory_MM::get_id");
  return this->shared_memory_.handle ();
}

ACE_INLINE int
ACE_Shared_Memory_MM::free (void *p)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::free");
  return p != 0;
}
