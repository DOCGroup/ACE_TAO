// -*- C++ -*-
// $Id$

#include "ace/OS_NS_sys_uio.h"

ACE_RCSID(ace, OS_NS_sys_uio, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_sys_uio.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

# if defined (ACE_LACKS_READV)

// "Fake" readv for operating systems without it.  Note that this is
// thread-safe.

ssize_t
ACE_OS::readv_emulation (ACE_HANDLE handle,
                         ACE_READV_TYPE *iov,
                         int n)
{
  ACE_OS_TRACE ("ACE_OS::readv_emulation");

  ssize_t length = 0;
  int i;

  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;
#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  length = ACE_OS::read (handle, buf, length);

  if (length != -1)
    {
      char *ptr = buf;
      ssize_t copyn = length;

      for (i = 0;
           i < n && copyn > 0;
           i++)
        {
          ACE_OS::memcpy (iov[i].iov_base, ptr,
                          // iov_len is int on some platforms, size_t on others
                          copyn > (int) iov[i].iov_len
                            ? (size_t) iov[i].iov_len
                            : (size_t) copyn);
          ptr += iov[i].iov_len;
          copyn -= iov[i].iov_len;
        }
    }

#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return length;
}
# endif /* ACE_LACKS_READV */

# if defined (ACE_LACKS_WRITEV)

// "Fake" writev for operating systems without it.  Note that this is
// thread-safe.

int
ACE_OS::writev_emulation (ACE_HANDLE handle, ACE_WRITEV_TYPE iov[], int n)
{
  ACE_OS_TRACE ("ACE_OS::writev_emulation");

  size_t length = 0;
  int i;

  // Determine the total length of all the buffers in <iov>.
  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;

#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  char *ptr = buf;

  for (i = 0; i < n; i++)
    {
      ACE_OS::memcpy (ptr, iov[i].iov_base, iov[i].iov_len);
      ptr += iov[i].iov_len;
    }

  ssize_t result = ACE_OS::write (handle, buf, length);
#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return result;
}
# endif /* ACE_LACKS_WRITEV */
