/* -*- C++ -*- */
// $Id$

#include "ace/OS_NS_errno.h"

ACE_INLINE int
ACE_OS::t_accept (ACE_HANDLE handle, 
                  ACE_HANDLE reshandle,
                  struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_accept (handle, reshandle, call), int, -1);
#else
  ACE_UNUSED_ARG (call);
  ACE_UNUSED_ARG (reshandle);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE char *
ACE_OS::t_alloc (ACE_HANDLE handle, int struct_type,
                 int fields)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_alloc (handle, struct_type, fields),
                     char *, 0);
#else
  ACE_UNUSED_ARG (fields);
  ACE_UNUSED_ARG (struct_type);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_bind (ACE_HANDLE handle, struct t_bind *req,
                struct t_bind *ret)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_bind (handle, req, ret), int, -1);
#else
  ACE_UNUSED_ARG (ret);
  ACE_UNUSED_ARG (req);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_close (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_close (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_connect(ACE_HANDLE fildes,
                  struct t_call *sndcall,
                  struct t_call *rcvcall)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_connect (fildes, sndcall, rcvcall), int, -1);
#else
  ACE_UNUSED_ARG (fildes);
  ACE_UNUSED_ARG (sndcall);
  ACE_UNUSED_ARG (rcvcall);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE void
ACE_OS::t_error (const char *errmsg)
{
#if defined (ACE_HAS_TLI)
#if defined (ACE_HAS_BROKEN_T_ERROR)
  ::t_error (ACE_const_cast (char *, errmsg));
#else
  ::t_error (errmsg);
#endif /* ACE_HAS_BROKEN_T_ERROR */
#else
  ACE_UNUSED_ARG (errmsg);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_free (char *ptr, int struct_type)
{
#if defined (ACE_HAS_TLI)
  if (ptr == 0)
    return 0;
  ACE_OSCALL_RETURN (::t_free (ptr, struct_type), int, -1);
#else
  ACE_UNUSED_ARG (struct_type);
  ACE_UNUSED_ARG (ptr);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_getinfo (ACE_HANDLE handle, struct t_info *info)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_getinfo (handle, info), int, -1);
#else
  ACE_UNUSED_ARG (info);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_getname (ACE_HANDLE handle,
                   struct netbuf *namep,
                   int type)
{
#if defined (ACE_HAS_XTI)
  struct t_bind bound, peer;
  // Depending on which address the caller wants, fill caller's values
  // into one of the t_bind netbufs. The other is set up to ignore that
  // address.
  switch (type)
    {
    case LOCALNAME:
      bound.addr.buf = namep->buf;
      bound.addr.maxlen = namep->maxlen;
      bound.addr.len = 0;
      peer.addr.buf = 0;
      peer.addr.maxlen = 0;
      peer.addr.len = 0;
      break;
    case REMOTENAME:
      bound.addr.buf = 0;
      bound.addr.maxlen = 0;
      bound.addr.len = 0;
      peer.addr.buf = namep->buf;
      peer.addr.maxlen = namep->maxlen;
      peer.addr.len = 0;
      break;
    default:
      ACE_OS::last_error (EINVAL);
      return -1;
    }
  if (t_getprotaddr (handle, &bound, &peer) == -1)
    return -1;
  // Call succeeded; put the caller's desired address length in his netbuf.
  if (type == LOCALNAME)
    namep->len = bound.addr.len;
  else
    namep->len = peer.addr.len;
  return 0;

#elif defined (ACE_HAS_SVR4_TLI)
  ACE_OSCALL_RETURN (::t_getname (handle, namep, type), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (namep);
  ACE_UNUSED_ARG (type);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SVR4_TLI */
}

ACE_INLINE int
ACE_OS::t_getstate (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_getstate (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_listen (ACE_HANDLE handle, struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_listen (handle, call), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (call);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_look (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_look (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE ACE_HANDLE
ACE_OS::t_open (char *path, int oflag, struct t_info *info)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_open (path, oflag, info), ACE_HANDLE, ACE_INVALID_HANDLE);
#else
  ACE_UNUSED_ARG (path);
  ACE_UNUSED_ARG (oflag);
  ACE_UNUSED_ARG (info);

  ACE_NOTSUP_RETURN (ACE_INVALID_HANDLE);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_optmgmt (ACE_HANDLE handle,
                   struct t_optmgmt *req,
                   struct t_optmgmt *ret)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_optmgmt (handle, req, ret), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (req);
  ACE_UNUSED_ARG (ret);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_rcv (ACE_HANDLE handle,
               char *buf,
               unsigned int nbytes,
               int *flags)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_rcv (handle, buf, nbytes, flags),
                     int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (nbytes);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_rcvdis (ACE_HANDLE handle, struct t_discon *discon)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_rcvdis (handle, discon), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (discon);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_rcvrel (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_rcvrel (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_rcvudata (ACE_HANDLE handle,
                    struct t_unitdata *unitdata,
                    int *flags)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_rcvudata (handle, unitdata, flags),
                     int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (unitdata);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_rcvuderr (ACE_HANDLE handle, struct t_uderr *uderr)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_rcvuderr (handle, uderr), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (uderr);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_snd (ACE_HANDLE handle,
               const char *buf,
               unsigned int nbytes,
               int flags)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_snd (handle, (char *) buf, nbytes, flags), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (nbytes);
  ACE_UNUSED_ARG (flags);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_snddis (ACE_HANDLE handle, struct t_call *call)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_snddis (handle, call), int, -1);
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (call);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_sndrel (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_sndrel (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_sync (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_sync (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}

ACE_INLINE int
ACE_OS::t_unbind (ACE_HANDLE handle)
{
#if defined (ACE_HAS_TLI)
  ACE_OSCALL_RETURN (::t_unbind (handle), int, -1);
#else
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_TLI */
}
