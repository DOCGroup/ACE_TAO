/* -*- C++ -*- */
// $Id$

// Mem_Map.i

ACE_INLINE ACE_HANDLE
ACE_Mem_Map::handle (void) const
{
  ACE_TRACE ("ACE_Mem_Map::handle");
  return this->handle_;
}

// Return the name of file that is mapped (if any).

ACE_INLINE const TCHAR *
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
		  off_t pos)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  return this->map_it (handle, len, prot, share, addr, pos);
}

// Remap the file associated with <this->handle_>.

ACE_INLINE int
ACE_Mem_Map::map (int len, 
		  int prot, 
		  int share, 
		  void *addr, 
		  off_t pos)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  return this->map_it (this->handle (), len, prot, 
		       share, addr, pos);
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

// Unmap the region starting at <this->base_addr_>.

ACE_INLINE int
ACE_Mem_Map::unmap (int len)
{
  ACE_TRACE ("ACE_Mem_Map::unmap");
  if (this->file_mapping_ != this->handle_)
    ACE_OS::close (this->file_mapping_);

  this->file_mapping_ = ACE_INVALID_HANDLE;

  return ACE_OS::munmap (this->base_addr_, len < 0 ? this->length_ : len);
}

// Unmap the region starting at <addr_>.

ACE_INLINE int
ACE_Mem_Map::unmap (void *addr, int len)
{
  ACE_TRACE ("ACE_Mem_Map::unmap");
  if (this->file_mapping_ != this->handle_)
    ACE_OS::close (this->file_mapping_);

  this->file_mapping_ = ACE_INVALID_HANDLE;

  return ACE_OS::munmap (addr, len < 0 ? this->length_ : len);
}

// Sync <len> bytes of the memory region to the backing store starting
// at <this->base_addr_>.  If <len> == -1 then sync the whole mapped
// region.

ACE_INLINE int
ACE_Mem_Map::sync (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_Mem_Map::sync");
  return ACE_OS::msync (this->base_addr_, len < 0 ? this->length_ : len, flags);
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
  return ACE_OS::madvise ((caddr_t) this->base_addr_, len, behavior);
}
