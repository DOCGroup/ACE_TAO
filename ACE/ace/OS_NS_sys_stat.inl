// -*- C++ -*-
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_macros.h"

#ifdef ACE_MQX
#  include "ace/MQX_Filesystem.h"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{

  ACE_INLINE ACE_HANDLE
  creat (const ACE_TCHAR *filename, mode_t mode)
  {
    ACE_OS_TRACE ("ACE_OS::creat");
#if defined (ACE_WIN32) || defined (ACE_MQX)
    return ACE_OS::open (filename, O_CREAT|O_TRUNC|O_WRONLY, mode);
#elif defined (ACE_LACKS_CREAT)
    ACE_UNUSED_ARG (filename);
    ACE_UNUSED_ARG (mode);
    ACE_NOTSUP_RETURN (-1);
#else
    return ::creat (ACE_TEXT_ALWAYS_CHAR (filename), mode);
#endif /* ACE_WIN32 */
  }

  ACE_INLINE int
  fstat (ACE_HANDLE handle, ACE_stat *stp)
  {
    ACE_OS_TRACE ("ACE_OS::fstat");
#if defined (ACE_WIN32)
    BY_HANDLE_FILE_INFORMATION fdata;

    if (::GetFileInformationByHandle (handle, &fdata) == FALSE)
      {
        ACE_OS::set_errno_to_last_error ();
        return -1;
      }
    if ((fdata.nFileSizeHigh != 0) && (sizeof (stp->st_size) < sizeof (ULONGLONG)))
      {
        errno = EINVAL; // return an error rather than incorrect values
        return -1;
      }

#if defined (_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
    ULARGE_INTEGER ul;
    ul.HighPart = fdata.nFileSizeHigh;
    ul.LowPart = fdata.nFileSizeLow;
    stp->st_size = ul.QuadPart;
#else
    stp->st_size = fdata.nFileSizeLow;
#endif /* _FILE_OFFSET_BITS */
    stp->st_atime = ACE_Time_Value (fdata.ftLastAccessTime).sec ();
    stp->st_mtime = ACE_Time_Value (fdata.ftLastWriteTime).sec ();
    stp->st_ctime = ACE_Time_Value (fdata.ftCreationTime).sec ();
    stp->st_nlink = static_cast<short> (fdata.nNumberOfLinks);
    stp->st_dev = stp->st_rdev = 0; // No equivalent conversion.
    mode_t const BASE_MODE = S_IXOTH | S_IROTH;
    mode_t const WRITE_MODE = (fdata.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? 0 : S_IWOTH;
    mode_t const TYPE_MODE = (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? S_IFDIR : S_IFREG;
    stp->st_mode = BASE_MODE | WRITE_MODE | TYPE_MODE;
    return 0;
#elif defined (ACE_LACKS_FSTAT)
    ACE_UNUSED_ARG (handle);
    ACE_UNUSED_ARG (stp);
    ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_MQX)
    return MQX_Filesystem::inst ().fstat (handle, stp);
#else
    return ::fstat (handle, stp);
#endif /* !ACE_WIN32 */
  }

  // This function returns the number of bytes in the file referenced by
  // FD.
  ACE_INLINE ACE_OFF_T
  filesize (ACE_HANDLE handle)
  {
    ACE_OS_TRACE ("ACE_OS::filesize");
#if defined (ACE_WIN32)
# if defined (_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
    LARGE_INTEGER size;
    return
      (::GetFileSizeEx (handle, &size)
       ? size.QuadPart
       : (ACE_OS::set_errno_to_last_error (), -1));
# else
    DWORD const size = ::GetFileSize (handle, 0);
    return
      (size != INVALID_FILE_SIZE
       ? static_cast<ACE_OFF_T> (size)
       : (ACE_OS::set_errno_to_last_error (), -1));
# endif  /* _FILE_OFFSET_BITS == 64 */
#else /* !ACE_WIN32 */
    ACE_stat sb;
    return ACE_OS::fstat (handle, &sb) == -1 ?
                    static_cast<ACE_OFF_T> (-1) : sb.st_size;
#endif
  }

  ACE_INLINE ACE_OFF_T
  filesize (const ACE_TCHAR *filename)
  {
    ACE_OS_TRACE ("ACE_OS::filesize");

#if defined (ACE_LACKS_STAT)
    ACE_HANDLE const h = ACE_OS::open (filename, O_RDONLY);
    if (h != ACE_INVALID_HANDLE)
      {
        ACE_OFF_T const size = ACE_OS::filesize (h);
        ACE_OS::close (h);
        return size;
      }
    else
      return -1;
#else /* !ACE_LACKS_STAT */
    ACE_stat sb;
    return ACE_OS::stat (filename, &sb) == -1 ?
                    static_cast<ACE_OFF_T> (-1) : sb.st_size;
#endif /* ACE_LACKS_STAT */
  }

  ACE_INLINE int
  lstat (const char *file, ACE_stat *stp)
  {
    ACE_OS_TRACE ("ACE_OS::lstat");
# if defined (ACE_LACKS_LSTAT)
    return ACE_OS::stat (file, stp);
# else /* !ACE_LACKS_LSTAT */
    return ::lstat (file, stp);
# endif /* ACE_LACKS_LSTAT */
  }

#if defined (ACE_HAS_WCHAR)
  ACE_INLINE int
  lstat (const wchar_t *file, ACE_stat *stp)
  {
    ACE_OS_TRACE ("ACE_OS::lstat");
# if defined (ACE_LACKS_LSTAT)
    return ACE_OS::stat (file, stp);
# else
    return ACE_OS::lstat (ACE_Wide_To_Ascii (file).char_rep (), stp);
# endif /* ACE_LACKS_LSTAT */
  }
#endif /* ACE_HAS_WCHAR */

  ACE_INLINE int
  mkdir (const char *path, mode_t mode)
  {
#if defined (ACE_LACKS_MKDIR)
    ACE_UNUSED_ARG (path);
    ACE_UNUSED_ARG (mode);
    ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_MKDIR_LACKS_MODE)
    ACE_UNUSED_ARG (mode);
#  if defined (ACE_MKDIR_EQUIVALENT)
    return ACE_MKDIR_EQUIVALENT (path);
#  else
    return ::mkdir (path);
#  endif
#else
    return::mkdir (path, mode);
#endif
  }

#if defined (ACE_HAS_WCHAR)

  ACE_INLINE int
  mkdir (const wchar_t *path, mode_t mode)
  {
#if defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
    ACE_UNUSED_ARG (mode);
    return ::_wmkdir (path);
#else
    return ACE_OS::mkdir (ACE_Wide_To_Ascii (path).char_rep (), mode);
#endif /* ACE_WIN32 && ACE_USES_WCHAR */
  }

#endif /* ACE_HAS_WCHAR */

  ACE_INLINE int
  mkfifo (const ACE_TCHAR *file, mode_t mode)
  {
    ACE_OS_TRACE ("ACE_OS::mkfifo");
#if defined (ACE_LACKS_MKFIFO)
    ACE_UNUSED_ARG (file);
    ACE_UNUSED_ARG (mode);
    ACE_NOTSUP_RETURN (-1);
#else
    return ::mkfifo (ACE_TEXT_ALWAYS_CHAR (file), mode);
#endif /* ACE_LACKS_MKFIFO */
  }

  ACE_INLINE int
  stat (const char *file, ACE_stat *stp)
  {
    ACE_OS_TRACE ("ACE_OS::stat");
#if defined (ACE_LACKS_STAT)
    ACE_UNUSED_ARG (file);
    ACE_UNUSED_ARG (stp);
    ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_MQX)
    return MQX_Filesystem::inst ().stat (file, stp);
#else
    return ACE_STAT_FUNC_NAME (file, stp);
#endif /* ACE_LACKS_STAT */
  }

#if defined (ACE_HAS_WCHAR)
  ACE_INLINE int
  stat (const wchar_t *file, ACE_stat *stp)
  {
    ACE_OS_TRACE ("ACE_OS::stat");
#if defined (__BORLANDC__) \
    || defined (_MSC_VER) \
    || (defined (__MINGW32__) && !defined (__MINGW64_VERSION_MAJOR))
    return ACE_WSTAT_FUNC_NAME (file, stp);
#else
    ACE_Wide_To_Ascii nfile (file);
    return ACE_OS::stat (nfile.char_rep (), stp);
#endif /* __BORLANDC__  */
  }
#endif /* ACE_HAS_WCHAR */

  ACE_INLINE mode_t
  umask (mode_t cmask)
  {
    ACE_OS_TRACE ("ACE_OS::umask");
# if defined (ACE_LACKS_UMASK)
    ACE_UNUSED_ARG (cmask);
    ACE_NOTSUP_RETURN ((mode_t)-1);
# elif defined (ACE_HAS_TR24731_2005_CRT)
    int old_mode;
    int const new_mode = static_cast<int> (cmask);
    ACE_SECURECRTCALL (_umask_s (new_mode, &old_mode), mode_t, -1, old_mode);
    return static_cast<mode_t> (old_mode);
# elif defined (ACE_WIN32) && !defined (__BORLANDC__)
    return ::_umask (cmask);
# else
    return ::umask (cmask); // This call shouldn't fail...
# endif /* ACE_LACKS_UMASK */
  }

} // ACE_OS namespace

ACE_END_VERSIONED_NAMESPACE_DECL
