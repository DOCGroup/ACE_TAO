// $Id$

// Defines the member functions for the memory mapping facility.

#define ACE_BUILD_DLL
#include "ace/Mem_Map.h"

#if !defined (__ACE_INLINE__)
#include "ace/Mem_Map.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Mem_Map, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Mem_Map)

void
ACE_Mem_Map::dump (void) const
{
  ACE_TRACE ("ACE_Mem_Map::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("base_addr_ = %x"), this->base_addr_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nfilename_ = %s"), this->filename_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nlength_ = %d"), this->length_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhandle_ = %d"), this->handle_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nfile_mapping_ = %d"), this->file_mapping_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nclose_handle_ = %d"), this->close_handle_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_Mem_Map::close (void)
{
  ACE_TRACE ("ACE_Mem_Map::close");

  this->unmap ();

  return this->close_handle ();
}

ACE_Mem_Map::~ACE_Mem_Map (void)
{
  ACE_TRACE ("ACE_Mem_Map::~ACE_Mem_Map");

  this->close ();
}

// This function does the dirty work of actually calling ACE_OS::mmap
// to map the file into memory.

int
ACE_Mem_Map::map_it (ACE_HANDLE handle,
                     int len_request,
                     int prot,
                     int share,
                     void *addr,
                     off_t pos,
                     LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map_it");
  this->base_addr_ = addr;
  this->handle_ = handle;

  long file_len = ACE_OS::filesize (this->handle_);

  if (file_len == -1)
    return -1;

  if (this->length_ < ACE_static_cast(size_t, file_len))
    {
      // If the length of the mapped region is less than the length of
      // the file then we force a complete new remapping by setting
      // the descriptor to ACE_INVALID_HANDLE (closing down the
      // descriptor if necessary).
      this->close_filemapping_handle ();
    }

  // At this point we know <file_len> is not negative...
  this->length_ = ACE_static_cast(size_t, file_len);

  if (len_request == -1)
    len_request = 0;

  if ((this->length_ == 0 && len_request > 0)
      || this->length_ < ACE_static_cast(size_t, len_request))
    {
      this->length_ = len_request;

      // Extend the backing store.
#if defined (ACE_HAS_P_READ_WRITE)
      if (ACE_OS::pwrite (this->handle_, "", 1,
                          len_request > 0 ? len_request - 1 : 0) == -1)
#else
      if (ACE_OS::lseek (this->handle_,
                         len_request > 0 ? len_request - 1 : 0,
                         SEEK_SET) == -1
          || ACE_OS::write (this->handle_, "", 1) == -1
          || ACE_OS::lseek (this->handle_, 0, SEEK_SET) == -1)
#endif /* ACE_HAD_P_READ_WRITE */
        return -1;
    }

#if defined (__Lynx__)
  // Set flag that indicates whether PROT_WRITE has been enabled.
  write_enabled_ = prot & PROT_WRITE  ?  1  :  0;
#endif /* __Lynx__ */

  this->base_addr_ = ACE_OS::mmap (this->base_addr_,
                                   this->length_,
                                   prot,
                                   share,
                                   this->handle_,
                                   off_t (ACE::round_to_pagesize (pos)),
                                   &this->file_mapping_,
                                   sa);

  return this->base_addr_ == MAP_FAILED ? -1 : 0;
}

int
ACE_Mem_Map::open (LPCTSTR file_name,
                   int flags,
                   int mode,
                   LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::open");

  ACE_OS::strncpy (this->filename_, file_name, MAXPATHLEN);

  this->handle_ = ACE_OS::open (file_name, flags, mode, sa);

  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;
  else
    {
      this->close_handle_ = 1;
      return 0;
    }
}

int
ACE_Mem_Map::map (LPCTSTR file_name,
                  int len,
                  int flags,
                  int mode,
                  int prot,
                  int share,
                  void *addr,
                  off_t pos,
                  LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  this->length_ = 0;

  if (this->open (file_name, flags, mode, sa) == -1)
    return -1;
  else
    return this->map_it (this->handle (), len, prot, share, addr, pos, sa);
}

ACE_Mem_Map::ACE_Mem_Map (void)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (0)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");
  ACE_OS::memset (this->filename_, 0, sizeof this->filename_);
}

// Map a file specified by FILE_NAME.

ACE_Mem_Map::ACE_Mem_Map (LPCTSTR file_name,
                          int len,
                          int flags,
                          int mode,
                          int prot,
                          int share,
                          void *addr,
                          off_t pos,
                          LPSECURITY_ATTRIBUTES sa)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (0)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");
  if (this->map (file_name, len, flags, mode, prot, share, addr, pos, sa) < 0)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Map a file from an open file descriptor HANDLE.  This function will
// lookup the length of the file if it is not given.

ACE_Mem_Map::ACE_Mem_Map (ACE_HANDLE handle,
                          int len,
                          int prot,
                          int share,
                          void *addr,
                          off_t pos,
                          LPSECURITY_ATTRIBUTES sa)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (0)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");

  ACE_OS::memset (this->filename_, 0, sizeof this->filename_);

  if (this->map (handle, len, prot, share, addr, pos, sa) < 0)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Close down and remove the file from the file system.

int
ACE_Mem_Map::remove (void)
{
  ACE_TRACE ("ACE_Mem_Map::remove");

  ACE_OS::ftruncate (this->handle_, 0);
  this->close ();

  if (this->filename_[0] != '\0')
    return ACE_OS::unlink (this->filename_);
  else
    return 0;
}
