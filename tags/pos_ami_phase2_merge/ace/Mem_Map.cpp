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
                     int length_request,
                     int prot,
                     int share,
                     void *addr,
                     off_t offset,
                     LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map_it");

#if defined (ACE_LACKS_AUTO_MMAP_REPLACEMENT)
  // If the system does not replace any previous mappings, then
  // unmap() before (potentially) mapping to the same location.
  int unmap_result = this->unmap ();
  if (unmap_result != 0)
    return unmap_result;
#endif /* ACE_LACKS_AUTO_MMAP_REMAPPING */
  
  this->base_addr_ = addr;
  this->handle_ = handle;

#if defined (CHORUS)
  // Chorus does not support filesize on a shared memory handle.  We
  // assume that <length_> = 0 when <ACE_Mem_Map> is initially
  // constructed (i.e., before <map_it> is called with a valid
  // <len_request>).
  long result = this->length_;

  if (result == -1)
    return -1;
#else
  long result = ACE_OS::filesize (this->handle_);
#endif /* CHORUS */

  // At this point we know <result> is not negative...
  size_t current_file_length = ACE_static_cast (size_t, result);

  // Flag to indicate if we need to extend the back store
  int extend_backing_store = 0;

  // File length requested by user
  size_t requested_file_length = 0;

  // Check <length_request>
  if (length_request == -1)
    // Set length to file_request
    this->length_ = current_file_length;
  else
    {
      // File length implicitly requested by user
      requested_file_length = length_request + offset;
        
      // Check to see if we need to extend the backing store
      if (requested_file_length > current_file_length)
        {
          // If the length of the mapped region is less than the
          // length of the file then we force a complete new remapping
          // by setting the descriptor to ACE_INVALID_HANDLE (closing
          // down the descriptor if necessary).
          this->close_filemapping_handle ();

          // Remember to extend the backing store
          extend_backing_store = 1;
        }

      // Set length to length_request
      this->length_ = length_request;
    }

  // Check if we need to extend the backing store.
  if (extend_backing_store)
    {
#if !defined (CHORUS)
      // Remember than write increases the size by one.
      size_t null_byte_position;
      if (requested_file_length > 0)
        // This will make the file size <requested_file_length>
        null_byte_position = requested_file_length - 1;
      else 
        // This will make the file size 1
        null_byte_position = 0;
      
      if (ACE_OS::pwrite (this->handle_,
                          "",
                          1,
                          null_byte_position) == -1)
        return -1;
#else
      // This nonsense is to make this code similar to the above code.
      size_t actual_file_length;
      if (requested_file_length > 0)
        // This will make the file size <requested_file_length>
        actual_file_length = requested_file_length;
      else 
        // This will make the file size 1
        actual_file_length = 1;
      
      if (ACE_OS::ftruncate (this->handle_,
                             actual_file_length) == -1)
        return -1;
#endif /* !CHORUS */
    }
  
#if defined (__Lynx__)
  // Set flag that indicates whether PROT_WRITE has been enabled.
  write_enabled_ = ACE_BIT_ENABLED (prot, PROT_WRITE);
#endif /* __Lynx__ */

  this->base_addr_ = ACE_OS::mmap (this->base_addr_,
                                   this->length_,
                                   prot,
                                   share,
                                   this->handle_,
                                   offset,
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

  ACE_OS::strncpy (this->filename_,
                   file_name,
                   MAXPATHLEN);

#if defined (CHORUS)
  this->handle_ = ACE_OS::shm_open (file_name, flags, mode, sa);
#else
  this->handle_ = ACE_OS::open (file_name, flags, mode, sa);
#endif /* CHORUS */

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
                  off_t offset,
                  LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::map");
  this->length_ = 0;

  if (this->open (file_name,
                  flags,
                  mode,
                  sa) == -1)
    return -1;
  else
    return this->map_it (this->handle (),
                         len,
                         prot,
                         share,
                         addr,
                         offset,
                         sa);
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
                          off_t offset,
                          LPSECURITY_ATTRIBUTES sa)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (0)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");
  if (this->map (file_name,
                 len,
                 flags,
                 mode,
                 prot,
                 share,
                 addr,
                 offset,
                 sa) < 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Map a file from an open file descriptor HANDLE.  This function will
// lookup the length of the file if it is not given.

ACE_Mem_Map::ACE_Mem_Map (ACE_HANDLE handle,
                          int len,
                          int prot,
                          int share,
                          void *addr,
                          off_t offset,
                          LPSECURITY_ATTRIBUTES sa)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (0)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");

  ACE_OS::memset (this->filename_,
                  0,
                  sizeof this->filename_);
  if (this->map (handle,
                 len,
                 prot,
                 share,
                 addr,
                 offset,
                 sa) < 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Close down and remove the file from the file system.

int
ACE_Mem_Map::remove (void)
{
  ACE_TRACE ("ACE_Mem_Map::remove");

  ACE_OS::ftruncate (this->handle_, 0);
  this->close ();

  if (this->filename_[0] != '\0')
#if defined (CHORUS)
  return ACE_OS::shm_unlink (this->filename_);
#else
  return ACE_OS::unlink (this->filename_);
#endif /* CHORUS */

  else
    return 0;
}
