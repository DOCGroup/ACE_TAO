// $Id$

#include "ace/Lib_Find.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

ACE_RCSID(ace, Lib_Find, "$Id$")


#if ! defined (ACE_PSOS_DIAB_MIPS)
int
ACE_Lib_Find::ldfind (const ACE_TCHAR* filename,
                      ACE_TCHAR pathname[],
                      size_t maxpathnamelen)
{
  ACE_TRACE ("ACE_Lib_Find::ldfind");

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE) && \
    !defined (ACE_HAS_PHARLAP)
  ACE_TCHAR expanded_filename[MAXPATHLEN];
  if (ACE_TEXT_ExpandEnvironmentStrings (filename,
                                         expanded_filename,
                                         sizeof expanded_filename
                                         / sizeof (ACE_TCHAR)))
    filename = expanded_filename;
#endif /* ACE_WIN32 && !ACE_HAS_WINCE && !ACE_HAS_PHARLAP */

  ACE_TCHAR tempcopy[MAXPATHLEN + 1];
  ACE_TCHAR searchpathname[MAXPATHLEN + 1];
#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
  ACE_TCHAR decorator[] = ACE_LD_DECORATOR_STR;
  ACE_TCHAR searchfilename[MAXPATHLEN + sizeof(decorator) / sizeof (ACE_TCHAR)];
#else
  ACE_TCHAR searchfilename[MAXPATHLEN + 1];
#endif /* ACE_WIN32 && ACE_LD_DECORATOR_STR && !ACE_DISABLE_DEBUG_DLL_CHECK */

  // Create a copy of filename to work with.
  if (ACE_OS::strlen (filename) + 1
      > (sizeof tempcopy / sizeof (ACE_TCHAR)))
    {
      errno = ENOMEM;
      return -1;
    }
  else
    ACE_OS::strcpy (tempcopy, filename);

  // Insert canonical directory separators.
  ACE_TCHAR *separator_ptr;

#if (ACE_DIRECTORY_SEPARATOR_CHAR != '/')
  // Make all the directory separators "canonical" to simplify
  // subsequent code.
  ACE_Lib_Find::strrepl (tempcopy, ACE_DIRECTORY_SEPARATOR_CHAR, '/');
#endif /* ACE_DIRECTORY_SEPARATOR_CHAR */

  // Separate filename from pathname.
  separator_ptr = ACE_OS::strrchr (tempcopy, '/');

  // This is a relative path.
  if (separator_ptr == 0)
    {
      searchpathname[0] = '\0';
      ACE_OS::strcpy (searchfilename, tempcopy);
    }
  else // This is an absolute path.
    {
      ACE_OS::strcpy (searchfilename, separator_ptr + 1);
      separator_ptr[1] = '\0';
      ACE_OS::strcpy (searchpathname, tempcopy);
    }

  int got_suffix = 0;

  // Check to see if this has an appropriate DLL suffix for the OS
  // platform.
  ACE_TCHAR *s = ACE_OS::strrchr (searchfilename, '.');

  const ACE_TCHAR *dll_suffix = ACE_DLL_SUFFIX;

  if (s != 0)
    {
      // If we have a dot, we have a suffix
      got_suffix = 1;

      // Check whether this matches the appropriate platform-specific
      // suffix.
#if defined (ACE_WIN32)
      // Use <ACE_OS::strcasecmp> on any platform with
      // case-insensitive filenames.
      if (ACE_OS::strcasecmp (s, dll_suffix) != 0)
#else
      if (ACE_OS::strcmp (s, dll_suffix) != 0)
#endif /* ACE_WIN32 */
        {
          ACE_ERROR ((LM_WARNING,
                      ACE_LIB_TEXT ("Warning: improper suffix for a ")
                      ACE_LIB_TEXT ("shared library on this platform: %s\n"),
                      s));
        }
    }

  // Make sure we've got enough space in searchfilename.
  if (ACE_OS::strlen (searchfilename)
      + ACE_OS::strlen (ACE_DLL_PREFIX)
      + got_suffix ? 0 : ACE_OS::strlen (dll_suffix) >= (sizeof searchfilename /
                                                         sizeof (ACE_TCHAR)))
    {
      errno = ENOMEM;
      return -1;
    }

#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
  size_t len_searchfilename = ACE_OS::strlen (searchfilename);
  if (! got_suffix)
    ACE_OS_String::strcpy (searchfilename + len_searchfilename,
                           decorator);

  for (int tag = 1; tag >= 0; tag --)
    {
      if (tag == 0)
        searchfilename [len_searchfilename] = 0;

#endif /* ACE_WIN32 && ACE_LD_DECORATOR_STR && !ACE_DISABLE_DEBUG_DLL_CHECK */
      // Use absolute pathname if there is one.
      if (ACE_OS::strlen (searchpathname) > 0)
        {
          if (ACE_OS::strlen (searchfilename)
              + ACE_OS::strlen (searchpathname) >= maxpathnamelen)
            {
              errno = ENOMEM;
              return -1;
            }
          else
            {
#if (ACE_DIRECTORY_SEPARATOR_CHAR != '/')
              // Revert to native path name separators.
              ACE_Lib_Find::strrepl (searchpathname,
                                     '/',
                                     ACE_DIRECTORY_SEPARATOR_CHAR);
#endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
              // First, try matching the filename *without* adding a
              // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
              ACE_OS::sprintf (pathname,
                               ACE_LIB_TEXT ("%s%s%s"),
                               searchpathname,
                               searchfilename,
                               got_suffix ? ACE_static_cast (ACE_TCHAR *,
                                                             ACE_LIB_TEXT (""))
                               : ACE_static_cast (ACE_TCHAR *,
                                                  dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              ACE_OS::sprintf (pathname,
                               ACE_LIB_TEXT ("%s%s%s"),
                               searchpathname,
                               searchfilename,
                               got_suffix ? ACE_LIB_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              if (ACE_OS::access (pathname, F_OK) == 0)
                return 0;

              // Second, try matching the filename *with* adding a prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
              ACE_OS::sprintf (pathname,
                               ACE_LIB_TEXT ("%s%s%s%s"),
                               searchpathname,
                               ACE_DLL_PREFIX,
                               searchfilename,
                               got_suffix ? ACE_static_cast (ACE_TCHAR *,
                                                             ACE_LIB_TEXT (""))
                               : ACE_static_cast (ACE_TCHAR *,
                                                  dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              ACE_OS::sprintf (pathname,
                               ACE_LIB_TEXT ("%s%s%s%s"),
                               searchpathname,
                               ACE_DLL_PREFIX,
                               searchfilename,
                               got_suffix ? ACE_LIB_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              if (ACE_OS::access (pathname, F_OK) == 0)
                return 0;
            }
        }

      // Use relative filenames via LD_LIBRARY_PATH or PATH (depending on
      // OS platform).
      else
        {
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
          ACE_TCHAR *file_component = 0;
          DWORD pathlen = ACE_TEXT_SearchPath (0,
                                               searchfilename,
                                               dll_suffix,
                                               ACE_static_cast (DWORD,
                                                                maxpathnamelen),
                                               pathname,
                                               &file_component);
          if (pathlen >= maxpathnamelen)
          {
              errno = ENOMEM;
              return -1;
          }
          else if (pathlen > 0)
              return 0;
#else
          ACE_TCHAR *ld_path =
#if defined ACE_DEFAULT_LD_SEARCH_PATH
            ACE_DEFAULT_LD_SEARCH_PATH;
#else
            ACE_OS::getenv (ACE_LD_SEARCH_PATH);
#endif /* ACE_DEFAULT_LD_SEARCH_PATH */

#if defined (ACE_HAS_WINCE)
            ACE_TCHAR *ld_path_temp = 0;
            if (ld_path != 0)
              {
                ld_path_temp = (ACE_TCHAR *)
                  ACE_OS::malloc ((ACE_OS::strlen (ld_path) + 2)
                                  * sizeof (ACE_TCHAR));
                if (ld_path_temp != 0)
                  {
                    ACE_OS::strcpy (ld_path_temp,
                                    ACE_LD_SEARCH_PATH_SEPARATOR_STR);

                    ACE_OS::strcat (ld_path_temp, ld_path);
                    ld_path = ld_path_temp;
                  }
                else
                  {
                    ACE_OS::free ((void *) ld_path_temp);
                    ld_path = ld_path_temp = 0;
                  }
              }
#endif /* ACE_HAS_WINCE */

          if (ld_path != 0
              && (ld_path = ACE_OS::strdup (ld_path)) != 0)
            {
              // strtok has the strange behavior of not separating the
              // string ":/foo:/bar" into THREE tokens.  One would expect
              // that the first iteration the token would be an empty
              // string, the second iteration would be "/foo", and the
              // third iteration would be "/bar".  However, this is not
              // the case; one only gets two iterations: "/foo" followed
              // by "/bar".

              // This is especially a problem in parsing Unix paths
              // because it is permissible to specify 'the current
              // directory' as an empty entry.  So, we introduce the
              // following special code to cope with this:

              // Look at each dynamic lib directory in the search path.

              ACE_TCHAR *nextholder = 0;
              const ACE_TCHAR *path_entry =
                ACE_Lib_Find::strsplit_r (ld_path,
                                          ACE_LD_SEARCH_PATH_SEPARATOR_STR,
                                          nextholder);
              int result = 0;

              for (;;)
                {
                  // Check if at end of search path.
                  if (path_entry == 0)
                    {
                      errno = ENOENT;
                      result = -1;
                      break;
                    }
                  else if (ACE_OS::strlen (path_entry)
                           + 1
                           + ACE_OS::strlen (searchfilename)
                           >= maxpathnamelen)
                    {
                      errno = ENOMEM;
                      result = -1;
                      break;
                    }
                  // This works around the issue where a path might have
                  // an empty component indicating 'current directory'.
                  // We need to do it here rather than anywhere else so
                  // that the loop condition will still work.
                  else if (path_entry[0] == '\0')
                    path_entry = ACE_LIB_TEXT (".");

                  // First, try matching the filename *without* adding a
                  // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
                  ACE_OS::sprintf (pathname,
                                   ACE_LIB_TEXT ("%s%c%s%s"),
                                   path_entry,
                                   ACE_DIRECTORY_SEPARATOR_CHAR,
                                   searchfilename,
                                   got_suffix ? ACE_static_cast (ACE_TCHAR *,
                                                                 ACE_LIB_TEXT (""))
                                   : ACE_static_cast (ACE_TCHAR *,
                                                      dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
                  ACE_OS::sprintf (pathname,
                                   ACE_LIB_TEXT ("%s%c%s%s"),
                                   path_entry,
                                   ACE_DIRECTORY_SEPARATOR_CHAR,
                                   searchfilename,
                                   got_suffix ? ACE_LIB_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
                  if (ACE_OS::access (pathname, F_OK) == 0)
                    break;

                  // Second, try matching the filename *with* adding a
                  // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
                  ACE_OS::sprintf (pathname,
                                   ACE_LIB_TEXT ("%s%c%s%s%s"),
                                   path_entry,
                                   ACE_DIRECTORY_SEPARATOR_CHAR,
                                   ACE_DLL_PREFIX,
                                   searchfilename,
                                   got_suffix ? ACE_static_cast (ACE_TCHAR *,
                                                                 ACE_LIB_TEXT (""))
                                   : ACE_static_cast (ACE_TCHAR *,
                                                      dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
                  ACE_OS::sprintf (pathname,
                                   ACE_LIB_TEXT ("%s%c%s%s%s"),
                                   path_entry,
                                   ACE_DIRECTORY_SEPARATOR_CHAR,
                                   ACE_DLL_PREFIX,
                                   searchfilename,
                                   got_suffix ? ACE_LIB_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
                  if (ACE_OS::access (pathname, F_OK) == 0)
                    break;

                  // Fetch the next item in the path
                  path_entry = ACE_Lib_Find::strsplit_r (0,
                                                         ACE_LD_SEARCH_PATH_SEPARATOR_STR,
                                                         nextholder);
                }

#if defined (ACE_HAS_WINCE)
              if (ld_path_temp != 0)
                ACE_OS::free (ld_path_temp);
#endif /* ACE_HAS_WINCE */
              ACE_OS::free ((void *) ld_path);
#if defined (ACE_HAS_WINCE) && defined (ACE_LD_DECORATOR_STR) && \
            !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
               if (result == 0 || tag == 0)
#endif /* ACE_HAS_WINCE && ACE_LD_DECORATOR_STR && !ACE_DISABLE_DEBUG_DLL_CHECK */
              return result;
            }
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
        }
#if defined (ACE_WIN32) && defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    }
#endif /* ACE_WIN32 && ACE_LD_DECORATOR_STR && !ACE_DISABLE_DEBUG_DLL_CHECK */

  errno = ENOENT;
  return -1;
}

FILE *
ACE_Lib_Find::ldopen (const ACE_TCHAR *filename,
             const ACE_TCHAR *type)
{
  ACE_TRACE ("ACE_Lib_Find::ldopen");

  ACE_TCHAR buf[MAXPATHLEN + 1];
  if (ACE_Lib_Find::ldfind (filename,
                   buf,
                   sizeof (buf) /sizeof (ACE_TCHAR)) == -1)
    return 0;
  else
    return ACE_OS::fopen (buf, type);
}

ACE_TCHAR *
ACE_Lib_Find::ldname (const ACE_TCHAR *entry_point)
{
  ACE_TRACE ("ACE_Lib_Find::ldname");

#if defined(ACE_NEEDS_DL_UNDERSCORE)
  size_t size =
    1 // leading '_'
    + ACE_OS::strlen (entry_point)
    + 1;

  ACE_TCHAR *new_name;
  ACE_NEW_RETURN (new_name,
                  ACE_TCHAR[size],
                  0);

  ACE_OS::strcpy (new_name, ACE_LIB_TEXT ("_"));
  ACE_OS::strcat (new_name, entry_point);

  return new_name;
#else /* ACE_NEEDS_DL_UNDERSCORE */
  size_t size =
    ACE_OS::strlen (entry_point)
    + 1;

  ACE_TCHAR *new_name;
  ACE_NEW_RETURN (new_name,
                  ACE_TCHAR[size],
                  0);

  ACE_OS::strcpy (new_name, entry_point);

  return new_name;
#endif /* ACE_NEEDS_DL_UNDERSCORE */
}

int
ACE_Lib_Find::get_temp_dir (ACE_TCHAR *buffer, size_t buffer_len)
{
  int result;
#if defined (ACE_WIN32)
  result = ACE_TEXT_GetTempPath (ACE_static_cast (DWORD, buffer_len),
                                 buffer);

  // Make sure to return -1 if there is an error
  if (result == 0 && ::GetLastError () != ERROR_SUCCESS
      || result > ACE_static_cast (int, buffer_len))
    result = -1;

#else /* ACE_WIN32 */

  // On non-win32 platforms, check to see what the TMPDIR environment
  // variable is defined to be.  If it doesn't exist, just use /tmp
  const ACE_TCHAR *tmpdir = ACE_OS::getenv (ACE_LIB_TEXT ("TMPDIR"));

  if (tmpdir == 0)
    tmpdir = ACE_LIB_TEXT ("/tmp");

  size_t len = ACE_OS::strlen (tmpdir);

  // Check to see if the buffer is large enough for the string,
  // another /, and its null character (hence the + 2)
  if ((len + 2) > buffer_len)
    {
      result = -1;
    }
  else
    {
      ACE_OS::strcpy (buffer, tmpdir);

      // Add a trailing slash because we cannot assume there is already one
      // at the end.  And having an extra one should not cause problems.
      buffer[len] = '/';
      buffer[len + 1] = 0;
      result = 0;
    }
#endif /* ACE_WIN32 */
  return result;
}

ACE_HANDLE
ACE_Lib_Find::open_temp_file (const ACE_TCHAR *name, int mode, int perm)
{
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG(perm);
  return ACE_OS::open (name,
                       mode | _O_TEMPORARY);
#else
  // Open it.
  ACE_HANDLE handle = ACE_OS::open (name, mode, perm);

  if (handle == ACE_INVALID_HANDLE)
    return ACE_INVALID_HANDLE;

  // Unlink it so that the file will be removed automatically when the
  // process goes away.
  if (ACE_OS::unlink (name) == -1)
    return -1;
  else
    // Return the handle.
    return handle;
#endif /* ACE_WIN32 */
}
#endif /* ! ACE_PSOS_DIAB_MIPS */

size_t
ACE_Lib_Find::strrepl (char *s, char search, char replace)
{
  ACE_TRACE ("ACE_Lib_Find::strrepl");

  size_t replaced = 0;

  for (size_t i = 0; s[i] != '\0'; i++)
    if (s[i] == search)
      {
        s[i] = replace;
        replaced++;
      }

  return replaced;
}


// Split a string up into 'token'-delimited pieces, ala Perl's
// "split".

char *
ACE_Lib_Find::strsplit_r (char *str,
                          const char *token,
                          char *&next_start)
{
  char *result = 0;

  if (str != 0)
    next_start = str;

  if (next_start != 0)
    {
      char *tok_loc = ACE_OS::strstr (next_start, token);

      if (tok_loc != 0)
        {
          // Return the beginning of the string.
          result = next_start;

          // Insure it's terminated.
          *tok_loc = '\0';
          next_start = tok_loc + ACE_OS::strlen (token);
        }
      else
        {
          result = next_start;
          next_start = (char *) 0;
        }
    }

  return result;
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_Lib_Find::strsplit_r (wchar_t *str,
                          const wchar_t *token,
                          wchar_t *&next_start)
{
  wchar_t *result = 0;

  if (str != 0)
    next_start = str;

  if (next_start != 0)
    {
      wchar_t *tok_loc = ACE_OS::strstr (next_start, token);

      if (tok_loc != 0)
        {
          // Return the beginning of the string.
          result = next_start;

          // Insure it's terminated.
          *tok_loc = '\0';
          next_start = tok_loc + ACE_OS::strlen (token);
        }
      else
        {
          result = next_start;
          next_start = (wchar_t *) 0;
        }
    }

  return result;
}

size_t
ACE_Lib_Find::strrepl (wchar_t *s, wchar_t search, wchar_t replace)
{
  ACE_TRACE ("ACE_Lib_Find::strrepl");

  size_t replaced = 0;

  for (size_t i = 0; s[i] != '\0'; i++)
    if (s[i] == search)
      {
        s[i] = replace;
        replaced++;
      }

  return replaced;
}
#endif /* ACE_HAS_WCHAR */
