// $Id$

#define ACE_BUILD_DLL
#include "ace/ACE.h"
#include "ace/Handle_Set.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Object_Manager.h"
#include "ace/Version.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/ACE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, ACE, "$Id$")

// Keeps track of whether we're in some global debug mode.
char ACE::debug_ = 0;

// Hex characters.
const char ACE::hex_chars_[] = "0123456789abcdef";

// Size of a VM page.
size_t ACE::pagesize_ = 0;

// Size of allocation granularity.
size_t ACE::allocation_granularity_ = 0;


// If on Win32 and using non-static Object Manager, compile in a DllMain
// which will init the ACE library upon loading and tear it down on unloading.
#if defined (ACE_WIN32) && defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
  switch( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
      ACE::init();
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      ACE::fini();
      break;
    }

  return TRUE;
}
#endif /* ACE_WIN32 && ACE_HAS_NONSTATIC_OBJECT_MANAGER */


int
ACE::init (void)
{
  return ACE_Object_Manager::instance ()->init ();
}

int
ACE::fini (void)
{
  return ACE_Object_Manager::instance ()->fini ();
}

u_int
ACE::major_version (void)
{
  return ACE_MAJOR_VERSION;
}

u_int
ACE::minor_version (void)
{
  return ACE_MINOR_VERSION;
}

u_int
ACE::beta_version (void)
{
  return ACE_BETA_VERSION;
}

const char*
ACE::compiler_name (void)
{
#ifdef ACE_CC_NAME
  return ACE_CC_NAME;
#else
  return "";
#endif
}

u_int
ACE::compiler_major_version (void)
{
#ifdef ACE_CC_MAJOR_VERSION
  return ACE_CC_MAJOR_VERSION;
#else
  return 0;
#endif
}

u_int
ACE::compiler_minor_version (void)
{
#ifdef ACE_CC_MINOR_VERSION
  return ACE_CC_MINOR_VERSION;
#else
  return 0;
#endif
}

u_int
ACE::compiler_beta_version (void)
{
#ifdef ACE_CC_BETA_VERSION
  return ACE_CC_BETA_VERSION;
#else
  return 0;
#endif
}

int
ACE::terminate_process (pid_t pid)
{
#if defined (ACE_WIN32)
  // Create a handle for the given process id.
  ACE_HANDLE process_handle =
    ::OpenProcess (PROCESS_TERMINATE,
                   FALSE, // New handle is not inheritable.
                   pid);

  if (process_handle == ACE_INVALID_HANDLE
      || process_handle == NULL)
    return -1;
  else
    {
      // Kill the process associated with process_handle.
      BOOL terminate_result =
        ::TerminateProcess (process_handle, 0);
      // Free up the kernel resources.
      ACE_OS::close (process_handle);
      return terminate_result;
    }
#elif defined (CHORUS)
  KnCap cap_;

  // Use the pid to find out the actor's capability, then kill it.
  if (::acap (AM_MYSITE, pid, &cap_) == 0)
    return ::akill (&cap_);
  else
    return -1;
#else
  return ACE_OS::kill (pid, 9);
#endif /* ACE_WIN32 */
}

int
ACE::process_active (pid_t pid)
{
#if !defined(ACE_WIN32)
  int retval = ACE_OS::kill (pid, 0);

  if (retval == 0)
    return 1;
  else if (errno == ESRCH)
    return 0;
  else
    return -1;
#else
  // Create a handle for the given process id.
  ACE_HANDLE process_handle =
    ::OpenProcess (PROCESS_QUERY_INFORMATION, FALSE, pid);
  if (process_handle == ACE_INVALID_HANDLE
      || process_handle == NULL)
    return 0;
  else
    {
      DWORD status;
      if (::GetExitCodeProcess (process_handle,
                                &status) == 0
          || status != STILL_ACTIVE)
        return 0;
      else
        return 1;
    }
#endif /* ACE_WIN32 */
}

// Split a string up into 'token'-delimited pieces, ala Perl's
// "split".

char *
ACE::strsplit_r (char *str,
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

const char *
ACE::execname (const char *old_name)
{
#if defined (ACE_HAS_WIN32)
  if (ACE_OS::strstr (old_name, ".exe") == 0)
    {
      char *new_name;

      size_t size =
        ACE_OS::strlen (old_name)
        + ACE_OS::strlen (".exe")
        + 1;

      ACE_NEW_RETURN (new_name,
                      char[size],
                      -1);
      char *end = new_name;

      end = ACE_OS::strecpy (new_name, old_name);

      // Concatenate the .exe suffix onto the end of the executable.
      ACE_OS::strcpy (end, ".exe");

      return new_name;
    }
#endif /* ACE_HAS_WIN32 */
  return old_name;
}

#if defined (ACE_HAS_UNICODE)
size_t
ACE::strrepl (wchar_t *s, wchar_t search, wchar_t replace)
{
  ACE_TRACE ("ACE::strrepl");

  size_t replaced = 0;

  for (size_t i = 0; s[i] != '\0'; i++)
    if (s[i] == search)
      {
        s[i] = replace;
        replaced++;
      }

  return replaced;
}

wchar_t *
ACE::strsplit_r (wchar_t *str,
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

const wchar_t *
ACE::execname (const wchar_t *old_name)
{
#if defined (ACE_HAS_WIN32)
  if (ACE_OS::strstr (old_name, L".exe") == 0)
    {
      wchar_t *new_name;

      size_t size =
        ACE_OS::strlen (old_name)
        + ACE_OS::strlen (L".exe")
        + 1;

      ACE_NEW_RETURN (new_name,
                      wchar_t[size],
                      -1);
      wchar_t *end = new_name;

      end = ACE_OS::strecpy (new_name, old_name);

      // Concatenate the .exe suffix onto the end of the executable.
      ACE_OS::strcpy (end, L".exe");

      return new_name;
    }
#endif /* ACE_HAS_WIN32 */
  return old_name;
}
#endif /* ACE_HAS_UNICODE */

u_long
ACE::hash_pjw (const char *str, size_t len)
{
  u_long hash = 0;

  for (size_t i = 0; i < len; i++)
    {
      const char temp = str[i];
      hash = (hash << 4) + (temp * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

u_long
ACE::hash_pjw (const char *str)
{
  return ACE::hash_pjw (str, ACE_OS::strlen (str));
}

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
u_long
ACE::hash_pjw (const wchar_t *str, size_t len)
{
  u_long hash = 0;

  for (size_t i = 0; i < len; i++)
    {
      const wchar_t temp = str[i];
      hash = (hash << 4) + (temp * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

u_long
ACE::hash_pjw (const wchar_t *str)
{
  return ACE::hash_pjw (str, ACE_OS::strlen (str));
}
#endif /* ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
u_long
ACE::hash_pjw (const ACE_USHORT16 *str, size_t len)
{
  u_long hash = 0;

  for (size_t i = 0; i < len; i++)
    {
      const ACE_USHORT16 temp = str[i];
      hash = (hash << 4) + (temp * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

u_long
ACE::hash_pjw (const ACE_USHORT16 *str)
{
  return ACE::hash_pjw (str, ACE_OS::strlen (str));
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

// The CRC routine was taken from the FreeBSD implementation of cksum,
// that falls under the following license:
/*-
 * Copyright (c) 1991, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * James W. Williams of NASA Goddard Space Flight Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

u_long ACE::crc_table_[] =
{
  0x0,
  0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
  0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6,
  0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
  0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac,
  0x5bd4b01b, 0x569796c2, 0x52568b75, 0x6a1936c8, 0x6ed82b7f,
  0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a,
  0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
  0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58,
  0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033,
  0xa4ad16ea, 0xa06c0b5d, 0xd4326d90, 0xd0f37027, 0xddb056fe,
  0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
  0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4,
  0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
  0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5,
  0x2ac12072, 0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
  0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca, 0x7897ab07,
  0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c,
  0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1,
  0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
  0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b,
  0xbb60adfc, 0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698,
  0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d,
  0x94ea7b2a, 0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
  0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2, 0xc6bcf05f,
  0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
  0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80,
  0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
  0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a,
  0x58c1663d, 0x558240e4, 0x51435d53, 0x251d3b9e, 0x21dc2629,
  0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c,
  0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
  0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e,
  0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65,
  0xeba91bbc, 0xef68060b, 0xd727bbb6, 0xd3e6a601, 0xdea580d8,
  0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
  0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2,
  0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
  0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74,
  0x857130c3, 0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
  0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c, 0x7b827d21,
  0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a,
  0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e, 0x18197087,
  0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
  0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d,
  0x2056cd3a, 0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce,
  0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb,
  0xdbee767c, 0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
  0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4, 0x89b8fd09,
  0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
  0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf,
  0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

// Compute a POSIX 1003.2 checksum.  The routine takes an string and
// computes the CRC for it (it stops on the first '\0' character).

u_long
ACE::crc32 (const char *string)
{
// UNICOS UINT32's are 64-bit on the Cray PVP architecture
#if !defined(_UNICOS)
#  define COMPUTE(var, ch) (var) = ((var) << 8) ^ ACE::crc_table_[((var) >> 24) ^ (ch)]
#else /* ! _UNICOS */
#  define COMPUTE(var, ch) (var) = ( 0x00000000ffffffff & ((var) << 8)) ^ ACE::crc_table_[((var) >> 24) ^ (ch)]
#endif /* ! _UNICOS */
  register ACE_UINT32 crc = 0;

  u_long len = 0;

  for (const char *p = string;
       *p != 0;
       ++p)
    {
      COMPUTE (crc, *p);
      ++len;
    }

  // Include the length of the string.

  for (; len != 0; len >>= 8)
    COMPUTE (crc, len & 0xff);

  return ~crc;
}

size_t
ACE::strrepl (char *s, char search, char replace)
{
  ACE_TRACE ("ACE::strrepl");

  size_t replaced = 0;

  for (size_t i = 0; s[i] != '\0'; i++)
    if (s[i] == search)
      {
        s[i] = replace;
        replaced++;
      }

  return replaced;
}

#if !defined (ACE_HAS_WINCE)
char *
ACE::strenvdup (const char *str)
{
  ACE_TRACE ("ACE::strenvdup");

  return ACE_OS::strenvdup (str);
}
#endif /* ACE_HAS_WINCE */

/*

Examples:

Source               NT                    UNIX
==================================================================
netsvc               netsvc.dll            libnetsvc.so
                     (PATH will be         (LD_LIBRARY_PATH
                      evaluated)            evaluated)

libnetsvc.dll        libnetsvc.dll         libnetsvc.dll + warning
netsvc.so            netsvc.so + warning   libnetsvc.so

..\../libs/netsvc    ..\..\libs\netsvc.dll ../../libs/netsvc.so
                     (absolute path used)  (absolute path used)

*/

#if ! defined (ACE_PSOS_DIAB_MIPS)
int
ACE::ldfind (const ASYS_TCHAR filename[],
             ASYS_TCHAR pathname[],
             size_t maxpathnamelen)
{
  ACE_TRACE ("ACE::ldfind");

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ASYS_TCHAR expanded_filename[MAXPATHLEN];
  if (::ExpandEnvironmentStringsA (filename,
                                   expanded_filename,
                                   sizeof expanded_filename))
    filename = expanded_filename;
#endif /* ACE_WIN32 */

  ASYS_TCHAR tempcopy[MAXPATHLEN + 1];
  ASYS_TCHAR searchpathname[MAXPATHLEN + 1];
  ASYS_TCHAR searchfilename[MAXPATHLEN + 2];

  // Create a copy of filename to work with.
  if (ACE_OS::strlen (filename) + 1
      > (sizeof tempcopy / sizeof (ASYS_TCHAR)))
    {
      errno = ENOMEM;
      return -1;
    }
  else
    ACE_OS::strcpy (tempcopy, filename);

  // Insert canonical directory separators.
  ASYS_TCHAR *separator_ptr;

#if (ACE_DIRECTORY_SEPARATOR_CHAR != '/')
  // Make all the directory separators ``canonical'' to simplify
  // subsequent code.
  ACE::strrepl (tempcopy, ACE_DIRECTORY_SEPARATOR_CHAR, '/');
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
  ASYS_TCHAR *s = ACE_OS::strrchr (searchfilename, '.');

  const ASYS_TCHAR *dll_suffix =
#if !defined (ACE_HAS_MOSTLY_UNICODE_APIS)
    ACE_DLL_SUFFIX;
#else
    _TEXT (ACE_DLL_SUFFIX);
#endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

  if (s != 0)
    {
      // If we have a dot, we have a suffix
      got_suffix = 1;

      // Check whether this matches the appropriate platform-specific
      // suffix.
      if (ACE_OS::strcmp (s, dll_suffix) != 0)
        {
          ACE_ERROR ((LM_WARNING,
                      ASYS_TEXT ("Warning: improper suffix for a ")
                      ASYS_TEXT ("shared library on this platform: %s\n"),
                      s));
        }
    }

  // Make sure we've got enough space in searchfilename.
  if (ACE_OS::strlen (searchfilename)
      + ACE_OS::strlen (ACE_DLL_PREFIX)
      + got_suffix ? 0 : ACE_OS::strlen (dll_suffix) >= (sizeof searchfilename /
                                                         sizeof (ASYS_TCHAR)))
    {
      errno = ENOMEM;
      return -1;
    }

#if defined (ACE_WIN32) && defined (_DEBUG) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
  size_t len_searchfilename = ACE_OS::strlen (searchfilename);
  if (! got_suffix)
    {
      searchfilename [len_searchfilename] = 'd';
      searchfilename [len_searchfilename+1] = 0;
    }

  for (int tag = 1; tag >= 0; tag --)
    {
      if (tag == 0)
        searchfilename [len_searchfilename] = 0;

#endif /* ACE_WIN32 && _DEBUG && !ACE_DISABLE_DEBUG_DLL_CHECK */
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
          ACE::strrepl (searchpathname,
                        '/',
                        ACE_DIRECTORY_SEPARATOR_CHAR);
#endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
          // First, try matching the filename *without* adding a
          // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
          ACE_OS::sprintf (pathname,
                           ASYS_TEXT ("%s%s%s"),
                           searchpathname,
                           searchfilename,
                           got_suffix ? ACE_static_cast (char *,
                                                         ASYS_TEXT (""))
                                      : ACE_static_cast (char *,
                                                         dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
          ACE_OS::sprintf (pathname,
                           ASYS_TEXT ("%s%s%s"),
                           searchpathname,
                           searchfilename,
                           got_suffix ? ASYS_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
          if (ACE_OS::access (pathname, F_OK) == 0)
            return 0;

          // Second, try matching the filename *with* adding a prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
          ACE_OS::sprintf (pathname,
                           ASYS_TEXT ("%s%s%s%s"),
                           searchpathname,
                           ACE_DLL_PREFIX,
                           searchfilename,
                           got_suffix ? ACE_static_cast (char *,
                                                         ASYS_TEXT (""))
                                      : ACE_static_cast (char *,
                                                         dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
          ACE_OS::sprintf (pathname,
                           ASYS_TEXT ("%s%s%s%s"),
                           searchpathname,
                           ACE_DLL_PREFIX,
                           searchfilename,
                           got_suffix ? ASYS_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
          if (ACE_OS::access (pathname, F_OK) == 0)
            return 0;
        }
    }

  // Use relative filenames via LD_LIBRARY_PATH or PATH (depending on
  // OS platform).
  else
    {
      char *ld_path = ACE_OS::getenv (ACE_LD_SEARCH_PATH);

#if defined (ACE_WIN32)
      char *ld_path_temp = 0;
      if (ld_path != 0)
        {
          ld_path_temp = (char *) ACE_OS::malloc (ACE_OS::strlen (ld_path) + 2);
          if (ld_path_temp != 0)
            {
              ACE_OS::strcpy (ld_path_temp, ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              ACE_OS::strcat (ld_path_temp, ld_path);
              ld_path = ld_path_temp;
            }
          else
            {
              ACE_OS::free ((void *) ld_path_temp);
              ld_path = ld_path_temp = 0;
            }
        }
#endif /* ACE_WIN32 */

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

          char *nextholder = 0;
          const char *path_entry =
            ACE::strsplit_r (ld_path,
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
                path_entry = ".";

              // First, try matching the filename *without* adding a
              // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
              ACE_OS::sprintf (pathname,
                               ASYS_TEXT ("%s%c%s%s"),
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               searchfilename,
                               got_suffix ? ACE_static_cast (char *,
                                                             ASYS_TEXT (""))
                                          : ACE_static_cast (char *,
                                                             dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              ACE_OS::sprintf (pathname,
                               ASYS_TEXT ("%s%c%s%s"),
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               searchfilename,
                               got_suffix ? ASYS_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              if (ACE_OS::access (pathname, F_OK) == 0)
                break;

              // Second, try matching the filename *with* adding a
              // prefix.
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
              ACE_OS::sprintf (pathname,
                               ASYS_TEXT ("%s%c%s%s%s"),
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               ACE_DLL_PREFIX,
                               searchfilename,
                               got_suffix ? ACE_static_cast (char *,
                                                            ASYS_TEXT (""))
                                          : ACE_static_cast (char *,
                                                             dll_suffix));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              ACE_OS::sprintf (pathname,
                               ASYS_TEXT ("%s%c%s%s%s"),
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               ACE_DLL_PREFIX,
                               searchfilename,
                               got_suffix ? ASYS_TEXT ("") : dll_suffix);
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
              if (ACE_OS::access (pathname, F_OK) == 0)
                break;

              // Fetch the next item in the path
              path_entry = ACE::strsplit_r (0,
                                            ACE_LD_SEARCH_PATH_SEPARATOR_STR,
                                            nextholder);
            }

#if defined (ACE_WIN32)
          if (ld_path_temp != 0)
            ACE_OS::free (ld_path_temp);
#endif /* ACE_WIN32 */
          ACE_OS::free ((void *) ld_path);
#if defined (ACE_WIN32) && defined (_DEBUG) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
          if (result == 0 || tag == 0)
#endif /* ACE_WIN32 && _DEBUG && !ACE_DISABLE_DEBUG_DLL_CHECK */
            return result;
        }
    }
#if defined (ACE_WIN32) && defined (_DEBUG) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
    }
#endif /* ACE_WIN32 && _DEBUG && !ACE_DISABLE_DEBUG_DLL_CHECK */

  errno = ENOENT;
  return -1;
}

FILE *
ACE::ldopen (const ASYS_TCHAR *filename,
             const ASYS_TCHAR *type)
{
  ACE_TRACE ("ACE::ldopen");

  ASYS_TCHAR buf[MAXPATHLEN + 1];
  if (ACE::ldfind (filename,
                   buf,
                   sizeof (buf) /sizeof (ASYS_TCHAR)) == -1)
    return 0;
  else
    return ACE_OS::fopen (buf, type);
}

ACE_HANDLE
ACE::open_temp_file (const char *name, int mode, int perm)
{
#if defined (ACE_WIN32)
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

const char *
ACE::basename (const char *pathname, char delim)
{
  ACE_TRACE ("ACE::basename");
  const char *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    return pathname;
  else
    return temp + 1;
}

const char *
ACE::dirname (const char *pathname, char delim)
{
  ACE_TRACE ("ACE::basename");
  static char return_dirname[MAXPATHLEN + 1];

  const char *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    {
      if (ACE_OS::strcmp (pathname, ".") == 0
          || ACE_OS::strcmp (pathname, "..") == 0)
        return_dirname[0] = '.';
      else
        return_dirname[0] = delim;

      return return_dirname;
    }
  else
    {
      ACE_OS::strncpy (return_dirname,
                       pathname,
                       MAXPATHLEN);
      return_dirname[temp - pathname] = '\0';
      return return_dirname;
    }
}

#if defined (ACE_HAS_UNICODE)
const wchar_t *
ACE::basename (const wchar_t *pathname, wchar_t delim)
{
  ACE_TRACE ("ACE::basename");
  const wchar_t *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    return pathname;
  else
    return temp + 1;
}
#endif /* ACE_HAS_UNICODE */

// Send N char *ptrs and int lengths.  Note that the char *'s precede
// the ints (basically, an varargs version of writev).  The count N is
// the *total* number of trailing arguments, *not* a couple of the
// number of tuple pairs!

ssize_t
ACE::send (ACE_HANDLE handle, size_t n, ...)
{
  ACE_TRACE ("ACE_SOCK_IO::send");

  va_list argp;
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp, iovec[total_tuples], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  va_start (argp, n);

  for (size_t i = 0; i < total_tuples; i++)
    {
      iovp[i].iov_base = va_arg (argp, char *);
      iovp[i].iov_len = va_arg (argp, ssize_t);
    }

  ssize_t result = ACE_OS::sendv (handle, iovp, total_tuples);
#if !defined (ACE_HAS_ALLOCA)
  delete [] iovp;
#endif /* !defined (ACE_HAS_ALLOCA) */
  va_end (argp);
  return result;
}

// This is basically an interface to ACE_OS::readv, that doesn't use
// the struct iovec explicitly.  The ... can be passed as an arbitrary
// number of (char *ptr, int len) tuples.  However, the count N is the
// *total* number of trailing arguments, *not* a couple of the number
// of tuple pairs!

ssize_t
ACE::recv (ACE_HANDLE handle, size_t n, ...)
{
  ACE_TRACE ("ACE_SOCK_IO::recv");

  va_list argp;
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp, iovec[total_tuples], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  va_start (argp, n);

  for (size_t i = 0; i < total_tuples; i++)
    {
      iovp[i].iov_base = va_arg (argp, char *);
      iovp[i].iov_len = va_arg (argp, ssize_t);
    }

  ssize_t result = ACE_OS::recvv (handle, iovp, total_tuples);
#if !defined (ACE_HAS_ALLOCA)
  delete [] iovp;
#endif /* !defined (ACE_HAS_ALLOCA) */
  va_end (argp);
  return result;
}

// Miscellaneous static methods used throughout ACE.

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             int flags)
{
  ACE_TRACE ("ACE::send_n");
  size_t bytes_written;
  ssize_t n;

  for (bytes_written = 0; bytes_written < len; bytes_written += n)
    {
      n = ACE_OS::send (handle,
                        (const char *) buf + bytes_written,
                        len - bytes_written,
                        flags);
      if (n == -1)
        {
          if (errno == EWOULDBLOCK)
            n = 0; // Keep trying to send.
          else
            return -1;
        }
    }

  return bytes_written;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle, void *buf, size_t len)
{
  ACE_TRACE ("ACE::recv_n");
  size_t bytes_read;
  ssize_t n;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    {
      n = ACE::recv (handle,
                     (char *) buf + bytes_read,
                     len - bytes_read);
      if (n == -1)
        {
          if (errno == EWOULDBLOCK)
            n = 0; // Keep trying to read.
          else
            return -1;
        }
      else if (n == 0)
        break;
    }

  return bytes_read;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle, void *buf, size_t len, int flags)
{
  ACE_TRACE ("ACE::recv_n");
  size_t bytes_read;
  ssize_t n;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    {
      n = ACE::recv (handle,
                     (char *) buf + bytes_read,
                     len - bytes_read,
                     flags);

      if (n == -1)
        {
          if (errno == EWOULDBLOCK)
            n = 0; // Keep trying to read.
          else
            return -1;
        }
      else if (n == 0)
        break;
    }

  return bytes_read;
}

int
ACE::enter_recv_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int &val)
{
  // Give value a default value to keep Purify happy!
  val = 0;

  if (timeout == 0)
    return 0;

  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // Wait for input to arrive or for the timeout to elapse.
  int a = ACE_OS::select (int (handle) + 1,
                          (fd_set *) handle_set, // read_fds.
                          (fd_set *) 0, // write_fds.
                          (fd_set *) 0, // exception_fds.
                          timeout);
   switch ( a )
   {
   case 0:  // Timer expired.  return -1
      errno = ETIME;
      /* FALLTHRU */
   case -1: // we got here directly - select() returned -1.
      return -1;
   case 1: // OK to read now.
      /* FALLTHRU */
   default: // default is case a > 0; return a
      // really should assert if a != 1
      //assert( a == 1 );
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_DISABLED (val, ACE_NONBLOCK))
        // Set the handle into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return a;
   }
}

void
ACE::leave_recv_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int val)
{
  if (timeout != 0
      && ACE_BIT_DISABLED (val, ACE_NONBLOCK))
    {
      // We need to stash errno here because ACE::clr_flags() may
      // reset it.
      int error = errno;

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (handle, ACE_NONBLOCK);
      errno = error;
    }
}

int
ACE::enter_send_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value* timeout,
                           int &val)
{
  // Give value a default value to keep Purify happy!
  val = 0;

  if (timeout==0)
    return 0;

  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // On timed writes we always go into select(); only if the
  // handle is available for writing within the specified amount
  // of time do we put it in non-blocking mode

  int a = ACE_OS::select (int (handle) + 1,
                          (fd_set *) 0,
                          (fd_set *) handle_set,
                          (fd_set *) 0,
                          timeout);
   switch ( a )
   {
   case 0: // Timer expired.
      errno = ETIME;
      /* FALLTHRU */
   case -1: // we got here directly - select() returned -1.
      return -1;
   case 1: // Ok to write now.
      /* FALLTHRU */
   default: // default is case a > 0; return a
      // really should assert if a != 1
      //assert( a == 1 );
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_DISABLED (val, ACE_NONBLOCK))
        // Set the handle into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return a;
    }
}

void
ACE::leave_send_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int val)
{
  if (timeout != 0
      && ACE_BIT_DISABLED (val, ACE_NONBLOCK))
    {
      // We need to stash errno here because ACE::clr_flags() may
      // reset it.
      int error = errno;

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (handle, ACE_NONBLOCK);
      errno = error;
    }
}

ssize_t
ACE::sendto (ACE_HANDLE handle,
             const char *buf,
             int len,
             int flags,
             const struct sockaddr *addr,
             int addrlen,
             const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::sendto");
#if defined (ACE_HAS_SENDTO_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::sendto (handle, buf, len, flags, addr, addrlen);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendto_timedwait (handle,
                                 buf,
                                 len,
                                 flags,
                                 addr,
                                 addrlen,
                                 &ts);
    }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      int bytes_written = ACE_OS::sendto (handle,
                                          buf,
                                          len,
                                          flags,
                                          addr,
                                          addrlen);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SENDTO_TIMEDWAIT */
}

ssize_t
ACE::sendmsg (ACE_HANDLE handle,
              const struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::sendmsg");
#if defined (ACE_HAS_SENDMSG_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::sendmsg (handle, msg, flags);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendmsg_timedwait (handle, msg, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      int bytes_written = ACE_OS::sendmsg (handle, msg, flags);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SENDMSG_TIMEDWAIT */
}

ssize_t
ACE::readv (ACE_HANDLE handle,
            iovec *iov,
            int iovcnt,
            const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::readv");
#if defined (ACE_HAS_READV_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::readv (handle, iov, iovcnt);
  else {
     ACE_Time_Value copy = *timeout;
     copy += ACE_OS::gettimeofday ();
     timespec_t ts = copy;
     return ::readv_timedwait (handle, iov, iovcnt, &ts);
  }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
     return -1;
  else
    {
      ssize_t bytes_read = ACE_OS::readv (handle, iov, iovcnt);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_READV_TIMEDWAIT */
}

ssize_t
ACE::writev (ACE_HANDLE handle,
             const iovec *iov,
             int iovcnt,
             const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::writev");
#if defined (ACE_HAS_WRITEV_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::writev (handle, iov, iovcnt);
  else {
     ACE_Time_Value copy = *timeout;
     copy += ACE_OS::gettimeofday ();
     timespec_t ts = copy;
     return ::writev_timedwait (handle, iov, iovcnt, &ts);
  }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
     return -1;
  else
    {
      ssize_t bytes_written = ACE_OS::writev (handle, iov, iovcnt);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_WRITEV_TIMEDWAIT */
}

ssize_t
ACE::writev_n (ACE_HANDLE h,
               const iovec *i,
               int iovcnt)
{
  ssize_t writelen = 0;
  iovec *iov = ACE_const_cast (iovec *, i);

  for (int s = 0;
       s < iovcnt;
       )
    {
      ssize_t n = ACE_OS::writev (h,
                                  iov + s,
                                  iovcnt - s);
      if (n == -1)
        return n;
      else
        {
          for (writelen += n;
               s < iovcnt
                 && n >= ACE_static_cast (ssize_t,
                                          iov[s].iov_len);
               s++)
            n -= iov[s].iov_len;

          if (n != 0)
            {
              char *base = ACE_reinterpret_cast (char *,
                                                 iov[s].iov_base);
              iov[s].iov_base = base + n;
              iov[s].iov_len = iov[s].iov_len - n;
            }
        }
    }

  return writelen;
}

ssize_t
ACE::sendv_n (ACE_HANDLE h,
              const iovec *i,
              int iovcnt)
{
  ssize_t writelen = 0;
  iovec *iov = ACE_const_cast (iovec *, i);

  for (int s = 0;
       s < iovcnt;
       )
    {
      ssize_t n = ACE_OS::sendv (h,
                                 iov + s,
                                 iovcnt - s);
      if (n == -1)
        return n;
      else
        {
          for (writelen += n;
               s < iovcnt
                 && n >= ACE_static_cast (ssize_t,
                                          iov[s].iov_len);
               s++)
            n -= iov[s].iov_len;

          if (n != 0)
            {
              char *base = ACE_reinterpret_cast (char *,
                                                 iov[s].iov_base);
              iov[s].iov_base = base + n;
              iov[s].iov_len = iov[s].iov_len - n;
            }
        }
    }

  return writelen;
}

// Format buffer into printable format.  This is useful for debugging.
// Portions taken from mdump by J.P. Knight (J.P.Knight@lut.ac.uk)
// Modifications by Todd Montgomery.

int
ACE::format_hexdump (const char *buffer, int size,
                     ASYS_TCHAR *obuf, int obuf_sz)
{
  ACE_TRACE ("ACE::format_hexdump");

  u_char c;
  ASYS_TCHAR textver[16 + 1];

  int maxlen = (obuf_sz / 68) * 16;

  if (size > maxlen)
    size = maxlen;

  int i;

  for (i = 0; i < (size >> 4); i++)
    {
      int j;

      for (j = 0 ; j < 16; j++)
        {
          c = (u_char) buffer[(i << 4) + j];
          ACE_OS::sprintf (obuf, ASYS_TEXT ("%02x "), c);
          obuf += 3;
          if (j == 7)
            {
              ACE_OS::sprintf (obuf, ASYS_TEXT (" "));
              obuf++;
            }
          textver[j] = (c < 0x20 || c > 0x7e) ? '.' : c;
        }

      textver[j] = 0;

      ACE_OS::sprintf (obuf, ASYS_TEXT ("  %s\n"), textver);

      while (*obuf != '\0')
        obuf++;
    }

  if (size % 16)
    {
      for (i = 0 ; i < size % 16; i++)
        {
          c = (u_char) buffer[size - size % 16 + i];
          ACE_OS::sprintf (obuf, ASYS_TEXT ("%02x "),c);
          obuf += 3;
          if (i == 7)
            {
              ACE_OS::sprintf (obuf, ASYS_TEXT (" "));
              obuf++;
            }
          textver[i] = (c < 0x20 || c > 0x7e) ? '.' : c;
        }

      for (i = size % 16; i < 16; i++)
        {
          ACE_OS::sprintf (obuf, ASYS_TEXT ("   "));
          obuf += 3;
          textver[i] = ' ';
        }

      textver[i] = 0;
      ACE_OS::sprintf (obuf, ASYS_TEXT ("  %s\n"), textver);
    }
  return size;
}

// Returns the current timestamp in the form
// "hour:minute:second:microsecond."  The month, day, and year are
// also stored in the beginning of the date_and_time array.  Returns 0
// if unsuccessful, else returns pointer to beginning of the "time"
// portion of <day_and_time>.

ASYS_TCHAR *
ACE::timestamp (ASYS_TCHAR date_and_time[], int date_and_timelen)
{
  //ACE_TRACE ("ACE::timestamp");

  if (date_and_timelen < 35)
    {
      errno = EINVAL;
      return 0;
    }

#if defined (WIN32)
  // @@ Jesper, I think Win32 supports all the UNIX versions below.
  // Therefore, we can probably remove this WIN32 ifdef altogether.
  SYSTEMTIME local;
  ::GetLocalTime (&local);

  ACE_OS::sprintf (date_and_time,
                   ASYS_TEXT ("%02d/%02d/%04d %02d.%02d.%02d.%06d"),
                   (int) local.wMonth, // new, also the %02d in sprintf
                   (int) local.wDay,   // new, also the %02d in sprintf
                   (int) local.wYear,  // new, also the %02d in sprintf
                   (int) local.wHour,
                   (int) local.wMinute,
                   (int) local.wSecond,
                   (int) local.wMilliseconds * 1000);
#else  /* UNIX */
  char timebuf[26]; // This magic number is based on the ctime(3c) man page.
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();
  time_t secs = cur_time.sec ();
  ACE_OS::ctime_r (&secs,
                   timebuf,
                   sizeof timebuf);
  ACE_OS::strncpy (date_and_time,
                   timebuf,
                   date_and_timelen);
  ACE_OS::sprintf (&date_and_time[19],
                   ".%06d",
                   cur_time.usec ());
#endif /* WIN32 */
  date_and_time[26] = '\0';
  return &date_and_time[11];
}

// This function rounds the request to a multiple of the page size.

size_t
ACE::round_to_pagesize (off_t len)
{
  ACE_TRACE ("ACE::round_to_pagesize");

  if (ACE::pagesize_ == 0)
    ACE::pagesize_ = ACE_OS::getpagesize ();

  return (len + (ACE::pagesize_ - 1)) & ~(ACE::pagesize_ - 1);
}

size_t
ACE::round_to_allocation_granularity (off_t len)
{
  ACE_TRACE ("ACE::round_to_allocation_granularity");

  if (ACE::allocation_granularity_ == 0)
    ACE::allocation_granularity_ = ACE_OS::allocation_granularity ();

  return (len + (ACE::allocation_granularity_ - 1)) & ~(ACE::allocation_granularity_ - 1);
}

ACE_HANDLE
ACE::handle_timed_complete (ACE_HANDLE h,
                            ACE_Time_Value *timeout,
                            int is_tli)
{
  ACE_TRACE ("ACE::handle_timed_complete");
  ACE_Handle_Set rd_handles;
  ACE_Handle_Set wr_handles;
  int need_to_check;

#if defined (ACE_WIN32)
  ACE_Handle_Set ex_handles;
  ex_handles.set_bit (h);
#endif /* ACE_WIN32 */
  rd_handles.set_bit (h);
  wr_handles.set_bit (h);

#if defined (ACE_WIN32)
  int n = ACE_OS::select (int (h) + 1,
                          rd_handles,
                          wr_handles,
                          ex_handles,
                          timeout);
#else
  int n = ACE_OS::select (int (h) + 1,
                          rd_handles,
                          wr_handles,
                          0,
                          timeout);
#endif /* ACE_WIN32 */

  // If we failed to connect within the time period allocated by the
  // caller, then we fail (e.g., the remote host might have been too
  // busy to accept our call).
  if (n <= 0)
    {
      if (n == 0 && timeout != 0)
        errno = ETIMEDOUT;
      return ACE_INVALID_HANDLE;
    }

  // Check if the handle is ready for reading and the handle is *not*
  // ready for writing, which may indicate a problem.  But we need to
  // make sure...
#if defined (ACE_WIN32)
  need_to_check = rd_handles.is_set (h) || ex_handles.is_set (h);
#elif defined (VXWORKS)
  ACE_UNUSED_ARG (is_tli);

  // Force the check on VxWorks.  The read handle for "h" is not set,
  // so "need_to_check" is false at this point.  The write handle is
  // set, for what it's worth.
  need_to_check = 1;
#else
  if (is_tli)
    need_to_check = rd_handles.is_set (h) && !wr_handles.is_set (h);
  else
#if defined(AIX)
    // AIX is broken... both success and failed connect will set the
    // write handle only, so always check.
    need_to_check = 1;
#else
    need_to_check = rd_handles.is_set (h);
#endif /* AIX */
#endif /* ACE_WIN32 */

  if (need_to_check)
    {
      char dummy;

      // The following recv() won't block provided that the
      // ACE_NONBLOCK flag has not been turned off .
      n = ACE::recv (h, &dummy, 1, MSG_PEEK);

      // If no data was read/peeked at, check to see if it's because
      // of a non-connected socket (and therefore an error) or there's
      // just no data yet.
      if (n <= 0)
        {
          if (n == 0)
            {
              errno = ECONNREFUSED;
              h = ACE_INVALID_HANDLE;
            }
          else if (errno != EWOULDBLOCK && errno != EAGAIN)
            h = ACE_INVALID_HANDLE;
        }
    }

  // 1. The HANDLE is ready for writing and doesn't need to be checked or
  // 2. recv() returned an indication of the state of the socket - if there is
  // either data present, or a recv is legit but there's no data yet,
  // the connection was successfully established.
  return h;
}

ACE_HANDLE
ACE::handle_timed_open (ACE_Time_Value *timeout,
                        LPCTSTR name,
                        int flags,
                        int perms)
{
  ACE_TRACE ("ACE::handle_timed_open");

  if (timeout != 0)
    {
      // Open the named pipe or file using non-blocking mode...
      ACE_HANDLE handle = ACE_OS::open (name,
                                        flags | ACE_NONBLOCK,
                                        perms);
      if (handle == ACE_INVALID_HANDLE
          && (errno == EWOULDBLOCK
              && (timeout->sec () > 0 || timeout->usec () > 0)))
        // This expression checks if we were polling.
        errno = ETIME;

      return handle;
    }
  else
    return ACE_OS::open (name, flags, perms);
}

// Wait up to <timeout> amount of time to accept a connection.

int
ACE::handle_timed_accept (ACE_HANDLE listener,
                          ACE_Time_Value *timeout,
                          int restart)
{
  ACE_TRACE ("ACE::handle_timed_accept");
  // Make sure we don't bomb out on erroneous values.
  if (listener == ACE_INVALID_HANDLE)
    return -1;

  // Use the select() implementation rather than poll().
  ACE_Handle_Set rd_handle;
  rd_handle.set_bit (listener);

  // We need a loop here if <restart> is enabled.

  for (;;)
    {
      switch (ACE_OS::select (int (listener) + 1,
                              rd_handle, 0, 0,
                              timeout))
        {
        case -1:
          if (errno == EINTR && restart)
            continue;
          else
            return -1;
          /* NOTREACHED */
        case 0:
          if (timeout != 0
              && timeout->sec () == 0
              && timeout->usec () == 0)
            errno = EWOULDBLOCK;
          else
            errno = ETIME;
          return -1;
          /* NOTREACHED */
        case 1:
          return 0;
          /* NOTREACHED */
        default:
          errno = EINVAL;
          return -1;
          /* NOTREACHED */
        }
    }
  ACE_NOTREACHED (return 0);
}

// Bind socket to an unused port.

int
ACE::bind_port (ACE_HANDLE handle,
                ACE_UINT32 ip_addr)
{
  ACE_TRACE ("ACE::bind_port");

  sockaddr_in sock_addr;

  ACE_OS::memset ((void *) &sock_addr, 0, sizeof sock_addr);
  sock_addr.sin_family = AF_INET;
#if defined (ACE_HAS_SIN_LEN)
  sock_addr.sin_len = sizeof sock_addr;
#endif /* ACE_HAS_SIN_LEN */
  sock_addr.sin_addr.s_addr = ip_addr;

#if !defined (ACE_LACKS_WILDCARD_BIND)
  // The OS kernel should select a free port for us.
  sock_addr.sin_port = 0;
  return ACE_OS::bind (handle,
                       ACE_reinterpret_cast(sockaddr *, &sock_addr),
                       sizeof sock_addr);
#else
  static u_short upper_limit = ACE_MAX_DEFAULT_PORT;
  int round_trip = upper_limit;
  int lower_limit = IPPORT_RESERVED;

  // We have to select the port explicitly.

  for (;;)
    {
      sock_addr.sin_port = htons (upper_limit);

      if (ACE_OS::bind (handle,
                        ACE_reinterpret_cast(sockaddr *, &sock_addr),
                        sizeof sock_addr) >= 0)
        {
#if defined (ACE_WIN32)
          upper_limit--;
#endif /* ACE_WIN32 */
          return 0;
        }
      else if (errno != EADDRINUSE)
        return -1;
      else
        {
          upper_limit--;

          // Wrap back around when we reach the bottom.
          if (upper_limit <= lower_limit)
            upper_limit = ACE_MAX_DEFAULT_PORT;

          // See if we have already gone around once!
          if (upper_limit == round_trip)
            {
              errno = EAGAIN;
              return -1;
            }
        }
    }
#endif /* ACE_HAS_WILDCARD_BIND */
}

// Make the current process a UNIX daemon.  This is based on Stevens
// code from APUE.

int
ACE::daemonize (const char pathname[],
                int close_all_handles,
                const char program_name[])
{
  ACE_TRACE ("ACE::daemonize");
#if !defined (ACE_LACKS_FORK)
  pid_t pid = ACE_OS::fork ();

  if (pid == -1)
    return -1;
  else if (pid != 0)
    ACE_OS::exit (0); // Parent exits.

  // 1st child continues.
  ACE_OS::setsid (); // Become session leader.

  ACE_OS::signal (SIGHUP, SIG_IGN);

  pid = ACE_OS::fork (program_name);

  if (pid != 0)
    ACE_OS::exit (0); // First child terminates.

  // Second child continues.

  if (pathname != 0)
    // change working directory.
    ACE_OS::chdir (pathname);

  ACE_OS::umask (0); // clear our file mode creation mask.

  // Close down the files.
  if (close_all_handles)
    for (int i = ACE::max_handles () - 1; i >= 0; i--)
      ACE_OS::close (i);

  return 0;
#else
  ACE_UNUSED_ARG (pathname);
  ACE_UNUSED_ARG (close_all_handles);
  ACE_UNUSED_ARG (program_name);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_LACKS_FORK */
}

pid_t
ACE::fork (const char *program_name,
           int avoid_zombies)
{
  if (avoid_zombies == 0)
    return ACE_OS::fork (program_name);
  else
    {
      // This algorithm is adapted from an example in the Stevens book
      // "Advanced Programming in the Unix Environment" and an item in
      // Andrew Gierth's Unix Programming FAQ.  It creates an orphan
      // process that's inherited by the init process; init cleans up
      // when the orphan process terminates.
      //
      // Another way to avoid zombies is to ignore or catch the
      // SIGCHLD signal; we don't use that approach here.

      pid_t pid = ACE_OS::fork ();
      if (pid == 0)
        {
          // The child process forks again to create a grandchild.
          switch (ACE_OS::fork (program_name))
            {
            case 0: // grandchild returns 0.
              return 0;
            case -1: // assumes all errnos are < 256
              ACE_OS::_exit (errno);
            default:  // child terminates, orphaning grandchild
              ACE_OS::_exit (0);
            }
        }

      // Parent process waits for child to terminate.
#if defined (ACE_HAS_UNION_WAIT)
      union wait status;
      if (pid < 0 || ACE_OS::waitpid (pid, &(status.w_status), 0) < 0)
#else
      int status;
      if (pid < 0 || ACE_OS::waitpid (pid, &status, 0) < 0)
#endif /* ACE_HAS_UNION_WAIT */
        return -1;

      // child terminated by calling exit()?
      if (WIFEXITED ((status)))
        {
          // child terminated normally?
          if (WEXITSTATUS ((status)) == 0)
            return 1;
          else
            errno = WEXITSTATUS ((status));
        }
      else
        // child didn't call exit(); perhaps it received a signal?
        errno = EINTR;

      return -1;
    }
}

int
ACE::max_handles (void)
{
  ACE_TRACE ("ACE::max_handles");
#if defined (RLIMIT_NOFILE) && !defined (ACE_LACKS_RLIMIT)
  rlimit rl;
  ACE_OS::getrlimit (RLIMIT_NOFILE, &rl);
# if defined (RLIM_INFINITY)
  if (rl.rlim_cur != RLIM_INFINITY)
    return rl.rlim_cur;
#else
    return rl.rlim_cur;
# endif /* RLIM_INFINITY */
# if defined (_SC_OPEN_MAX)
  return ACE_OS::sysconf (_SC_OPEN_MAX);
# elif defined (FD_SETSIZE)
  return FD_SETSIZE;
# else
  ACE_NOTSUP_RETURN (-1);
# endif /* _SC_OPEN_MAX */
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* defined (RLIMIT_NOFILE) && !defined (ACE_LACKS_RLIMIT) */
}

// Set the number of currently open handles in the process.
//
// If NEW_LIMIT == -1 set the limit to the maximum allowable.
// Otherwise, set it to be the value of NEW_LIMIT.

int
ACE::set_handle_limit (int new_limit)
{
  ACE_TRACE ("ACE::set_handle_limit");
  int cur_limit = ACE::max_handles ();
  int max_limit = cur_limit;

  if (cur_limit == -1)
    return -1;

#if !defined (ACE_LACKS_RLIMIT) && defined (RLIMIT_NOFILE)
  struct rlimit rl;

  ACE_OS::memset ((void *) &rl, 0, sizeof rl);
  ACE_OS::getrlimit (RLIMIT_NOFILE, &rl);
  max_limit = rl.rlim_max;
#endif /* ACE_LACKS_RLIMIT */

  if (new_limit == -1)
    new_limit = max_limit;

  if (new_limit < 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if (new_limit > cur_limit)
    {
#if !defined (ACE_LACKS_RLIMIT) && defined (RLIMIT_NOFILE)
      rl.rlim_cur = new_limit;
      return ACE_OS::setrlimit (RLIMIT_NOFILE, &rl);
#else
      // Must return EINVAL errno.
      ACE_NOTSUP_RETURN (-1);
#endif /* ACE_LACKS_RLIMIT */
    }
  else
    {
#if !defined (ACE_LACKS_RLIMIT) && defined (RLIMIT_NOFILE)
      rl.rlim_cur = new_limit;
      return ACE_OS::setrlimit (RLIMIT_NOFILE, &rl);
#else
      // We give a chance to platforms without RLIMIT to work.
      // Instead of ACE_NOTSUP_RETURN (0), just return 0 because
      // new_limit is <= cur_limit, so it's a no-op.
      return 0;
#endif /* ACE_LACKS_RLIMIT */
    }

  // Irix complains without this return statement.  DEC cxx
  // (correctly) says that it's not reachable.  ACE_NOTREACHED won't
  // work here, because it handles both platforms the same.
  // IRIX does not complain anymore [7.2]
  ACE_NOTREACHED (return 0);
}

// Flags are file status flags to turn on.

int
ACE::set_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::set_flags");
#if defined (ACE_WIN32) || defined (ACE_LACKS_FCNTL)
  switch (flags)
    {
    case ACE_NONBLOCK:
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 1;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else
  int val = ACE_OS::fcntl (handle, F_GETFL, 0);

  if (val == -1)
    return -1;

  // Turn on flags.
  ACE_SET_BITS (val, flags);

  if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
    return -1;
  else
    return 0;
#endif /* ACE_WIN32 || ACE_LACKS_FCNTL */
}

// Flags are the file status flags to turn off.

int
ACE::clr_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::clr_flags");

#if defined (ACE_WIN32) || defined (ACE_LACKS_FCNTL)
  switch (flags)
    {
    case ACE_NONBLOCK:
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 0;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else
  int val = ACE_OS::fcntl (handle, F_GETFL, 0);

  if (val == -1)
    return -1;

  // Turn flags off.
  ACE_CLR_BITS (val, flags);

  if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
    return -1;
  else
    return 0;
#endif /* ACE_WIN32 || ACE_LACKS_FCNTL */
}

int
ACE::map_errno (int error)
{
  switch (error)
    {
#if defined (ACE_WIN32)
    case WSAEWOULDBLOCK:
      return EAGAIN; // Same as UNIX errno EWOULDBLOCK.
#endif /* ACE_WIN32 */
    }

  return error;
}

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           int flags,
           const ACE_Time_Value *timeout)
{
#if defined (ACE_HAS_SEND_TIMEDWAIT)
  if (timeout == 0)
    return ACE::send (handle, buf, n, flags);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday();
      timespec_t ts = copy;
      return ::send_timedwait (handle, buf, n, flags, &ts);
    }
#else
  int val;

  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      ssize_t bytes_written = ACE::send (handle, buf, n, flags);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SEND_TIMEDWAIT */
}

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE_OS::write");
#if defined (ACE_HAS_WRITE_TIMEDWAIT)
  if (timeout == 0)
    return ACE::send (handle, buf, n);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::write_timedwait (handle, buf, n, &ts);
    }
#else
  int val;

  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      ssize_t bytes_written = ACE::send (handle, buf, n);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_WRITE_TIMEDWAIT */
}

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             int flags,
             const ACE_Time_Value *timeout)
{
  // Total number of bytes written.
  size_t bytes_written;

  // Actual number of bytes written in each <send> attempt.
  ssize_t n;

  for (bytes_written = 0;
       bytes_written < len;
       bytes_written += n)
    {
      n = ACE::send (handle,
                     (char *) buf + bytes_written,
                     len - bytes_written,
                     flags,
                     timeout);
      if (n == -1)
        if (errno == EWOULDBLOCK)
          n = 0; // Keep trying to send.
        else
          return -1;
    }

  return bytes_written;
}

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             const ACE_Time_Value *timeout)
{
  // Total number of bytes written.
  size_t bytes_written;

  // Actual number of bytes written in each <send> attempt.
  ssize_t n;

  for (bytes_written = 0;
       bytes_written < len;
       bytes_written += n)
    {
      n = ACE::send (handle, (char *) buf + bytes_written,
                     len - bytes_written, timeout);
      if (n == -1)
        if (errno == EWOULDBLOCK)
          n = 0; // Keep trying to send.
        else
          return -1;
    }

  return bytes_written;
}

ssize_t
ACE::recvfrom (ACE_HANDLE handle,
               char *buf,
               int len,
               int flags,
               struct sockaddr *addr,
               int *addrlen,
               const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recvfrom");
#if defined (ACE_HAS_RECVFROM_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::recvfrom (handle,
                              buf,
                              len,
                              flags,
                              addr,
                              addrlen);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvfrom_timedwait (handle,
                                   buf,
                                   len,
                                   flags,
                                   addr,
                                   addrlen,
                                   &ts)a
;
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle,
                                 timeout,
                                 val) == -1)
     return -1;
  else
    {
      int bytes_read = ACE_OS::recvfrom (handle,
                                         buf,
                                         len,
                                         flags,
                                         addr,
                                         addrlen);
      ACE::leave_recv_timedwait (handle,
                                 timeout,
                                 val);
      return bytes_read;
    }
#endif /* ACE_HAS_RECVFROM_TIMEDWAIT */
}

ssize_t
ACE::recvmsg (ACE_HANDLE handle,
              struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recvmsg");
#if defined (ACE_HAS_RECVMSG_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::recvmsg (handle, msg, flags);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvmsg_timedwait (handle, msg, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      int bytes_read = ACE_OS::recvmsg (handle, msg, flags);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_RECVMSG_TIMEDWAIT */
}

ssize_t
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::read");
#if defined (ACE_HAS_READ_TIMEDWAIT)
  if (timeout == 0)
     return ACE::recv (handle, buf, n);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::read_timedwait (handle, buf, n, &ts);
    }
#else
  int val;

  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      ssize_t bytes_read = ACE::recv (handle, buf, n);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_READ_TIMEDWAIT */
}

ssize_t
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t len,
           int flags,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recv");
#if defined (ACE_HAS_RECV_TIMEDWAIT)
  if (timeout == 0)
     return ACE::recv (handle, buf, len, flags);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recv_timedwait (handle, buf, len, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val)==-1)
     return -1;
  else
    {
      ssize_t bytes_recv = ACE::recv (handle, buf, len, flags);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_recv;
    }
#endif /* ACE_HAS_RECV_TIMEDWAIT */
}

ssize_t
ACE::recv_n (ACE_HANDLE handle,
             void *buf,
             size_t len,
             int flags,
             const ACE_Time_Value *timeout)
{
  size_t bytes_received;

  // Actual number of bytes read in each attempt.
  ssize_t n;

  for (bytes_received = 0;
       bytes_received < len;
       bytes_received += n)
    {
      n = ACE::recv (handle,
                     (char *) buf + bytes_received,
                     len - bytes_received,
                     flags,
                     timeout);
      if (n == -1 || n == 0)
        break;
    }

  return bytes_received;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle,
             void *buf,
             size_t len,
             const ACE_Time_Value *timeout)
{
  size_t bytes_received;

  // Actual number of bytes read in each attempt.
  ssize_t n;

  for (bytes_received = 0;
       bytes_received < len;
       bytes_received += n)
    {
      n = ACE::recv (handle,
                     (char *) buf + bytes_received,
                     len - bytes_received,
                     timeout);
      if (n == -1 || n == 0)
        break;
    }

  return bytes_received;
}


// Euclid's greatest common divisor algorithm.
u_long
ACE::gcd (u_long x, u_long y)
{
  if (y == 0)
  {
    return x;
  }
  else
  {
    return ACE::gcd (y, x % y);
  }
}


// Calculates the minimum enclosing frame size for the given values.
u_long
ACE::minimum_frame_size (u_long period1, u_long period2)
{
  // if one of the periods is zero, treat it as though it as
  // uninitialized and return the other period as the frame size
  if (0 == period1)
  {
    return period2;
  }
  if (0 == period2)
  {
    return period1;
  }

  // if neither is zero, find the greatest common divisor of the two periods
  u_long greatest_common_divisor = ACE::gcd (period1, period2);

  // explicitly consider cases to reduce risk of possible overflow errors
  if (greatest_common_divisor == 1)
  {
    // periods are relative primes: just multiply them together
    return period1 * period2;
  }
  else if (greatest_common_divisor == period1)
  {
    // the first period divides the second: return the second
    return period2;
  }
  else if (greatest_common_divisor == period2)
  {
    // the second period divides the first: return the first
    return period1;
  }
  else
  {
    // the current frame size and the entry's effective period
    // have a non-trivial greatest common divisor: return the
    // product of factors divided by those in their gcd.
    return (period1 * period2) / greatest_common_divisor;
  }
}


u_long
ACE::is_prime (const u_long n,
               const u_long min_factor,
               const u_long max_factor)
{
  if (n > 3)
    for (u_long factor = min_factor;
         factor <= max_factor;
         ++factor)
      if (n / factor * factor == n)
        return factor;

  return 0;
}

const ASYS_TCHAR *
ACE::sock_error (int error)
{
#if defined (ACE_WIN32)
  switch (error)
    {
    case WSAVERNOTSUPPORTED:
      return ASYS_TEXT ("version of WinSock not supported");
      /* NOTREACHED */
    case WSASYSNOTREADY:
      return ASYS_TEXT ("WinSock not present or not responding");
      /* NOTREACHED */
    case WSAEINVAL:
      return ASYS_TEXT ("app version not supported by DLL");
      /* NOTREACHED */
    case WSAHOST_NOT_FOUND:
      return ASYS_TEXT ("Authoritive: Host not found");
      /* NOTREACHED */
    case WSATRY_AGAIN:
      return ASYS_TEXT ("Non-authoritive: host not found or server failure");
      /* NOTREACHED */
    case WSANO_RECOVERY:
      return ASYS_TEXT ("Non-recoverable: refused or not implemented");
      /* NOTREACHED */
    case WSANO_DATA:
      return ASYS_TEXT ("Valid name, no data record for type");
      /* NOTREACHED */
      /*
        case WSANO_ADDRESS:
        return "Valid name, no MX record";
          */
    case WSANOTINITIALISED:
      return ASYS_TEXT ("WSA Startup not initialized");
      /* NOTREACHED */
    case WSAENETDOWN:
      return ASYS_TEXT ("Network subsystem failed");
      /* NOTREACHED */
    case WSAEINPROGRESS:
      return ASYS_TEXT ("Blocking operation in progress");
      /* NOTREACHED */
    case WSAEINTR:
      return ASYS_TEXT ("Blocking call cancelled");
      /* NOTREACHED */
    case WSAEAFNOSUPPORT:
      return ASYS_TEXT ("address family not supported");
      /* NOTREACHED */
    case WSAEMFILE:
      return ASYS_TEXT ("no file handles available");
      /* NOTREACHED */
    case WSAENOBUFS:
      return ASYS_TEXT ("no buffer space available");
      /* NOTREACHED */
    case WSAEPROTONOSUPPORT:
      return ASYS_TEXT ("specified protocol not supported");
      /* NOTREACHED */
    case WSAEPROTOTYPE:
      return ASYS_TEXT ("protocol wrong type for this socket");
      /* NOTREACHED */
    case WSAESOCKTNOSUPPORT:
      return ASYS_TEXT ("socket type not supported for address family");
      /* NOTREACHED */
    case WSAENOTSOCK:
      return ASYS_TEXT ("handle is not a socket");
      /* NOTREACHED */
    case WSAEWOULDBLOCK:
      return ASYS_TEXT ("socket marked as non-blocking and SO_LINGER set not 0");
      /* NOTREACHED */
    case WSAEADDRINUSE:
      return ASYS_TEXT ("address already in use");
      /* NOTREACHED */
    case WSAECONNABORTED:
      return ASYS_TEXT ("connection aborted");
      /* NOTREACHED */
    case WSAECONNRESET:
      return ASYS_TEXT ("connection reset");
      /* NOTREACHED */
    case WSAENOTCONN:
      return ASYS_TEXT ("not connected");
      /* NOTREACHED */
    case WSAETIMEDOUT:
      return ASYS_TEXT ("connection timed out");
      /* NOTREACHED */
    case WSAECONNREFUSED:
      return ASYS_TEXT ("connection refused");
      /* NOTREACHED */
    case WSAEHOSTDOWN:
      return ASYS_TEXT ("host down");
      /* NOTREACHED */
    case WSAEHOSTUNREACH:
      return ASYS_TEXT ("host unreachable");
      /* NOTREACHED */
    case WSAEADDRNOTAVAIL:
      return ASYS_TEXT ("address not available");
      /* NOTREACHED */
    default:
      return ASYS_TEXT ("unknown error");
      /* NOTREACHED */
    }
#else
  ACE_UNUSED_ARG (error);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

int
ACE::get_bcast_addr (ACE_UINT32 &bcast_addr,
                     const char *host_name,
                     ACE_UINT32 host_addr,
                     ACE_HANDLE handle)
{
  ACE_TRACE ("ACE::get_bcast_addr");

#if !defined(ACE_WIN32)
  ACE_HANDLE s = handle;

  if (s == ACE_INVALID_HANDLE)
    s = ACE_OS::socket (AF_INET, SOCK_STREAM, 0);

  if (s == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE_OS::socket")),
                      -1);

  struct ifconf ifc;
  char buf[BUFSIZ];

  ifc.ifc_len = sizeof buf;
  ifc.ifc_buf = buf;

  // Get interface structure and initialize the addresses using UNIX
  // techniques
#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (s, cmd, (char *) &ifc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE::get_bcast_addr:")
                       ASYS_TEXT ("ioctl (get interface configuration)")),
                      -1);

  struct ifreq *ifr = ifc.ifc_req;

  struct sockaddr_in ip_addr;

  // Get host ip address if necessary.
  if (host_name)
    {
      hostent *hp = ACE_OS::gethostbyname (host_name);

      if (hp == 0)
        return -1;
      else
#if !defined(_UNICOS)
        ACE_OS::memcpy ((char *) &ip_addr.sin_addr.s_addr,
                        (char *) hp->h_addr,
                        hp->h_length);
#else /* _UNICOS */
        {
          ACE_UINT64 haddr;  // a place to put the address
          char * haddrp = (char *) &haddr;  // convert to char pointer
          haddr += 4;   // adjust within the word
          ACE_OS::memcpy(haddrp,(char *) hp->h_addr,hp->h_length);
          ip_addr.sin_addr.s_addr = haddr;
        }
#endif /* ! _UNICOS */
    }
  else
    {
      ACE_OS::memset ((void *) &ip_addr, 0, sizeof ip_addr);
#if !defined(_UNICOS)
      ACE_OS::memcpy ((void *) &ip_addr.sin_addr,
                      (void*) &host_addr,
                      sizeof ip_addr.sin_addr);
#else /* _UNICOS */
      ip_addr.sin_addr.s_addr = host_addr;   // just copy to the bitfield
#endif /* ! _UNICOS */
    }

  for (int n = ifc.ifc_len / sizeof (struct ifreq);
       n > 0;
       n--, ifr++)
    {
      struct sockaddr_in if_addr;

      // Compare host ip address with interface ip address.
      ACE_OS::memcpy (&if_addr,
                      &ifr->ifr_addr,
                      sizeof if_addr);

      if (ip_addr.sin_addr.s_addr != if_addr.sin_addr.s_addr)
        continue;

      if (ifr->ifr_addr.sa_family != AF_INET)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("ACE::get_bcast_addr:")
                      ASYS_TEXT ("Not AF_INET")));
          continue;
        }

      struct ifreq flags = *ifr;
      struct ifreq if_req = *ifr;

      if (ACE_OS::ioctl (s, SIOCGIFFLAGS, (char *) &flags) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("ACE::get_bcast_addr:")
                      ASYS_TEXT (" ioctl (get interface flags)")));
          continue;
        }

      if (ACE_BIT_DISABLED (flags.ifr_flags, IFF_UP))
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("ACE::get_bcast_addr:")
                      ASYS_TEXT ("Network interface is not up")));
          continue;
        }

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_LOOPBACK))
        continue;

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_BROADCAST))
        {
          if (ACE_OS::ioctl (s,
                             SIOCGIFBRDADDR,
                             (char *) &if_req) == -1)
            ACE_ERROR ((LM_ERROR,
                        ASYS_TEXT ("%p\n"),
                        ASYS_TEXT ("ACE::get_bcast_addr:")
                        ASYS_TEXT ("ioctl (get broadaddr)")));
          else
            {
              ACE_OS::memcpy (ACE_reinterpret_cast(sockaddr_in *, &ip_addr),
                              ACE_reinterpret_cast(sockaddr_in *, &if_req.ifr_broadaddr),
                              sizeof if_req.ifr_broadaddr);

              ACE_OS::memcpy ((void *) &host_addr,
                              (void *) &ip_addr.sin_addr,
                              sizeof host_addr);

              if (handle == ACE_INVALID_HANDLE)
                ACE_OS::close (s);

              bcast_addr = host_addr;
              return 0;
            }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("%p\n"),
                    ASYS_TEXT ("ACE::get_bcast_addr:")
                    ASYS_TEXT ("Broadcast is not enable for this interface.")));

      if (handle == ACE_INVALID_HANDLE)
        ACE_OS::close (s);

      bcast_addr = host_addr;
      return 0;
    }

  return 0;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (host_addr);
  ACE_UNUSED_ARG (host_name);
  bcast_addr = (ACE_UINT32 (INADDR_BROADCAST));
  return 0;
#endif /* !ACE_WIN32 */
}

// Helper routine for get_ip_interfaces, differs by UNIX platform so
// put into own subroutine.  perform some ioctls to retrieve ifconf
// list of ifreq structs.

int
ACE::count_interfaces (ACE_HANDLE handle,
                       size_t &how_many)
{
#if defined (sparc) && defined (SIOCGIFNUM)
  if (ACE_OS::ioctl (handle,
                     SIOCGIFNUM,
                     (caddr_t) &how_many) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("ACE::get_ip_interfaces:")
                       ASYS_TEXT ("ioctl - SIOCGIFNUM failed")),
                      -1);
   return 0;
#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
   // Note: DEC CXX doesn't define "unix".  BSD compatible OS: HP UX,
   // AIX, SunOS 4.x perform some ioctls to retrieve ifconf list of
   // ifreq structs no SIOCGIFNUM on SunOS 4.x, so use guess and scan
   // algorithm

   // Probably hard to put this many ifs in a unix box..
  const int MAX_IF = 50;

  // HACK - set to an unreasonable number
  int num_ifs = MAX_IF;

  struct ifconf ifcfg;
  size_t ifreq_size = num_ifs * sizeof (struct ifreq);
  struct ifreq *p_ifs =
    (struct ifreq *) ACE_OS::malloc (ifreq_size);

  if (!p_ifs)
    {
      errno = ENOMEM;
      return -1;
    }

  ACE_OS::memset (p_ifs, 0, ifreq_size);
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));

  ifcfg.ifc_req = p_ifs;
  ifcfg.ifc_len = ifreq_size;

#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::free (ifcfg.ifc_req);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("count_interfaces:ioctl:")
                         ASYS_TEXT ("SIOCGIFCONF failed")),
                        -1);
    }

  int if_count = 0, i;

  // get if address out of ifreq buffers.  ioctl puts a blank-named
  // interface to mark the end of the returned interfaces.
  for (i = 0;
       i < num_ifs;
       i++)
    {
      if (p_ifs->ifr_name[0] == '\0')
        break;

      if_count++;
      p_ifs++;
    }

  ACE_OS::free (ifcfg.ifc_req);
  how_many = if_count;
  return 0;
#else
   ACE_UNUSED_ARG (handle);
   ACE_UNUSED_ARG (how_many);
   ACE_NOTSUP_RETURN (-1);; // no implmentation
#endif /* sparc && SIOCGIFNUM */
}

// Routine to return a handle from which ioctl() requests can be made.

ACE_HANDLE
ACE::get_handle (void)
{
// Solaris 2.x
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
#if defined (sparc)
  handle = ACE_OS::open ("/dev/udp", O_RDONLY);
#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
  // Note: DEC CXX doesn't define "unix" BSD compatible OS: HP UX,
  // AIX, SunOS 4.x

  handle = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);
#endif /* sparc */
  return handle;
}

#if defined (ACE_WIN32)
// Return value in buffer for a key/name pair from the Windows
// Registry up to buf_len size.

static int
get_reg_value (const TCHAR *key,
               const TCHAR *name,
               TCHAR *buffer,
               DWORD &buf_len)
{
  HKEY hk;
  DWORD buf_type;
  LONG rc = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                            key,
                            0,
                            KEY_READ,
                            &hk);
  // 1. open key that defines the interfaces used for TCP/IP?
  if (rc != ERROR_SUCCESS)
    // print_error_string(TEXT("RegOpenKeyEx"), rc);
    return -1;

  rc = ::RegQueryValueEx (hk,
                          name,
                          0,
                          &buf_type,
                          (u_char *) buffer,
                          &buf_len);
  if (rc != ERROR_SUCCESS)
    {
      // print_error_string(TEXT("RegEnumKeyEx"), rc);
      RegCloseKey (hk);
      return -2;
    }

  ::RegCloseKey (hk);
  return 0;
}
#endif /* ACE_WIN32 */

// return an array of all configured IP interfaces on this host, count
// rc = 0 on success (count == number of interfaces else -1 caller is
// responsible for calling delete [] on parray

int
ACE::get_ip_interfaces (size_t &count,
                        ACE_INET_Addr *&addrs)
{
  ACE_TRACE ("ACE::get_ip_interfaces");

  count = 0;
  addrs = 0;

#if defined (ACE_WIN32)
  // Win32 can do this by a simple API call if MSVC 5 or later is the compiler.
  // Not sure if Borland supplies the needed header/lib, but it might.
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#if 0
  // If this also needs to be predicated on MSVC 5 or later, add the
  // following condition to the #if above.  It tests ok at Riverace w/ 4.2,
  // but this isn't a virgin install of 4.2 so there's a minimal risk that
  // it may need work later.
     defined (_MSC_VER) && (_MSC_VER >= 1100)
#endif /* 0 */

  int i, n_interfaces, status;
  INTERFACE_INFO info[64];
  LPINTERFACE_INFO lpii;
  SOCKET sock;

  // Get an (overlapped) DGRAM socket to test with
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock == INVALID_SOCKET)
    return -1;

  DWORD bytes;
  status = WSAIoctl(sock,
                    SIO_GET_INTERFACE_LIST,
                    0, 0,
                    info, sizeof(info),
                    &bytes,
                    NULL,
                    NULL);
  closesocket (sock);
  if (status == SOCKET_ERROR)
      return -1;

  n_interfaces = bytes / sizeof(INTERFACE_INFO);
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs, ACE_INET_Addr[n_interfaces], -1);

  // Now go through the list and transfer the good ones to the list of
  // because they're down or don't have an IP address.
  for (count = 0, i = 0; i < n_interfaces; i++)
    {
      struct sockaddr_in *addrp;

      lpii = &info[i];
      if (!(lpii->iiFlags & IFF_UP))
        continue;

      // We assume IPv4 addresses here
      addrp = ACE_reinterpret_cast(struct sockaddr_in *, &(lpii->iiAddress));
      if (addrp->sin_addr.s_addr == INADDR_ANY)
        continue;

      // Set the address for the caller.
      addrs[count].set(addrp, sizeof(lpii->iiAddress));
      ++count;
    }

  if (count == 0)
    {
      delete [] addrs;
      addrs = 0;
    }

  return 0;

#else /* Winsock 2 && MSVC 5 or later */

  const TCHAR *SVCS_KEY1 =
    ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\");
  const TCHAR *LINKAGE_KEY1 =
    ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Linkage");
  const TCHAR *TCP_PARAM_SUBKEY = ACE_TEXT ("\\Parameters\\Tcpip");
  const TCHAR *BIND_NAME_ID =  ACE_TEXT ("Bind");
  const TCHAR *IPADDR_NAME_ID = ACE_TEXT ("IPAddress");
  const TCHAR *INVALID_TCPIP_DEVICE_ADDR = ACE_TEXT ("0.0.0.0");

  TCHAR raw_buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD raw_buflen = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
  TCHAR buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD buf_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;

  if (::get_reg_value (LINKAGE_KEY1,
                       BIND_NAME_ID,
                       raw_buffer,
                       raw_buflen))
    return -1;
  // return buffer contains NULL delimited strings

  ACE_Tokenizer dev_names (raw_buffer);
  dev_names.delimiter (ACE_TEXT('\0'));
  int n_interfaces = 0;

  // Count the number of interfaces
  while (dev_names.next () != NULL)
    n_interfaces ++;

  // case 1. no interfaces present, empty string? OS version change?
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[n_interfaces],
                  -2);
  count = 0;
  for (int i = 0; i < n_interfaces; i++)
    {
      // a. construct name to access IPAddress for this interface
      ACE_TEXT_STRING ifdevkey (SVCS_KEY1);
      ACE_TEXT_STRING the_dev = dev_names.next ();

      // chop off the "\Device" and keep last name.
      if (the_dev.length() < 8)
        return -3;              // Something's wrong
      else
        {
          // rest of string from offset 8
          the_dev = the_dev.substring (8);
          ifdevkey += the_dev;
          ifdevkey += TCP_PARAM_SUBKEY;

          // b. extract value
          // Gets overwritten on each call
          buf_len = sizeof(buffer);
          if (get_reg_value (ifdevkey.fast_rep (),
                             IPADDR_NAME_ID,
                             buffer,
                             buf_len))
            return -4;

          if (ACE_OS::strcmp (buffer,
                              INVALID_TCPIP_DEVICE_ADDR) == 0)
            continue; // Don't count this device

          // c. store in hostinfo object array and up the counter
          addrs[count++] =
            ACE_INET_Addr ((u_short) 0,
                           ASYS_MULTIBYTE_STRING (buffer));
        }
    }
  return 0;
# endif /* Winsock 2 && MSVC 5 or later */

#elif defined (__unix) || defined (__Lynx__) || defined (_AIX)
  // COMMON (SVR4 and BSD) UNIX CODE

  size_t num_ifs;

  // Call specific routine as necessary.
  ACE_HANDLE handle = get_handle();

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE::get_ip_interfaces:open")),
                      -1);
  if (ACE::count_interfaces (handle, num_ifs))
    {
      ACE_OS::close (handle);
      return -1;
    }

  // ioctl likes to have an extra ifreq structure to mark the end of
  // what it returned, so increase the num_ifs by one.
  ++num_ifs;

  struct ifreq *ifs = 0;
  ACE_NEW_RETURN (ifs,
                  struct ifreq[num_ifs],
                  -1);
  ACE_OS::memset (ifs, 0, num_ifs * sizeof (struct ifreq));

  ACE_Auto_Array_Ptr<struct ifreq> p_ifs (ifs);

  if (p_ifs.get() == 0)
    {
      ACE_OS::close (handle);
      errno = ENOMEM;
      return -1;
    }

  struct ifconf ifcfg;
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));
  ifcfg.ifc_req = p_ifs.get ();
  ifcfg.ifc_len = num_ifs * sizeof (struct ifreq);

#if defined (AIX)
  int cmd = CSIOCGIFCONF;
#else
  int cmd = SIOCGIFCONF;
#endif /* AIX */
  if (ACE_OS::ioctl (handle,
                     cmd,
                     (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::close (handle);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("%p\n"),
                         ASYS_TEXT ("is_address_local:")
                         ASYS_TEXT ("ioctl - SIOCGIFCONF failed")),
                        -1);
    }

  ACE_OS::close (handle);

  // Now create and initialize output array.

  ACE_NEW_RETURN (addrs,
                  ACE_INET_Addr[num_ifs],
                  -1); // caller must free

  struct ifreq *pcur = p_ifs.get ();
  // Pull the address out of each INET interface.  Not every interface
  // is for IP, so be careful to count properly.  When setting the
  // INET_Addr, note that the 3rd arg (0) says to leave the byte order
  // (already in net byte order from the interface structure) as is.
  count = 0;

  for (size_t i = 0;
       i < num_ifs;
       i++)
    {
      if (pcur->ifr_addr.sa_family == AF_INET)
        {
#if !defined(_UNICOS)
          struct sockaddr_in *addr =
            ACE_reinterpret_cast(sockaddr_in *, &pcur->ifr_addr);

          // Sometimes the kernel returns 0.0.0.0 as the interface
          // address, skip those...
          if (addr->sin_addr.s_addr != 0)
            {
              addrs[count].set ((u_short) 0,
                                addr->sin_addr.s_addr,
                                0);
              count++;
            }
#else /* ! _UNICOS */
          // need to explicitly copy on the Cray, since the bitfields kinda
          // screw things up here
          struct sockaddr_in inAddr;

          inAddr.sin_len = pcur->ifr_addr.sa_len;
          inAddr.sin_family = pcur->ifr_addr.sa_family;
          memcpy((void *)&(inAddr.sin_addr),
                 (const void *)&(pcur->ifr_addr.sa_data[8]),
                 sizeof(struct in_addr));

          if (inAddr.sin_addr.s_addr != 0)
            {
              addrs[count].set(&inAddr, sizeof(struct sockaddr_in));
              count++;
            }
#endif /* ! _UNICOS */
        }

      pcur++;
    }
  return 0;
#else
  ACE_UNUSED_ARG (count);
  ACE_UNUSED_ARG (addrs);
  ACE_NOTSUP_RETURN (-1);;                      // no implementation
#endif /* ACE_WIN32 */
}

char *
ACE::strndup (const char *str, size_t n)
{
  const char *t = str;
  size_t len;

  // Figure out how long this string is (remember, it might not be
  // NUL-terminated).

  for (len = 0;
       len < n && *t++ != '\0';
       len++)
    continue;

  char *s;
  ACE_ALLOCATOR_RETURN (s,
                        (char *) ACE_OS::malloc (len + 1),
                        0);
  s[len] = '\0';
  return ACE_OS::strncpy (s, str, len);
}

char *
ACE::strnnew (const char *str, size_t n)
{
  const char *t = str;
  size_t len;

  // Figure out how long this string is (remember, it might not be
  // NUL-terminated).

  for (len = 0;
       len < n && *t++ != '\0';
       len++)
    continue;

  char *s;
  ACE_NEW_RETURN (s,
                  char[len + 1],
                  0);
  s[len] = '\0';
  return ACE_OS::strncpy (s, str, len);
}

const char *
ACE::strend (const char *s)
{
  while (*s++ != '\0')
    continue;

  return s;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) && (defined (__unix) || defined (__Lynx__) || defined (_AIX))
template class ACE_Auto_Array_Ptr<struct ifreq>;
template class ACE_Auto_Basic_Array_Ptr<struct ifreq>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Array_Ptr<struct ifreq>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<struct ifreq>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION && (__unix || __Lynx_) */
