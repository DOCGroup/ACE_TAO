// $Id$

// ATM_Stream.i

#include "ace/ATM_Stream.h"

ACE_INLINE
ACE_ATM_Stream::ACE_ATM_Stream (void)
{
  ACE_TRACE ("ACE_ATM_Stream::ACE_ATM_Stream");
}

ACE_INLINE
int
ACE_ATM_Stream::open (ACE_ATM_Params params)
{
  ACE_TRACE ("ACE_ATM_Stream::open");
#if defined (ACE_HAS_FORE_ATM_XTI)
  ACE_HANDLE handle = stream_.open (params.get_device(),
                                    params.get_oflag(),
                                    params.get_info());
  return (handle == ACE_INVALID_HANDLE ? -1 : 0);
#elif defined (ACE_HAS_FORE_ATM_WS2)
  params.set_flags( ACE_FLAG_MULTIPOINT_C_ROOT | ACE_FLAG_MULTIPOINT_D_ROOT );

  return stream_.open (params.get_type(),
                       params.get_protocol_family(),
                       params.get_protocol(),
                       params.get_protocol_info(),
                       params.get_sock_group(),
                       params.get_flags(),
                       params.get_reuse_addr());
#else
  ACE_UNUSED_ARG(params);
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

ACE_INLINE
int
ACE_ATM_Stream::close (void)
{
  ACE_TRACE ("ACE_ATM_Stream::close");
#if defined (ACE_HAS_FORE_ATM_XTI) || defined (ACE_HAS_FORE_ATM_WS2)
  return stream_.close ();
#else
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI || ACE_HAS_FORE_ATM_WS2 */
}

//ACE_INLINE
//ACE_HANDLE
//ACE_ATM_Stream::get_handle (void) const
//{
//  ACE_TRACE ("ACE_ATM_Stream::get_handle");
//#if defined (ACE_HAS_FORE_ATM_XTI) || defined (ACE_HAS_FORE_ATM_WS2)
//  return ((ACE_SOCK_Stream)stream_).get_handle ();
//#else
//  return 0;
//#endif /* ACE_HAS_FORE_ATM_XTI || ACE_HAS_FORE_ATM_WS2 */
//}

ACE_INLINE
ATM_Stream&
ACE_ATM_Stream::get_stream (void)
{
  ACE_TRACE ("ACE_ATM_Stream::get_stream");
  return stream_;
}

ACE_INLINE
ssize_t
ACE_ATM_Stream::recv (void *buf,
                      size_t n,
                      int *flags) const
{
  ACE_TRACE ("ACE_ATM_Stream::recv");
#if defined (ACE_HAS_FORE_ATM_XTI)
  return stream_.recv (buf,
                       n,
                       flags);
#elif defined (ACE_HAS_FORE_ATM_WS2)
  return stream_.recv (buf,
                       n);
#else
  ACE_UNUSED_ARG(buf);
  ACE_UNUSED_ARG(n);
  ACE_UNUSED_ARG(flags);
  return (0);
#endif /* ACE_HAS_FORE_ATM_XTI */
}

ACE_INLINE
ssize_t
ACE_ATM_Stream::send_n (const void *buf,
                        size_t n,
                        int flags) const
{
  ACE_TRACE ("ACE_ATM_Stream::send_n");
#if defined (ACE_HAS_FORE_ATM_XTI)
  return stream_.send_n (buf,
                         n,
                         flags);
#elif defined (ACE_HAS_FORE_ATM_WS2)
  return stream_.send_n (buf,
                         n,
                         flags);
#else
  ACE_UNUSED_ARG(buf);
  ACE_UNUSED_ARG(n);
  ACE_UNUSED_ARG(flags);
  return (0);
#endif /* ACE_HAS_FORE_ATM_XTI */
}

