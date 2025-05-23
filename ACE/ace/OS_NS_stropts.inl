// -*- C++ -*-
#include "ace/os_include/os_errno.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_macros.h"
#include "ace/OS_Memory.h"
#include "ace/OS_QoS.h"
#include "ace/Global_Macros.h"

#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

typedef const struct strbuf *ACE_STRBUF_TYPE;

ACE_INLINE
ACE_Str_Buf::ACE_Str_Buf (void *b, int l, int max)
{
  this->maxlen = max;
  this->len = l;
  this->buf = (char *) b;
}

ACE_INLINE
ACE_Str_Buf::ACE_Str_Buf (strbuf &sb)
{
  this->maxlen = sb.maxlen;
  this->len = sb.len;
  this->buf = sb.buf;
}

/*****************************************************************************/

ACE_INLINE int
ACE_OS::getmsg (ACE_HANDLE handle,
                struct strbuf *ctl,
                struct strbuf *data,
                int *flags)
{
  ACE_OS_TRACE ("ACE_OS::getmsg");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::getmsg (handle, ctl, data, flags);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (ctl);
  ACE_UNUSED_ARG (data);
  ACE_UNUSED_ARG (flags);

  // I'm not sure how to implement this correctly.
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::getpmsg (ACE_HANDLE handle,
                 struct strbuf *ctl,
                 struct strbuf *data,
                 int *band,
                 int *flags)
{
  ACE_OS_TRACE ("ACE_OS::getpmsg");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::getpmsg (handle, ctl, data, band, flags);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (ctl);
  ACE_UNUSED_ARG (data);
  ACE_UNUSED_ARG (band);
  ACE_UNUSED_ARG (flags);

  // I'm not sure how to implement this correctly.
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::fattach (int handle, const char *path)
{
  ACE_OS_TRACE ("ACE_OS::fattach");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::fattach (handle, path);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (path);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::fdetach (const char *file)
{
  ACE_OS_TRACE ("ACE_OS::fdetach");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::fdetach (file);
#else
  ACE_UNUSED_ARG (file);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::ioctl (ACE_HANDLE handle,
               ACE_IOCTL_TYPE_ARG2 cmd,
               void *val)
{
  ACE_OS_TRACE ("ACE_OS::ioctl");
#if defined (ACE_LACKS_IOCTL)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (cmd);
  ACE_UNUSED_ARG (val);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_WIN32)
  ACE_SOCKET sock = (ACE_SOCKET) handle;
  ACE_SOCKCALL_RETURN (::ioctlsocket (sock, cmd, reinterpret_cast<unsigned long *> (val)), int, -1);
#elif defined (ACE_HAS_IOCTL_INT_3_PARAM)
  return ::ioctl (handle, cmd, reinterpret_cast<int> (val));
#elif defined (ACE_MQX)
  // TBD: See if there is a way to provide this functionality
  ACE_NOTSUP_RETURN (0);
#else
  return ::ioctl (handle, cmd, val);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::isastream (ACE_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::isastream");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::isastream (handle);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::putmsg (ACE_HANDLE handle, const struct strbuf *ctl,
                const struct strbuf *data, int flags)
{
  ACE_OS_TRACE ("ACE_OS::putmsg");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::putmsg (handle, (ACE_STRBUF_TYPE) ctl, (ACE_STRBUF_TYPE) data, flags);
#else
  ACE_UNUSED_ARG (flags);
  ssize_t result;
  if (ctl == 0 && data == 0)
    {
      errno = EINVAL;
      return 0;
    }
  // Handle the two easy cases.
  else if (ctl != 0)
    {
      result = ACE_OS::write (handle, ctl->buf, static_cast<size_t> (ctl->len));
      return static_cast<int> (result);
    }
  else if (data != 0)
    {
      result = ACE_OS::write (handle, data->buf, static_cast<size_t> (data->len));
      return static_cast<int> (result);
    }
  else
    {
      // This is the hard case.
      char *buf;
      const auto alloc {static_cast<unsigned int> (ctl->len) + static_cast<unsigned int> (data->len)};
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_ALLOCATOR_RETURN (buf, static_cast<char*> (ACE_Allocator::instance ()->malloc (alloc)), -1);
#else
      ACE_NEW_RETURN (buf, char [alloc], -1);
#endif /* ACE_HAS_ALLOC_HOOKS */
      ACE_OS::memcpy (buf, ctl->buf, static_cast<size_t> (ctl->len));
      ACE_OS::memcpy (buf + ctl->len, data->buf, static_cast<size_t> (data->len));
      result = ACE_OS::write (handle, buf, alloc);
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance()->free(buf);
#else
      delete [] buf;
#endif /* ACE_HAS_ALLOC_HOOKS */

      return static_cast<int> (result);
    }
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_INLINE int
ACE_OS::putpmsg (ACE_HANDLE handle,
                 const struct strbuf *ctl,
                 const struct strbuf *data,
                 int band,
                 int flags)
{
  ACE_OS_TRACE ("ACE_OS::putpmsg");
#if defined (ACE_HAS_STREAM_PIPES)
  return ::putpmsg (handle, (ACE_STRBUF_TYPE) ctl, (ACE_STRBUF_TYPE) data, band, flags);
#else
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (band);
  return ACE_OS::putmsg (handle, ctl, data, flags);
#endif /* ACE_HAS_STREAM_PIPES */
}

ACE_END_VERSIONED_NAMESPACE_DECL
