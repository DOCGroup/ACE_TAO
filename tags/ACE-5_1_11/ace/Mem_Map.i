/* -*- C++ -*- */
// $Id$

ACE_INLINE ACE_HANDLE
ACE_Mem_Map::handle (void) const
{
  ACE_TRACE ("ACE_Mem_Map::handle");
  return this->handle_;
}

// Return the name of file that is mapped (if any).

ACE_INLINE const ACE_TCHAR *
ACE_Mem_Map::filename (void) const
{
  return this->filename_;
}

ACE_INLINE int
ACE_Mem_Map::map (ACE_HANDLE handle,
                  int len,
                  int prot,
                  int share,
                  void *addr,
                  off_t offset,
                  LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  return this->map_it (handle, len, prot, share, addr, offset, sa);
}

// Remap the file associated with <this->handle_>.

ACE_INLINE int
ACE_Mem_Map::map (int len,
                  int prot,
                  int share,
                  void *addr,
                  off_t offset,
                  LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  // If we're already mapped at a particular location then try to
  // remap the file using the same base address.
  if (addr == 0 && this->base_addr_ != 0 && this->base_addr_ != MAP_FAILED)
    {
      share |= MAP_FIXED;
      addr = this->base_addr_;
    }
  return this->map_it (this->handle (), len, prot,
                       share, addr, offset, sa);
}

// This operator passes back the starting address of the mapped file.

ACE_INLINE int
ACE_Mem_Map::operator () (void *&addr)
{
  ACE_TRACE ("ACE_Mem_Map::operator");

  if (this->base_addr_ == MAP_FAILED)
    return -1;
  else
    {
      addr = this->base_addr_;
      return 0;
    }
}

// Return the base address.

ACE_INLINE void *
ACE_Mem_Map::addr (void) const
{
  ACE_TRACE ("ACE_Mem_Map::addr");

  return this->base_addr_;
}

// This function returns the number of bytes currently mapped in the
// file.

ACE_INLINE size_t
ACE_Mem_Map::size (void) const
{
  ACE_TRACE ("ACE_Mem_Map::size");
  return this->length_;
}

ACE_INLINE int
ACE_Mem_Map::close_filemapping_handle (void)
{
  int result = 0;

  if (this->file_mapping_ != this->handle_
      && this->file_mapping_ != ACE_INVALID_HANDLE)
    {
      // On LynxOS, this will result in unlinking of the (hidden)
      // shared memory file if there are no more references to it.
      result = ACE_OS::close (this->file_mapping_);
      this->file_mapping_ = ACE_INVALID_HANDLE;
    }

  return result;
}

// Unmap the region starting at <this->base_addr_>.

ACE_INLINE int
ACE_Mem_Map::unmap (int len)
{
  ACE_TRACE ("ACE_Mem_Map::unmap");

  this->close_filemapping_handle ();

#if defined (__Lynx__)
  int writeback_result = 0;
  if (write_enabled_)
    {
      // Write back the contents of the shared memory object to the
      // file.
      const off_t filesize = ACE_OS::filesize (handle_);
      writeback_result =
        ACE_OS::lseek (handle_, 0, 0) != -1  
        && ACE_OS::write (handle_,
                          base_addr_,
                          (int) filesize) == filesize ? 0 : -1;
    }
#endif /* __Lynx__ */
  if (this->base_addr_ != MAP_FAILED)
    {
      int result = ACE_OS::munmap (this->base_addr_,
                                   len < 0 ? this->length_ : len);
      this->base_addr_ = MAP_FAILED;
      return result;
    }
  else
#if defined (__Lynx__)
    return writeback_result;
#else  /* ! __Lynx__ */
    return 0;
#endif /* ! __Lynx__ */
}

// Unmap the region starting at <addr_>.

ACE_INLINE int
ACE_Mem_Map::unmap (void *addr, int len)
{
  ACE_TRACE ("ACE_Mem_Map::unmap");

  this->close_filemapping_handle ();

#if defined (__Lynx__)
  int writeback_result = 0;
  if (write_enabled_)
    {
      // Write back the contents of the shared memory object to the file.
      const off_t filesize = ACE_OS::filesize (handle_);
      writeback_result =
        ACE_OS::lseek (handle_, 0, 0) != -1 
        && ACE_OS::write (handle_,
                          base_addr_,
                          (int) filesize) == filesize ? 0 : -1;
    }
#endif /* __Lynx__ */

#if defined (__Lynx__)
  return ACE_OS::munmap (addr,
                         len < 0 ? this->length_ : len) 
    | writeback_result;;
#else  /* ! __Lynx__ */
  return ACE_OS::munmap (addr,
                         len < 0 ? this->length_ : len);
#endif /* ! __Lynx__ */
}

// Sync <len> bytes of the memory region to the backing store starting
// at <this->base_addr_>.  If <len> == -1 then sync the whole mapped
// region.

ACE_INLINE int
ACE_Mem_Map::sync (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Mem_Map::sync");
  return ACE_OS::msync (this->base_addr_,
                        len < 0 ? this->length_ : len,
                        flags);
}

// Sync <len> bytes of the memory region to the backing store starting
// at <addr_>.

ACE_INLINE int
ACE_Mem_Map::sync (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_Mem_Map::sync");
  return ACE_OS::msync (addr, len, flags);
}

// Change the protection of the pages of the mapped region to <prot>
// starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
// then change protection of all pages in the mapped region.

ACE_INLINE int
ACE_Mem_Map::protect (ssize_t len, int prot)
{
  ACE_TRACE ("ACE_Mem_Map::protect");
  if (len < 0)
    len = this->length_;
  return ACE_OS::mprotect (this->base_addr_, len, prot);
}

// Change the protection of the pages of the mapped region to <prot>
// starting at <addr> up to <len> bytes.

ACE_INLINE int
ACE_Mem_Map::protect (void *addr, size_t len, int prot)
{
  ACE_TRACE ("ACE_Mem_Map::protect");
  return ACE_OS::mprotect (addr, len, prot);
}

// Hook into the underlying VM system.

ACE_INLINE int
ACE_Mem_Map::advise (int behavior, int len)
{
  ACE_TRACE ("ACE_Mem_Map::advise");
  if (len < 0)
    len = this->length_;

  return ACE_OS::madvise ((caddr_t) this->base_addr_,
                          len,
                          behavior);
}

ACE_INLINE int
ACE_Mem_Map::close_handle (void)
{
  int result = 0;

  if (this->close_handle_)
    {
      this->close_handle_ = 0;
      result = ACE_OS::close (this->handle_);
      this->handle_ = ACE_INVALID_HANDLE;
    }

  return result;
}
