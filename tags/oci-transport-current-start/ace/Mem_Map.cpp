// $Id$

// Defines the member functions for the memory mapping facility.

#include "ace/Mem_Map.h"
#if !defined (__ACE_INLINE__)
#include "ace/Mem_Map.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

#if defined (ACE_WIN32) \
    && (!defined(ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0))
# define ACE_USE_MAPPING_NAME 1
#endif /* ACE_WIN32 */

#if defined (ACE_USE_MAPPING_NAME)
#include "ace/SString.h"
#endif /* ACE_USE_MAPPING_NAME */

ACE_RCSID(ace, Mem_Map, "Mem_Map.cpp,v 4.39 2003/11/01 11:15:13 dhinton Exp")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Mem_Map)

#if defined (ACE_USE_MAPPING_NAME)
// Gets a mapping object name from a file name.  TODO: The file name
// is used as the key to the mapping. We should try to avoid mapping
// the same object name to different files (there is a mapping object
// name length limitation).

static void
to_mapping_name (ACE_TCHAR *mapobjname,
    const ACE_TCHAR *filename,
    size_t len)
{
  --len;
  size_t i = 0;

  while (*filename && i < len)
    {
      if (*filename == ACE_LIB_TEXT ('\\'))
        // Can't use backslash in mapping object name.
        mapobjname[i] = ACE_LIB_TEXT ('.');
      else
        mapobjname[i] = *filename;
      ++filename;
      ++i;
    }

  mapobjname[i] = 0;
}
#endif /* ACE_USE_MAPPING_NAME */

void
ACE_Mem_Map::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Mem_Map::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("base_addr_ = %x"), this->base_addr_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nfilename_ = %s"), this->filename_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nlength_ = %d"), this->length_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nhandle_ = %d"), this->handle_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nfile_mapping_ = %d"), this->file_mapping_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nclose_handle_ = %d"), this->close_handle_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
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
                     ssize_t length_request,
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
  int const unmap_result = this->unmap ();
  if (unmap_result != 0)
    return unmap_result;
#endif /* ACE_LACKS_AUTO_MMAP_REPLACEMENT */

  this->base_addr_ = addr;
  this->handle_ = handle;

  ACE_LOFF_T const result = ACE_OS::filesize (this->handle_);

  // At this point we know <result> is not negative...
  size_t const current_file_length = static_cast<size_t> (result);

  // Flag to indicate if we need to extend the back store
  bool extend_backing_store = false;

  // File length requested by user
  size_t requested_file_length = 0;

  // Check <length_request>
  if (length_request == -1)
    // Set length to file_request.
    this->length_ = current_file_length - offset;
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
          extend_backing_store = true;
        }

      // Set length to length_request
      this->length_ = length_request;
    }

  // Check if we need to extend the backing store.
  if (extend_backing_store)
    {
      // Remember than write increases the size by one.
      off_t null_byte_position;
      if (requested_file_length > 0)
        // This will make the file size <requested_file_length>
        null_byte_position =
          static_cast<off_t> (requested_file_length - 1);
      else
        // This will make the file size 1
        null_byte_position = 0;

      if (ACE_OS::pwrite (this->handle_,
                          "",
                          1,
                          null_byte_position) == -1)
        return -1;
    }

#if defined (ACE_HAS_LYNXOS_BROKEN_MMAP)
  // Set flag that indicates whether PROT_WRITE has been enabled.
  write_enabled_ = ACE_BIT_ENABLED (prot, PROT_WRITE);
#endif /* ACE_HAS_LYNXOS_BROKEN_MMAP */

#if defined (ACE_USE_MAPPING_NAME)
  if (ACE_BIT_ENABLED (share, MAP_SHARED))
    {
# if defined(__MINGW32__)
      const int max_mapping_name_length = 32;
# else
      const int max_mapping_name_length = 31;
# endif /* __MINGW32__ */
      ACE_TCHAR file_mapping_name[max_mapping_name_length + 1];
      to_mapping_name (file_mapping_name,
                       filename_,
                       max_mapping_name_length + 1);

      this->base_addr_ = ACE_OS::mmap (this->base_addr_,
                                       this->length_,
                                       prot,
                                       share,
                                       this->handle_,
                                       offset,
                                       &this->file_mapping_,
                                       sa,
                                       file_mapping_name);
    }
  else
#endif /* ACE_USE_MAPPING_NAME */
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
ACE_Mem_Map::open (const ACE_TCHAR *file_name,
                   int flags,
                   int mode,
                   LPSECURITY_ATTRIBUTES sa)
{
  ACE_TRACE ("ACE_Mem_Map::open");

#if defined(INTEGRITY)  || defined (__QNXNTO__)
  this->handle_ = ACE_OS::shm_open (file_name, flags, mode, sa);
#elif defined (ACE_OPENVMS)
  ACE_OSCALL (::open (file_name, flags, mode, "shr=get,put,upd"), ACE_HANDLE, -1, this->handle_);
#else
  this->handle_ = ACE_OS::open (file_name, flags, mode, sa);
#endif /* INTEGRITY */

  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;
  else
    {
      ACE_OS::strsncpy (this->filename_,
                        file_name,
                        MAXPATHLEN);

      this->close_handle_ = true;
      return 0;
    }
}

int
ACE_Mem_Map::map (const ACE_TCHAR *file_name,
                  ssize_t len,
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
    close_handle_ (false)
{
  ACE_TRACE ("ACE_Mem_Map::ACE_Mem_Map");
  ACE_OS::memset (this->filename_, 0, sizeof this->filename_);
}

// Map a file specified by FILE_NAME.

ACE_Mem_Map::ACE_Mem_Map (const ACE_TCHAR *file_name,
                          ssize_t len,
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
    close_handle_ (false)
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
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Map a file from an open file descriptor HANDLE.  This function will
// lookup the length of the file if it is not given.

ACE_Mem_Map::ACE_Mem_Map (ACE_HANDLE handle,
                          ssize_t len,
                          int prot,
                          int share,
                          void *addr,
                          off_t offset,
                          LPSECURITY_ATTRIBUTES sa)
  : base_addr_ (MAP_FAILED),
    length_ (0),
    handle_ (ACE_INVALID_HANDLE),
    file_mapping_ (ACE_INVALID_HANDLE),
    close_handle_ (false)
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
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Mem_Map::ACE_Mem_Map")));
}

// Close down and remove the file from the file system.

int
ACE_Mem_Map::remove (void)
{
  ACE_TRACE ("ACE_Mem_Map::remove");

  ACE_OS::ftruncate (this->handle_, 0);
  this->close ();

  if (this->filename_[0] != '\0')
#if defined (__QNXNTO__)
  return ACE_OS::shm_unlink (this->filename_);
#else
  return ACE_OS::unlink (this->filename_);
#endif /* __QNXNTO__ */

  else
    return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
