// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_Thread.h"

ACE_RCSID (ace,
           OS_NS_stdio,
           "$Id$")


#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_stdio.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

# if defined (ACE_WIN32)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
ACE_TEXT_OSVERSIONINFO ACE_OS::win32_versioninfo_;
HINSTANCE ACE_OS::win32_resource_module_;
ACE_END_VERSIONED_NAMESPACE_DECL

#   if defined (ACE_HAS_DLL) && (ACE_HAS_DLL == 1) && !defined (ACE_HAS_WINCE)
// This function is called by the OS when the ACE DLL is loaded. We
// use it to determine the default module containing ACE's resources.
extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH)
    {
#     if defined (ACE_DISABLES_THREAD_LIBRARY_CALLS) && (ACE_DISABLES_THREAD_LIBRARY_CALLS == 1)
      ::DisableThreadLibraryCalls (instance);
#     endif /* ACE_DISABLES_THREAD_LIBRARY_CALLS */
      ACE_OS::set_win32_resource_module(instance);
    }
  else if (reason == DLL_THREAD_DETACH)
    {
      ACE_OS::cleanup_tss (0);
    }
  return TRUE;
}
#   endif /* ACE_HAS_DLL && ACE_HAS_DLL == 1 */
# endif /* ACE_WIN32 */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

void
ACE_OS::ace_flock_t::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_OS_TRACE ("ACE_OS::ace_flock_t::dump");

# if 0
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("handle_ = %u"), this->handle_));
#   if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nInternal = %d"),
              this->overlapped_.Internal));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nInternalHigh = %d"),
              this->overlapped_.InternalHigh));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nOffsetHigh = %d"),
              this->overlapped_.OffsetHigh));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nhEvent = %d"),
              this->overlapped_.hEvent));
#   else
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_whence = %d"),
              this->lock_.l_whence));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_start = %d"), this->lock_.l_start));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_len = %d"), this->lock_.l_len));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_type = %d"), this->lock_.l_type));
#   endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
# endif /* 0 */
#endif /* ACE_HAS_DUMP */
}

/*****************************************************************************/

#if defined (ACE_USES_WCHAR)
void ACE_OS::checkUnicodeFormat (FILE* fp)
{
  if (fp != 0)
    {
      // Due to the ACE_TCHAR definition, all default input files, such as
      // svc.conf, have to be in Unicode format (small endian) on WinCE
      // because ACE has all 'char' converted into ACE_TCHAR.
      // However, for TAO, ASCII files, such as IOR file, can still be read
      // and be written without any error since given buffers are all in 'char'
      // type instead of ACE_TCHAR.  Therefore, it is user's reponsibility to
      // select correct buffer type.

      // At this point, check if the file is Unicode or not.
      ACE_UINT16 first_two_bytes;
      size_t numRead =
        ACE_OS::fread(&first_two_bytes, sizeof (first_two_bytes), 1, fp);

      if (numRead == 1)
        {
          if ((first_two_bytes != 0xFFFE) && // not a small endian Unicode file
              (first_two_bytes != 0xFEFF))   // not a big endian Unicode file
            {
              // set file pointer back to the beginning
#if defined (ACE_WIN32)
              ACE_OS::fseek(fp, 0, FILE_BEGIN);
#else
              ACE_OS::fseek(fp, 0, SEEK_SET);
#endif /* ACE_WIN32 */
            }
        }
      // if it is a Unicode file, file pointer will be right next to the first
      // two-bytes
    }
}
#endif  // ACE_USES_WCHAR

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
namespace
{
  /// Translate fopen's mode char to open's mode.  This helper function
  /// is here to avoid maintaining several pieces of identical code.
  void
  fopen_mode_to_open_mode_converter (ACE_TCHAR x, int & hmode)
  {
    switch (x)
      {
      case ACE_LIB_TEXT ('r'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_WRONLY);
            ACE_SET_BITS (hmode, _O_RDONLY);
          }
        break;
      case ACE_LIB_TEXT ('w'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_RDONLY);
            ACE_SET_BITS (hmode, _O_WRONLY);
          }
        ACE_SET_BITS (hmode, _O_CREAT | _O_TRUNC);
        break;
      case ACE_LIB_TEXT ('a'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_RDONLY);
            ACE_SET_BITS (hmode, _O_WRONLY);
          }
        ACE_SET_BITS (hmode, _O_CREAT | _O_APPEND);
        break;
      case ACE_LIB_TEXT ('+'):
        ACE_CLR_BITS (hmode, _O_RDONLY | _O_WRONLY);
        ACE_SET_BITS (hmode, _O_RDWR);
        break;
      case ACE_LIB_TEXT ('t'):
        ACE_CLR_BITS (hmode, _O_BINARY);
        ACE_SET_BITS (hmode, _O_TEXT);
        break;
      case ACE_LIB_TEXT ('b'):
        ACE_CLR_BITS (hmode, _O_TEXT);
        ACE_SET_BITS (hmode, _O_BINARY);
        break;
      }
  }
}  // Close anonymous namespace

FILE *
ACE_OS::fopen (const char *filename,
               const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  // Let the chips fall where they may if the user passes in a NULL
  // mode string.  Convert to an empty mode string to prevent a
  // crash.
  ACE_TCHAR const empty_mode[] = ACE_TEXT ("");
  if (!mode)
    mode = empty_mode;

  for (ACE_TCHAR const* mode_ptr = mode; *mode_ptr != 0; ++mode_ptr)
    fopen_mode_to_open_mode_converter (*mode_ptr, hmode);

  ACE_HANDLE const handle = ACE_OS::open (filename, hmode);
  if (handle != ACE_INVALID_HANDLE)
    {
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;

      int const fd = ::_open_osfhandle (intptr_t (handle), hmode);

      if (fd != -1)
        {
#   if defined (__BORLANDC__) && !defined (ACE_USES_WCHAR)
          FILE * const fp = ::_fdopen (fd, const_cast<ACE_TCHAR *> (mode));
#   elif defined (__BORLANDC__) && defined (ACE_USES_WCHAR)
          FILE * const fp = ::_wfdopen (fd, const_cast<ACE_TCHAR *> (mode));
#   elif defined (ACE_USES_WCHAR)
          FILE * const fp = ::_wfdopen (fd, mode);
#   else
          FILE * const fp = ::fdopen (fd, mode);
#   endif /* defined(__BORLANDC__) && !defined (ACE_USES_WCHAR)) */
          if (fp != 0)
          {
#   if defined (ACE_USES_WCHAR)
            checkUnicodeFormat(fp);
#   endif  // ACE_USES_WCHAR
            return fp;
          }
          ::_close (fd);
        }

      ACE_OS::close (handle);
    }
  return 0;
}

#if defined (ACE_HAS_WCHAR)
FILE *
ACE_OS::fopen (const wchar_t *filename,
               const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  for (const ACE_TCHAR *mode_ptr = mode; *mode_ptr != 0; mode_ptr++)
    fopen_mode_to_open_mode_converter (*mode_ptr, hmode);

  ACE_HANDLE handle = ACE_OS::open (filename, hmode);
  if (handle != ACE_INVALID_HANDLE)
    {
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;

      int const fd = ::_open_osfhandle (intptr_t (handle), hmode);

      if (fd != -1)
        {
#   if defined (__BORLANDC__) && !defined (ACE_USES_WCHAR)
          FILE *fp = ::_fdopen (fd, const_cast<char *> (mode));
#   elif defined (__BORLANDC__) && defined (ACE_USES_WCHAR)
          FILE *fp = ::_wfdopen (fd, const_cast<wchar_t *> (mode));
#   elif defined (ACE_USES_WCHAR)
          FILE *fp = ::_wfdopen (fd, mode);
#   else
          FILE *fp = ::fdopen (fd, mode);
#   endif /* defined(__BORLANDC__) && !defined (ACE_USES_WCHAR)) */
          if (fp != 0)
          {
#   if defined (ACE_USES_WCHAR)
            checkUnicodeFormat(fp);
#   endif  // ACE_USES_WCHAR
            return fp;
          }
          ::_close (fd);
        }

      ACE_OS::close (handle);
    }
  return 0;
}
#endif /* ACE_HAS_WCHAR */

#endif /* ACE_WIN32 */

int
ACE_OS::fprintf (FILE *fp, const char *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::fprintf");
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vfprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::fprintf (FILE *fp, const wchar_t *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::fprintf");

# if !defined (ACE_HAS_VFWPRINTF)
  ACE_UNUSED_ARG (fp);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);

# else
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (ACE_STD_NAMESPACE::vfwprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;

# endif /* ACE_HAS_VFWPRINTF */
}
#endif /* ACE_HAS_WCHAR */


// The following *printf functions aren't inline because
// they use varargs.

int
ACE_OS::printf (const char *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::printf");
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vprintf (format, ap), int, -1, result);
  va_end (ap);
  return result;
}

int
ACE_OS::snprintf (char *buf, size_t maxlen, const char *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::snprintf");
  int result;
  va_list ap;
  va_start (ap, format);
  result = ACE_OS::vsnprintf (buf, maxlen, format, ap);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)

int
ACE_OS::snprintf (wchar_t *buf, size_t maxlen, const wchar_t *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::snprintf");
# if (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500) || \
     (defined (sun) && !defined (_XPG4) || defined(_XPG5)) || \
     defined (ACE_WIN32) || defined (ACE_HAS_VSWPRINTF)
  int result;
  va_list ap;
  va_start (ap, format);
#  if defined (ACE_WIN32)
  // Microsoft's vswprintf() doesn't have the maxlen argument that
  // XPG4/UNIX98 define. They do, however, recommend use of _vsnwprintf()
  // as a substitute, which does have the same signature as the UNIX98 one.
  ACE_OSCALL (::_vsnwprintf (buf, maxlen, format, ap), int, -1, result);
  // Win32 doesn't regard a full buffer with no 0-terminate as an overrun.
  if (result == static_cast <int> (maxlen))
    result = -1;

  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buf[maxlen-1] = '\0';
#  else
  ACE_OSCALL (::vswprintf (buf, maxlen, format, ap), int, -1, result);
#  endif /* ACE_WIN32 */
  va_end (ap);
  // In out-of-range conditions, C99 defines vsnprintf to return the number
  // of characters that would have been written if enough space was available.
  // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
  // -1. This method follows the C99 standard, but needs to guess at the
  // value; uses maxlen + 1.
  if (result == -1)
    result = static_cast <int> (maxlen + 1);
  return result;

# else
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);
# endif /* _XOPEN_SOURCE ...  */
}
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::sprintf (char *buf, const char *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::sprintf");

  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vsprintf (buf, format, ap), int, -1, result);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::sprintf (wchar_t *buf, const wchar_t *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::sprintf");

# if (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500) || \
     (defined (sun) && !defined (_XPG4) || defined(_XPG5)) || \
     (defined ACE_HAS_DINKUM_STL) || defined (__DMC__) || \
      defined ACE_HAS_VSWPRINTF

  // The XPG4/UNIX98/C99 signature of the wide-char sprintf has a
  // maxlen argument. Since this method doesn't supply one, pass in
  // a length that works (ULONG_MAX doesn't on all platform since some check
  // to see if the operation will remain in bounds). If this isn't ok, use
  // ACE_OS::snprintf().
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (ACE_STD_NAMESPACE::vswprintf (buf, 4096, format, ap), int, -1, result);
  va_end (ap);
  return result;

# elif defined (ACE_WIN32)
  // Windows has vswprintf, but the signature is from the older ISO C
  // standard. Also see ACE_OS::snprintf() for more info on this.

  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vswprintf (buf, format, ap), int, -1, result);
  va_end (ap);
  return result;

# else

  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);

# endif /* XPG5 || ACE_HAS_DINKUM_STL */
}
#endif /* ACE_HAS_WCHAR */

ACE_END_VERSIONED_NAMESPACE_DECL
