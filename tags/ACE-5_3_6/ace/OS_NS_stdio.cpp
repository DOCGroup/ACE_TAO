// -*- C++ -*-
// $Id$

#include "ace/OS_NS_stdio.h"

ACE_RCSID(ace, OS_NS_stdio, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_stdio.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

# if defined (ACE_WIN32)

OSVERSIONINFO ACE_OS::win32_versioninfo_;
HINSTANCE ACE_OS::win32_resource_module_;

#   if defined (ACE_OS_HAS_DLL) && (ACE_OS_HAS_DLL == 1) && !defined (ACE_HAS_WINCE)
// This function is called by the OS when the ACE DLL is loaded. We
// use it to determine the default module containing ACE's resources.
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH)
    {
#     if defined (ACE_DISABLES_THREAD_LIBRARY_CALLS) && (ACE_DISABLES_THREAD_LIBRARY_CALLS == 1)
      ::DisableThreadLibraryCalls (instance);
#     endif /* ACE_DISABLES_THREAD_LIBRARY_CALLS */
      ACE_OS::set_win32_resource_module(instance);
    }
  return TRUE;
}
#   endif /* ACE_OS_HAS_DLL && ACE_OS_HAS_DLL == 1 */
# endif /* ACE_WIN32 */

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
#   elif !defined (CHORUS)
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
      WORD first_two_bytes;
      int numRead = ACE_OS::fread(&first_two_bytes, sizeof(WORD), 1, fp);

      if (numRead == 1)
        {
          if ((first_two_bytes != 0xFFFE) && // not a small endian Unicode file
              (first_two_bytes != 0xFEFF))   // not a big endian Unicode file
            {
              // set file pointer back to the beginning
              ACE_OS::fseek(fp, 0, FILE_BEGIN);
            }
        }
      // if it is a Unicode file, file pointer will be right next to the first
      // two-bytes
    }
}
#endif  // ACE_USES_WCHAR

#if defined (ACE_WIN32)
FILE *
ACE_OS::fopen (const ACE_TCHAR *filename,
               const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  for (const ACE_TCHAR *mode_ptr = mode; *mode_ptr != 0; mode_ptr++)
    ACE_OS::fopen_mode_to_open_mode_converter (*mode_ptr, hmode);

  ACE_HANDLE handle = ACE_OS::open (filename, hmode);
  if (handle != ACE_INVALID_HANDLE)
    {
# if defined (ACE_HAS_WINCE)
      FILE *fp = ::_wfdopen (handle, mode);
      if (fp != 0)
      {
        checkUnicodeFormat(fp);
        return fp;
      }
# else
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;
#   if defined (ACE_WIN64)
      int fd = _open_osfhandle (intptr_t (handle), hmode);
#   else
      int fd = _open_osfhandle (long (handle), hmode);
#   endif /* ACE_WIN64 */
      if (fd != -1)
        {
#   if defined (__BORLANDC__) && !defined (ACE_USES_WCHAR)
          FILE *fp = ::_fdopen (fd, ACE_const_cast (char *, mode));
#   elif defined (__BORLANDC__) && defined (ACE_USES_WCHAR)
          FILE *fp = ::_wfdopen (fd, ACE_const_cast (wchar_t *, mode));
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
          _close (fd);
        }
# endif  // ACE_HAS_WINCE

      ACE_OS::close (handle);
    }
  return 0;
}
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
  ACE_OSCALL (::vfwprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;

# endif /* ACE_HAS_VFWPRINTF */
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS::gets (char *str, int n)
{
  ACE_OS_TRACE ("ACE_OS::gets");
  int c;
  char *s = str;

  if (str == 0 || n < 0) n = 0;
  if (n == 0) str = 0;
  else n--;

  while ((c = getchar ()) != '\n')
    {

      if (c == EOF && errno == EINTR)
        {
#   if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
          continue;
#   else
          break;
#   endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */
        }

      if (c == EOF)
        break;

      if (n > 0)
        n--, *s++ = c;
    }
  if (s) *s = '\0';

  return (c == EOF) ? 0 : str;
}

// The following *printf functions aren't inline because
// they use varargs.

int
ACE_OS::printf (const char *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::printf");
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
#if defined (ACE_HAS_SNPRINTF)
  int result;
  va_list ap;
  va_start (ap, format);
#  if defined (ACE_WIN32)
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::_vsnprintf (buf, maxlen, format, ap)),
              int, -1, result);
  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buf[maxlen-1] = '\0';
#  else
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vsnprintf (buf, maxlen, format, ap)),
              int, -1, result);
#  endif /* ACE_WIN32 */
  va_end (ap);
  // In out-of-range conditions, C99 defines vsnprintf to return the number
  // of characters that would have been written if enough space was available.
  // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
  // -1. This method follows the C99 standard, but needs to guess at the
  // value; uses maxlen + 1.
  if (result == -1)
    result = ACE_static_cast (int, (maxlen + 1));
  return result;

#else
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SNPRINTF */
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::snprintf (wchar_t *buf, size_t maxlen, const wchar_t *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::snprintf");
#if (defined (_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) || defined (ACE_WIN32)
  int result;
  va_list ap;
  va_start (ap, format);
#  if defined (ACE_WIN32)
  // Microsoft's vswprintf() doesn't have the maxlen argument that
  // XPG4/UNIX98 define. They do, however, recommend use of _vsnwprintf()
  // as a substitute, which does have the same signature as the UNIX98 one.
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::_vsnwprintf (buf, maxlen, format, ap)),
              int, -1, result);
  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buf[maxlen-1] = '\0';
#  else
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vswprintf (buf, maxlen, format, ap)),
              int, -1, result);
#  endif /* ACE_WIN32 */
  va_end (ap);
  // In out-of-range conditions, C99 defines vsnprintf to return the number
  // of characters that would have been written if enough space was available.
  // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
  // -1. This method follows the C99 standard, but needs to guess at the
  // value; uses maxlen + 1.
  if (result == -1)
    result = ACE_static_cast (int, (maxlen + 1));
  return result;

#else
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SNPRINTF */
}
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::sprintf (char *buf, const char *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::sprintf");

  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vsprintf (buf, format, ap)), int, -1, result);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::sprintf (wchar_t *buf, const wchar_t *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::sprintf");

# if defined (_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)

  // The XPG4/UNIX98/C99 signature of the wide-char sprintf has a
  // maxlen argument. Since this method doesn't supply one, pass in
  // the max possible length. If this isn't ok, use ACE_OS::snprintf().
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vswprintf (buf, ULONG_MAX, format, ap), int, -1, result);
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

# endif /* ACE_HAS_VSWPRINTF */
}
#endif /* ACE_HAS_WCHAR */
