// $Id$

#include "ace/ACE.h"
#include "ace/Handle_Set.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#include "ace/Version.h"
#include "ace/Message_Block.h"
#include "ace/Log_Msg.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/ACE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, ACE, "$Id$")

  // Static data members.

// Keeps track of whether we're in some global debug mode.
char ACE::debug_ = 0;

// Hex characters.
const ACE_TCHAR ACE::hex_chars_[] = ACE_LIB_TEXT ("0123456789abcdef");

// Size of a VM page.
size_t ACE::pagesize_ = 0;

// Size of allocation granularity.
size_t ACE::allocation_granularity_ = 0;

int
ACE::out_of_handles (int error)
{
  // EMFILE is common to all platforms.
  if (error == EMFILE ||
#if defined (ACE_WIN32)
      // On Win32, we need to check for ENOBUFS also.
      error == ENOBUFS ||
#elif defined (HPUX)
      // On HPUX, we need to check for EADDRNOTAVAIL also.
      error == EADDRNOTAVAIL ||
#elif defined (linux)
      // On linux, we need to check for ENOENT also.
      error == ENOENT ||
      // For RedHat5.2, need to check for EINVAL too.
      error == EINVAL ||
      // Without threads check for EOPNOTSUPP
      error == EOPNOTSUPP ||
#elif defined (sun)
      // On sun, we need to check for ENOSR also.
      error == ENOSR ||
      // Without threads check for ENOTSUP
      error == ENOTSUP ||
#elif defined (__FreeBSD__)
      // On FreeBSD we need to check for EOPNOTSUPP (LinuxThreads) or
      // ENOSYS (libc_r threads) also.
      error == EOPNOTSUPP ||
      error == ENOSYS ||
#elif defined (__sgi) // irix
      error == ENOTSUP ||
#elif defined (DIGITAL_UNIX) // osf1
      error == ENOTSUP ||
#endif /* ACE_WIN32 */
      error == ENFILE)
    return 1;
  else
    return 0;
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

const ACE_TCHAR *
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
ACE::select (int width,
             ACE_Handle_Set *readfds,
             ACE_Handle_Set *writefds,
             ACE_Handle_Set *exceptfds,
             const ACE_Time_Value *timeout)
{
  int result = ACE_OS::select (width,
                               readfds ? readfds->fdset () : 0,
                               writefds ? writefds->fdset () : 0,
                               exceptfds ? exceptfds->fdset () : 0,
                               timeout);
  if (result > 0)
    {
      if (readfds)
        readfds->sync ((ACE_HANDLE) width);
      if (writefds)
        writefds->sync ((ACE_HANDLE) width);
      if (exceptfds)
        exceptfds->sync ((ACE_HANDLE) width);
    }
  return result;
}

int
ACE::select (int width,
             ACE_Handle_Set &readfds,
             const ACE_Time_Value *timeout)
{
  int result = ACE_OS::select (width,
                               readfds.fdset (),
                               0,
                               0,
                               timeout);
  if (result > 0)
    readfds.sync ((ACE_HANDLE) width);
  return result;
}

int
ACE::terminate_process (pid_t pid)
{
#if defined (ACE_HAS_PACE)
  return pace_kill (pid, 9);
#elif defined (ACE_HAS_PHARLAP)
  ACE_UNUSED_ARG (pid);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_WIN32)
  // Create a handle for the given process id.
  ACE_HANDLE process_handle =
    ::OpenProcess (PROCESS_TERMINATE,
                   FALSE, // New handle is not inheritable.
                   pid);

  if (process_handle == ACE_INVALID_HANDLE
      || process_handle == 0)
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
# if defined(CHORUS_4)
  if (::acap (pid, &cap_) == 0)
# else
    if (::acap (AM_MYSITE, pid, &cap_) == 0)
# endif
      return ::akill (&cap_);
    else
      return -1;
#else
  return ACE_OS::kill (pid, 9);
#endif /* ACE_HAS_PACE */
}

int
ACE::process_active (pid_t pid)
{
#if defined (ACE_HAS_PACE)
  int retval = pace_kill (pid, 0);

  if (retval == 0)
    return 1;
  else if (errno == ESRCH)
    return 0;
  else
    return -1;
#elif !defined(ACE_WIN32)
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
      || process_handle == 0)
    return 0;
  else
    {
      DWORD status;
      int result = 1;
      if (::GetExitCodeProcess (process_handle,
                                &status) == 0
          || status != STILL_ACTIVE)
        result = 0;

      ::CloseHandle (process_handle);
      return result;
    }
#endif /* ACE_HAS_PACE */
}

const ACE_TCHAR *
ACE::execname (const ACE_TCHAR *old_name)
{
#if defined (ACE_WIN32)
  if (ACE_OS::strstr (old_name, ACE_LIB_TEXT (".exe")) == 0)
    {
      ACE_TCHAR *new_name;

      size_t size =
        ACE_OS::strlen (old_name)
        + ACE_OS::strlen (ACE_LIB_TEXT (".exe"))
        + 1;

      ACE_NEW_RETURN (new_name,
                      ACE_TCHAR[size],
                      0);
      ACE_TCHAR *end = new_name;

      end = ACE_OS::strecpy (new_name, old_name);

      // Concatenate the .exe suffix onto the end of the executable.
      ACE_OS::strcpy (end, ACE_LIB_TEXT (".exe"));

      return new_name;
    }
#endif /* ACE_WIN32 */
  return old_name;
}

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

#if defined (ACE_HAS_WCHAR)
u_long
ACE::hash_pjw (const wchar_t *str, size_t len)
{
  u_long hash = 0;

  for (size_t i = 0; i < len; i++)
    {
      // @@ UNICODE: Does this function do the correct thing with wchar's?

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
#endif /* ACE_HAS_WCHAR */

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
  0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4};

// Compute a POSIX 1003.2 checksum.  The routine takes an string and
// computes the CRC for it (it stops on the first '\0' character).

// UNICOS UINT32's are 64-bit on the Cray PVP architecture
#if defined(_UNICOS) || (ACE_SIZEOF_LONG == 8)
#  define COMPUTE(var, ch) (var) = ( 0x00000000ffffffff & ((var) << 8)) ^ ACE::crc_table_[(((var) >> 24) ^ (ch))&0xff]
#else /* _UNICOS */
#  define COMPUTE(var, ch) (var) = ((var) << 8) ^ ACE::crc_table_[(((var) >> 24) ^ (ch))&0xff]
#endif /* _UNICOS */

u_long
ACE::crc32 (const char *string)
{
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

u_long
ACE::crc32 (const char *buffer, ACE_UINT32 len)
{
  register ACE_UINT32 crc = 0;

  for (const char *p = buffer;
       p != buffer + len;
       ++p)
    {
      COMPUTE (crc, *p);
    }

  // Include the length of the string.

  for (; len != 0; len >>= 8)
    COMPUTE (crc, len & 0xff);

  return ~crc;
}

#undef COMPUTE

#if !defined (ACE_HAS_WINCE)
ACE_TCHAR *
ACE::strenvdup (const ACE_TCHAR *str)
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

const ACE_TCHAR *
ACE::basename (const ACE_TCHAR *pathname, ACE_TCHAR delim)
{
  ACE_TRACE ("ACE::basename");
  const ACE_TCHAR *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    return pathname;
  else
    return temp + 1;
}

const ACE_TCHAR *
ACE::dirname (const ACE_TCHAR *pathname, ACE_TCHAR delim)
{
  ACE_TRACE ("ACE::dirname");
  static ACE_TCHAR return_dirname[MAXPATHLEN + 1];

  const ACE_TCHAR *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    {
      return_dirname[0] = '.';
      return_dirname[1] = '\0';

      return return_dirname;
    }
  else
    {
      // When the len is truncated, there are problems!  This should
      // not happen in normal circomstances
      size_t len = temp - pathname + 1;
      if (len > (sizeof return_dirname / sizeof (ACE_TCHAR)))
        len = sizeof return_dirname / sizeof (ACE_TCHAR);

      ACE_OS::strsncpy (return_dirname,
                        pathname,
                        len);
      return return_dirname;
    }
}

ssize_t
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t len,
           int flags,
           const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::recv (handle, (char *) buf, len, flags);
  else
    {
#if defined (ACE_HAS_RECV_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recv_timedwait (handle, buf, len, flags, &ts);
#else
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) ==-1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::recv (handle, (char *) buf, len, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_RECV_TIMEDWAIT */
    }
}

#if defined (ACE_HAS_TLI)

ssize_t
ACE::t_rcv (ACE_HANDLE handle,
            void *buf,
            size_t len,
            int *flags,
            const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::t_rcv (handle, (char *) buf, len, flags);
  else
    {
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) ==-1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::t_rcv (handle, (char *) buf, len, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
    }
}

#endif /* ACE_HAS_TLI */

ssize_t
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE::recv_i (handle, buf, n);
  else
    {
#if defined (ACE_HAS_READ_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::read_timedwait (handle, buf, n, &ts);
#else
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE::recv_i (handle, buf, n);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_READ_TIMEDWAIT */
    }
}

ssize_t
ACE::recvmsg (ACE_HANDLE handle,
              struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::recvmsg (handle, msg, flags);
  else
    {
#if defined (ACE_HAS_RECVMSG_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvmsg_timedwait (handle, msg, flags, &ts);
#else
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          int bytes_transferred = ACE_OS::recvmsg (handle, msg, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_RECVMSG_TIMEDWAIT */
    }
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
  if (timeout == 0)
    return ACE_OS::recvfrom (handle, buf, len, flags, addr, addrlen);
  else
    {
#if defined (ACE_HAS_RECVFROM_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvfrom_timedwait (handle, buf, len, flags, addr, addrlen, &ts);
#else
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          int bytes_transferred = ACE_OS::recvfrom (handle, buf, len, flags, addr, addrlen);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_RECVFROM_TIMEDWAIT */
    }
}

ssize_t
ACE::recv_n_i (ACE_HANDLE handle,
               void *buf,
               size_t len,
               int flags,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE_OS::recv (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred,
                        flags);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_read_ready (handle,
                                                   0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::recv_n_i (ACE_HANDLE handle,
               void *buf,
               size_t len,
               int flags,
               const ACE_Time_Value *timeout,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE_OS::recv (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred,
                        flags);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_read_ready (handle,
                                                timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

#if defined (ACE_HAS_TLI)

ssize_t
ACE::t_rcv_n_i (ACE_HANDLE handle,
                void *buf,
                size_t len,
                int *flags,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE_OS::t_rcv (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred,
                         flags);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_read_ready (handle,
                                                   0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::t_rcv_n_i (ACE_HANDLE handle,
                void *buf,
                size_t len,
                int *flags,
                const ACE_Time_Value *timeout,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE_OS::t_rcv (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred,
                         flags);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_read_ready (handle,
                                                timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

#endif /* ACE_HAS_TLI */

ssize_t
ACE::recv_n_i (ACE_HANDLE handle,
               void *buf,
               size_t len,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE::recv_i (handle,
                       (char *) buf + bytes_transferred,
                       len - bytes_transferred);
      // Check EOF.
      if (n == 0)
        {
          return 0;
        }
      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_read_ready (handle,
                                                   0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::recv_n_i (ACE_HANDLE handle,
               void *buf,
               size_t len,
               const ACE_Time_Value *timeout,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE::recv_i (handle,
                       (char *) buf + bytes_transferred,
                       len - bytes_transferred);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_read_ready (handle,
                                                timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

// This is basically an interface to ACE_OS::readv, that doesn't use
// the struct iovec explicitly.  The ... can be passed as an arbitrary
// number of (char *ptr, int len) tuples.  However, the count N is the
// *total* number of trailing arguments, *not* a couple of the number
// of tuple pairs!

ssize_t
ACE::recv (ACE_HANDLE handle, size_t n, ...)
{
  va_list argp;
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp,
                  iovec[total_tuples],
                  -1);
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

ssize_t
ACE::recvv (ACE_HANDLE handle,
            iovec *iov,
            int iovcnt,
            const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::recvv (handle, iov, iovcnt);
  else
    {
#if defined (ACE_HAS_READV_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::readv_timedwait (handle, iov, iovcnt, &ts);
#else
      int val = 0;
      if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::recvv (handle, iov, iovcnt);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_READV_TIMEDWAIT */
    }
}

ssize_t
ACE::recvv_n_i (ACE_HANDLE handle,
                iovec *iov,
                int iovcnt,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  for (int s = 0;
       s < iovcnt;
       )
    {
      // Try to transfer as much of the remaining data as possible.
      ssize_t n = ACE_OS::recvv (handle,
                                 iov + s,
                                 iovcnt - s);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_read_ready (handle,
                                                   0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }

      for (bytes_transferred += n;
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

  return bytes_transferred;
}

ssize_t
ACE::recvv_n_i (ACE_HANDLE handle,
                iovec *iov,
                int iovcnt,
                const ACE_Time_Value *timeout,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (int s = 0;
       s < iovcnt;
       )
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      ssize_t n = ACE_OS::recvv (handle,
                                 iov + s,
                                 iovcnt - s);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_read_ready (handle,
                                                timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }

      for (bytes_transferred += n;
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

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle,
             ACE_Message_Block *message_block,
             const ACE_Time_Value *timeout,
             size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;

  while (message_block != 0)
    {
      // Our current message block chain.
      const ACE_Message_Block *current_message_block = message_block;

      while (current_message_block != 0)
        {
          size_t current_message_block_length =
            current_message_block->length ();

          // Check if this block has any space for incoming data.
          if (current_message_block_length > 0)
            {
              // Collect the data in the iovec.
              iov[iovcnt].iov_base = current_message_block->rd_ptr ();
              iov[iovcnt].iov_len  = current_message_block_length;

              // Increment iovec counter.
              iovcnt++;

              // The buffer is full make a OS call.  @@ TODO find a way to
              // find ACE_IOV_MAX for platforms that do not define it rather
              // than simply setting ACE_IOV_MAX to some arbitrary value such
              // as 16.
              if (iovcnt == ACE_IOV_MAX)
                {
                  size_t current_transfer = 0;

                  ssize_t result = ACE::recvv_n (handle,
                                                 iov,
                                                 iovcnt,
                                                 timeout,
                                                 &current_transfer);

                  // Add to total bytes transferred.
                  bytes_transferred += current_transfer;

                  // Errors.
                  if (result == -1 || result == 0)
                    return result;

                  // Reset iovec counter.
                  iovcnt = 0;
                }
            }

          // Select the next message block in the chain.
          current_message_block = current_message_block->cont ();
        }

      // Selection of the next message block chain.
      message_block = message_block->next ();
    }

  // Check for remaining buffers to be sent.  This will happen when
  // ACE_IOV_MAX is not a multiple of the number of message blocks.
  if (iovcnt != 0)
    {
      size_t current_transfer = 0;

      ssize_t result = ACE::recvv_n (handle,
                                     iov,
                                     iovcnt,
                                     timeout,
                                     &current_transfer);

      // Add to total bytes transferred.
      bytes_transferred += current_transfer;

      // Errors.
      if (result == -1 || result == 0)
        return result;
    }

  // Return total bytes transferred.
  return bytes_transferred;
}

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           int flags,
           const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::send (handle, (const char *) buf, n, flags);
  else
    {
#if defined (ACE_HAS_SEND_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday();
      timespec_t ts = copy;
      return ::send_timedwait (handle, buf, n, flags, &ts);
#else
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::send (handle, (const char *) buf, n, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_SEND_TIMEDWAIT */
    }
}

#if defined (ACE_HAS_TLI)

ssize_t
ACE::t_snd (ACE_HANDLE handle,
            const void *buf,
            size_t n,
            int flags,
            const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::t_snd (handle, (const char *) buf, n, flags);
  else
    {
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::t_snd (handle, (const char *) buf, n, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
    }
}

#endif /* ACE_HAS_TLI */

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE::send_i (handle, buf, n);
  else
    {
#if defined (ACE_HAS_WRITE_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::write_timedwait (handle, buf, n, &ts);
#else
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE::send_i (handle, buf, n);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_WRITE_TIMEDWAIT */
    }
}

ssize_t
ACE::sendmsg (ACE_HANDLE handle,
              const struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::sendmsg (handle, msg, flags);
  else
    {
#if defined (ACE_HAS_SENDMSG_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendmsg_timedwait (handle, msg, flags, &ts);
#else
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          int bytes_transferred = ACE_OS::sendmsg (handle, msg, flags);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_SENDMSG_TIMEDWAIT */
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
  if (timeout == 0)
    return ACE_OS::sendto (handle, buf, len, flags, addr, addrlen);
  else
    {
#if defined (ACE_HAS_SENDTO_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendto_timedwait (handle, buf, len, flags, addr, addrlen, ts);
#else
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          int bytes_transferred = ACE_OS::sendto (handle, buf, len, flags, addr, addrlen);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_SENDTO_TIMEDWAIT */
    }
}

ssize_t
ACE::send_n_i (ACE_HANDLE handle,
               const void *buf,
               size_t len,
               int flags,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE_OS::send (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred,
                        flags);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_write_ready (handle,
                                                    0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::send_n_i (ACE_HANDLE handle,
               const void *buf,
               size_t len,
               int flags,
               const ACE_Time_Value *timeout,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE_OS::send (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred,
                        flags);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_write_ready (handle,
                                                 timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

#if defined (ACE_HAS_TLI)

ssize_t
ACE::t_snd_n_i (ACE_HANDLE handle,
                const void *buf,
                size_t len,
                int flags,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE_OS::t_snd (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred,
                         flags);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_write_ready (handle,
                                                    0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::t_snd_n_i (ACE_HANDLE handle,
                const void *buf,
                size_t len,
                int flags,
                const ACE_Time_Value *timeout,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE_OS::t_snd (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred,
                         flags);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_write_ready (handle,
                                                 timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

#endif /* ACE_HAS_TLI */

ssize_t
ACE::send_n_i (ACE_HANDLE handle,
               const void *buf,
               size_t len,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      n = ACE::send_i (handle,
                       (char *) buf + bytes_transferred,
                       len - bytes_transferred);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_write_ready (handle,
                                                    0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }
    }

  return bytes_transferred;
}

ssize_t
ACE::send_n_i (ACE_HANDLE handle,
               const void *buf,
               size_t len,
               const ACE_Time_Value *timeout,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      n = ACE::send_i (handle,
                       (char *) buf + bytes_transferred,
                       len - bytes_transferred);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_write_ready (handle,
                                                 timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }
    }

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

// Send N char *ptrs and int lengths.  Note that the char *'s precede
// the ints (basically, an varargs version of writev).  The count N is
// the *total* number of trailing arguments, *not* a couple of the
// number of tuple pairs!

ssize_t
ACE::send (ACE_HANDLE handle, size_t n, ...)
{
  va_list argp;
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp,
                  iovec[total_tuples],
                  -1);
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

ssize_t
ACE::sendv (ACE_HANDLE handle,
            const iovec *iov,
            int iovcnt,
            const ACE_Time_Value *timeout)
{
  if (timeout == 0)
    return ACE_OS::sendv (handle, iov, iovcnt);
  else
    {
#if defined (ACE_HAS_WRITEV_TIMEDWAIT)
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendv_timedwait (handle, iov, iovcnt, &ts);
#else
      int val = 0;
      if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
        return -1;
      else
        {
          ssize_t bytes_transferred = ACE_OS::sendv (handle, iov, iovcnt);
          ACE::restore_non_blocking_mode (handle, val);
          return bytes_transferred;
        }
#endif /* ACE_HAS_WRITEV_TIMEDWAIT */
    }
}

ssize_t
ACE::sendv_n_i (ACE_HANDLE handle,
                const iovec *i,
                int iovcnt,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  iovec *iov = ACE_const_cast (iovec *, i);

  for (int s = 0;
       s < iovcnt;
       )
    {
      // Try to transfer as much of the remaining data as possible.
      ssize_t n = ACE_OS::sendv (handle,
                                 iov + s,
                                 iovcnt - s);
      // Check EOF.
      if (n == 0)
        return 0;

      // Check for other errors.
      if (n == -1)
        {
          // Check for possible blocking.
          if (errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait for the blocking to subside.
              int result = ACE::handle_write_ready (handle,
                                                    0);

              // Did select() succeed?
              if (result != -1)
                {
                  // Blocking subsided.  Continue data transfer.
                  n = 0;
                  continue;
                }
            }

          // Other data transfer or select() failures.
          return -1;
        }

      for (bytes_transferred += n;
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

  return bytes_transferred;
}

ssize_t
ACE::sendv_n_i (ACE_HANDLE handle,
                const iovec *i,
                int iovcnt,
                const ACE_Time_Value *timeout,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;
  ssize_t result = 0;
  int error = 0;

  int val = 0;
  ACE::record_and_set_non_blocking_mode (handle, val);

  iovec *iov = ACE_const_cast (iovec *, i);

  for (int s = 0;
       s < iovcnt;
       )
    {
      // Try to transfer as much of the remaining data as possible.
      // Since the socket is in non-blocking mode, this call will not
      // block.
      ssize_t n = ACE_OS::sendv (handle,
                                 iov + s,
                                 iovcnt - s);

      // Check for errors.
      if (n == 0 ||
          n == -1)
        {
          // Check for possible blocking.
          if (n == -1 &&
              errno == EWOULDBLOCK || errno == ENOBUFS)
            {
              // Wait upto <timeout> for the blocking to subside.
              int rtn = ACE::handle_write_ready (handle,
                                                 timeout);

              // Did select() succeed?
              if (rtn != -1)
                {
                  // Blocking subsided in <timeout> period.  Continue
                  // data transfer.
                  n = 0;
                  continue;
                }
            }

          // Wait in select() timed out or other data transfer or
          // select() failures.
          error = 1;
          result = n;
          break;
        }

      for (bytes_transferred += n;
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

  ACE::restore_non_blocking_mode (handle, val);

  if (error)
    return result;
  else
    return bytes_transferred;
}

ssize_t
ACE::write_n (ACE_HANDLE handle,
              const ACE_Message_Block *message_block,
              size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;

  while (message_block != 0)
    {
      // Our current message block chain.
      const ACE_Message_Block *current_message_block = message_block;

      while (current_message_block != 0)
        {
          size_t current_message_block_length =
            current_message_block->length ();

          // Check if this block has any data to be sent.
          if (current_message_block_length > 0)
            {
              // Collect the data in the iovec.
              iov[iovcnt].iov_base = current_message_block->rd_ptr ();
              iov[iovcnt].iov_len  = current_message_block_length;

              // Increment iovec counter.
              iovcnt++;

              // The buffer is full make a OS call.  @@ TODO find a way to
              // find ACE_IOV_MAX for platforms that do not define it rather
              // than simply setting ACE_IOV_MAX to some arbitrary value such
              // as 16.
              if (iovcnt == ACE_IOV_MAX)
                {
                  size_t current_transfer = 0;

                  ssize_t result = ACE::writev_n (handle,
                                                  iov,
                                                  iovcnt,
                                                  &current_transfer);

                  // Add to total bytes transferred.
                  bytes_transferred += current_transfer;

                  // Errors.
                  if (result == -1 || result == 0)
                    return result;

                  // Reset iovec counter.
                  iovcnt = 0;
                }
            }

          // Select the next message block in the chain.
          current_message_block = current_message_block->cont ();
        }

      // Selection of the next message block chain.
      message_block = message_block->next ();
    }

  // Check for remaining buffers to be sent.  This will happen when
  // ACE_IOV_MAX is not a multiple of the number of message blocks.
  if (iovcnt != 0)
    {
      size_t current_transfer = 0;

      ssize_t result = ACE::writev_n (handle,
                                      iov,
                                      iovcnt,
                                      &current_transfer);

      // Add to total bytes transferred.
      bytes_transferred += current_transfer;

      // Errors.
      if (result == -1 || result == 0)
        return result;
    }

  // Return total bytes transferred.
  return bytes_transferred;
}

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const ACE_Message_Block *message_block,
             const ACE_Time_Value *timeout,
             size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;

  while (message_block != 0)
    {
      // Our current message block chain.
      const ACE_Message_Block *current_message_block = message_block;

      while (current_message_block != 0)
        {
          size_t current_message_block_length =
            current_message_block->length ();

          // Check if this block has any data to be sent.
          if (current_message_block_length > 0)
            {
              // Collect the data in the iovec.
              iov[iovcnt].iov_base = current_message_block->rd_ptr ();
              iov[iovcnt].iov_len  = current_message_block_length;

              // Increment iovec counter.
              iovcnt++;

              // The buffer is full make a OS call.  @@ TODO find a way to
              // find ACE_IOV_MAX for platforms that do not define it rather
              // than simply setting ACE_IOV_MAX to some arbitrary value such
              // as 16.
              if (iovcnt == ACE_IOV_MAX)
                {
                  size_t current_transfer = 0;

                  ssize_t result = ACE::sendv_n (handle,
                                                 iov,
                                                 iovcnt,
                                                 timeout,
                                                 &current_transfer);

                  // Add to total bytes transferred.
                  bytes_transferred += current_transfer;

                  // Errors.
                  if (result == -1 || result == 0)
                    return result;

                  // Reset iovec counter.
                  iovcnt = 0;
                }
            }

          // Select the next message block in the chain.
          current_message_block = current_message_block->cont ();
        }

      // Selection of the next message block chain.
      message_block = message_block->next ();
    }

  // Check for remaining buffers to be sent.  This will happen when
  // ACE_IOV_MAX is not a multiple of the number of message blocks.
  if (iovcnt != 0)
    {
      size_t current_transfer = 0;

      ssize_t result = ACE::sendv_n (handle,
                                     iov,
                                     iovcnt,
                                     timeout,
                                     &current_transfer);

      // Add to total bytes transferred.
      bytes_transferred += current_transfer;

      // Errors.
      if (result == -1 || result == 0)
        return result;
    }

  // Return total bytes transferred.
  return bytes_transferred;
}

ssize_t
ACE::readv_n (ACE_HANDLE handle,
              iovec *iov,
              int iovcnt,
              size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  for (int s = 0;
       s < iovcnt;
       )
    {
      ssize_t n = ACE_OS::readv (handle,
                                 iov + s,
                                 iovcnt - s);

      if (n == -1 || n == 0)
        return n;

      for (bytes_transferred += n;
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

  return bytes_transferred;
}

ssize_t
ACE::writev_n (ACE_HANDLE handle,
               const iovec *i,
               int iovcnt,
               size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  bytes_transferred = 0;

  iovec *iov = ACE_const_cast (iovec *, i);

  for (int s = 0;
       s < iovcnt;
       )
    {
      ssize_t n = ACE_OS::writev (handle,
                                  iov + s,
                                  iovcnt - s);
      if (n == -1 || n == 0)
        return n;

      for (bytes_transferred += n;
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

  return bytes_transferred;
}

int
ACE::handle_ready (ACE_HANDLE handle,
                   const ACE_Time_Value *timeout,
                   int read_ready,
                   int write_ready,
                   int exception_ready)
{
#if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)
  ACE_UNUSED_ARG (write_ready);
  ACE_UNUSED_ARG (exception_ready);

  struct pollfd fds;

  fds.fd = handle;
  fds.events = read_ready ? POLLIN : POLLOUT;
  fds.revents = 0;

  int result = ACE_OS::poll (&fds, 1, timeout);
#else
  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // Wait for data or for the timeout to elapse.
  int result = ACE_OS::select (int (handle) + 1,
                               read_ready ? handle_set.fdset () : 0, // read_fds.
                               write_ready ? handle_set.fdset () : 0, // write_fds.
                               exception_ready ? handle_set.fdset () : 0, // exception_fds.
                               timeout);

#endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

  switch (result)
    {
    case 0:  // Timer expired.
      errno = ETIME;
      /* FALLTHRU */
    case -1: // we got here directly - select() returned -1.
      return -1;
    case 1: // Handle has data.
      /* FALLTHRU */
    default: // default is case result > 0; return a
      // ACE_ASSERT (result == 1);
      return result;
    }
}

int
ACE::enter_recv_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int &val)
{
  int result = ACE::handle_read_ready (handle,
                                       timeout);

  if (result == -1)
    return -1;

  ACE::record_and_set_non_blocking_mode (handle,
                                         val);

  return result;
}

int
ACE::enter_send_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int &val)
{
  int result = ACE::handle_write_ready (handle,
                                        timeout);

  if (result == -1)
    return -1;

  ACE::record_and_set_non_blocking_mode (handle,
                                         val);

  return result;
}

void
ACE::record_and_set_non_blocking_mode (ACE_HANDLE handle,
                                       int &val)
{
  // We need to record whether we are already *in* nonblocking mode,
  // so that we can correctly reset the state when we're done.
  val = ACE::get_flags (handle);

  if (ACE_BIT_DISABLED (val, ACE_NONBLOCK))
    // Set the handle into non-blocking mode if it's not already in
    // it.
    ACE::set_flags (handle, ACE_NONBLOCK);
}

void
ACE::restore_non_blocking_mode (ACE_HANDLE handle,
                                int val)
{
  if (ACE_BIT_DISABLED (val,
                        ACE_NONBLOCK))
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (handle, ACE_NONBLOCK);
    }
}


// Format buffer into printable format.  This is useful for debugging.
// Portions taken from mdump by J.P. Knight (J.P.Knight@lut.ac.uk)
// Modifications by Todd Montgomery.

int
ACE::format_hexdump (const char *buffer,
                     int size,
                     ACE_TCHAR *obuf,
                     int obuf_sz)
{
  ACE_TRACE ("ACE::format_hexdump");

  u_char c;
  ACE_TCHAR textver[16 + 1];

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
          ACE_OS::sprintf (obuf,
                           ACE_LIB_TEXT ("%02x "),
                           c);
          obuf += 3;
          if (j == 7)
            {
              ACE_OS::sprintf (obuf,
                               ACE_LIB_TEXT (" "));
              obuf++;
            }
          textver[j] = ACE_OS::ace_isprint (c) ? c : '.';
        }

      textver[j] = 0;

      ACE_OS::sprintf (obuf,
                       ACE_LIB_TEXT ("  %s\n"),
                       textver);

      while (*obuf != '\0')
        obuf++;
    }

  if (size % 16)
    {
      for (i = 0 ; i < size % 16; i++)
        {
          c = (u_char) buffer[size - size % 16 + i];
          ACE_OS::sprintf (obuf,
                           ACE_LIB_TEXT ("%02x "),
                           c);
          obuf += 3;
          if (i == 7)
            {
              ACE_OS::sprintf (obuf,
                               ACE_LIB_TEXT (" "));
              obuf++;
            }
          textver[i] = ACE_OS::ace_isprint (c) ? c : '.';
        }

      for (i = size % 16; i < 16; i++)
        {
          ACE_OS::sprintf (obuf,
                           ACE_LIB_TEXT ("   "));
          obuf += 3;
          textver[i] = ' ';
        }

      textver[i] = 0;
      ACE_OS::sprintf (obuf,
                       ACE_LIB_TEXT ("  %s\n"),
                       textver);
    }
  return size;
}

// Returns the current timestamp in the form
// "hour:minute:second:microsecond."  The month, day, and year are
// also stored in the beginning of the date_and_time array.

ACE_TCHAR *
ACE::timestamp (ACE_TCHAR date_and_time[],
                int date_and_timelen,
                int return_pointer_to_first_digit)
{
  //ACE_TRACE ("ACE::timestamp");

  if (date_and_timelen < 35)
    {
      errno = EINVAL;
      return 0;
    }

#if defined (WIN32)
   // Emulate Unix.  Win32 does NOT support all the UNIX versions
   // below, so DO we need this ifdef.
  static const ACE_TCHAR *day_of_week_name[] =
  {
    ACE_LIB_TEXT ("Sun"),
    ACE_LIB_TEXT ("Mon"),
    ACE_LIB_TEXT ("Tue"),
    ACE_LIB_TEXT ("Wed"),
    ACE_LIB_TEXT ("Thu"),
    ACE_LIB_TEXT ("Fri"),
    ACE_LIB_TEXT ("Sat")
  };

  static const ACE_TCHAR *month_name[] =
  {
    ACE_LIB_TEXT ("Jan"),
    ACE_LIB_TEXT ("Feb"),
    ACE_LIB_TEXT ("Mar"),
    ACE_LIB_TEXT ("Apr"),
    ACE_LIB_TEXT ("May"),
    ACE_LIB_TEXT ("Jun"),
    ACE_LIB_TEXT ("Jul"),
    ACE_LIB_TEXT ("Aug"),
    ACE_LIB_TEXT ("Sep"),
    ACE_LIB_TEXT ("Oct"),
    ACE_LIB_TEXT ("Nov"),
    ACE_LIB_TEXT ("Dec")
  };

  SYSTEMTIME local;
  ::GetLocalTime (&local);

  ACE_OS::sprintf (date_and_time,
                   ACE_LIB_TEXT ("%3s %3s %2d %04d %02d:%02d:%02d.%06d"),
                   day_of_week_name[local.wDayOfWeek],
                   month_name[local.wMonth - 1],
                   (int) local.wDay,
                   (int) local.wYear,
                   (int) local.wHour,
                   (int) local.wMinute,
                   (int) local.wSecond,
                   (int) (local.wMilliseconds * 1000));
  return &date_and_time[15 + (return_pointer_to_first_digit != 0)];
#else  /* UNIX */
  ACE_TCHAR timebuf[26]; // This magic number is based on the ctime(3c) man page.
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();
  time_t secs = cur_time.sec ();

  ACE_OS::ctime_r (&secs,
                   timebuf,
                   sizeof timebuf);
  // date_and_timelen > sizeof timebuf!
  ACE_OS::strsncpy (date_and_time,
                    timebuf,
                    date_and_timelen);
  char yeartmp[5];
  ACE_OS::strsncpy (yeartmp,
                    &date_and_time[20],
                    5);
  char timetmp[9];
  ACE_OS::strsncpy (timetmp,
                    &date_and_time[11],
                    9);
  ACE_OS::sprintf (&date_and_time[11],
                   "%s %s.%06ld",
                   yeartmp,
                   timetmp,
                   cur_time.usec ());
  date_and_time[33] = '\0';
  return &date_and_time[15 + (return_pointer_to_first_digit != 0)];
#endif /* WIN32 */
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
                            const ACE_Time_Value *timeout,
                            int is_tli)
{
  ACE_TRACE ("ACE::handle_timed_complete");

#if !defined (ACE_WIN32) && defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

  struct pollfd fds;

  fds.fd = h;
  fds.events = POLLIN | POLLOUT;
  fds.revents = 0;

#else
  ACE_Handle_Set rd_handles;
  ACE_Handle_Set wr_handles;

  rd_handles.set_bit (h);
  wr_handles.set_bit (h);
#endif /* !ACE_WIN32 && ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

#if defined (ACE_WIN32)
  // Winsock is different - it sets the exception bit for failed connect,
  // unlike other platforms, where the read bit is set.
  ACE_Handle_Set ex_handles;
  ex_handles.set_bit (h);
#endif /* ACE_WIN32 */

  int need_to_check = 0;
  int known_failure = 0;

#if defined (ACE_WIN32)
  int n = ACE_OS::select (int (h) + 1,
                          0,
                          wr_handles,
                          ex_handles,
                          timeout);
#else
# if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

  int n = ACE_OS::poll (&fds, 1, timeout);

# else
  int n = ACE_OS::select (int (h) + 1,
                          rd_handles,
                          wr_handles,
                          0,
                          timeout);
# endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */
#endif /* ACE_WIN32 */

  // If we failed to connect within the time period allocated by the
  // caller, then we fail (e.g., the remote host might have been too
  // busy to accept our call).
  if (n <= 0)
    {
      if (n == 0 && timeout != 0)
        errno = ETIME;
      return ACE_INVALID_HANDLE;
    }

  // Usually, a ready-for-write handle is successfully connected, and
  // ready-for-read (exception on Win32) is a failure. On fails, we
  // need to grab the error code via getsockopt. On possible success for
  // any platform where we can't tell just from select() (e.g. AIX),
  // we also need to check for success/fail.
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (is_tli);

  // On Win32, ex_handle set indicates a failure. We'll do the check
  // to try and get an errno value, but the connect failed regardless of
  // what getsockopt says about the error.
  if (ex_handles.is_set (h))
    {
      need_to_check = 1;
      known_failure = 1;
    }
#elif defined (VXWORKS)
  ACE_UNUSED_ARG (is_tli);

  // Force the check on VxWorks.  The read handle for "h" is not set,
  // so "need_to_check" is false at this point.  The write handle is
  // set, for what it's worth.
  need_to_check = 1;
#else
  if (is_tli)

# if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)
    need_to_check = (fds.revents & POLLIN) && !(fds.revents & POLLOUT);
# else
    need_to_check = rd_handles.is_set (h) && !wr_handles.is_set (h);
# endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

  else
#if defined(AIX)
    // AIX is broken... both success and failed connect will set the
    // write handle only, so always check.
    need_to_check = 1;
#else
# if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)
  need_to_check = (fds.revents & POLLIN);
# else
  need_to_check = rd_handles.is_set (h);
# endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */
#endif /* AIX */
#endif /* ACE_WIN32 */

  if (need_to_check)
    {
#if defined (SOL_SOCKET) && defined (SO_ERROR)
      int sock_err = 0;
      int sock_err_len = sizeof (sock_err);
      ACE_OS::getsockopt (h, SOL_SOCKET, SO_ERROR,
                          (char *)&sock_err, &sock_err_len);
      if (sock_err != 0 || known_failure)
        {
          h = ACE_INVALID_HANDLE;
          errno = sock_err;
        }
#else
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
#endif
    }

  // 1. The HANDLE is ready for writing and doesn't need to be checked or
  // 2. recv() returned an indication of the state of the socket - if there is
  // either data present, or a recv is legit but there's no data yet,
  // the connection was successfully established.
  return h;
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

#if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

  struct pollfd fds;

  fds.fd = listener;
  fds.events = POLLIN;
  fds.revents = 0;

#else
  // Use the select() implementation rather than poll().
  ACE_Handle_Set rd_handle;
  rd_handle.set_bit (listener);
#endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

  // We need a loop here if <restart> is enabled.

  for (;;)
    {
#if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

      int n = ACE_OS::poll (&fds, 1, timeout);

#else
      int n = ACE_OS::select (int (listener) + 1,
                              rd_handle, 0, 0,
                              timeout);
#endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

      switch (n)
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
            errno = ETIMEDOUT;
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

// Make the current process a UNIX daemon.  This is based on Stevens
// code from APUE.

int
ACE::daemonize (const ACE_TCHAR pathname[],
                int close_all_handles,
                const ACE_TCHAR program_name[])
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
ACE::fork (const ACE_TCHAR *program_name,
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
        ACE_exitcode status;
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
  int r = ACE_OS::getrlimit (RLIMIT_NOFILE, &rl);
# if !defined (RLIM_INFINITY)
  if (r == 0)
    return rl.rlim_cur;
#else
  if (r == 0 && rl.rlim_cur != RLIM_INFINITY)
    return rl.rlim_cur;
  // If == RLIM_INFINITY, fall through to the ACE_LACKS_RLIMIT sections
# endif /* RLIM_INFINITY */
#endif /* RLIMIT_NOFILE && !ACE_LACKS_RLIMIT */

#if defined (_SC_OPEN_MAX)
  return ACE_OS::sysconf (_SC_OPEN_MAX);
#elif defined (FD_SETSIZE)
  return FD_SETSIZE;
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* _SC_OPEN_MAX */
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
  int r = ACE_OS::getrlimit (RLIMIT_NOFILE, &rl);
  if (r == 0)
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

const ACE_TCHAR *
ACE::sock_error (int error)
{
#if defined (ACE_WIN32)
  static ACE_TCHAR unknown_msg[64];

  switch (error)
    {
    case WSAVERNOTSUPPORTED:
      return ACE_LIB_TEXT ("version of WinSock not supported");
      /* NOTREACHED */
    case WSASYSNOTREADY:
      return ACE_LIB_TEXT ("WinSock not present or not responding");
      /* NOTREACHED */
    case WSAEINVAL:
      return ACE_LIB_TEXT ("app version not supported by DLL");
      /* NOTREACHED */
    case WSAHOST_NOT_FOUND:
      return ACE_LIB_TEXT ("Authoritive: Host not found");
      /* NOTREACHED */
    case WSATRY_AGAIN:
      return ACE_LIB_TEXT ("Non-authoritive: host not found or server failure");
      /* NOTREACHED */
    case WSANO_RECOVERY:
      return ACE_LIB_TEXT ("Non-recoverable: refused or not implemented");
      /* NOTREACHED */
    case WSANO_DATA:
      return ACE_LIB_TEXT ("Valid name, no data record for type");
      /* NOTREACHED */
      /*
        case WSANO_ADDRESS:
        return "Valid name, no MX record";
      */
    case WSANOTINITIALISED:
      return ACE_LIB_TEXT ("WSA Startup not initialized");
      /* NOTREACHED */
    case WSAENETDOWN:
      return ACE_LIB_TEXT ("Network subsystem failed");
      /* NOTREACHED */
    case WSAEINPROGRESS:
      return ACE_LIB_TEXT ("Blocking operation in progress");
      /* NOTREACHED */
    case WSAEINTR:
      return ACE_LIB_TEXT ("Blocking call cancelled");
      /* NOTREACHED */
    case WSAEAFNOSUPPORT:
      return ACE_LIB_TEXT ("address family not supported");
      /* NOTREACHED */
    case WSAEMFILE:
      return ACE_LIB_TEXT ("no file handles available");
      /* NOTREACHED */
    case WSAENOBUFS:
      return ACE_LIB_TEXT ("no buffer space available");
      /* NOTREACHED */
    case WSAEPROTONOSUPPORT:
      return ACE_LIB_TEXT ("specified protocol not supported");
      /* NOTREACHED */
    case WSAEPROTOTYPE:
      return ACE_LIB_TEXT ("protocol wrong type for this socket");
      /* NOTREACHED */
    case WSAESOCKTNOSUPPORT:
      return ACE_LIB_TEXT ("socket type not supported for address family");
      /* NOTREACHED */
    case WSAENOTSOCK:
      return ACE_LIB_TEXT ("handle is not a socket");
      /* NOTREACHED */
    case WSAEWOULDBLOCK:
      return ACE_LIB_TEXT ("socket marked as non-blocking and SO_LINGER set not 0");
      /* NOTREACHED */
    case WSAEADDRINUSE:
      return ACE_LIB_TEXT ("address already in use");
      /* NOTREACHED */
    case WSAECONNABORTED:
      return ACE_LIB_TEXT ("connection aborted");
      /* NOTREACHED */
    case WSAECONNRESET:
      return ACE_LIB_TEXT ("connection reset");
      /* NOTREACHED */
    case WSAENOTCONN:
      return ACE_LIB_TEXT ("not connected");
      /* NOTREACHED */
    case WSAETIMEDOUT:
      return ACE_LIB_TEXT ("connection timed out");
      /* NOTREACHED */
    case WSAECONNREFUSED:
      return ACE_LIB_TEXT ("connection refused");
      /* NOTREACHED */
    case WSAEHOSTDOWN:
      return ACE_LIB_TEXT ("host down");
      /* NOTREACHED */
    case WSAEHOSTUNREACH:
      return ACE_LIB_TEXT ("host unreachable");
      /* NOTREACHED */
    case WSAEADDRNOTAVAIL:
      return ACE_LIB_TEXT ("address not available");
      /* NOTREACHED */
    default:
      ACE_OS::sprintf (unknown_msg, ACE_LIB_TEXT ("unknown error: %d"), error);
      return unknown_msg;
      /* NOTREACHED */
    }
#else
  ACE_UNUSED_ARG (error);
  ACE_NOTSUP_RETURN (0);
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
  return ACE_OS::strsncpy (s, str, len + 1);
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE::strndup (const wchar_t *str, size_t n)
{
  const wchar_t *t = str;
  size_t len;

  // Figure out how long this string is (remember, it might not be
  // NUL-terminated).

  for (len = 0;
       len < n && *t++ != '\0';
       len++)
    continue;

  wchar_t *s;
  ACE_ALLOCATOR_RETURN (s,
                        ACE_static_cast (wchar_t *,
                                         ACE_OS::malloc ((len + 1)
                                                         * sizeof (wchar_t))),
                        0);
  return ACE_OS::strsncpy (s, str, len + 1);
}
#endif /* ACE_HAS_WCHAR */

char *
ACE::strnnew (const char *str, size_t n)
{
  const char *t = str;
  size_t len;

  // Figure out how long this string is (remember, it might not be
  // NUL-terminated).

  for (len = 0;
       len < n && *t++ != L'\0';
       len++)
    continue;

  char *s;
  ACE_NEW_RETURN (s,
                  char[len + 1],
                  0);
  return ACE_OS::strsncpy (s, str, len + 1);
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE::strnnew (const wchar_t *str, size_t n)
{
  const wchar_t *t = str;
  size_t len;

  // Figure out how long this string is (remember, it might not be
  // NUL-terminated).

  for (len = 0;
       len < n && *t++ != ACE_TEXT_WIDE ('\0');
       len++)
    continue;

  wchar_t *s;
  ACE_NEW_RETURN (s,
                  wchar_t[len + 1],
                  0);
  return ACE_OS::strsncpy (s, str, len + 1);
}
#endif /* ACE_HAS_WCHAR */

const char *
ACE::strend (const char *s)
{
  while (*s++ != '\0')
    continue;

  return s;
}

#if defined ACE_HAS_WCHAR
const wchar_t *
ACE::strend (const wchar_t *s)
{
  while (*s++ != ACE_TEXT_WIDE ('\0'))
    continue;

  return s;
}
#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) && (defined (__unix) || defined (__unix__) || defined (__Lynx__) || defined (_AIX))
template class ACE_Auto_Array_Ptr<struct ifreq>;
template class ACE_Auto_Basic_Array_Ptr<struct ifreq>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Array_Ptr<struct ifreq>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<struct ifreq>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION && (__unix || __Lynx_) */
